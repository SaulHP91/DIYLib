#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsPercentage::TdsPercentage(void) :
			Percentage(0.0f)
		{
			;
		}

		TdsPercentage::~TdsPercentage(void)
		{
			;
		}

		bool TdsPercentage::LoadFromChunksAndFStream(TdsChunks& chunks, std::ifstream& in)
		{
			chunks.GetChunk().SetFirst();

			while (chunks.LoadNextChunk(in))
			{
				switch (chunks.GetChunk().Type)
				{
				case TDS_INT_PERCENTAGE:
				{
					unsigned short percentage = chunks.GetChunk().GetShort(in);

					if (chunks.OnString)
					{
						char buffer[8];
						sprintf(buffer, "%i%%", percentage);
						chunks.OnString(buffer, chunks.GetChunk().User);
					}

					Percentage = static_cast<float>(percentage);
					break;
				}
				case TDS_FLOAT_PERCENTAGE:
				{
					Percentage = chunks.GetChunk().GetFloat(in);

					if (chunks.OnString)
					{
						char buffer[8];
						sprintf(buffer, "%.1f%%", Percentage);
						chunks.OnString(buffer, chunks.GetChunk().User);
					}

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

		TdsPercentage::operator float(void)
		{
			return Percentage;
		}
	}
}
