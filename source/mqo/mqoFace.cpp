#include "mqoFace.hpp"

#include <sstream>

namespace diy
{

	MqoFace::MqoFace(void) :
		VertexCount(0),
		VertexIndices(-1),
		MaterialIndex(-1)
	{
		;
	}

	MqoFace::~MqoFace(void)
	{
		;
	}

	bool MqoFace::ParseFromFStream(std::ifstream& in)
	{
		static char buffer[256];

		std::streampos pos = in.tellg();

		in >> VertexCount;
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		in.getline(buffer, 255);
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		std::istringstream str_in(buffer);
		char token;
		while (true)
		{
			while (isspace(str_in.peek()))
			{
				str_in.ignore();
			}

			str_in.get();
			if (str_in.eof())
			{
				break;
			}
			str_in.unget();

			str_in.getline(buffer, 255, '(');
			if (!_stricmp(buffer, "V"))
			{
				for (int i = 0; i < VertexCount; ++i)
				{
					str_in >> VertexIndices[i];
					if (!str_in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}

				str_in >> token;
				if (token != ')' || !str_in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}
			else if (!_stricmp(buffer, "M"))
			{
				str_in >> MaterialIndex;
				if (!str_in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}

				str_in >> token;
				if (token != ')' || !str_in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}
			else if (!_stricmp(buffer, "UV"))
			{
				for (int i = 0; i < VertexCount; ++i)
				{
					if (!VertexTexCoords[i].ParseFromSStream(str_in))
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}

				str_in >> token;
				if (token != ')' || !str_in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}
			else
			{
				str_in.ignore(255, ')');
			}
		}

		return true;
	}

}
