#ifndef OBJVECTOR_HPP
#define OBJVECTOR_HPP

#include <sstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace obj
	{
		class ObjVector
		{
		public:
			DIYLIB_API ObjVector(void);
			DIYLIB_API ~ObjVector(void);

			float X, Y, Z;

			DIYLIB_API bool ParseFromSStream(std::istringstream& in);

			DIYLIB_API operator glm::vec3(void);
		};
	}
}

#endif
