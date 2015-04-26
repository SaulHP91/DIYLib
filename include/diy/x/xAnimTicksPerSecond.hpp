#ifndef XANIMTICKSPERSECOND_HPP
#define XANIMTICKSPERSECOND_HPP

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
		class XAnimTicksPerSecond
		{
		public:
			DIYLIB_API XAnimTicksPerSecond(void);
			DIYLIB_API ~XAnimTicksPerSecond(void);

			XDWord AnimTicksPerSecond;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
