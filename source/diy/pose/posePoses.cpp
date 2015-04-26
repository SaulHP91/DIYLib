#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace pose
	{
		PosePoses::PosePoses(PoseFile* file) :
			mFile(file)
		{
			;
		}

		PosePoses::~PosePoses(void)
		{
			Clear();
		}

		PoseFile* PosePoses::File(void)
		{
			return mFile;
		}

		PosePose* PosePoses::New(void)
		{
			int index = 0;
			char name[256];

			PosePose* new_pose;
			do
			{
				sprintf(name, "Pose%i", index++);
				new_pose = New(name);
			} while (!new_pose);

			return new_pose;
		}

		PosePose* PosePoses::New(std::string name)
		{
			if (!Find(name))
			{
				mPoses.push_back(new PosePose(this, name, mPoses.size()));
				return mPoses.back();
			}

			return 0;
		}

		void PosePoses::Delete(PosePose* pose)
		{
			std::vector<PosePose*>::iterator iter = find(mPoses.begin(), mPoses.end(), pose);
			if (iter == mPoses.end())
			{
				return;
			}

			mPoses.erase(iter);
		}

		void PosePoses::Delete(int index)
		{
			Delete(operator[](index));
		}


		void PosePoses::Delete(std::string name)
		{
			Delete(Find(name));
		}

		void PosePoses::Clear(void)
		{
			while (mPoses.size())
			{
				delete mPoses.back();
				mPoses.pop_back();
			}
			mPoses.swap(std::vector<PosePose*>(mPoses));

			mPose = mPoses.end();
		}

		PosePose* PosePoses::Find(std::string name, bool create)
		{
			for (std::vector<PosePose*>::iterator pose_iterator = mPoses.begin(); pose_iterator != mPoses.end(); ++pose_iterator)
			{
				PosePose* pose = *pose_iterator;
				if (!_stricmp(pose->mName.c_str(), name.c_str()))
				{
					return pose;
				}
			}

			if (create)
			{
				return New(name);
			}

			return 0;
		}

		PosePose* PosePoses::operator[](int index)
		{
			if (index < 0 || index >= static_cast<int>(mPoses.size()))
			{
				return 0;
			}

			return mPoses[index];
		}

		PosePose* PosePoses::operator()(void)
		{
			if (mPose == mPoses.end())
			{
				return 0;
			}

			return *mPose;
		}

		PosePose* PosePoses::First(void)
		{
			if (mPoses.size())
			{
				mPose = mPoses.begin();
			}
			else
			{
				mPose = mPoses.end();
			}

			return operator()();
		}

		PosePose* PosePoses::Next(void)
		{
			if (mPose != mPoses.end())
			{
				++mPose;
			}

			return operator()();
		}

		PosePose* PosePoses::Last(void)
		{
			if (mPoses.size())
			{
				mPose = mPoses.end() - 1;
			}
			else
			{
				mPose = mPoses.end();
			}

			return operator()();
		}

		PosePose* PosePoses::Previous(void)
		{
			if (mPose == mPoses.begin())
			{
				mPose = mPoses.end();
			}
			else
			{
				--mPose;
			}

			return operator()();
		}

		int PosePoses::Count(void)
		{
			return mPoses.size();
		}

		bool PosePoses::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			for (int i = 0; i < count; ++i)
			{
				int length;
				in.read(reinterpret_cast<char*>(&length), 4);

				char* name = new char[length];
				in.read(name, length);

				PosePose* pose = Find(name, true);
				delete[] name;

				in.read(reinterpret_cast<char*>(&length), 4);

				char* comment = new char[length];
				in.read(comment, length);

				pose->Comment = comment;
				delete[] comment;

				pose->mNodes->LoadFromFStream(in);
				pose->mObjects->LoadFromFStream(in);
			}

			return true;
		}

		void PosePoses::SaveToFStream(std::ofstream& out)
		{
			int count = mPoses.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<PosePose*>::iterator pose = mPoses.begin(); pose != mPoses.end(); ++pose)
			{
				int length = strlen((*pose)->mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*pose)->mName.c_str(), length);

				length = strlen((*pose)->Comment.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*pose)->Comment.c_str(), length);

				(*pose)->mNodes->SaveToFStream(out);
				(*pose)->mObjects->SaveToFStream(out);
			}
		}
	}
}
