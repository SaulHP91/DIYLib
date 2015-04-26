#ifndef TDSOBJECT_HPP
#define TDSOBJECT_HPP

#include "tdsMatrix.hpp"

#include <string>
#include <vector>
#include <fstream>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace tds
	{
		class TdsObject
		{
		public:
			DIYLIB_API TdsObject(void);
			DIYLIB_API ~TdsObject(void);

			std::string Name;
			std::vector<class TdsPoint> PointArray;
			TdsMatrix Matrix;
			std::vector<class TdsTexVert> TexVerts;
			std::vector<class TdsFace> FaceArray;
			std::vector<class TdsMshMatGroup> MshMatGroups;

			DIYLIB_API bool LoadNameFromChunkAndFStream(class TdsChunk& chunk, std::ifstream& in);
			DIYLIB_API bool LoadPointArrayFromChunkAndFStream(class TdsChunk& chunk, std::ifstream& in);
			DIYLIB_API bool LoadTexVertsFromChunkAndFStream(class TdsChunk& chunk, std::ifstream& in);
			DIYLIB_API bool LoadFaceArrayFromChunkAndFStream(class TdsChunk& chunk, std::ifstream& in);
		};
	}
}

#endif
