#include "diy/obj/objFace.hpp"

namespace diy
{

	ObjFace::ObjFace(void) :
		GroupIndex(-1),
		MaterialIndex(-1)
	{
		;
	}

	ObjFace::~ObjFace(void)
	{
		IndexedVertices.clear();
		IndexedVertices.swap(std::vector<ObjIndexedVertex>(IndexedVertices));
	}

	bool ObjFace::ParseFromSStream(std::istringstream& in)
	{
		ObjIndexedVertex objIndexedVertex;

		IndexedVertices.clear();
		std::streampos pos = in.tellg();
		while (objIndexedVertex.ParseFromSStream(in))
		{
			IndexedVertices.push_back(objIndexedVertex);
		}

		return true;
	}

}
