#ifndef XCOLORRGB_HPP
#define XCOLORRGB_HPP

#include <diy/x/xFloat.hpp>

#include <fstream>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		class XColorRGB
		{
		public:
			DIYLIB_API XColorRGB(void);
			DIYLIB_API ~XColorRGB(void);

			XFloat Red;
			XFloat Green;
			XFloat Blue;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);

			DIYLIB_API operator glm::vec3(void);
		};
	}
}

#endif
