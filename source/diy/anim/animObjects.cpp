#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace anim
	{
		AnimObjects::AnimObjects(AnimKeyFrame* key_frame) :
			mKeyFrame(key_frame)
		{
			;
		}

		AnimObjects::~AnimObjects(void)
		{
			Clear();
		}

		AnimKeyFrame* AnimObjects::GetKeyFrame(void)
		{
			return mKeyFrame;
		}

		AnimObject* AnimObjects::New(std::string name)
		{
			if (!Find(name))
			{
				mObjects.push_back(new AnimObject(this, name));
				return mObjects.back();
			}

			return 0;
		}

		void AnimObjects::Delete(AnimObject* object)
		{
			std::vector<AnimObject*>::iterator iter = find(mObjects.begin(), mObjects.end(), object);
			if (iter == mObjects.end())
			{
				return;
			}

			mObjects.erase(iter);
		}

		void AnimObjects::Delete(std::string name)
		{
			Delete(Find(name));
		}

		void AnimObjects::Clear(void)
		{
			while (mObjects.size())
			{
				delete mObjects.back();
				mObjects.pop_back();
			}
			mObjects.swap(std::vector<AnimObject*>(mObjects));

			mObject = mObjects.end();
		}

		AnimObject* AnimObjects::Find(std::string name, bool create)
		{
			for (std::vector<AnimObject*>::iterator object_iterator = mObjects.begin(); object_iterator != mObjects.end(); ++object_iterator)
			{
				AnimObject* object = *object_iterator;
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

		AnimObject* AnimObjects::operator()(void)
		{
			if (mObject == mObjects.end())
			{
				return 0;
			}

			return *mObject;
		}

		AnimObject* AnimObjects::First(void)
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

		AnimObject* AnimObjects::Next(void)
		{
			if (mObject != mObjects.end())
			{
				++mObject;
			}

			return operator()();
		}

		AnimObject* AnimObjects::Last(void)
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

		AnimObject* AnimObjects::Previous(void)
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

		int AnimObjects::GetCount(void)
		{
			return mObjects.size();
		}

		bool AnimObjects::LoadFromFStream(std::ifstream& in)
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

				AnimObject* object = Find(name, true);
				delete[] name;

				object->mVisible = visible;

				object->mMorphTargets->LoadFromFStream(in);
			}

			return true;
		}

		void AnimObjects::SaveToFStream(std::ofstream& out)
		{
			int count = mObjects.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<AnimObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
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
