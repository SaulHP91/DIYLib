#ifndef TDSMATERIAL_HPP
#define TDSMATERIAL_HPP

#include <string>
#include <fstream>

#include <glm/glm.hpp>

#include "tdsColor.hpp"
#include "tdsPercentage.hpp"
#include "tdsMap.hpp"

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace tds
	{
		class TdsChunks;
		class TdsChunk;
		class TdsMaterial
		{
		public:
			DIYLIB_API TdsMaterial(void);
			DIYLIB_API ~TdsMaterial(void);

			std::string Name;
			TdsColor Ambient;
			TdsColor Diffuse;
			TdsPercentage Transparency;
			TdsColor Specular;
			TdsPercentage Shininess;
			TdsMap TexMap;
			TdsMap OpacMap;

			DIYLIB_API bool LoadFromChunksAndFStream(TdsChunks& chunks, std::ifstream& in);
			DIYLIB_API bool LoadNameFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);
			DIYLIB_API bool LoadTransparencyFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);
			DIYLIB_API bool LoadShininessFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in);
		};
	}
}

#endif
 