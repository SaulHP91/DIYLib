#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XName::XName(void)
		{
			;
		}

		XName::~XName(void)
		{
			;
		}

		bool XName::ParseFromFStream(std::ifstream& in)
		{
			static char buffer[256];
			char* pBuffer = buffer;

			in.clear();
			std::streampos pos = in.tellg();

			if (!in.good())
			{
				return false;
			}

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

			switch (in.peek())
			{
			case '{':
			case '}':
			case ';':
			{
				*pBuffer = in.get();
				++pBuffer;
				goto END_OF_WHILE;
			}
			default:
			{
				;
			}
			}

			*pBuffer = in.get();
			++pBuffer;

			while (true)
			{
				if (!in.good())
				{
					break;
				}

				if (isspace(in.peek()))
				{
					break;
				}

				switch (in.peek())
				{
				case '{':
				case '}':
				case ';':
				{
					goto END_OF_WHILE;
				}
				default:
				{
					*pBuffer = in.get();
					++pBuffer;
				}
				}
			}
		END_OF_WHILE:;

			*pBuffer = 0;
			Text = buffer;

			return true;
		}

		XName::operator std::string(void)
		{
			return Text;
		}
	}
}
