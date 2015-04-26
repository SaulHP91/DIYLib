#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsObjectNode::TdsObjectNode(void) :
			ID(-1)
		{
			;
		}

		TdsObjectNode::~TdsObjectNode(void)
		{
			;
		}

		bool TdsObjectNode::LoadIDFromFStreamAndChunk(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_NODE_ID)
			{
				return false;
			}

			ID = chunk.GetShort(in);

			if (chunk.OnString)
			{
				char buffer[8];
				sprintf(buffer, "%i", ID);
				chunk.OnString(buffer, chunk.User);
			}

			return true;
		}

		bool TdsObjectNode::LoadPivotFromFStreamAndChunk(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_PIVOT)
			{
				return false;
			}

			Pivot.LoadFromChunkAndFStream(chunk, in);

			if (chunk.OnString)
			{
				char buffer[32];
				sprintf(buffer, "%.1f, %.1f, %.1f", Pivot.X, Pivot.Y, Pivot.Z);
				chunk.OnString(buffer, chunk.User);
			}

			return true;
		}
	}
}
