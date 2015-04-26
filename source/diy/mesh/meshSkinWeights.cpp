#include <diy/diy.hpp>

namespace diy
{
	namespace mesh
	{
		MeshSkinWeights::MeshSkinWeights(MeshSkin* skin) :
			mSkin(skin),
			mCapacity(256),
			mSize(0)
		{
			mSkinWeight = mSkinWeights.end();

			mIndexArray = new unsigned int[mCapacity];
			mWeightArray = new float[mCapacity];
		}

		MeshSkinWeights::~MeshSkinWeights(void)
		{
			Clear();

			delete[] mIndexArray;
			delete[] mWeightArray;
		}

		MeshSkin* MeshSkinWeights::GetSkin(void)
		{
			return mSkin;
		}

		int MeshSkinWeights::GetCount(void)
		{
			return mSkinWeights.size();
		}

		MeshSkinWeight* MeshSkinWeights::New(MeshAnimatedVertex* animated_vertex, float weight)
		{
			mSkinWeights.push_back(new MeshSkinWeight(this, animated_vertex, weight));

			return mSkinWeights.back();
		}

		MeshSkinWeight* MeshSkinWeights::New(int animatedVertexIndex, float weight)
		{
			return New(mSkin->mSkins->mObject->mAnimatedVertices->operator[](animatedVertexIndex), weight);
		}

		void MeshSkinWeights::Clear(void)
		{
			while (mSkinWeights.size())
			{
				delete mSkinWeights.back();
				mSkinWeights.pop_back();
			}
			mSkinWeights.swap(std::vector<MeshSkinWeight*>(mSkinWeights));

			mSkinWeight = mSkinWeights.end();

			mSize = 0;
		}

		void MeshSkinWeights::Delete(MeshSkinWeight* skin_weight)
		{
			std::vector<MeshSkinWeight*>::iterator iterator = find(mSkinWeights.begin(), mSkinWeights.end(), skin_weight);

			if (iterator == mSkinWeights.end())
			{
				return;
			}

			delete *iterator;
			mSkinWeights.erase(iterator);

			if (mSkinWeights.empty())
			{
				mSkin->mSkins->Delete(mSkin);
			}
		}

		MeshSkinWeight* MeshSkinWeights::Find(MeshAnimatedVertex* animated_vertex)
		{
			for (std::vector<MeshSkinWeight*>::iterator iterator = mSkinWeights.begin(); iterator != mSkinWeights.end(); ++iterator)
			{
				if ((*iterator)->mAnimatedVertex == animated_vertex)
				{
					return *iterator;
				}
			}

			return 0;
		}

		MeshSkinWeight* MeshSkinWeights::Find(int animatedVertexIndex)
		{
			return Find(mSkin->mSkins->mObject->mAnimatedVertices->operator [](animatedVertexIndex));
		}

		MeshSkinWeight* MeshSkinWeights::operator()(void)
		{
			if (mSkinWeight == mSkinWeights.end())
			{
				return 0;
			}

			return *mSkinWeight;
		}

		MeshSkinWeight* MeshSkinWeights::First(void)
		{
			if (mSkinWeights.empty())
			{
				mSkinWeight = mSkinWeights.end();
			}
			else
			{
				mSkinWeight = mSkinWeights.begin();
			}

			return operator()();
		}

		MeshSkinWeight* MeshSkinWeights::Next(void)
		{
			if (mSkinWeight != mSkinWeights.end())
			{
				++mSkinWeight;
			}

			return operator()();
		}

		MeshSkinWeight* MeshSkinWeights::Last(void)
		{
			if (mSkinWeights.empty())
			{
				mSkinWeight = mSkinWeights.end();
			}
			else
			{
				mSkinWeight = mSkinWeights.end() - 1;
			}

			return operator()();
		}

		MeshSkinWeight* MeshSkinWeights::Previous(void)
		{
			if (mSkinWeight == mSkinWeights.begin())
			{
				mSkinWeight = mSkinWeights.end();
			}
			else if (mSkinWeight != mSkinWeights.end())
			{
				--mSkinWeight;
			}

			return operator()();
		}

		void MeshSkinWeights::Realloc(unsigned int size)
		{
			while (mCapacity < size)
			{
				mCapacity *= 2;
			}

			delete[] mIndexArray;
			mIndexArray = new unsigned int[mCapacity];

			delete[] mWeightArray;
			mWeightArray = new float[mCapacity];
		}

		void MeshSkinWeights::UpdateArray(void)
		{
			if (mSkinWeights.size() > mCapacity)
			{
				Realloc(mSkinWeights.size());
			}

			mSize = 0;
			for (std::vector<MeshSkinWeight*>::iterator skin_weight = mSkinWeights.begin(); skin_weight != mSkinWeights.end(); ++skin_weight)
			{
				mIndexArray[mSize] = (*skin_weight)->mAnimatedVertex->mIndex;

				mWeightArray[mSize] = (*skin_weight)->mWeight;

				++mSize;
			}
		}
	}
}
