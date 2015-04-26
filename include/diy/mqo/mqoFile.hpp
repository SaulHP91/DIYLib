#ifndef MQOFILE_HPP
#define MQOFILE_HPP

#include <diy/mqo/mqoScene.hpp>

#include <fstream>
#include <vector>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mqo
	{
		class MqoMaterial;
		class MqoObject;
		class MqoFile
		{
		public:
			DIYLIB_API MqoFile(void);
			DIYLIB_API ~MqoFile(void);

			std::string Name;
			MqoScene Scene;
			std::vector<MqoMaterial> Materials;
			std::vector<MqoObject> Objects;

			DIYLIB_API void Clear(void);
			DIYLIB_API bool ParseFromFile(std::string path);
		};
	}
}

#endif
