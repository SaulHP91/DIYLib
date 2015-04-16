#include "diy/obj/objFile.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

namespace diy
{

	ObjFile::ObjFile(void)
	{
		;
	}

	ObjFile::~ObjFile(void)
	{
		VertexPositions.clear();
		VertexPositions.swap(std::vector<ObjVector>(VertexPositions));

		VertexTexCoords.clear();
		VertexTexCoords.swap(std::vector<ObjTexCoord>(VertexTexCoords));

		VertexNormals.clear();
		VertexNormals.swap(std::vector<ObjVector>(VertexNormals));

		Groups.clear();
		Groups.swap(std::vector<std::string>(Groups));

		Faces.clear();
		Faces.swap(std::vector<ObjFace>(Faces));
	}

	void ObjFile::Clear(void)
	{
		Name.clear();

		VertexPositions.clear();
		VertexPositions.swap(std::vector<ObjVector>(VertexPositions));

		VertexTexCoords.clear();
		VertexTexCoords.swap(std::vector<ObjTexCoord>(VertexTexCoords));

		VertexNormals.clear();
		VertexNormals.swap(std::vector<ObjVector>(VertexNormals));

		Groups.clear();
		Groups.swap(std::vector<std::string>(Groups));

		Faces.clear();
		Faces.swap(std::vector<ObjFace>(Faces));

		MaterialLibrary.Clear();
	}

	bool ObjFile::ParseFromFile(std::string path)
	{
		char line[256];
		char token[8];

		Clear();

		char fname[256];
		_splitpath(path.c_str(), 0, 0, fname, 0);
		Name = fname;

		std::ifstream in(path.c_str());
		if (in.is_open())
		{
			int groupIndex = -1;
			int materialIndex = -1;
			ObjIndexedVertex::PositionCount = 0;
			ObjIndexedVertex::TexCoordCount = 0;
			ObjIndexedVertex::NormalCount = 0;

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

				in.getline(line, 255);
				std::istringstream str_in(line);

				str_in >> token;
				if (*token == '#')
				{
					continue;
				}
				else if (!_stricmp(token, "o"))
				{
					while (isspace(str_in.peek()))
					{
						str_in.ignore();
					}

					str_in.getline(line, 255);
					Name = line;
				}
				else if (!_stricmp(token, "mtllib"))
				{
					while (isspace(str_in.peek()))
					{
						str_in.ignore();
					}

					str_in.getline(line, 255);
					MaterialLibrary.ParseFromFile(line);
				}
				else if (!_stricmp(token, "v"))
				{
					ObjVector objVector;

					if (objVector.ParseFromSStream(str_in))
					{
						VertexPositions.push_back(objVector);
						++ObjIndexedVertex::PositionCount;
					}
					else
					{
						return false;
					}
				}
				else if (!_stricmp(token, "vt"))
				{
					ObjTexCoord objTexCoord;

					if (objTexCoord.ParseFromSStream(str_in))
					{
						VertexTexCoords.push_back(objTexCoord);
						++ObjIndexedVertex::TexCoordCount;
					}
					else
					{
						return false;
					}
				}
				else if (!_stricmp(token, "vn"))
				{
					ObjVector objVector;

					if (objVector.ParseFromSStream(str_in))
					{
						VertexNormals.push_back(objVector);
						++ObjIndexedVertex::NormalCount;
					}
					else
					{
						return false;
					}
				}
				else if (!_stricmp(token, "g"))
				{
					while (isspace(str_in.peek()))
					{
						str_in.ignore();
					}

					str_in.getline(line, 255);
					if (str_in.fail())
					{
						return false;
					}

					groupIndex = IndexOf(line);
				}
				else if (!_stricmp(token, "usemtl"))
				{
					while (isspace(str_in.peek()))
					{
						str_in.ignore();
					}

					str_in.getline(line, 255);
					if (str_in.fail())
					{
						return false;
					}

					materialIndex = MaterialLibrary.IndexOf(line);
				}
				else if (!_stricmp(token, "f"))
				{
					ObjFace objFace;
					objFace.GroupIndex = groupIndex;
					objFace.MaterialIndex = materialIndex;

					if (objFace.ParseFromSStream(str_in))
					{
						Faces.push_back(objFace);
					}
					else
					{
						return false;
					}
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

	int ObjFile::IndexOf(std::string groupName)
	{
		std::vector<std::string>::iterator group;

		for (group = Groups.begin(); group != Groups.end(); ++group)
		{
			if (!_stricmp(group->c_str(), groupName.c_str()))
			{
				break;
			}
		}

		if (group == Groups.end())
		{
			Groups.push_back(groupName);
			return Groups.size() - 1;
		}
		else
		{
			return distance(Groups.begin(), group);
		}
	}

}
