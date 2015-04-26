#include <diy/diy.hpp>

namespace diy
{
	namespace mesh
	{
		MeshMorphTargetVertex::MeshMorphTargetVertex(MeshMorphTargetVertices* morph_target_vertices, MeshAnimatedVertex* animated_vertex, MeshVertex& vertex) :
			mMorphTargetVertices(morph_target_vertices),
			mAnimatedVertex(animated_vertex),
			mVertex(vertex)
		{
			mAnimatedVertex->mMorphTargetVertices.push_back(this);

			mDeltaVertex = mVertex - mAnimatedVertex->mVertex;
		}

		MeshMorphTargetVertex::~MeshMorphTargetVertex(void)
		{
			if (mAnimatedVertex)
			{
				std::vector<MeshMorphTargetVertex*>::iterator morph_target_vertex = find(mAnimatedVertex->mMorphTargetVertices.begin(), mAnimatedVertex->mMorphTargetVertices.end(), this);

				if (morph_target_vertex != mAnimatedVertex->mMorphTargetVertices.end())
				{
					mAnimatedVertex->mMorphTargetVertices.erase(morph_target_vertex);
				}

				mAnimatedVertex = 0;
			}
		}

		MeshVertex MeshMorphTargetVertex::GetVertex(void)
		{
			return mVertex;
		}

		void MeshMorphTargetVertex::SetVertex(MeshVertex vertex)
		{
			mVertex = vertex;
			mDeltaVertex = mVertex - mAnimatedVertex->mVertex;
		}

		MeshMorphTargetVertices* MeshMorphTargetVertex::GetMorphTargetVertices(void)
		{
			return mMorphTargetVertices;
		}

		MeshAnimatedVertex* MeshMorphTargetVertex::GetAnimatedVertex(void)
		{
			return mAnimatedVertex;
		}
	}
}
