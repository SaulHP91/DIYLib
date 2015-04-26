#ifndef TDSTEXVERT_HPP
#define TDSTEXVERT_HPP

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
		class TdsChunk;
		class TdsTexVert
		{
		public:
			DIYLIB_API TdsTexVert(void);
			DIYLIB_API ~TdsTexVert(void);

			float X, Y;

			DIYLIB_API bool LoadFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);

			DIYLIB_API operator glm::vec2(void);
		};
	}
}

#endif

 