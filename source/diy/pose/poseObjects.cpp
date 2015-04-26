#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace pose
	{
		PoseObjects::PoseObjects(PosePose* pose) :
			mPose(pose)
		{
			;
		}

		PoseObjects::~PoseObjects(void)
		{
			Clear();
		}

		PosePose* PoseObjects::Pose(void)
		{
			return mPose;
		}

		PoseObject* PoseObjects::New(std::string name)
		{
			if (!Find(name))
			{
				mObjects.push_back(new PoseObject(this, name));
				return mObjects.back();
			}

			return 0;
		}

		void PoseObjects::Delete(PoseObject* object)
		{
			std::vector<PoseObject*>::iterator iter = find(mObjects.begin(), mObjects.end(), object);
			if (iter == mObjects.end())
			{
				return;
			}

			mObjects.erase(iter);
		}

		void PoseObjects::Delete(std::string name)
		{
			Delete(Find(name));
		}

		void PoseObjects::Clear(void)
		{
			while (mObjects.size())
			{
				delete mObjects.back();
				mObjects.pop_back();
			}
			mObjects.swap(std::vector<PoseObject*>(mObjects));

			mObject = mObjects.end();
		}

		PoseObject* PoseObjects::Find(std::string name, bool create)
		{
			for (std::vector<PoseObject*>::iterator object_iterator = mObjects.begin(); object_iterator != mObjects.end(); ++object_iterator)
			{
				PoseObject* object = *object_iterator;
				if (!_stricmp(object->mName.c_str(), name.c_str()))
				{
					return object;
				}
			}

			if (create)
			{
				return New(name);
			}

			return 0;
		}

		PoseObject* PoseObjects::operator()(void)
		{
			if (mObject == mObjects.end())
			{
				return 0;
			}

			return *mObject;
		}

		PoseObject* PoseObjects::First(void)
		{
			if (mObjects.size())
			{
				mObject = mObjects.begin();
			}
			else
			{
				mObject = mObjects.end();
			}

			return operator()();
		}

		PoseObject* PoseObjects::Next(void)
		{
			if (mObject != mObjects.end())
			{
				++mObject;
			}

			return operator()();
		}

		PoseObject* PoseObjects::Last(void)
		{
			if (mObjects.size())
			{
				mObject = mObjects.end() - 1;
			}
			else
			{
				mObject = mObjects.end();
			}

			return operator()();
		}

		PoseObject* PoseObjects::Previous(void)
		{
			if (mObject == mObjects.begin())
			{
				mObject = mObjects.end();
			}
			else
			{
				--mObject;
			}

			return operator()();
		}

		int PoseObjects::Count(void)
		{
			return mObjects.size();
		}

		bool PoseObjects::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			for (int i = 0; i < count; ++i)
			{
				int length;
				in.read(reinterpret_cast<char*>(&length), 4);

				char* name = new char[length];
				in.read(name, length);

				int visible;
				in.read(reinterpret_cast<char*>(&visible), 4);

				PoseObject* object = Find(name, true);
				delete[] name;

				object->mVisible = visible;

				object->mMorphTargets->LoadFromFStream(in);
			}

			return true;
		}

		void PoseObjects::SaveToFStream(std::ofstream& out)
		{
			int count = mObjects.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<PoseObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				int length = strlen((*object)->mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*object)->mName.c_str(), length);

				int visible = (*object)->mVisible;
				out.write(reinterpret_cast<char*>(&visible), 4);

				(*object)->mMorphTargets->SaveToFStream(out);
			}
		}
	}
}
