#ifndef PMDMATERIAL_HPP
#define PMDMATERIAL_HPP

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
		class PmdMaterial
		{
		public:
			DIYLIB_API PmdMaterial(void);
			DIYLIB_API ~PmdMaterial(void);

			glm::vec3 Diffuse;
			float Alpha;
			float Shininess;
			glm::vec3 Specular;
			glm::vec3 Ambient;
			char ToonNumber;
			char EdgeFlag;
			long IndexCount;
			char Texture[20];

			DIYLIB_API void LoadFromFStream(std::ifstream& in);
		};
	}
}

#endif
 