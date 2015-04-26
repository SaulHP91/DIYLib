#ifndef TDSPERCENTAGE_HPP
#define TDSPERCENTAGE_HPP

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
		class TdsChunks;
		class TdsPercentage
		{
		public:
			DIYLIB_API TdsPercentage(void);
			DIYLIB_API ~TdsPercentage(void);

			float Percentage;

			DIYLIB_API bool LoadFromChunksAndFStream(TdsChunks& chunks, std::ifstream& in);

			DIYLIB_API operator float(void);
		};
	}
}

#endif
 