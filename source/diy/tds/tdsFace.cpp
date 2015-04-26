#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsFace::TdsFace(void) :
			Vertex1(0), Vertex2(0), Vertex3(0),
			Flags(0)
		{
			;
		}

		TdsFace::~TdsFace(void)
		{
			;
		}

		bool TdsFace::LoadFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			Vertex1 = chunk.GetShort(in);
			Vertex2 = chunk.GetShort(in);
			Vertex3 = chunk.GetShort(in);
			Flags = chunk.GetShort(in);

			return true;
		}

		TdsFace::operator glm::ivec3(void)
		{
			return glm::ivec3(Vertex1, Vertex2, Vertex3);
		}
	}
}
