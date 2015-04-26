#ifndef XMATERIAL_HPP
#define XMATERIAL_HPP

#include <diy/x/xcolorrgba.hpp>
#include <diy/x/xfloat.hpp>
#include <diy/x/xcolorrgb.hpp>

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
		class XMaterial
		{
		public:
			DIYLIB_API XMaterial(void);
			DIYLIB_API ~XMaterial(void);

			XColorRGBA FaceColor;
			XFloat Power;
			XColorRGB SpecularColor;
			XColorRGB EmissiveColor;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
