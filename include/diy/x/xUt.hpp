#ifndef XUT_HPP
#define XUT_HPP

#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		__inline bool XParseSymbolFromFStream(char* symbol, std::ifstream& in)
		{
			in.clear();
			std::streampos pos = in.tellg();

			char* pSymbol = symbol;

			while (isspace(in.peek()))
			{
				in.ignore();
			}

			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			char c = in.peek();
			switch (c)
			{
			case '=':
			case '(':
			case ')':
			case '[':
			case ']':
			case '{':
			case '}':
			case '<':
			case '>':
			case ',':
			case ';':
			{
				*pSymbol = in.get();
				++pSymbol;
			}
			break;
			case '.':
			{
				*pSymbol = in.get(); ++pSymbol;
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
				if (in.peek() != '.')
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
				*pSymbol = in.get(); ++pSymbol;
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
				if (in.peek() != '.')
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
				*pSymbol = in.get(); ++pSymbol;
			}
			break;
			default:
			{
				in.clear();
				in.seekg(pos);
				return false;
			}
			}

			*pSymbol = 0;
			return true;
		}

		__inline bool XParseNameFromFStream(char* name, std::ifstream& in)
		{
			in.clear();
			std::streampos pos = in.tellg();

			char* pName = name;

			while (isspace(in.peek()))
			{
				in.ignore();
			}

			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			if (isalpha(in.peek()) || in.peek() == '_')
			{
				*pName = in.get();
				++pName;

				if (in.good())
				{
					while (isalnum(in.peek()) || in.peek() == '_')
					{
						*pName = in.get();
						++pName;
						if (!in.good())
						{
							break;
						}
					}
				}
			}
			else
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			*pName = 0;
			return true;
		}

		__inline bool XParseIntegerFromFStream(char* number, std::ifstream& in)
		{
			in.clear();
			std::streampos pos = in.tellg();

			char* pNumber = number;

			while (isspace(in.peek()))
			{
				in.ignore();
			}

			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			if (isdigit(in.peek()))
			{
				*pNumber = in.get();
				++pNumber;

				if (in.good())
				{
					while (isdigit(in.peek()))
					{
						*pNumber = in.get();
						++pNumber;
						if (!in.good())
						{
							break;
						}
					}
				}
			}
			else
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			*pNumber = 0;
			return true;
		}

		__inline bool XParseFloatFromFStream(char* number, std::ifstream& in)
		{
			in.clear();
			std::streampos pos = in.tellg();

			char* pNumber = number;

			while (isspace(in.peek()))
			{
				in.ignore();
			}

			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			if (in.peek() == '+' || in.peek() == '-')
			{
				*pNumber = in.get();
				++pNumber;
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}

			if (isdigit(in.peek()))
			{
				*pNumber = in.get();
				++pNumber;

				if (in.good())
				{
					while (isdigit(in.peek()))
					{
						*pNumber = in.get();
						++pNumber;
						if (!in.good())
						{
							break;
						}
					}
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
					if (in.peek() == '.')
					{
						*pNumber = in.get();
						++pNumber;
					}
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
					while (isdigit(in.peek()))
					{
						*pNumber = in.get();
						++pNumber;
						if (!in.good())
						{
							break;
						}
					}
				}
			}
			if (in.peek() == '.')
			{
				*pNumber = in.get();
				++pNumber;

				if (in.good())
				{
					while (isdigit(in.peek()))
					{
						*pNumber = in.get();
						++pNumber;
						if (!in.good())
						{
							break;
						}
					}
				}
			}
			else if (toupper(in.peek()) == 'E')
			{
				*pNumber = in.get();
				++pNumber;
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
				if (in.peek() == '+' || in.peek() == '-' || isdigit(in.peek()))
				{
					*pNumber = in.get();
					++pNumber;
				}
				else
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
				while (isdigit(in.peek()))
				{
					*pNumber = in.get();
					++pNumber;
					if (!in.good())
					{
						break;
					}
				}
			}

			if (pNumber == number)
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			*pNumber = 0;
			return true;
		}

		__inline bool XParseStringFromFStream(char* name, std::ifstream& in)
		{
			in.clear();
			std::streampos pos = in.tellg();

			char* pName = name;

			while (isspace(in.peek()))
			{
				in.ignore();
			}

			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			if (in.peek() == '\"')
			{
				in.ignore();

				while (true)
				{
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
					if (in.peek() == '\"')
					{
						in.ignore();
						break;
					}
					if (in.peek() == '\\')
					{
						in.ignore();
						switch (in.peek())
						{
						case 't':
						{
							in.ignore();
							*pName = '\t';
						}
						break;
						case 'n':
						{
							in.ignore();
							*pName = '\n';
						}
						break;
						case 'r':
						{
							in.ignore();
							*pName = '\r';
						}
						break;
						case '0':
						{
							in.ignore();
							*pName = 0;
						}
						break;
						default:
						{
							*pName = in.get();
						}
						}
					}
					else
					{
						*pName = in.get();
					}
					++pName;
				}
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}
			else
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			*pName = 0;
			return true;
		}
	}
}

#endif
