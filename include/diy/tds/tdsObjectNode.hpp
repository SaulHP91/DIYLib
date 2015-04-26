#ifndef TDSOBJECTNODE_HPP
#define TDSOBJECTNODE_HPP

#include <diy/tds/tdsNodeHeader.hpp>
#include <diy/tds/tdsPoint.hpp>

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
		class TdsObjectNode
		{
		public:
			DIYLIB_API TdsObjectNode(void);
			DIYLIB_API ~TdsObjectNode(void);

			unsigned short ID;
			TdsNodeHeader Header;
			TdsPoint Pivot;

			DIYLIB_API bool LoadIDFromFStreamAndChunk(TdsChunk& chunk, std::ifstream& in);
			DIYLIB_API bool LoadPivotFromFStreamAndChunk(TdsChunk& chunk, std::ifstream& in);
		};
	}
}

#endif
 