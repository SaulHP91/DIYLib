#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsMshMatGroup::TdsMshMatGroup(void)
		{
			;
		}

		TdsMshMatGroup::~TdsMshMatGroup(void)
		{
			Clear();
		}

		void TdsMshMatGroup::Clear(void)
		{
			MaterialName.clear();

			FaceNum.clear();
			FaceNum.swap(std::vector<unsigned short>(FaceNum));
		}

		bool TdsMshMatGroup::LoadFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			MaterialName = chunk.GetString(in);

			unsigned short nFaces = chunk.GetShort(in);

			FaceNum.resize(static_cast<unsigned int>(nFaces));
			for (std::vector<unsigned short>::iterator face = FaceNum.begin(); face != FaceNum.end(); ++face)
			{
				*face = chunk.GetShort(in);
			}

			if (chunk.OnString)
			{
				char buffer[32];
				sprintf(buffer, "%s, %i", MaterialName.c_str(), FaceNum.size());
				chunk.OnString(buffer, chunk.User);
			}

			return true;
		}
	}
}
