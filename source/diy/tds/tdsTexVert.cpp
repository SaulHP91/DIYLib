#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsTexVert::TdsTexVert(void) :
			X(0.0f), Y(0.0f)
		{
			;
		}

		TdsTexVert::~TdsTexVert(void)
		{
			;
		}

		bool TdsTexVert::LoadFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			X = chunk.GetFloat(in);
			Y = chunk.GetFloat(in);

			return true;
		}

		TdsTexVert::operator glm::vec2(void)
		{
			return glm::vec2(X, Y);
		}
	}
}
