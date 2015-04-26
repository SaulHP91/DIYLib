#ifndef PMDVECTOR_HPP
#define PMDVECTOR_HPP

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
		class PmdVector
		{
		public:
			DIYLIB_API PmdVector(void);
			DIYLIB_API ~PmdVector(void);

			float X, Y, Z;

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);

			DIYLIB_API operator glm::vec3(void);
		};
	}
}

#endif
 