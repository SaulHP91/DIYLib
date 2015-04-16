#include "diy/obj/mtlMaterial.hpp"
#include "diy/obj/mtlFile.hpp"

#include <fstream>
#include <sstream>

namespace diy
{

	MtlFile::MtlFile(void)
	{
		;
	}

	MtlFile::~MtlFile(void)
	{
		Clear();
	}

	void MtlFile::Clear(void)
	{
		Name.clear();

		Materials.clear();
		Materials.swap(std::vector<MtlMaterial>(Materials));
	}

	bool MtlFile::ParseFromFile(std::string path)
	{
		char buffer[256];

		_splitpath(path.c_str(), 0, 0, buffer, 0);
		Name = buffer;

		std::vector<MtlMaterial>::iterator mtl_material = Materials.end();

		std::ifstream in(path.c_str());
		if (in.is_open())
		{
			while (true)
			{
				while (isspace(in.peek()))
				{
					in.ignore();
				}

				in.get();
				if (in.eof())
				{
					break;
				}
				in.unget();

				in.getline(buffer, 255);
				std::istringstream str_in(buffer);
				str_in >> buffer;
				if (buffer[0] == '#')
				{
					continue;
				}
				else if (!_stricmp(buffer, "newmtl"))
				{
					while (isspace(str_in.peek()))
					{
						str_in.ignore();
					}

					str_in.getline(buffer, 255);
					if (str_in.fail())
					{
						return false;
					}

					mtl_material = SearchByName(buffer);
				}
				else if (!_stricmp(buffer, "Kd"))
				{
					if (mtl_material == Materials.end())
					{
						return false;
					}

					if (!mtl_material->Diffuse.ParseFromSStream(str_in))
					{
						return false;
					}
				}
				else if (!_stricmp(buffer, "d"))
				{
					if (mtl_material == Materials.end())
					{
						return false;
					}

					str_in >> mtl_material->Dissolve;
					if (str_in.fail())
					{
						return false;
					}
				}
				else if (!_stricmp(buffer, "Ka"))
				{
					if (mtl_material == Materials.end())
					{
						return false;
					}

					if (!mtl_material->Ambient.ParseFromSStream(str_in))
					{
						return false;
					}
				}
				else if (!_stricmp(buffer, "Ks"))
				{
					if (mtl_material == Materials.end())
					{
						return false;
					}

					if (!mtl_material->Specular.ParseFromSStream(str_in))
					{
						return false;
					}
				}
				else if (!_stricmp(buffer, "Ns"))
				{
					if (mtl_material == Materials.end())
					{
						return false;
					}

					str_in >> mtl_material->SpecularExponent;
					if (str_in.fail())
					{
						return false;
					}
				}
				else if (!_stricmp(buffer, "map_Kd"))
				{
					while (isspace(str_in.peek()))
					{
						str_in.ignore();
					}

					str_in.getline(buffer, 255);
					if (str_in.fail())
					{
						return false;
					}
					mtl_material->DiffuseMap = buffer;
				}
				else if (!_stricmp(buffer, "map_d"))
				{
					while (isspace(str_in.peek()))
					{
						str_in.ignore();
					}

					str_in.getline(buffer, 255);
					if (str_in.fail())
					{
						return false;
					}
					mtl_material->DissolveMap = buffer;
				}
				else if (!_stricmp(buffer, "map_Ka"))
				{
					while (isspace(str_in.peek()))
					{
						str_in.ignore();
					}

					str_in.getline(buffer, 255);
					if (str_in.fail())
					{
						return false;
					}
					mtl_material->AmbientMap = buffer;
				}
				else if (!_stricmp(buffer, "map_Ks"))
				{
					while (isspace(str_in.peek()))
					{
						str_in.ignore();
					}
					str_in.getline(buffer, 255);

					if (str_in.fail())
					{
						return false;
					}
					mtl_material->SpecularMap = buffer;
				}
			}

			in.close();
			return true;
		}
		else
		{
			return false;
		}
	}

	std::vector<MtlMaterial>::iterator MtlFile::SearchByName(std::string materialName)
	{
		MtlMaterial mtlMaterial;
		mtlMaterial.Name = materialName;

		std::vector<MtlMaterial>::iterator mtl_material = find(Materials.begin(), Materials.end(), mtlMaterial);
		if (mtl_material == Materials.end())
		{
			Materials.push_back(mtlMaterial);
			mtl_material = Materials.end() - 1;
		}

		return mtl_material;
	}

	int MtlFile::IndexOf(std::string materialName)
	{
		std::vector<MtlMaterial>::iterator mtl_material = SearchByName(materialName);

		return distance(Materials.begin(), mtl_material);
	}

}
