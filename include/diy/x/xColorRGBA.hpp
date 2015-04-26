#ifndef XCOLORRGBA_HPP
#define XCOLORRGBA_HPP

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
		class XColorRGBA
		{
		public:
			DIYLIB_API XColorRGBA(void);
			DIYLIB_API ~XColorRGBA(void);

			XFloat Red;
			XFloat Green;
			XFloat Blue;
			XFloat Alpha;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);

			DIYLIB_API operator glm::vec4(void);
		};
	}
}

#endif
