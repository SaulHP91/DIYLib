#ifndef MQOMATERIAL_HPP
#define MQOMATERIAL_HPP

#include <diy/mqo/mqoString.hpp>
#include <diy/mqo/mqoVector.hpp>

#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mqo
	{
		class MqoMaterial
		{
		public:
			DIYLIB_API MqoMaterial(void);
			DIYLIB_API ~MqoMaterial(void);

			MqoString Name;
			int Shader;
			MqoVector Color;
			float Alpha;
			float Diffuse;
			float Emission;
			float Specular;
			float Power;
			float Ambient;
			MqoString Texture;
			MqoString AlphaPlane;
			MqoString Bump;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
 