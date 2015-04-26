#ifndef XMATRIX4X4_HPP
#define XMATRIX4X4_HPP

#include <diy/x/xArray.hpp>

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
	
		class XFloat;
		class XMatrix4X4
		{
		public:
			DIYLIB_API XMatrix4X4(void);
			DIYLIB_API ~XMatrix4X4(void);

			XArray<XFloat> Matrix;

			DIYLIB_API bool ParseFromFStream(std::ifstream&  in);

			DIYLIB_API operator glm::mat4(void);
		};
	}
}

#endif
