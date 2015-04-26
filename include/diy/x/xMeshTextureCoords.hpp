#ifndef XMESHTEXTURECOORDS_HPP
#define XMESHTEXTURECOORDS_HPP

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
		class XCoords2D;
		class XMeshTextureCoords
		{
		public:
			DIYLIB_API XMeshTextureCoords(void);
			DIYLIB_API ~XMeshTextureCoords(void);

			XDWord NTextureCoords;
			XArray<XCoords2D> TextureCoords;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
