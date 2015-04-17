#include "mqoFile.hpp"

#include <fstream>

namespace diy
{

	MqoFile::MqoFile(void)
	{
		;
	}

	MqoFile::~MqoFile(void)
	{
		Clear();
	}

	void MqoFile::Clear(void)
	{
		Name.clear();

		Materials.clear();
		Materials.swap(std::vector<MqoMaterial>(Materials));

		Objects.clear();
		Objects.swap(std::vector<MqoObject>(Objects));
	}


	bool MqoFile::ParseFromFile(std::string path)
	{
		static char buffer[256];

		bool result = false;

		std::ifstream in(path.c_str());
		if (in.is_open())
		{
			in.getline(buffer, 255);
			if (!_stricmp(buffer, "Metasequoia Document"))
			{
				in.getline(buffer, 255);
				if (!_stricmp(buffer, "Format Text Ver 1.0"))
				{
					Clear();

					_splitpath(path.c_str(), 0, 0, buffer, 0);
					Name = buffer;
					char token;

					while (true)
					{
						in.get();
						if (!in.good())
						{
							break;
						}
						in.unget();

						in >> buffer;
						if (!_stricmp(buffer, "IncludeXml"))
						{
							in.getline(buffer, 255);
						}
						else if (!_stricmp(buffer, "Scene"))
						{
							if (!Scene.ParseFromFStream(in))
							{
								break;
							}
						}
						else if (!_stricmp(buffer, "BackImage"))
						{
							in.ignore(255, '}');
						}
						else if (!_stricmp(buffer, "Material"))
						{
							int materialCount;
							in >> materialCount;
							if (!in.good())
							{
								break;
							}

							in >> token;
							if (token != '{' || !in.good())
							{
								break;
							}

							Materials.resize(materialCount);
							for (std::vector<MqoMaterial>::iterator mqo_material = Materials.begin(); mqo_material != Materials.end(); ++mqo_material)
							{
								if (!mqo_material->ParseFromFStream(in))
								{
									break;
								}
							}

							in >> token;
							if (token != '}' || !in.good())
							{
								break;
							}
						}
						else if (!_stricmp(buffer, "Object"))
						{
							Objects.push_back(MqoObject());
							if (!Objects.back().ParseFromFStream(in))
							{
								break;
							}
						}
						else if (!_stricmp(buffer, "Blob"))
						{
							in.ignore(255, '}');
						}
						else if (!_stricmp(buffer, "Eof"))
						{
							while (isspace(in.peek()))
							{
								in.ignore();
							}
							result = in.eof();
							break;
						}
						else
						{
							break;
						}
					}
				}
			}
			in.close();
		}

		return result;
	}

}
