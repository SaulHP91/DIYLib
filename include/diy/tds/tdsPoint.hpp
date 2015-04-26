#ifndef TDSPOINT_HPP
#define TDSPOINT_HPP

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
		class TdsPoint
		{
		public:
			DIYLIB_API TdsPoint(void);
			DIYLIB_API ~TdsPoint(void);

			float X, Y, Z;

			DIYLIB_API bool LoadFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);

			DIYLIB_API operator glm::vec3(void);
		};
	}
}

#endif
 