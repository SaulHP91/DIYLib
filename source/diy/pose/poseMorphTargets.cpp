#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace pose
	{
		PoseMorphTargets::PoseMorphTargets(PoseObject* object) :
			mObject(object)
		{
			;
		}

		PoseMorphTargets::~PoseMorphTargets(void)
		{
			Clear();
		}

		PoseObject* PoseMorphTargets::Object(void)
		{
			return mObject;
		}

		PoseMorphTarget* PoseMorphTargets::New(std::string name)
		{
			if (!Find(name))
			{
				mMorphTargets.push_back(new PoseMorphTarget(this, name));
				return mMorphTargets.back();
			}

			return 0;
		}

		void PoseMorphTargets::Delete(PoseMorphTarget* morph_target)
		{
			std::vector<PoseMorphTarget*>::iterator iter = find(mMorphTargets.begin(), mMorphTargets.end(), morph_target);
			if (iter == mMorphTargets.end())
			{
				return;
			}

			mMorphTargets.erase(iter);
		}

		void PoseMorphTargets::Delete(std::string name)
		{
			Delete(Find(name));
		}

		void PoseMorphTargets::Clear(void)
		{
			while (mMorphTargets.size())
			{
				delete mMorphTargets.back();
				mMorphTargets.pop_back();
			}
			mMorphTargets.swap(std::vector<PoseMorphTarget*>(mMorphTargets));

			mMorphTarget = mMorphTargets.end();
		}

		PoseMorphTarget* PoseMorphTargets::Find(std::string name, bool create)
		{
			for (std::vector<PoseMorphTarget*>::iterator morph_target_iterator = mMorphTargets.begin(); morph_target_iterator != mMorphTargets.end(); ++morph_target_iterator)
			{
				PoseMorphTarget* morph_target = *morph_target_iterator;
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

		PoseMorphTarget* PoseMorphTargets::operator()(void)
		{
			if (mMorphTarget == mMorphTargets.end())
			{
				return 0;
			}

			return *mMorphTarget;
		}

		PoseMorphTarget* PoseMorphTargets::First(void)
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

		PoseMorphTarget* PoseMorphTargets::Next(void)
		{
			if (mMorphTarget != mMorphTargets.end())
			{
				++mMorphTarget;
			}

			return operator()();
		}

		PoseMorphTarget* PoseMorphTargets::Last(void)
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

		PoseMorphTarget* PoseMorphTargets::Previous(void)
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

		int PoseMorphTargets::Count(void)
		{
			return mMorphTargets.size();
		}

		bool PoseMorphTargets::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			for (int i = 0; i < count; ++i)
			{
				int length;
				in.read(reinterpret_cast<char*>(&length), 4);

				char* name = new char[length];
				in.read(name, length);

				PoseMorphTarget* morph_target = Find(name, true);
				delete[] name;

				in.read(reinterpret_cast<char*>(&morph_target->mBlendFactor), 4);
			}

			return true;
		}

		void PoseMorphTargets::SaveToFStream(std::ofstream& out)
		{
			int count = mMorphTargets.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<PoseMorphTarget*>::iterator morph_target = mMorphTargets.begin(); morph_target != mMorphTargets.end(); ++morph_target)
			{
				int length = strlen((*morph_target)->mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*morph_target)->mName.c_str(), length);

				out.write(reinterpret_cast<char*>(&(*morph_target)->mBlendFactor), 4);
			}
		}
	}
}
