#ifndef XMESHFACE_HPP
#define XMESHFACE_HPP

#include <diy/x/xDWord.hpp>
#include <diy/x/xArray.hpp>

#include <vector>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		class XMeshFace
		{
		public:
			DIYLIB_API XMeshFace(void);
			DIYLIB_API ~XMeshFace(void);

			XDWord NFaceVertexIndices;
			XArray<XDWord> FaceVertexIndices;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif

