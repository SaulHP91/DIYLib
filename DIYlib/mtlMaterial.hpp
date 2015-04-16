#ifndef MTLMATERIAL_HPP
#define MTLMATERIAL_HPP

#include "mtlColorRGB.hpp"

#include <string>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	class MtlMaterial
	{
	public:
		DIYLIB_API MtlMaterial(void);
		DIYLIB_API ~MtlMaterial(void);

		std::string Name;
		MtlColorRGB Diffuse;
		float Dissolve;
		MtlColorRGB Ambient;
		MtlColorRGB Specular;
		float SpecularExponent;
		std::string DiffuseMap;
		std::string DissolveMap;
		std::string AmbientMap;
		std::string SpecularMap;
		std::string BumpMap;

		DIYLIB_API bool operator==(MtlMaterial mtlMaterial);
	};

}

#endif
