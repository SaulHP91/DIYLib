#ifndef TDSFACE_HPP
#define TDSFACE_HPP

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
		class TdsFace
		{
		public:
			DIYLIB_API TdsFace(void);
			DIYLIB_API ~TdsFace(void);

			unsigned short Vertex1;
			unsigned short Vertex2;
			unsigned short Vertex3;
			unsigned short Flags;

			DIYLIB_API bool LoadFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);

			DIYLIB_API operator glm::ivec3(void);
		};
	}
}

#endif
 