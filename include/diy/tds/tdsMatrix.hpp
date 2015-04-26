#ifndef TDSMATRIX_HPP
#define TDSMATRIX_HPP

#include <diy/tds/tdsPoint.hpp>

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
		class TdsMatrix
		{
		public:
			DIYLIB_API TdsMatrix(void);
			DIYLIB_API ~TdsMatrix(void);

			TdsPoint X, Y, Z, Origin;

			DIYLIB_API bool LoadFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);

			DIYLIB_API operator glm::mat4(void);
		};
	}
}

#endif
 