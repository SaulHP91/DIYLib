#ifndef XANIMATIONKEY_HPP
#define XANIMATIONKEY_HPP

#include <diy/x/xDWord.hpp>
#include <diy/x/xArray.hpp>

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
		class XTimedFloatKeys;
		class XAnimationKey
		{
		public:
			DIYLIB_API XAnimationKey(void);
			DIYLIB_API ~XAnimationKey(void);

			XDWord KeyType;
			XDWord NKeys;
			XArray<XTimedFloatKeys> Keys;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif

