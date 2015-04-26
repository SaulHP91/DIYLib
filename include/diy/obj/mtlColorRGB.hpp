#ifndef MTLCOLORRGB_HPP
#define MTLCOLORRGB_HPP

#include <sstream>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace obj
	{
		class MtlColorRGB
		{
		public:
			DIYLIB_API MtlColorRGB(void);
			DIYLIB_API MtlColorRGB(float gs);
			DIYLIB_API MtlColorRGB(float r, float g, float b);
			DIYLIB_API ~MtlColorRGB(void);

			float R, G, B;

			bool ParseFromSStream(std::istringstream& in);

			DIYLIB_API operator glm::vec3(void);
		};
	}
}

#endif

