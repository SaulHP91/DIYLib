#include <diy/diy.hpp>

namespace diy
{
	namespace mesh
	{
		MeshMorphTarget::MeshMorphTarget(class MeshMorphTargets* morph_targets, int index, std::string name) :
			mMorphTargets(morph_targets),
			mIndex(index),
			mName(name),
			mBlendFactor(0.0f)
		{
			mMorphTargetVertices = new MeshMorphTargetVertices(this);
		}

		MeshMorphTarget::~MeshMorphTarget(void)
		{
			delete mMorphTargetVertices;
		}

		MeshMorphTargets* MeshMorphTarget::MorphTargets(void)
		{
			return mMorphTargets;
		}

		int MeshMorphTarget::GetIndex(void)
		{
			return mIndex;
		}

		std::string MeshMorphTarget::GetName(void)
		{
			return mName;
		}

		MeshMorphTargetVertices* MeshMorphTarget::GetMorphTargetVertices(void)
		{
			return mMorphTargetVertices;
		}

		void MeshMorphTarget::SetBlendFactor(float blendFactor)
		{
			if (mBlendFactor != blendFactor)
			{
				mBlendFactor = blendFactor;

				mMorphTargets->mObject->Update();
			}
		}

		float MeshMorphTarget::GetBlendFactor(void)
		{
			return mBlendFactor;
		}

		void MeshMorphTarget::AppendAnimatedVertex(class MeshAnimatedVertex* animated_vertex, MeshVertex vertex)
		{
			mMorphTargetVertices->New(animated_vertex, vertex);
		}

		void MeshMorphTarget::AppendAnimatedVertex(int animatedVertexIndex, MeshVertex vertex)
		{
			mMorphTargetVertices->New(animatedVertexIndex, vertex);
		}

		void MeshMorphTarget::RemoveAnimatedVertex(class MeshAnimatedVertex* animated_vertex)
		{
			mMorphTargetVertices->Delete(mMorphTargetVertices->Find(animated_vertex));
		}

		void MeshMorphTarget::RemoveAnimatedVertex(int animatedVertexIndex)
		{
			mMorphTargetVertices->Delete(mMorphTargetVertices->Find(animatedVertexIndex));
		}

		bool MeshMorphTarget::SetName(std::string name)
		{
			if (mMorphTargets->Find(name))
			{
				return false;
			}

			mName = name;

			return true;
		}
	}
}
