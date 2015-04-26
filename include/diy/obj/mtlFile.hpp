#ifndef MTLFILE_HPP
#define MTLFILE_HPP

#include <string>
#include <vector>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace obj
	{		
		class MtlMaterial;
		class MtlFile
		{
		public:
			DIYLIB_API MtlFile(void);
			DIYLIB_API ~MtlFile(void);

			std::string Name;
			std::vector<MtlMaterial> Materials;

			DIYLIB_API void Clear(void);
			DIYLIB_API bool ParseFromFile(std::string path);
			DIYLIB_API std::vector<MtlMaterial>::iterator SearchByName(std::string materialName);
			DIYLIB_API int IndexOf(std::string materialName);
		};
	}
}

#endif
