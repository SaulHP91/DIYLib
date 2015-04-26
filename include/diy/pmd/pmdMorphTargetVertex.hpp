#ifndef PMDMORPHTARGETVERTEX_HPP
#define PMDMORPHTARGETVERTEX_HPP

#include "pmdVector.hpp"

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
		class PmdMorphTargetVertex
		{
		public:
			DIYLIB_API PmdMorphTargetVertex(void);
			DIYLIB_API ~PmdMorphTargetVertex(void);

			unsigned int Index;
			PmdVector Position;

			DIYLIB_API void LoadFromFStream(std::ifstream& in);
		};
	}
}

#endif
