#include <diy/diy.hpp>

namespace diy
{
	namespace pose
	{
		PoseFile::PoseFile(void)
		{
			mPoses = new PosePoses(this);
		}

		PoseFile::~PoseFile(void)
		{
			delete mPoses;
		}

		PosePoses* PoseFile::Poses(void)
		{
			return mPoses;
		}

		void PoseFile::Clear(void)
		{
			mPoses->Clear();
		}

		bool PoseFile::LoadFromFile(std::string path)
		{
			std::ifstream in(path.c_str(), std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				char header[5];
				in.read(header, 4);
				header[4] = 0;
				if (_stricmp(header, POSE_HEADER))
				{
					return false;
				}

				float version;
				in.read(reinterpret_cast<char*>(&version), 4);
				if (version != POSE_VERSION)
				{
					return false;
				}

				int length;
				in.read(reinterpret_cast<char*>(&length), 4);

				char* name = new char[length];
				in.read(name, length);
				Name = name;
				delete[] name;

				int commentLength;
				in.read(reinterpret_cast<char*>(&commentLength), 4);

				char* comment = new char[commentLength];
				in.read(comment, commentLength);
				Comment = comment;
				delete[] comment;

				mPoses->LoadFromFStream(in);

				in.close();

				return true;
			}

			return false;
		}

		void PoseFile::SaveToFile(std::string path)
		{
			std::ofstream out(path.c_str(), std::ios::out | std::ios::binary);
			if (out.is_open())
			{
				char header[] = POSE_HEADER;
				out.write(header, 4);

				float version = POSE_VERSION;
				out.write(reinterpret_cast<char*>(&version), 4);

				int length = strlen(Name.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write(Name.c_str(), length);

				int commentLength = strlen(Comment.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&commentLength), 4);
				out.write(Comment.c_str(), commentLength);

				mPoses->SaveToFStream(out);

				out.close();
			}
		}
	}
}
