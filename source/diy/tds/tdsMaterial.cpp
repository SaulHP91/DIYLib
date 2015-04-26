#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsMaterial::TdsMaterial(void)
		{
			;
		}

		TdsMaterial::~TdsMaterial(void)
		{
			;
		}

		bool TdsMaterial::LoadFromChunksAndFStream(class TdsChunks& chunks, std::ifstream& in)
		{
			if (chunks.GetChunk().Type != TDS_MAT_ENTRY)
			{
				return false;
			}

			chunks.GetChunk().SetFirst();

			while (chunks.LoadNextChunk(in))
			{
				switch (chunks.GetChunk().Type)
				{
				case TDS_MAT_NAME:
				{
					LoadNameFromChunkAndFStream(chunks.GetChunk(), in);
					break;
				}
				case TDS_MAT_AMBIENT:
				{
					Ambient.LoadFromChunksAndFStream(chunks, in);
					break;
				}
				case TDS_MAT_DIFFUSE:
				{
					Diffuse.LoadFromChunksAndFStream(chunks, in);
					break;
				}
				case TDS_MAT_TRANSPARENCY:
				{
					Transparency.LoadFromChunksAndFStream(chunks, in);
					break;
				}
				case TDS_MAT_SPECULAR:
				{
					Specular.LoadFromChunksAndFStream(chunks, in);
					break;
				}
				case TDS_MAT_SHININESS:
				{
					Shininess.LoadFromChunksAndFStream(chunks, in);
					break;
				}
				case TDS_MAT_TEXMAP:
				{
					TexMap.LoadFromChunksAndFStream(chunks, in);
					break;
				}
				case TDS_MAT_OPACMAP:
				{
					OpacMap.LoadFromChunksAndFStream(chunks, in);
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

		bool TdsMaterial::LoadNameFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_MAT_NAME)
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
	}
}
