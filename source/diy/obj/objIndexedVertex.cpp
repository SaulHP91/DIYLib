#include <diy/obj/obj.hpp>

namespace diy
{
	namespace obj
	{
		int ObjIndexedVertex::PositionCount;
		int ObjIndexedVertex::TexCoordCount;
		int ObjIndexedVertex::NormalCount;

		ObjIndexedVertex::ObjIndexedVertex(void) :
			PositionIndex(0),
			TexCoordIndex(0),
			NormalIndex(0)
		{
			;
		}

		ObjIndexedVertex::~ObjIndexedVertex(void)
		{
			;
		}

		bool ObjIndexedVertex::ParseFromSStream(std::istringstream& in)
		{
			char buffer[256];

			std::streampos pos = in.tellg();

			in >> buffer;
			if (in.fail())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			if (sscanf(buffer, "%i/%i/%i", &PositionIndex, &TexCoordIndex, &NormalIndex) != 3)
			{
				if (sscanf(buffer, "%i/%i", &PositionIndex, &TexCoordIndex) != 2)
				{
					if (sscanf(buffer, "%i//%i", &PositionIndex, &NormalIndex) != 2)
					{
						if (sscanf(buffer, "%i", &PositionIndex) != 1)
						{
							in.clear();
							in.seekg(pos);
							return false;
						}
					}
				}
			}

			if (PositionIndex < 0)
			{
				PositionIndex += PositionCount + 1;
			}

			if (TexCoordIndex < 0)
			{
				TexCoordIndex += TexCoordCount + 1;
			}

			if (NormalIndex < 0)
			{
				NormalIndex += NormalCount + 1;
			}

			return true;
		}

		bool ObjIndexedVertex::operator==(ObjIndexedVertex objIndexedVertex)
		{
			return
				PositionIndex == objIndexedVertex.PositionIndex &&
				TexCoordIndex == objIndexedVertex.TexCoordIndex &&
				NormalIndex == objIndexedVertex.NormalIndex;
		}
	}
}
