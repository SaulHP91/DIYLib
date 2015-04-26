#ifndef TDSMAP_HPP
#define TDSMAP_HPP

#include "tdsPercentage.hpp"

#include <string>

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
		class TdsMap
		{
		public:
			DIYLIB_API TdsMap(void);
			DIYLIB_API ~TdsMap(void);

			TdsPercentage Percentage;
			std::string Name;
			float UScale;
			float VScale;
			float UOffset;
			float VOffset;
			unsigned short Tiling;

			DIYLIB_API bool LoadFromChunksAndFStream(TdsChunks& chunks, std::ifstream& in);
			DIYLIB_API bool LoadNameFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);
			DIYLIB_API bool LoadUScaleFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);
			DIYLIB_API bool LoadVScaleFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);
			DIYLIB_API bool LoadUOffsetFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);
			DIYLIB_API bool LoadVOffsetFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);
			DIYLIB_API bool LoadTilingFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);
		};
	}
}

#endif
 