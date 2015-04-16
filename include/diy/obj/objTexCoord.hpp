#ifndef OBJTEXCOORD_HPP
#define OBJTEXCOORD_HPP

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

	class ObjTexCoord
	{
	public:
		DIYLIB_API ObjTexCoord(void);
		DIYLIB_API ~ObjTexCoord(void);

		float U, V;

		DIYLIB_API bool ParseFromSStream(std::istringstream& in);

		DIYLIB_API operator glm::vec2(void);
	};

}

#endif

