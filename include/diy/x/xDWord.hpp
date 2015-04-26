#ifndef XDWORD_HPP
#define XDWORD_HPP

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
		class XDWord
		{
		public:
			DIYLIB_API XDWord(int value = 0);
			DIYLIB_API ~XDWord(void);

			int Value;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);

			DIYLIB_API operator int(void);
		};
	}
}

#endif

