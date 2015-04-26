#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsPoint::TdsPoint(void) :
			X(0.0f), Y(0.0f), Z(0.0f)
		{
			;
		}

		TdsPoint::~TdsPoint(void)
		{
			;
		}

		bool TdsPoint::LoadFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			X = chunk.GetFloat(in);
			Y = chunk.GetFloat(in);
			Z = chunk.GetFloat(in);

			return true;
		}

		TdsPoint::operator glm::vec3(void)
		{
			return glm::vec3(X, Y, Z);
		}
	}
}
