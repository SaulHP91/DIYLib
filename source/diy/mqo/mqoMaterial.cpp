#include <diy/mqo/mqo.hpp>

#include <sstream>

namespace diy
{
	namespace mqo
	{
		MqoMaterial::MqoMaterial(void) :
			Shader(0),
			Color(1.0f),
			Alpha(1.0f),
			Emission(0.0f),
			Specular(0.0f),
			Power(0.0f),
			Ambient(0.0f)
		{
			;
		}

		MqoMaterial::~MqoMaterial(void)
		{
			;
		}

		bool MqoMaterial::ParseFromFStream(std::ifstream& in)
		{
			static char buffer[256];

			std::streampos pos = in.tellg();

			if (!Name.ParseFromFStream(in))
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
				if (!_stricmp(buffer, "shader"))
				{
					str_in >> Shader;
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
				else if (!_stricmp(buffer, "col"))
				{
					if (!Color.ParseFromSStream(str_in))
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
					str_in >> Alpha;
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
				else if (!_stricmp(buffer, "dif"))
				{
					str_in >> Diffuse;
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
				else if (!_stricmp(buffer, "emi"))
				{
					str_in >> Emission;
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
				else if (!_stricmp(buffer, "spc"))
				{
					str_in >> Specular;
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
				else if (!_stricmp(buffer, "power"))
				{
					str_in >> Power;
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
				else if (!_stricmp(buffer, "amb"))
				{
					str_in >> Ambient;
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
				else if (!_stricmp(buffer, "tex"))
				{
					if (!Texture.ParseFromSStream(str_in))
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
				else if (!_stricmp(buffer, "aplane"))
				{
					if (!AlphaPlane.ParseFromSStream(str_in))
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
				else if (!_stricmp(buffer, "bump"))
				{
					if (!Bump.ParseFromSStream(str_in))
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
				else
				{
					str_in.ignore(255, ')');
				}
			}

			return true;
		}
	}
}
