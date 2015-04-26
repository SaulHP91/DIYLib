#ifndef XFRAMETRANSFORMMATRIX_HPP
#define XFRAMETRANSFORMMATRIX_HPP

#include <diy/x/xMatrix4X4.hpp>

#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		class XFrameTransformMatrix
		{
		public:
			DIYLIB_API XFrameTransformMatrix(void);
			DIYLIB_API ~XFrameTransformMatrix(void);

			XMatrix4X4 FrameMatrix;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
