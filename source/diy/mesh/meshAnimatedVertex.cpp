#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace mesh
	{
		MeshAnimatedVertex::MeshAnimatedVertex(MeshAnimatedVertices* animated_vertices, int index, MeshVertex vertex) :
			mAnimatedVertices(animated_vertices),
			mIndex(index),
			mVertex(vertex),
			mReserved(0)
		{
			mAnimatedVertices->mObject->mObjects->mAnimatedVertexCount++;
		}

		MeshAnimatedVertex::~MeshAnimatedVertex(void)
		{
			while (mFaces.size())
			{
				mFaces.back()->RemoveAnimatedVertex(this);
			}
			mFaces.swap(std::vector<MeshFace*>(mFaces));

			while (mSkinWeights.size())
			{
				mSkinWeights.back()->mSkinWeights->mSkin->RemoveAnimatedVertex(this);
			}
			mSkinWeights.swap(std::vector<MeshSkinWeight*>(mSkinWeights));

			while (mMorphTargetVertices.size())
			{
				mMorphTargetVertices.back()->mMorphTargetVertices->mMorphTarget->RemoveAnimatedVertex(this);
			}
			mMorphTargetVertices.swap(std::vector<MeshMorphTargetVertex*>(mMorphTargetVertices));

			mAnimatedVertices->mObject->mObjects->mAnimatedVertexCount--;
		}

		MeshAnimatedVertices* MeshAnimatedVertex::GetAnimatedVertices(void)
		{
			return mAnimatedVertices;
		}

		int MeshAnimatedVertex::GetIndex(void)
		{
			return mIndex;
		}

		MeshVertex MeshAnimatedVertex::GetVertex(void)
		{
			return mVertex;
		}

		MeshVertex MeshAnimatedVertex::GetFinalVertex(void)
		{
			glm::vec3 finalPosition =
				glm::vec3
				(
				mAnimatedVertices->mFinalPositionArray[3 * mIndex],
				mAnimatedVertices->mFinalPositionArray[3 * mIndex + 1],
				mAnimatedVertices->mFinalPositionArray[3 * mIndex + 2]
				);

			glm::vec2 finalTexCoord =
				glm::vec2
				(
				mAnimatedVertices->mTexCoordArray[2 * mIndex],
				mAnimatedVertices->mTexCoordArray[2 * mIndex + 1]
				);

			glm::vec3 finalTangent =
				glm::vec3
				(
				mAnimatedVertices->mFinalTangentArray[3 * mIndex],
				mAnimatedVertices->mFinalTangentArray[3 * mIndex + 1],
				mAnimatedVertices->mFinalTangentArray[3 * mIndex + 2]
				);

			glm::vec3 finalBitangent =
				glm::vec3
				(
				mAnimatedVertices->mFinalBitangentArray[3 * mIndex],
				mAnimatedVertices->mFinalBitangentArray[3 * mIndex + 1],
				mAnimatedVertices->mFinalBitangentArray[3 * mIndex + 2]
				);

			glm::vec3 finalNormal =
				glm::vec3
				(
				mAnimatedVertices->mFinalNormalArray[3 * mIndex],
				mAnimatedVertices->mFinalNormalArray[3 * mIndex + 1],
				mAnimatedVertices->mFinalNormalArray[3 * mIndex + 2]
				);

			return MeshVertex(finalPosition, finalTexCoord, finalTangent, finalBitangent, finalNormal);
		}

		void MeshAnimatedVertex::SetVertex(MeshVertex vertex)
		{
			mVertex = vertex;
			mVertex.Normalize();

			mAnimatedVertices->mObject->UpdateLimits(mVertex.Position);

			for (std::vector<MeshFace*>::iterator face = mFaces.begin(); face != mFaces.end(); ++face)
			{
				(*face)->Update();
			}
		}

		std::vector<MeshFace*> MeshAnimatedVertex::GetFaces(void)
		{
			return mFaces;
		}


		std::vector<MeshSkinWeight*> MeshAnimatedVertex::GetSkinWeights(void)
		{
			return mSkinWeights;
		}

		std::vector<MeshMorphTargetVertex*> MeshAnimatedVertex::GetMorphTargetVertices(void)
		{
			return mMorphTargetVertices;
		}

		void MeshAnimatedVertex::AppendSkinWeight(MeshNode* node, float weight)
		{
			MeshSkin* skin = mAnimatedVertices->mObject->mSkins->Find(node, true);
			skin->AppendAnimatedVertex(this, weight);
		}

		void MeshAnimatedVertex::AppendSkinWeight(std::string nodeName, float weight)
		{
			MeshSkin* skin = mAnimatedVertices->mObject->mSkins->Find(nodeName, true);
			skin->AppendAnimatedVertex(this, weight);
		}

		void MeshAnimatedVertex::AppendSkinWeight(int nodeIndex, float weight)
		{
			MeshSkin* skin = mAnimatedVertices->mObject->mSkins->Find(nodeIndex, true);
			skin->AppendAnimatedVertex(this, weight);
		}

		void MeshAnimatedVertex::RemoveSkinWeight(MeshNode* node)
		{
			MeshSkin* skin = mAnimatedVertices->mObject->mSkins->Find(node);

			if (skin)
			{
				skin->RemoveAnimatedVertex(this);
			}
		}

		void MeshAnimatedVertex::RemoveSkinWeight(std::string nodeName)
		{
			MeshSkin* skin = mAnimatedVertices->mObject->mSkins->Find(nodeName);

			if (skin)
			{
				skin->RemoveAnimatedVertex(this);
			}
		}

		void MeshAnimatedVertex::RemoveSkinWeight(int nodeIndex)
		{
			MeshSkin* skin = mAnimatedVertices->mObject->mSkins->Find(nodeIndex);

			if (skin)
			{
				skin->RemoveAnimatedVertex(this);
			}
		}

		void MeshAnimatedVertex::AppendMorphTargetVertex(MeshMorphTarget* morph_target, MeshVertex vertex)
		{
			morph_target->AppendAnimatedVertex(this, vertex);
		}

		void MeshAnimatedVertex::AppendMorphTargetVertex(std::string morphTargetName, MeshVertex vertex)
		{
			MeshMorphTarget* morph_target = mAnimatedVertices->mObject->mMorphTargets->Find(morphTargetName, true);
			morph_target->AppendAnimatedVertex(this, vertex);
		}

		void MeshAnimatedVertex::RemoveMorphTargetVertex(MeshMorphTarget* morph_target)
		{
			morph_target->RemoveAnimatedVertex(this);
		}

		void MeshAnimatedVertex::RemoveMorphTargetVertex(std::string morphTargetName)
		{
			MeshMorphTarget* morph_target = mAnimatedVertices->mObject->mMorphTargets->Find(morphTargetName);

			if (morph_target)
			{
				morph_target->RemoveAnimatedVertex(this);
			}
		}

		void MeshAnimatedVertex::CalculateTangents(void)
		{
			mVertex.Tangent = glm::vec3(0.0f);
			mVertex.Bitangent = glm::vec3(0.0f);

			for (std::vector<MeshFace*>::iterator face = mFaces.begin(); face != mFaces.end(); ++face)
			{
				mVertex.Tangent += (*face)->mTangent;
				mVertex.Bitangent += (*face)->mBitangent;
			}

			mVertex.Normalize();
		}

		void MeshAnimatedVertex::CalculateNormal(void)
		{
			mVertex.Normal = glm::vec3(0.0f);

			for (std::vector<MeshFace*>::iterator face = mFaces.begin(); face != mFaces.end(); ++face)
			{
				mVertex.Normal += (*face)->mNormal;
			}

			mVertex.Normalize();
		}

		void MeshAnimatedVertex::ReverseNormal(void)
		{
			mVertex.Normal *= -1.0f;
		}

		void MeshAnimatedVertex::LimitSkinWeights(unsigned int maxSkinWeightsPerVertex)
		{
			sort(mSkinWeights.begin(), mSkinWeights.end(), SortDecreasing);

			while (mSkinWeights.size() > maxSkinWeightsPerVertex)
			{
				float weight = mSkinWeights.back()->mWeight;
				mSkinWeights.back()->mSkinWeights->Delete(mSkinWeights.back());
				if (mSkinWeights.size())
				{
					mSkinWeights.back()->mWeight += weight;
				}
			}
		}

		void MeshAnimatedVertex::NormalizeSkinWeights(void)
		{
			float sum = 0.0f;

			for (std::vector<MeshSkinWeight*>::iterator skin_weight = mSkinWeights.begin(); skin_weight != mSkinWeights.end(); ++skin_weight)
			{
				sum += (*skin_weight)->mWeight;
			}

			if (sum > 0.0f)
			{
				for (std::vector<MeshSkinWeight*>::iterator skin_weight = mSkinWeights.begin(); skin_weight != mSkinWeights.end(); ++skin_weight)
				{
					(*skin_weight)->mWeight /= sum;
				}
			}
		}

		bool SortAnimatedVerticesRespectPosition(MeshAnimatedVertex* animated_vertex_a, MeshAnimatedVertex* animated_vertex_b)
		{
			return animated_vertex_a->mVertex < animated_vertex_b->mVertex;
		}

		bool SortAnimatedVerticesRespectIndex(MeshAnimatedVertex* animated_vertex_a, MeshAnimatedVertex* animated_vertex_b)
		{
			return animated_vertex_a->mIndex < animated_vertex_b->mIndex;
		}

		MeshAnimatedVertex* MeshAnimatedVertex::Duplicate()
		{
			return mAnimatedVertices->Duplicate(this);
		}
	}
}