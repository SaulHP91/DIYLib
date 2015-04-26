#ifndef SMDVERTEX_HPP
#define SMDVERTEX_HPP

#include <diy/smd/smdVector.hpp>
#include <diy/smd/smdTexCoord.hpp>

#include <vector>
#include <sstream>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace smd
	{
		class SmdSkinWeight;
		class SmdVertex
		{
		public:
			DIYLIB_API SmdVertex(void);
			DIYLIB_API ~SmdVertex(void);

			int BoneIndex;
			SmdVector Position;
			SmdVector Normal;
			SmdTexCoord TexCoord;
			std::vector<SmdSkinWeight> SkinWeights;

			DIYLIB_API bool ParseFromSStream(std::istringstream& in);
		};
	}
}

#endif