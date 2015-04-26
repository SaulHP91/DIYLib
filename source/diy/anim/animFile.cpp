#include <diy/diy.hpp>

namespace diy
{
	namespace anim
	{
		AnimFile::AnimFile(void)
		{
			mAnimations = new AnimAnimations(this);
		}

		AnimFile::~AnimFile(void)
		{
			delete mAnimations;
		}

		AnimAnimations* AnimFile::GetAnimations(void)
		{
			return mAnimations;
		}

		void AnimFile::Clear(void)
		{
			mAnimations->Clear();
		}

		bool AnimFile::LoadFromFile(std::string path)
		{
			std::ifstream in(path.c_str(), std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				char header[5];
				in.read(header, 4);
				header[4] = 0;
				if (_stricmp(header, ANIM_HEADER))
				{
					return false;
				}

				float version;
				in.read(reinterpret_cast<char*>(&version), 4);
				if (version != ANIM_VERSION)
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

				mAnimations->LoadFromFStream(in);

				in.close();

				return true;
			}

			return false;
		}

		void AnimFile::SaveToFile(std::string path)
		{
			std::ofstream out(path.c_str(), std::ios::out | std::ios::binary);
			if (out.is_open())
			{
				char header[] = ANIM_HEADER;
				out.write(header, 4);

				float version = ANIM_VERSION;
				out.write(reinterpret_cast<char*>(&version), 4);

				int length = strlen(Name.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write(Name.c_str(), length);

				int commentLength = strlen(Comment.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&commentLength), 4);
				out.write(Comment.c_str(), commentLength);

				mAnimations->SaveToFStream(out);

				out.close();
			}
		}
	}
}
 