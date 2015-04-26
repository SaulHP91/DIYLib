#ifndef TDSCHUNK_HPP
#define TDSCHUNK_HPP

#include <fstream>
#include <string>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace tds
	{
		class TdsChunk
		{
		public:
			DIYLIB_API TdsChunk(void);
			DIYLIB_API ~TdsChunk(void);

			unsigned short Type;
			unsigned int Size;
			unsigned int Processed;

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API bool UnloadFromFStream(std::ifstream& in);

			DIYLIB_API void Ignore(std::ifstream& in, int count = 1);
			DIYLIB_API unsigned char GetChar(std::ifstream& in);
			DIYLIB_API unsigned short GetShort(std::ifstream& in);
			DIYLIB_API unsigned int GetInt(std::ifstream& in);
			DIYLIB_API float GetFloat(std::ifstream& in);
			DIYLIB_API std::string GetString(std::ifstream& in);
			DIYLIB_API TdsChunk GetSubChunk(std::ifstream& in);

			DIYLIB_API void Finish(std::ifstream& in);
			DIYLIB_API void Finish(TdsChunk& subChunk, std::ifstream& in);
			DIYLIB_API bool Finished(void);

			void* User;

			DIYLIB_API std::string ToString(void);

			bool First;
			DIYLIB_API void SetFirst(bool first = true);

			void(*OnString)(const char* str, void* user);
		};
	}
}

#endif
