#ifndef XVECTOR_HPP
#define XVECTOR_HPP

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
		class XVector
		{
		public:
			DIYLIB_API XVector(void);
			DIYLIB_API ~XVector(void);

			XFloat X;
			XFloat Y;
			XFloat Z;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);

			DIYLIB_API operator glm::vec3(void);
		};
	}
}

#endif
