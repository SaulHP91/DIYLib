#ifndef PMDFACE_HPP
#define PMDFACE_HPP

#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace pmd
	{
		class PmdFace
		{
		public:
			DIYLIB_API PmdFace(void);
			DIYLIB_API ~PmdFace(void);

			unsigned short VertexIndices[3];

			DIYLIB_API void LoadFromFStream(std::ifstream& in);
		};
	}
}

#endif
 