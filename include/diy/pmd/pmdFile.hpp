#ifndef PMDFILE_HPP
#define PMDFILE_HPP

#include "pmdVertex.hpp"
#include "pmdFace.hpp"
#include "pmdMaterial.hpp"
#include "pmdBone.hpp"
#include "pmdIKChain.hpp"
#include "pmdMorphTarget.hpp"

#include <vector>
#include <string>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace pmd
	{
		class PmdVertex;
		class PmdFace;
		class PmdMaterial;
		class PmdBone;
		class PmdIKChain;
		class PmdMorphTarget;
		class PmdFile
		{
		public:
			DIYLIB_API PmdFile(void);
			DIYLIB_API ~PmdFile(void);

			char Name[20];
			char Comment[256];
			std::vector<PmdVertex> Vertices;
			std::vector<PmdFace> Faces;
			std::vector<PmdMaterial> Materials;
			std::vector<PmdBone> Bones;
			std::vector<PmdIKChain> IKChains;
			std::vector<PmdMorphTarget> MorphTargets;

			DIYLIB_API void Clear(void);
			DIYLIB_API bool LoadFromFile(std::string path);
		};
	}
}

#endif
