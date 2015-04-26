#ifndef TDSMSHMATGROUP_HPP
#define TDSMSHMATGROUP_HPP

#include <string>
#include <vector>
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
		class TdsMshMatGroup
		{
		public:
			DIYLIB_API TdsMshMatGroup(void);
			DIYLIB_API ~TdsMshMatGroup(void);

			DIYLIB_API void Clear(void);

			std::string MaterialName;
			std::vector<unsigned short> FaceNum;

			DIYLIB_API bool LoadFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);
		};
	}
}

#endif
 