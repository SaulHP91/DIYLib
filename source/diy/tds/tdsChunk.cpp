#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsChunk::TdsChunk(void) :
			Type(TDS_NULL_CHUNK),
			Size(0),
			Processed(0),
			User(0),
			First(false),
			OnString(0)
		{
			;
		}

		TdsChunk::~TdsChunk(void)
		{
			;
		}

		bool TdsChunk::LoadFromFStream(std::ifstream& in)
		{
			in.read(reinterpret_cast<char*>(&Type), 2);

			in.read(reinterpret_cast<char*>(&Size), 4);

			Processed = 6;

			return true;
		}

		bool TdsChunk::UnloadFromFStream(std::ifstream& in)
		{
			in.unget();
			in.unget();

			in.unget();
			in.unget();
			in.unget();
			in.unget();

			Processed = 0;

			return true;
		}

		void TdsChunk::Ignore(std::ifstream& in, int count)
		{
			if (Processed + count > Size)
			{
				count = Size - Processed;
			}

			in.ignore(count);

			Processed += count;
		}

		unsigned char TdsChunk::GetChar(std::ifstream& in)
		{
			if (Finished())
			{
				return 0;
			}

			unsigned char value;
			in.read(reinterpret_cast<char*>(&value), 1);

			Processed++;

			return value;
		}

		unsigned short TdsChunk::GetShort(std::ifstream& in)
		{
			if (Finished())
			{
				return 0;
			}

			unsigned short value;
			in.read(reinterpret_cast<char*>(&value), 2);

			Processed += 2;

			return value;
		}

		unsigned int TdsChunk::GetInt(std::ifstream& in)
		{
			if (Finished())
			{
				return 0;
			}

			unsigned int value;
			in.read(reinterpret_cast<char*>(&value), 4);

			Processed += 4;

			return value;
		}

		float TdsChunk::GetFloat(std::ifstream& in)
		{
			if (Finished())
			{
				return 0.0f;
			}

			float value;
			in.read(reinterpret_cast<char*>(&value), 4);

			Processed += 4;

			return value;
		}

		std::string TdsChunk::GetString(std::ifstream& in)
		{
			if (Finished())
			{
				return std::string();
			}

			std::string value;
			char letter;
			do
			{
				letter = GetChar(in);
				value += letter;
			} while (letter);

			return value;
		}

		TdsChunk TdsChunk::GetSubChunk(std::ifstream& in)
		{
			if (Finished())
			{
				return TdsChunk();
			}

			TdsChunk value;
			value.LoadFromFStream(in);

			Processed += 6;

			return value;
		}

		void TdsChunk::Finish(std::ifstream& in)
		{
			in.ignore(Size - Processed);

			Processed = Size;
		}

		void TdsChunk::Finish(TdsChunk& subChunk, std::ifstream& in)
		{
			Processed += subChunk.Size - subChunk.Processed;

			subChunk.Finish(in);
		}

		bool TdsChunk::Finished(void)
		{
			return Processed == Size;
		}

		std::string TdsChunk::ToString(void)
		{
			char buffer[256];
			sprintf(buffer, "%s(%#04X), Size=%i", TdsChunkTypeToString(Type), Type, Size);

			return std::string(buffer);
		}

		void TdsChunk::SetFirst(bool first)
		{
			First = first;
		}
	}
}
