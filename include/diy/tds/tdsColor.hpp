#ifndef TDSCOLOR_HPP
#define TDSCOLOR_HPP

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
		class TdsChunks;
		class TdsColor
		{
		public:
			DIYLIB_API TdsColor(void);
			DIYLIB_API ~TdsColor(void);

			float R, G, B;

			DIYLIB_API bool LoadFromChunksAndFStream(TdsChunks& chunks, std::ifstream& in);

			DIYLIB_API operator glm::vec3(void);
		};
	}
}

#endif
 