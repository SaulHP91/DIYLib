#ifndef TDSFILE_HPP
#define TDSFILE_HPP

#include <string>
#include <vector>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace tds
	{
		class TdsMaterial;
		class TdsObject;
		class TdsObjectNode;
		class TdsChunk;
		class TdsFile
		{
		public:
			DIYLIB_API TdsFile(void);
			DIYLIB_API ~TdsFile(void);

			DIYLIB_API void Clear(void);

			std::string Name;
			std::vector<TdsMaterial> Materials;
			std::vector<TdsObject> Objects;
			std::vector<TdsObjectNode> ObjectNodes;

			DIYLIB_API bool LoadFromFile(std::string file);

			void(*OnChunk)(TdsChunk& chunk, void* parentUser);
			void(*OnString)(const char* str, void* user);
		};
	}
}

#endif
 