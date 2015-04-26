#include <diy/diy.hpp>

namespace diy
{
	namespace mesh
	{
		MeshSkins::MeshSkins(MeshObject* object) :
			mObject(object),
			mMaxSkinWeightsPerVertex(0)
		{
			mSkin = mSkins.end();
		}

		MeshSkins::~MeshSkins(void)
		{
			Clear();
		}

		MeshObject* MeshSkins::GetObject(void)
		{
			return mObject;
		}

		int MeshSkins::GetCount(void)
		{
			return mSkins.size();
		}

		unsigned int MeshSkins::GetMaxSkinWeightsPerVertex(void)
		{
			return mMaxSkinWeightsPerVertex;
		}

		MeshSkin* MeshSkins::New(class MeshNode* node)
		{
			if (!Find(node))
			{
				mSkins.push_back(new MeshSkin(this, node, mSkins.size()));
				return mSkins.back();
			}

			return 0;
		}

		MeshSkin* MeshSkins::New(std::string nodeName)
		{
			return New(mObject->mObjects->mFile->mNodes->Find(nodeName));
		}

		MeshSkin* MeshSkins::New(int nodeIndex)
		{
			return New(mObject->mObjects->mFile->mNodes->operator[](nodeIndex));
		}

		void MeshSkins::Clear(void)
		{
			while (mSkins.size())
			{
				delete mSkins.back();
				mSkins.pop_back();
			}
			mSkins.swap(std::vector<MeshSkin*>(mSkins));

			mSkin = mSkins.end();

			mMaxSkinWeightsPerVertex = 0;
		}

		void MeshSkins::Delete(MeshSkin* skin)
		{
			std::vector<MeshSkin*>::iterator iterator = find(mSkins.begin(), mSkins.end(), skin);

			if (iterator == mSkins.end())
			{
				return;
			}

			delete *iterator;
			iterator = mSkins.erase(iterator);

			for (iterator = iterator; iterator != mSkins.end(); ++iterator)
			{
				--(*iterator)->mIndex;
			}
		}

		MeshSkin* MeshSkins::operator[](int index)
		{
			if (index < 0 || index >= int(mSkins.size()))
			{
				return 0;
			}

			return mSkins[index];
		}

		MeshSkin* MeshSkins::Find(MeshNode* node, bool create)
		{
			for (std::vector<MeshSkin*>::iterator skin = mSkins.begin(); skin != mSkins.end(); ++skin)
			{
				if ((*skin)->mNode == node)
				{
					return *skin;
				}
			}

			if (create)
			{
				return New(node);
			}

			return 0;
		}

		MeshSkin* MeshSkins::Find(std::string nodeName, bool create)
		{
			return Find(mObject->mObjects->mFile->mNodes->Find(nodeName), create);
		}

		MeshSkin* MeshSkins::Find(int nodeIndex, bool create)
		{
			return Find(mObject->mObjects->mFile->mNodes->operator[](nodeIndex), create);
		}

		MeshSkin* MeshSkins::operator()(void)
		{
			if (mSkin == mSkins.end())
			{
				return 0;
			}

			return *mSkin;
		}

		MeshSkin* MeshSkins::First(void)
		{
			if (mSkins.empty())
			{
				mSkin = mSkins.end();
			}
			else
			{
				mSkin = mSkins.begin();
			}

			return operator()();
		}

		MeshSkin* MeshSkins::Next(void)
		{
			if (mSkin != mSkins.end())
			{
				++mSkin;
			}

			return operator()();
		}

		MeshSkin* MeshSkins::Last(void)
		{
			if (mSkins.empty())
			{
				mSkin = mSkins.end();
			}
			else
			{
				mSkin = mSkins.end() - 1;
			}

			return operator()();
		}

		MeshSkin* MeshSkins::Previous(void)
		{
			if (mSkin == mSkins.begin())
			{
				mSkin = mSkins.end();
			}
			else if (mSkin != mSkins.end())
			{
				--mSkin;
			}

			return operator()();
		}

		void MeshSkins::UpdateArrays(void)
		{
			for (std::vector<MeshSkin*>::iterator skin = mSkins.begin(); skin != mSkins.end(); ++skin)
			{
				(*skin)->mSkinWeights->UpdateArray();
			}
		}
	}
}
