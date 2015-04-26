#ifndef TDSCLASSES_HPP
#define TDSCLASSES_HPP

namespace diy
{
	namespace tds
	{
		class TdsChunk;
		class TdsChunks;
		class TdsColor;
		class TdsFace;
		class TdsFile;
		class TdsMap;
		class TdsMaterial;
		class TdsMatrix;
		class TdsMshMatGroup;
		class TdsNodeHeader;
		class TdsObject;
		class TdsObjectNode;
		class TdsPercentage;
		class TdsPoint;
		class TdsTexVert;

		typedef void(*TDS_ONCHUNK_CALLBACK)(TdsChunk& chunk, void* parentUser);
		typedef void(*TDS_ONSTRING_CALLBACK)(const char* str, void* user);
	}
}

#endif