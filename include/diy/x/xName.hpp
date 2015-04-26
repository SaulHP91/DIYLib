#ifndef XNAME_HPP
#define XNAME_HPP

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
		class XName
		{
		public:
			DIYLIB_API XName(void);
			DIYLIB_API ~XName(void);

			std::string Text;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);

			DIYLIB_API operator std::string(void);
		};
	}
}

#endif
