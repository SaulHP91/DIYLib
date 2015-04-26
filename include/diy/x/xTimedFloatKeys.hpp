#ifndef XTIMEDFLOATKEYS_HPP
#define XTIMEDFLOATKEYS_HPP

#include <diy/x/xDWord.hpp>
#include <diy/x/xFloatKeys.hpp>

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
		class XTimedFloatKeys
		{
		public:
			DIYLIB_API XTimedFloatKeys(void);
			DIYLIB_API ~XTimedFloatKeys(void);

			XDWord Time;
			XFloatKeys TFKeys;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
