#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace anim
	{
		AnimMorphTargets::AnimMorphTargets(AnimObject* object) :
			mObject(object)
		{
			;
		}

		AnimMorphTargets::~AnimMorphTargets(void)
		{
			Clear();
		}

		AnimObject* AnimMorphTargets::GetObject(void)
		{
			return mObject;
		}

		AnimMorphTarget* AnimMorphTargets::New(std::string name)
		{
			if (!Find(name))
			{
				mMorphTargets.push_back(new AnimMorphTarget(this, name));
				return mMorphTargets.back();
			}

			return 0;
		}

		void AnimMorphTargets::Delete(AnimMorphTarget* morph_target)
		{
			std::vector<AnimMorphTarget*>::iterator iter = find(mMorphTargets.begin(), mMorphTargets.end(), morph_target);
			if (iter == mMorphTargets.end())
			{
				return;
			}

			mMorphTargets.erase(iter);
		}

		void AnimMorphTargets::Delete(std::string name)
		{
			Delete(Find(name));
		}

		void AnimMorphTargets::Clear(void)
		{
			while (mMorphTargets.size())
			{
				delete mMorphTargets.back();
				mMorphTargets.pop_back();
			}
			mMorphTargets.swap(std::vector<AnimMorphTarget*>(mMorphTargets));

			mMorphTarget = mMorphTargets.end();
		}

		AnimMorphTarget* AnimMorphTargets::Find(std::string name, bool create)
		{
			for (std::vector<AnimMorphTarget*>::iterator morph_target_iterator = mMorphTargets.begin(); morph_target_iterator != mMorphTargets.end(); ++morph_target_iterator)
			{
				AnimMorphTarget* morph_target = *morph_target_iterator;
				if (!_stricmp(morph_target->mName.c_str(), name.c_str()))
				{
					return morph_target;
				}
			}

			if (create)
			{
				return New(name);
			}

			return 0;
		}

		AnimMorphTarget* AnimMorphTargets::operator()(void)
		{
			if (mMorphTarget == mMorphTargets.end())
			{
				return 0;
			}

			return *mMorphTarget;
		}

		AnimMorphTarget* AnimMorphTargets::First(void)
		{
			if (mMorphTargets.size())
			{
				mMorphTarget = mMorphTargets.begin();
			}
			else
			{
				mMorphTarget = mMorphTargets.end();
			}

			return operator()();
		}

		AnimMorphTarget* AnimMorphTargets::Next(void)
		{
			if (mMorphTarget != mMorphTargets.end())
			{
				++mMorphTarget;
			}

			return operator()();
		}

		AnimMorphTarget* AnimMorphTargets::Last(void)
		{
			if (mMorphTargets.size())
			{
				mMorphTarget = mMorphTargets.end() - 1;
			}
			else
			{
				mMorphTarget = mMorphTargets.end();
			}

			return operator()();
		}

		AnimMorphTarget* AnimMorphTargets::Previous(void)
		{
			if (mMorphTarget == mMorphTargets.begin())
			{
				mMorphTarget = mMorphTargets.end();
			}
			else
			{
				--mMorphTarget;
			}

			return operator()();
		}

		int AnimMorphTargets::GetCount(void)
		{
			return mMorphTargets.size();
		}

		bool AnimMorphTargets::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			for (int i = 0; i < count; ++i)
			{
				int length;
				in.read(reinterpret_cast<char*>(&length), 4);

				char* name = new char[length];
				in.read(name, length);

				AnimMorphTarget* morph_target = Find(name, true);
				delete[] name;

				in.read(reinterpret_cast<char*>(&morph_target->mBlendFactor), 4);
			}

			return true;
		}

		void AnimMorphTargets::SaveToFStream(std::ofstream& out)
		{
			int count = mMorphTargets.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<AnimMorphTarget*>::iterator morph_target = mMorphTargets.begin(); morph_target != mMorphTargets.end(); ++morph_target)
			{
				int length = strlen((*morph_target)->mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*morph_target)->mName.c_str(), length);

				out.write(reinterpret_cast<char*>(&(*morph_target)->mBlendFactor), 4);
			}
		}
	}
}
