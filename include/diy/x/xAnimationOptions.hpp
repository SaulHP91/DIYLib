#ifndef XANIMATIONOPTIONS_HPP
#define XANIMATIONOPTIONS_HPP

#include <diy/x/xDWord.hpp>

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
		class XAnimationOptions
		{
		public:
			DIYLIB_API XAnimationOptions(void);
			DIYLIB_API ~XAnimationOptions(void);

			XDWord OpenClosed;
			XDWord PositionQuality;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
