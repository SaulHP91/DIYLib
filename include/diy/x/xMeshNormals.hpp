#ifndef XMESHNORMALS_HPP
#define XMESHNORMALS_HPP

#include <diy/x/xDWord.hpp>
#include <diy/x/xArray.hpp>
#include <diy/x/xVector.hpp>
#include <diy/x/xMeshFace.hpp>

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
		class XMeshNormals
		{
		public:
			DIYLIB_API XMeshNormals(void);
			DIYLIB_API ~XMeshNormals(void);

			XDWord NNormals;
			XArray<XVector> Normals;
			XDWord NFaceNormals;
			XArray<XMeshFace> FaceNormals;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
