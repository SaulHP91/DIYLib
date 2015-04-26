#ifndef PMDTEXCOORD_HPP
#define PMDTEXCOORD_HPP

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
		class PmdTexCoord
		{
		public:
			DIYLIB_API PmdTexCoord(void);
			DIYLIB_API ~PmdTexCoord(void);

			float U, V;

			bool LoadFromFStream(std::ifstream& in);

			DIYLIB_API operator glm::vec2(void);
		};
	}
}

#endif
 