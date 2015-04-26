#ifndef SMDTEXCOORD_HPP
#define SMDTEXCOORD_HPP

#include <sstream>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace smd
	{
		class SmdTexCoord
		{
		public:
			DIYLIB_API SmdTexCoord(void);
			DIYLIB_API ~SmdTexCoord(void);

			float U, V;

			DIYLIB_API bool ParseFromSStream(std::istringstream& in);

			DIYLIB_API operator glm::vec2(void);
		};
	}
}

#endif

 