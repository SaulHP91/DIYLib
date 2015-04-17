#include "mqoScene.hpp"

namespace diy
{

	MqoScene::MqoScene(void) :
		Position(0.0f, 0.0f, 1.0f),
		LookAt(0.0f),
		Head(0.0f),
		Pich(0.0f),
		Ortho(false),
		Zoom2(1.0f),
		Ambient(0.0f)
	{
		;
	}

	MqoScene::~MqoScene(void)
	{
		Lights.clear();
		Lights.swap(std::vector<MqoLight>(Lights));
	}

	bool MqoScene::ParseFromFStream(std::ifstream& in)
	{
		static char buffer[256];

		std::streampos pos = in.tellg();

		char token;
		in >> token;
		if (token != '{' || !in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		while (true)
		{
			std::streampos alter_pos = in.tellg();

			in >> token;
			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}
			if (token == '}')
			{
				break;
			}

			in.clear();
			in.seekg(alter_pos);

			in >> buffer;
			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}
			if (!stricmp(buffer, "pos"))
			{
				if (!Position.ParseFromFStream(in))
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}
			else if (!stricmp(buffer, "lookat"))
			{
				if (!LookAt.ParseFromFStream(in))
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}
			else if (!stricmp(buffer, "head"))
			{
				in >> Head;
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}
			else if (!stricmp(buffer, "pich"))
			{
				in >> Pich;
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}
			else if (!stricmp(buffer, "ortho"))
			{
				in >> Ortho;
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}
			else if (!stricmp(buffer, "zoom2"))
			{
				in >> Zoom2;
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}
			else if (!stricmp(buffer, "amb"))
			{
				if (!Ambient.ParseFromFStream(in))
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}
			else if (!stricmp(buffer, "dirlights"))
			{
				int lightCount;
				in >> lightCount;
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}

				in >> token;
				if (token != '{' || !in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}

				Lights.resize(lightCount);
				for (std::vector<MqoLight>::iterator mqo_light = Lights.begin(); mqo_light != Lights.end(); ++mqo_light)
				{
					in >> buffer;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
					if (!stricmp(buffer, "light"))
					{
						if (!mqo_light->ParseFromFStream(in))
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
				}

				in >> token;
				if (token != '}' || !in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
			}
			else
			{
				in.getline(buffer, 255);
			}
		}

		return true;
	}

}
