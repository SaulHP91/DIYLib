#ifndef PMDBONE_HPP
#define PMDBONE_HPP

#include <diy/pmd/pmdVector.hpp>

#include <fstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace pmd
	{
		class PmdBone
		{
		public:
			DIYLIB_API PmdBone(void);
			DIYLIB_API ~PmdBone(void);

			char Name[20];
			short ParentIndex;
			short ChildIndex;
			char Type;
			short TargetBone;
			PmdVector Translation;

			DIYLIB_API void LoadFromFStream(std::ifstream& in);
		};
	}
}

#endif
