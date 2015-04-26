#ifndef PMDVERTEX_HPP
#define PMDVERTEX_HPP

#include "pmdVector.hpp"
#include "pmdTexcoord.hpp"

#include <fstream>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace pmd
	{
		class PmdVertex
		{
		public:
			DIYLIB_API PmdVertex(void);

			PmdVector Position;
			PmdVector Normal;
			PmdTexCoord TexCoord;
			short BoneIndices[2];
			char Bone0Weight;
			char EdgeFlag;

			DIYLIB_API void LoadFromFStream(std::ifstream& in);
		};
	}
}

#endif
 