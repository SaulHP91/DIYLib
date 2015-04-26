#ifndef XSTRING_HPP
#define XSTRING_HPP

#include <string>
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
		class XString
		{
		public:
			DIYLIB_API XString(void);
			DIYLIB_API ~XString(void);

			std::string Text;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);

			DIYLIB_API operator std::string(void);
		};
	}
}

#endif
