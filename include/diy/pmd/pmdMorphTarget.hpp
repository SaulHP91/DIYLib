#ifndef PMDMORPHTARGET_HPP
#define PMDMORPHTARGET_HPP

#include <vector>
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
		class PmdMorphTargetVertex;
		class PmdMorphTarget
		{
		public:
			DIYLIB_API PmdMorphTarget(void);
			DIYLIB_API ~PmdMorphTarget(void);

			char Name[20];
			unsigned char Type;
			std::vector<PmdMorphTargetVertex> Vertices;

			DIYLIB_API void LoadFromFStream(std::ifstream& in);
		};
	}
}

#endif
 