#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsChunks::TdsChunks(void) :
			OnChunk(0),
			OnString(0)
		{
			;
		}

		TdsChunks::~TdsChunks(void)
		{
			;
		}

		bool TdsChunks::LoadFirstChunk(std::ifstream& in)
		{
			Chunks.push(TdsChunk());
			Chunks.top().LoadFromFStream(in);
			Chunks.top().SetFirst();

			if (OnChunk)
			{
				OnChunk(Chunks.top(), 0);
			}

			return true;
		}

		bool TdsChunks::LoadNextChunk(std::ifstream& in)
		{
			while (Chunks.top().Finished())
			{
				if (Chunks.top().First)
				{
					return false;
				}

				int processed = Chunks.top().Processed;
				Chunks.pop();
				if (Chunks.empty())
				{
					break;
				}
				else
				{
					Chunks.top().Processed += processed;
				}
			}

			TdsChunk chunk;
			chunk.LoadFromFStream(in);

			if (in.eof())
			{
				return false;
			}

			if (OnChunk)
			{
				OnChunk(chunk, Chunks.top().User);
			}

			chunk.OnString = OnString;

			Chunks.push(chunk);

			return true;
		}

		TdsChunk& TdsChunks::GetChunk(void)
		{
			return Chunks.top();
		}
	}
}
