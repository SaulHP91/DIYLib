#ifndef SMDVECTOR_HPP
#define SMDVECTOR_HPP

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
		class SmdVector
		{
		public:
			DIYLIB_API SmdVector(void);
			DIYLIB_API ~SmdVector(void);

			float X, Y, Z;

			DIYLIB_API bool ParseFromSStream(std::istringstream& in);

			DIYLIB_API operator glm::vec3(void);
		};
	}
}

#endif
 