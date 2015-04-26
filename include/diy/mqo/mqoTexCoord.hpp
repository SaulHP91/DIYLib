#ifndef MQOTEXCOORD_HPP
#define MQOTEXCOORD_HPP

#include <sstream>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mqo
	{
		class MqoTexCoord
		{
		public:
			DIYLIB_API MqoTexCoord(void);
			DIYLIB_API ~MqoTexCoord(void);

			float U, V;

			DIYLIB_API bool ParseFromSStream(std::istringstream& in);

			DIYLIB_API operator glm::vec2(void);
		};
	}
}

#endif

 