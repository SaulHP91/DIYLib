#include <diy/diy.hpp>

namespace diy
{
	namespace mesh
	{
		MeshSkin::MeshSkin(MeshSkins* skins, MeshNode* node, int index) :
			mSkins(skins),
			mNode(0),
			mIndex(index),
			mMatrix(1.0f),
			mNormalMatrix(1.0f),
			mOrigin(0.0f)
		{
			if (node)
			{
				node->AppendSkin(this);
			}

			mSkinWeights = new MeshSkinWeights(this);
			OffsetMatrix = node->GetOffsetMatrix();
		}

		MeshSkin::~MeshSkin(void)
		{
			if (mNode)
			{
				mNode->RemoveSkin(this);
			}

			delete mSkinWeights;
		}

		MeshSkins* MeshSkin::GetSkins(void)
		{
			return mSkins;
		}

		MeshNode* MeshSkin::GetNode(void)
		{
			return mNode;
		}

		int MeshSkin::GetIndex(void)
		{
			return mIndex;
		}

		MeshSkinWeights* MeshSkin::GetSkinWeights(void)
		{
			return mSkinWeights;
		}

		void MeshSkin::AppendAnimatedVertex(class MeshAnimatedVertex* animated_vertex, float weight)
		{
			mSkinWeights->New(animated_vertex, weight);
		}

		void MeshSkin::AppendAnimatedVertex(int animatedVertexIndex, float weight)
		{
			mSkinWeights->New(animatedVertexIndex, weight);
		}

		void MeshSkin::RemoveAnimatedVertex(class MeshAnimatedVertex* animated_vertex)
		{
			mSkinWeights->Delete(mSkinWeights->Find(animated_vertex));
		}

		void MeshSkin::RemoveAnimatedVertex(int animatedVertexIndex)
		{
			mSkinWeights->Delete(mSkinWeights->Find(animatedVertexIndex));
		}
	}
}
