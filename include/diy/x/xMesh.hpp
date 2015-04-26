#ifndef XMESH_HPP
#define XMESH_HPP

#include <diy/x/xDWord.hpp>
#include <diy/x/xArray.hpp>

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
		class XVector;
		class XMeshFace;
		class XMesh
		{
		public:
			DIYLIB_API XMesh(void);
			DIYLIB_API ~XMesh(void);

			XDWord NVertices;
			XArray<XVector> Vertices;
			XDWord NFaces;
			XArray<XMeshFace> Faces;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
