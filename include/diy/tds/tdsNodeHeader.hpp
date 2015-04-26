#ifndef TDSNODEHEADER_HPP
#define TDSNODEHEADER_HPP

#include <string>
#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace tds
	{
		class TdsChunk;
		class TdsNodeHeader
		{
		public:
			DIYLIB_API TdsNodeHeader(void);
			DIYLIB_API ~TdsNodeHeader(void);

			std::string Name;
			short Hierarchy;

			DIYLIB_API bool LoadFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);
		};
	}
}

#endif
 