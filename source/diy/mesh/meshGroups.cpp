#include <diy/diy.hpp>

namespace diy
{
	namespace mesh
	{
		MeshGroups::MeshGroups(MeshObject* object) :
			mObject(object)
		{
			mGroup = mGroups.end();
		}

		MeshGroups::~MeshGroups(void)
		{
			Clear();
		}

		MeshObject* MeshGroups::GetObject(void)
		{
			return mObject;
		}

		int MeshGroups::GetCount(void)
		{
			return mGroups.size();
		}

		MeshGroup* MeshGroups::New(MeshMaterial* material)
		{
			if (!Find(material))
			{
				mGroups.push_back(new MeshGroup(this, material, mGroups.size()));
				return mGroups.back();
			}

			return 0;
		}

		MeshGroup* MeshGroups::New(std::string materialName)
		{
			return New(mObject->mObjects->mFile->mMaterials->Find(materialName, true));
		}

		MeshGroup* MeshGroups::New(int materialIndex)
		{
			return New(mObject->mObjects->mFile->mMaterials->operator[](materialIndex));
		}

		void MeshGroups::Clear(void)
		{
			while (mGroups.size())
			{
				delete mGroups.back();
				mGroups.pop_back();
			}
			mGroups.swap(std::vector<MeshGroup*>(mGroups));

			mGroup = mGroups.end();
		}

		void MeshGroups::Delete(MeshGroup* group)
		{
			std::vector<MeshGroup*>::iterator iterator = find(mGroups.begin(), mGroups.end(), group);

			if (iterator == mGroups.end())
			{
				return;
			}

			delete *iterator;
			iterator = mGroups.erase(iterator);

			for (iterator = iterator; iterator != mGroups.end(); ++iterator)
			{
				--(*iterator)->mIndex;
			}
		}

		MeshGroup* MeshGroups::operator[](int index)
		{
			if (index < 0 || index >= int(mGroups.size()))
			{
				return 0;
			}

			return mGroups[index];
		}

		MeshGroup* MeshGroups::Find(MeshMaterial* material, bool create)
		{
			for (std::vector<MeshGroup*>::iterator group = mGroups.begin(); group != mGroups.end(); ++group)
			{
				if ((*group)->mMaterial == material)
				{
					return *group;
				}
			}

			if (create)
			{
				return New(material);
			}

			return 0;
		}

		MeshGroup* MeshGroups::Find(std::string materialName, bool create)
		{
			return Find(mObject->mObjects->mFile->mMaterials->Find(materialName, create), create);
		}

		MeshGroup* MeshGroups::Find(int materialIndex, bool create)
		{
			return Find(mObject->mObjects->mFile->mMaterials->operator[](materialIndex), create);
		}

		MeshGroup* MeshGroups::operator()(void)
		{
			if (mGroup == mGroups.end())
			{
				return 0;
			}

			return *mGroup;
		}

		MeshGroup* MeshGroups::First(void)
		{
			if (mGroups.empty())
			{
				mGroup = mGroups.end();
			}
			else
			{
				mGroup = mGroups.begin();
			}

			return operator()();
		}

		MeshGroup* MeshGroups::Next(void)
		{
			if (mGroup != mGroups.end())
			{
				++mGroup;
			}

			return operator()();
		}

		MeshGroup* MeshGroups::Last(void)
		{
			if (mGroups.empty())
			{
				mGroup = mGroups.end();
			}
			else
			{
				mGroup = mGroups.end() - 1;
			}

			return operator()();
		}

		MeshGroup* MeshGroups::Previous(void)
		{
			if (mGroup == mGroups.begin())
			{
				mGroup = mGroups.end();
			}
			else if (mGroup != mGroups.end())
			{
				--mGroup;
			}

			return operator()();
		}
	}
}
