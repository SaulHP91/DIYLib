#ifndef XSKINMESHHEADER_HPP
#define XSKINMESHHEADER_HPP

#include <diy/x/xDWord.hpp>

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
		class XSkinMeshHeader
		{
		public:
			DIYLIB_API XSkinMeshHeader(void);
			DIYLIB_API ~XSkinMeshHeader(void);

			XDWord NMaxSkinWeightsPerVertex;
			XDWord NMaxSkinWeightsPerFace;
			XDWord NBones;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
