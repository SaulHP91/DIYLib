#include <diy/diy.hpp>

namespace diy
{
	namespace mesh
	{
		MeshMorphTargets::MeshMorphTargets(MeshObject* object) :
			mObject(object)
		{
			mMorphTarget = mMorphTargets.end();
		}

		MeshMorphTargets::~MeshMorphTargets(void)
		{
			Clear();
		}

		MeshObject* MeshMorphTargets::GetObject(void)
		{
			return mObject;
		}

		int MeshMorphTargets::GetCount(void)
		{
			return mMorphTargets.size();
		}

		MeshMorphTarget* MeshMorphTargets::New(void)
		{
			int index = 0;
			char name[256];

			MeshMorphTarget* new_morph_target;
			do
			{
				sprintf(name, "MorphTarget%i", index++);
				new_morph_target = New(name);
			} while (!new_morph_target);

			return new_morph_target;
		}

		MeshMorphTarget* MeshMorphTargets::New(std::string name)
		{
			if (name.empty())
			{
				return 0;
			}

			if (Find(name))
			{
				return 0;
			}

			mMorphTargets.push_back(new MeshMorphTarget(this, mMorphTargets.size(), name));

			return mMorphTargets.back();
		}

		void MeshMorphTargets::Clear(void)
		{
			while (mMorphTargets.size())
			{
				delete mMorphTargets.back();
				mMorphTargets.pop_back();
			}
			mMorphTargets.swap(std::vector<MeshMorphTarget*>(mMorphTargets));

			mMorphTarget = mMorphTargets.end();
		}

		void MeshMorphTargets::Delete(MeshMorphTarget* morph_target)
		{
			std::vector<MeshMorphTarget*>::iterator iterator = find(mMorphTargets.begin(), mMorphTargets.end(), morph_target);

			if (iterator == mMorphTargets.end())
			{
				return;
			}

			delete *iterator;
			mMorphTargets.erase(iterator);
		}

		MeshMorphTarget* MeshMorphTargets::operator[](int index)
		{
			if (index < 0 || index >= int(mMorphTargets.size()))
			{
				return 0;
			}

			return mMorphTargets[index];
		}

		MeshMorphTarget* MeshMorphTargets::Find(std::string name, bool create)
		{
			for (std::vector<MeshMorphTarget*>::iterator morph_target = mMorphTargets.begin(); morph_target != mMorphTargets.end(); ++morph_target)
			{
				if (!_stricmp((*morph_target)->mName.c_str(), name.c_str()))
				{
					return *morph_target;
				}
			}

			if (create)
			{
				return New(name);
			}

			return 0;
		}

		MeshMorphTarget* MeshMorphTargets::operator()(void)
		{
			if (mMorphTarget == mMorphTargets.end())
			{
				return 0;
			}

			return *mMorphTarget;
		}

		MeshMorphTarget* MeshMorphTargets::First(void)
		{
			if (mMorphTargets.empty())
			{
				mMorphTarget = mMorphTargets.end();
			}
			else
			{
				mMorphTarget = mMorphTargets.begin();
			}

			return operator()();
		}

		MeshMorphTarget* MeshMorphTargets::Next(void)
		{
			if (mMorphTarget != mMorphTargets.end())
			{
				++mMorphTarget;
			}

			return operator()();
		}

		MeshMorphTarget* MeshMorphTargets::Last(void)
		{
			if (mMorphTargets.empty())
			{
				mMorphTarget = mMorphTargets.end();
			}
			else
			{
				mMorphTarget = mMorphTargets.end() - 1;
			}

			return operator()();
		}

		MeshMorphTarget* MeshMorphTargets::Previous(void)
		{
			if (mMorphTarget == mMorphTargets.begin())
			{
				mMorphTarget = mMorphTargets.end();
			}
			else if (mMorphTarget != mMorphTargets.end())
			{
				--mMorphTarget;
			}

			return operator()();
		}

		void MeshMorphTargets::UpdateArrays(void)
		{
			for (std::vector<MeshMorphTarget*>::iterator morph_target = mMorphTargets.begin(); morph_target != mMorphTargets.end(); ++morph_target)
			{
				(*morph_target)->mMorphTargetVertices->UpdateArray();
			}
		}
	}
}