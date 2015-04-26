#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsColor::TdsColor(void) :
			R(0.0f), G(0.0f), B(0.0f)
		{
			;
		}

		TdsColor::~TdsColor(void)
		{
			;
		}

		bool TdsColor::LoadFromChunksAndFStream(TdsChunks& chunks, std::ifstream& in)
		{
			chunks.GetChunk().SetFirst();

			while (chunks.LoadNextChunk(in))
			{
				switch (chunks.GetChunk().Type)
				{
				case TDS_COLOR_F:
				{
					R = chunks.GetChunk().GetFloat(in);
					G = chunks.GetChunk().GetFloat(in);
					B = chunks.GetChunk().GetFloat(in);

					if (chunks.OnString)
					{
						char buffer[32];
						sprintf(buffer, "%.1f, %.1f, %.1f", R, G, B);
						chunks.OnString(buffer, chunks.GetChunk().User);
					}

					break;
				}
				case TDS_COLOR_24:
				{
					unsigned char r = chunks.GetChunk().GetChar(in);
					unsigned char g = chunks.GetChunk().GetChar(in);
					unsigned char b = chunks.GetChunk().GetChar(in);

					if (chunks.OnString)
					{
						char buffer[32];
						sprintf(buffer, "%i, %i, %i", r, g, b);
						chunks.OnString(buffer, chunks.GetChunk().User);
					}

					R = static_cast<float>(r) / 255.0f;
					G = static_cast<float>(g) / 255.0f;
					B = static_cast<float>(b) / 255.0f;
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

		TdsColor::operator glm::vec3(void)
		{
			return glm::vec3(R, G, B);
		}
	}
}
