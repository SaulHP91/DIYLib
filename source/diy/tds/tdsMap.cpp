#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsMap::TdsMap(void) :
			UScale(1.0f),
			VScale(1.0f),
			UOffset(0.0f),
			VOffset(0.0f),
			Tiling(0)
		{
			;
		}

		TdsMap::~TdsMap(void)
		{
			;
		}

		bool TdsMap::LoadFromChunksAndFStream(class TdsChunks& chunks, std::ifstream& in)
		{
			switch (chunks.GetChunk().Type)
			{
			case TDS_MAT_TEXMAP:
			case TDS_MAT_OPACMAP:
			{
				break;
			}
			default:
			{
				return false;
			}
			}

			chunks.GetChunk().SetFirst();

			while (chunks.LoadNextChunk(in))
			{
				switch (chunks.GetChunk().Type)
				{
				case TDS_INT_PERCENTAGE:
				case TDS_FLOAT_PERCENTAGE:
				{
					chunks.GetChunk().UnloadFromFStream(in);
					Percentage.LoadFromChunksAndFStream(chunks, in);
					continue;
				}
				case TDS_MAT_MAPNAME:
				{
					LoadNameFromChunkAndFStream(chunks.GetChunk(), in);
					break;
				}
				case TDS_MAT_MAP_USCALE:
				{
					LoadUScaleFromChunkAndFStream(chunks.GetChunk(), in);
					break;
				}
				case TDS_MAT_MAP_VSCALE:
				{
					LoadVScaleFromChunkAndFStream(chunks.GetChunk(), in);
					break;
				}
				case TDS_MAT_MAP_UOFFSET:
				{
					LoadUOffsetFromChunkAndFStream(chunks.GetChunk(), in);
					break;
				}
				case TDS_MAT_MAP_VOFFSET:
				{
					LoadVOffsetFromChunkAndFStream(chunks.GetChunk(), in);
					break;
				}
				case TDS_MAT_MAP_TILING:
				{
					LoadTilingFromChunkAndFStream(chunks.GetChunk(), in);
					break;
				}
				default:
				{
					chunks.GetChunk().Finish(in);
				}
				}
			}

			chunks.GetChunk().SetFirst();

			return true;
		}

		bool TdsMap::LoadNameFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_MAT_MAPNAME)
			{
				return false;
			}

			Name = chunk.GetString(in);

			if (chunk.OnString)
			{
				chunk.OnString(Name.c_str(), chunk.User);
			}

			return true;
		}

		bool TdsMap::LoadUScaleFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_MAT_MAP_USCALE)
			{
				return false;
			}

			UScale = chunk.GetFloat(in);

			if (chunk.OnString)
			{
				char buffer[8];
				sprintf(buffer, "%.1f", UScale);
				chunk.OnString(buffer, chunk.User);
			}

			return true;
		}

		bool TdsMap::LoadVScaleFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_MAT_MAP_VSCALE)
			{
				return false;
			}

			VScale = chunk.GetFloat(in);

			if (chunk.OnString)
			{
				char buffer[8];
				sprintf(buffer, "%.1f", VScale);
				chunk.OnString(buffer, chunk.User);
			}

			return true;
		}

		bool TdsMap::LoadUOffsetFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_MAT_MAP_UOFFSET)
			{
				return false;
			}

			UOffset = chunk.GetFloat(in);

			if (chunk.OnString)
			{
				char buffer[8];
				sprintf(buffer, "%.1f", UOffset);
				chunk.OnString(buffer, chunk.User);
			}

			return true;
		}

		bool TdsMap::LoadVOffsetFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_MAT_MAP_VOFFSET)
			{
				return false;
			}

			VOffset = chunk.GetFloat(in);

			if (chunk.OnString)
			{
				char buffer[8];
				sprintf(buffer, "%.1f", VOffset);
				chunk.OnString(buffer, chunk.User);
			}

			return true;
		}

		bool TdsMap::LoadTilingFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_MAT_MAP_TILING)
			{
				return false;
			}

			Tiling = chunk.GetShort(in);

			if (chunk.OnString)
			{
				char buffer[8];
				sprintf(buffer, "%#04X", Tiling);
				chunk.OnString(buffer, chunk.User);
			}

			return true;
		}
	}
}
