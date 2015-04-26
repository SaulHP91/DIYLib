#ifndef XCOORDS2D_HPP
#define XCOORDS2D_HPP

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
		class XCoords2D
		{
		public:
			DIYLIB_API XCoords2D(void);
			DIYLIB_API ~XCoords2D(void);

			XFloat U;
			XFloat V;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);

			DIYLIB_API operator glm::vec2(void);
		};
	}
}

#endif

