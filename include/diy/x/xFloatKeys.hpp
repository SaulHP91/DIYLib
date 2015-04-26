#ifndef XFLOATKEYS_HPP
#define XFLOATKEYS_HPP

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
		class XFloat;
		class XFloatKeys
		{
		public:
			DIYLIB_API XFloatKeys(void);
			DIYLIB_API ~XFloatKeys(void);

			XDWord NValues;
			XArray<XFloat> Values;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif