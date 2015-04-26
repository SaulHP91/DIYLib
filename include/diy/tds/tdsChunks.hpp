#ifndef TDSCHUNKS_HPP
#define TDSCHUNKS_HPP

#include <stack>
#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace tds
	{
		class TdsChunk;
		class TdsChunks
		{
		public:
			DIYLIB_API TdsChunks(void);
			DIYLIB_API ~TdsChunks(void);

			std::stack<TdsChunk> Chunks;

			DIYLIB_API bool LoadFirstChunk(std::ifstream& in);
			DIYLIB_API bool LoadNextChunk(std::ifstream& in);

			DIYLIB_API TdsChunk& GetChunk(void);

			void(*OnChunk)(TdsChunk& chunk, void* parentUser);
			void(*OnString)(const char* str, void* user);
		};
	}
}

#endif
