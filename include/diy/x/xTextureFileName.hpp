#ifndef XTEXTUREFILENAME_HPP
#define XTEXTUREFILENAME_HPP

#include <diy/x/xString.hpp>

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
		class XTextureFileName
		{
		public:
			DIYLIB_API XTextureFileName(void);
			DIYLIB_API ~XTextureFileName(void);

			XString FileName;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
