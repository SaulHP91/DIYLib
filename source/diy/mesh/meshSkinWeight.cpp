#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace mesh
	{
		MeshSkinWeight::MeshSkinWeight(MeshSkinWeights* skin_weights, MeshAnimatedVertex* animated_vertex, float weight) :
			mSkinWeights(skin_weights),
			mAnimatedVertex(animated_vertex),
			mWeight(weight)
		{
			mAnimatedVertex->mSkinWeights.push_back(this);

			if (mAnimatedVertex->mSkinWeights.size() > mSkinWeights->mSkin->mSkins->mMaxSkinWeightsPerVertex)
			{
				mSkinWeights->mSkin->mSkins->mMaxSkinWeightsPerVertex = mAnimatedVertex->mSkinWeights.size();
			}
		}

		MeshSkinWeight::~MeshSkinWeight(void)
		{
			if (mAnimatedVertex)
			{
				std::vector<MeshSkinWeight*>::iterator skin_weight = find(mAnimatedVertex->mSkinWeights.begin(), mAnimatedVertex->mSkinWeights.end(), this);

				if (skin_weight != mAnimatedVertex->mSkinWeights.end())
				{
					mAnimatedVertex->mSkinWeights.erase(skin_weight);
				}
			}
		}

		MeshSkinWeights* MeshSkinWeight::GetSkinWeights(void)
		{
			return mSkinWeights;
		}

		MeshAnimatedVertex* MeshSkinWeight::GetAnimatedVertex(void)
		{
			return mAnimatedVertex;
		}

		void MeshSkinWeight::SetWeight(float weight)
		{
			mWeight = weight;
		}

		float MeshSkinWeight::GetWeight(void)
		{
			return mWeight;
		}

		bool SortDecreasing(MeshSkinWeight* skin_weight_a, MeshSkinWeight* skin_weight_b)
		{
			return skin_weight_a->mWeight > skin_weight_b->mWeight;
		}
	}
}
