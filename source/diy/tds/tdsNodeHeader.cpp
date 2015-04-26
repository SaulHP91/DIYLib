#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsNodeHeader::TdsNodeHeader(void) :
			Hierarchy(-1)
		{
			;
		}

		TdsNodeHeader::~TdsNodeHeader(void)
		{
			;
		}

		bool TdsNodeHeader::LoadFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_NODE_HDR)
			{
				return false;
			}

			Name = chunk.GetString(in);

			unsigned short unknown1 = chunk.GetShort(in);

			unsigned short unknown2 = chunk.GetShort(in);

			Hierarchy = chunk.GetShort(in);

			if (chunk.OnString)
			{
				char buffer[64];
				sprintf(buffer, "%s, %#04X?, %#04X?, %i", Name.c_str(), unknown1, unknown2, Hierarchy);
				chunk.OnString(buffer, chunk.User);
			}

			return true;
		}
	}
}
