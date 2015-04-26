#include <diy/diy.hpp>

namespace diy
{
	namespace mesh
	{
		MeshMorphTargetVertices::MeshMorphTargetVertices(MeshMorphTarget* morph_target) :
			mMorphTarget(morph_target),
			mCapacity(1024),
			mSize(0)
		{
			mMorphTargetVertex = mMorphTargetVertices.end();

			mIndexArray = new unsigned int[mCapacity];
			mDeltaPositionArray = new float[3 * mCapacity];
		}

		MeshMorphTargetVertices::~MeshMorphTargetVertices(void)
		{
			Clear();

			delete[] mIndexArray;
			delete[] mDeltaPositionArray;
		}

		MeshMorphTarget* MeshMorphTargetVertices::GetMorphTarget(void)
		{
			return mMorphTarget;
		}

		int MeshMorphTargetVertices::GetCount(void)
		{
			return mMorphTargetVertices.size();
		}

		MeshMorphTargetVertex* MeshMorphTargetVertices::New(MeshAnimatedVertex* animated_vertex, MeshVertex vertex)
		{
			mMorphTargetVertices.push_back(new MeshMorphTargetVertex(this, animated_vertex, vertex));

			return mMorphTargetVertices.back();
		}

		MeshMorphTargetVertex* MeshMorphTargetVertices::New(int animatedVertexIndex, MeshVertex vertex)
		{
			return New(mMorphTarget->mMorphTargets->mObject->mAnimatedVertices->operator[](animatedVertexIndex), vertex);
		}

		void MeshMorphTargetVertices::Clear(void)
		{
			while (mMorphTargetVertices.size())
			{
				delete mMorphTargetVertices.back();
				mMorphTargetVertices.pop_back();
			}
			mMorphTargetVertices.swap(std::vector<MeshMorphTargetVertex*>(mMorphTargetVertices));

			mMorphTargetVertex = mMorphTargetVertices.end();

			mSize = 0;
		}

		void MeshMorphTargetVertices::Delete(MeshMorphTargetVertex* morph_target_vertex)
		{
			std::vector<MeshMorphTargetVertex*>::iterator iterator = find(mMorphTargetVertices.begin(), mMorphTargetVertices.end(), morph_target_vertex);

			if (iterator == mMorphTargetVertices.end())
			{
				return;
			}

			delete *iterator;
			mMorphTargetVertices.erase(iterator);

			if (mMorphTargetVertices.empty())
			{
				mMorphTarget->mMorphTargets->Delete(mMorphTarget);
			}
		}

		MeshMorphTargetVertex* MeshMorphTargetVertices::Find(MeshAnimatedVertex* animated_vertex)
		{
			for (std::vector<MeshMorphTargetVertex*>::iterator iterator = mMorphTargetVertices.begin(); iterator != mMorphTargetVertices.end(); ++iterator)
			{
				if ((*iterator)->mAnimatedVertex == animated_vertex)
				{
					return *iterator;
				}
			}

			return 0;
		}

		MeshMorphTargetVertex* MeshMorphTargetVertices::Find(int animatedVertexIndex)
		{
			return Find(mMorphTarget->mMorphTargets->mObject->mAnimatedVertices->operator [](animatedVertexIndex));
		}

		MeshMorphTargetVertex* MeshMorphTargetVertices::operator()(void)
		{
			if (mMorphTargetVertex == mMorphTargetVertices.end())
			{
				return 0;
			}

			return *mMorphTargetVertex;
		}

		MeshMorphTargetVertex* MeshMorphTargetVertices::First(void)
		{
			if (mMorphTargetVertices.empty())
			{
				mMorphTargetVertex = mMorphTargetVertices.end();
			}
			else
			{
				mMorphTargetVertex = mMorphTargetVertices.begin();
			}

			return operator()();
		}

		MeshMorphTargetVertex* MeshMorphTargetVertices::Next(void)
		{
			if (mMorphTargetVertex != mMorphTargetVertices.end())
			{
				++mMorphTargetVertex;
			}

			return operator()();
		}

		MeshMorphTargetVertex* MeshMorphTargetVertices::Last(void)
		{
			if (mMorphTargetVertices.empty())
			{
				mMorphTargetVertex = mMorphTargetVertices.end();
			}
			else
			{
				mMorphTargetVertex = mMorphTargetVertices.end() - 1;
			}

			return operator()();
		}

		MeshMorphTargetVertex* MeshMorphTargetVertices::Previous(void)
		{
			if (mMorphTargetVertex == mMorphTargetVertices.begin())
			{
				mMorphTargetVertex = mMorphTargetVertices.end();
			}
			else if (mMorphTargetVertex != mMorphTargetVertices.end())
			{
				--mMorphTargetVertex;
			}

			return operator()();
		}

		void MeshMorphTargetVertices::Realloc(unsigned int size)
		{
			while (mCapacity < size)
			{
				mCapacity *= 2;
			}

			delete[] mIndexArray;
			mIndexArray = new unsigned int[mCapacity];

			delete[] mDeltaPositionArray;
			mDeltaPositionArray = new float[3 * mCapacity];
		}

		void MeshMorphTargetVertices::UpdateArray(void)
		{
			if (mMorphTargetVertices.size() > mCapacity)
			{
				Realloc(mMorphTargetVertices.size());
			}

			mSize = 0;
			for (std::vector<MeshMorphTargetVertex*>::iterator morph_target_vertex = mMorphTargetVertices.begin(); morph_target_vertex != mMorphTargetVertices.end(); ++morph_target_vertex)
			{
				mIndexArray[mSize] = (*morph_target_vertex)->mAnimatedVertex->mIndex;

				mDeltaPositionArray[3 * mSize] = (*morph_target_vertex)->mDeltaVertex.Position.x;
				mDeltaPositionArray[3 * mSize + 1] = (*morph_target_vertex)->mDeltaVertex.Position.y;
				mDeltaPositionArray[3 * mSize + 2] = (*morph_target_vertex)->mDeltaVertex.Position.z;

				++mSize;
			}
		}

		void MeshMorphTargetVertices::Reverse(void)
		{
			for (std::vector<MeshMorphTargetVertex*>::iterator morph_target_vertex = mMorphTargetVertices.begin(); morph_target_vertex != mMorphTargetVertices.end(); ++morph_target_vertex)
			{
				(*morph_target_vertex)->mDeltaVertex.Position *= -1.0f;
				(*morph_target_vertex)->mVertex = (*morph_target_vertex)->mAnimatedVertex->mVertex + (*morph_target_vertex)->mDeltaVertex;
			}

			UpdateArray();
			mMorphTarget->mMorphTargets->mObject->Update();
		}
	}
}
