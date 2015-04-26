#include <diy/diy.hpp>

#include <algorithm>
#include <stack>

#include <gl/glew.h>

namespace diy
{
	namespace mesh
	{
		MeshAnimatedVertices::MeshAnimatedVertices(MeshObject* object) :
			mObject(object),
			mCapacity(256),
			mSize(0)
		{
			mAnimatedVertex = mAnimatedVertices.end();

			mPositionArray = new float[3 * mCapacity];
			mMorphedPositionArray = new float[3 * mCapacity];
			mFinalPositionArray = new float[3 * mCapacity];

			mTexCoordArray = new float[2 * mCapacity];

			mTangentArray = new float[3 * mCapacity];
			mFinalTangentArray = new float[3 * mCapacity];

			mBitangentArray = new float[3 * mCapacity];
			mFinalBitangentArray = new float[3 * mCapacity];

			mNormalArray = new float[3 * mCapacity];
			mFinalNormalArray = new float[3 * mCapacity];

			mSkinNodeArray = new int[4 * mCapacity];
			mSkinWeightArray = new float[4 * mCapacity];
		}

		MeshAnimatedVertices::~MeshAnimatedVertices(void)
		{
			Clear();

			delete[] mPositionArray;
			delete[] mMorphedPositionArray;
			delete[] mFinalPositionArray;

			delete[] mTexCoordArray;

			delete[] mTangentArray;
			delete[] mFinalTangentArray;

			delete[] mBitangentArray;
			delete[] mFinalBitangentArray;

			delete[] mNormalArray;
			delete[] mFinalNormalArray;

			delete[] mSkinNodeArray;
			delete[] mSkinWeightArray;
		}

		MeshObject* MeshAnimatedVertices::GetObject(void)
		{
			return mObject;
		}

		int MeshAnimatedVertices::GetCount(void)
		{
			return mAnimatedVertices.size();
		}

		MeshAnimatedVertex* MeshAnimatedVertices::New(MeshVertex vertex)
		{
			mAnimatedVertices.push_back(new MeshAnimatedVertex(this, mAnimatedVertices.size(), vertex));

			mObject->UpdateLimits(vertex.Position);

			return mAnimatedVertices.back();
		}

		MeshAnimatedVertex* MeshAnimatedVertices::Duplicate(MeshAnimatedVertex* animated_vertex)
		{
			if (!animated_vertex)
			{
				return 0;
			}

			MeshAnimatedVertex* duplicated_animated_vertex = New(animated_vertex->mVertex);

			for (std::vector<MeshSkinWeight*>::iterator skin_weight = animated_vertex->mSkinWeights.begin(); skin_weight != animated_vertex->mSkinWeights.end(); ++skin_weight)
			{
				duplicated_animated_vertex->AppendSkinWeight((*skin_weight)->mSkinWeights->mSkin->mNode, (*skin_weight)->mWeight);
			}

			for (std::vector<MeshMorphTargetVertex*>::iterator morph_target_vertex = animated_vertex->mMorphTargetVertices.begin(); morph_target_vertex != animated_vertex->mMorphTargetVertices.end(); ++morph_target_vertex)
			{
				duplicated_animated_vertex->AppendMorphTargetVertex((*morph_target_vertex)->mMorphTargetVertices->mMorphTarget, (*morph_target_vertex)->mVertex);
			}

			return duplicated_animated_vertex;
		}

		MeshAnimatedVertex* MeshAnimatedVertices::Duplicate(int animatedVertexIndex)
		{
			return Duplicate(operator[](animatedVertexIndex));
		}

		void MeshAnimatedVertices::Clear(void)
		{
			while (mAnimatedVertices.size())
			{
				delete mAnimatedVertices.back();
				mAnimatedVertices.pop_back();
			}
			mAnimatedVertices.swap(std::vector<MeshAnimatedVertex*>(mAnimatedVertices));

			mAnimatedVertex = mAnimatedVertices.end();

			mSize = 0;
		}

		void MeshAnimatedVertices::Delete(MeshAnimatedVertex* animated_vertex)
		{
			std::vector<MeshAnimatedVertex*>::iterator iterator = find(mAnimatedVertices.begin(), mAnimatedVertices.end(), animated_vertex);

			if (iterator == mAnimatedVertices.end())
			{
				return;
			}

			delete *iterator;
			iterator = mAnimatedVertices.erase(iterator);

			for (iterator = iterator; iterator != mAnimatedVertices.end(); ++iterator)
			{
				--(*iterator)->mIndex;
			}
		}

		MeshAnimatedVertex* MeshAnimatedVertices::operator[](int index)
		{
			if (index < 0 || index >= int(mAnimatedVertices.size()))
			{
				return 0;
			}

			return mAnimatedVertices[index];
		}

		MeshAnimatedVertex* MeshAnimatedVertices::operator()(void)
		{
			if (mAnimatedVertex == mAnimatedVertices.end())
			{
				return 0;
			}

			return *mAnimatedVertex;
		}

		MeshAnimatedVertex* MeshAnimatedVertices::First(void)
		{
			if (mAnimatedVertices.empty())
			{
				mAnimatedVertex = mAnimatedVertices.end();
			}
			else
			{
				mAnimatedVertex = mAnimatedVertices.begin();
			}

			return operator()();
		}

		MeshAnimatedVertex* MeshAnimatedVertices::Next(void)
		{
			if (mAnimatedVertex != mAnimatedVertices.end())
			{
				++mAnimatedVertex;
			}

			return operator()();
		}

		MeshAnimatedVertex* MeshAnimatedVertices::Last(void)
		{
			if (mAnimatedVertices.empty())
			{
				mAnimatedVertex = mAnimatedVertices.end();
			}
			else
			{
				mAnimatedVertex = mAnimatedVertices.end() - 1;
			}

			return operator()();
		}

		MeshAnimatedVertex* MeshAnimatedVertices::Previous(void)
		{
			if (mAnimatedVertex == mAnimatedVertices.begin())
			{
				mAnimatedVertex = mAnimatedVertices.end();
			}
			else if (mAnimatedVertex != mAnimatedVertices.end())
			{
				--mAnimatedVertex;
			}

			return operator()();
		}

		void MeshAnimatedVertices::CalculateTangents(void)
		{
			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = mAnimatedVertices.begin(); animated_vertex != mAnimatedVertices.end(); ++animated_vertex)
			{
				(*animated_vertex)->CalculateTangents();
			}

			UpdateArray();
		}

		void MeshAnimatedVertices::CalculateNormals(void)
		{
			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = mAnimatedVertices.begin(); animated_vertex != mAnimatedVertices.end(); ++animated_vertex)
			{
				(*animated_vertex)->CalculateNormal();
			}

			UpdateArray();
		}

		void MeshAnimatedVertices::ReverseNormals(void)
		{
			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = mAnimatedVertices.begin(); animated_vertex != mAnimatedVertices.end(); ++animated_vertex)
			{
				(*animated_vertex)->ReverseNormal();
			}

			UpdateArray();
		}

		void MeshAnimatedVertices::LimitSkinWeights(unsigned int maxSkinWeightsPerVertex)
		{
			if (maxSkinWeightsPerVertex >= mObject->mSkins->mMaxSkinWeightsPerVertex)
			{
				return;
			}

			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = mAnimatedVertices.begin(); animated_vertex != mAnimatedVertices.end(); ++animated_vertex)
			{
				(*animated_vertex)->LimitSkinWeights(maxSkinWeightsPerVertex);
			}

			mObject->mSkins->mMaxSkinWeightsPerVertex = maxSkinWeightsPerVertex;
			mObject->mSkins->UpdateArrays();
			mObject->Update();
		}

		void MeshAnimatedVertices::NormalizeSkinWeights(void)
		{
			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = mAnimatedVertices.begin(); animated_vertex != mAnimatedVertices.end(); ++animated_vertex)
			{
				(*animated_vertex)->NormalizeSkinWeights();
			}

			mObject->mSkins->UpdateArrays();
			mObject->Update();
		}

		void MeshAnimatedVertices::Merge(void)
		{
			sort(mAnimatedVertices.begin(), mAnimatedVertices.end(), SortAnimatedVerticesRespectPosition);

			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex_a = mAnimatedVertices.begin(); animated_vertex_a != mAnimatedVertices.end() - 1; ++animated_vertex_a)
			{
				std::vector<MeshAnimatedVertex*>::iterator animated_vertex_b = animated_vertex_a + 1;
				if ((*animated_vertex_a)->mVertex == (*animated_vertex_b)->mVertex)
				{
					while ((*animated_vertex_a)->mFaces.size())
					{
						(*animated_vertex_a)->mFaces.back()->ReplaceAnimatedVertex(*animated_vertex_a, *animated_vertex_b);
					}

					delete *animated_vertex_a;
					animated_vertex_a = mAnimatedVertices.erase(animated_vertex_a);
					--animated_vertex_a;
				}
			}

			int index = 0;
			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = mAnimatedVertices.begin(); animated_vertex != mAnimatedVertices.end(); ++animated_vertex)
			{
				(*animated_vertex)->mIndex = index++;
			}

			mObject->UpdateArrays();
		}

		void MeshAnimatedVertices::AverageNormals(void)
		{
			sort(mAnimatedVertices.begin(), mAnimatedVertices.end(), SortAnimatedVerticesRespectPosition);

			std::stack<MeshAnimatedVertex*> animatedVertexStack;
			glm::vec3 normalAv;
			float normalAvLength;

			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex_a = mAnimatedVertices.begin(); animated_vertex_a != mAnimatedVertices.end() - 1; ++animated_vertex_a)
			{
				normalAv = (*animated_vertex_a)->mVertex.Normal;
				animatedVertexStack.push(*animated_vertex_a);

				std::vector<MeshAnimatedVertex*>::iterator animated_vertex_b = animated_vertex_a + 1;
				while ((*animated_vertex_b)->mVertex.Position == (*animated_vertex_a)->mVertex.Position)
				{
					normalAv += (*animated_vertex_b)->mVertex.Normal;
					animatedVertexStack.push(*animated_vertex_b);

					++animated_vertex_b;
					if (animated_vertex_b == mAnimatedVertices.end())
					{
						break;
					}
				}
				normalAv /= animatedVertexStack.size();
				normalAvLength = glm::length(normalAv);
				if (normalAvLength > 0.0f)
				{
					normalAv /= normalAvLength;
				}

				while (!animatedVertexStack.empty())
				{
					animatedVertexStack.top()->mVertex.Normal = normalAv;
					animatedVertexStack.pop();
				}

				animated_vertex_a = animated_vertex_b - 1;
				if (animated_vertex_a == mAnimatedVertices.end() - 1)
				{
					break;
				}
			}

			sort(mAnimatedVertices.begin(), mAnimatedVertices.end(), SortAnimatedVerticesRespectIndex);

			UpdateArray();

			mObject->Update();
		}

		void MeshAnimatedVertices::TransformTexCoords(glm::mat3 transform)
		{
			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = mAnimatedVertices.begin(); animated_vertex != mAnimatedVertices.end(); ++animated_vertex)
			{
				(*animated_vertex)->mVertex.TexCoord = glm::vec2(transform * glm::vec3((*animated_vertex)->mVertex.TexCoord, 1.0f));
			}

			UpdateArray();
		}

		const void* MeshAnimatedVertices::GetPositionPointer(void)
		{
			return mFinalPositionArray;
		}

		const void* MeshAnimatedVertices::GetTexCoordPointer(void)
		{
			return mTexCoordArray;
		}

		const void* MeshAnimatedVertices::GetTangentPointer(void)
		{
			return mFinalTangentArray;
		}

		const void* MeshAnimatedVertices::GetBitangentPointer(void)
		{
			return mFinalBitangentArray;
		}

		const void* MeshAnimatedVertices::GetNormalPointer(void)
		{
			return mFinalNormalArray;
		}

		void MeshAnimatedVertices::Realloc(unsigned int size)
		{
			while (mCapacity < size)
			{
				mCapacity *= 2;
			}

			delete[] mPositionArray;
			mPositionArray = new float[3 * mCapacity];

			delete[] mMorphedPositionArray;
			mMorphedPositionArray = new float[3 * mCapacity];

			delete[] mFinalPositionArray;
			mFinalPositionArray = new float[3 * mCapacity];

			delete[] mTexCoordArray;
			mTexCoordArray = new float[2 * mCapacity];

			delete[] mTangentArray;
			mTangentArray = new float[3 * mCapacity];

			delete[] mFinalTangentArray;
			mFinalTangentArray = new float[3 * mCapacity];

			delete[] mBitangentArray;
			mBitangentArray = new float[3 * mCapacity];

			delete[] mFinalBitangentArray;
			mFinalBitangentArray = new float[3 * mCapacity];

			delete[] mNormalArray;
			mNormalArray = new float[3 * mCapacity];

			delete[] mFinalNormalArray;
			mFinalNormalArray = new float[3 * mCapacity];

			delete[] mSkinNodeArray;
			mSkinNodeArray = new int[4 * mCapacity];

			delete[] mSkinWeightArray;
			mSkinWeightArray = new float[4 * mCapacity];
		}

		void MeshAnimatedVertices::UpdateArray(void)
		{
			if (mAnimatedVertices.size() > mCapacity)
			{
				Realloc(mAnimatedVertices.size());
			}

			mSize = 0;
			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = mAnimatedVertices.begin(); animated_vertex != mAnimatedVertices.end(); ++animated_vertex)
			{
				mPositionArray[3 * mSize] = (*animated_vertex)->mVertex.Position.x;
				mPositionArray[3 * mSize + 1] = (*animated_vertex)->mVertex.Position.y;
				mPositionArray[3 * mSize + 2] = (*animated_vertex)->mVertex.Position.z;

				mTexCoordArray[2 * mSize] = (*animated_vertex)->mVertex.TexCoord.x;
				mTexCoordArray[2 * mSize + 1] = (*animated_vertex)->mVertex.TexCoord.y;

				mTangentArray[3 * mSize] = (*animated_vertex)->mVertex.Tangent.x;
				mTangentArray[3 * mSize + 1] = (*animated_vertex)->mVertex.Tangent.y;
				mTangentArray[3 * mSize + 2] = (*animated_vertex)->mVertex.Tangent.z;

				mBitangentArray[3 * mSize] = (*animated_vertex)->mVertex.Bitangent.x;
				mBitangentArray[3 * mSize + 1] = (*animated_vertex)->mVertex.Bitangent.y;
				mBitangentArray[3 * mSize + 2] = (*animated_vertex)->mVertex.Bitangent.z;

				mNormalArray[3 * mSize] = (*animated_vertex)->mVertex.Normal.x;
				mNormalArray[3 * mSize + 1] = (*animated_vertex)->mVertex.Normal.y;
				mNormalArray[3 * mSize + 2] = (*animated_vertex)->mVertex.Normal.z;

				sort((*animated_vertex)->mSkinWeights.begin(), (*animated_vertex)->mSkinWeights.end(), SortDecreasing);
				switch ((*animated_vertex)->mSkinWeights.size())
				{
				case 0:
				{
					mSkinNodeArray[4 * mSize] = -1;
					mSkinWeightArray[4 * mSize] = 0.0f;

					mSkinNodeArray[4 * mSize + 1] = -1;
					mSkinWeightArray[4 * mSize + 1] = 0.0f;

					mSkinNodeArray[4 * mSize + 2] = -1;
					mSkinWeightArray[4 * mSize + 2] = 0.0f;

					mSkinNodeArray[4 * mSize + 3] = -1;
					mSkinWeightArray[4 * mSize + 3] = 0.0f;
				}
				break;
				case 1:
				{
					mSkinNodeArray[4 * mSize] = (*animated_vertex)->mSkinWeights[0]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize] = (*animated_vertex)->mSkinWeights[0]->mWeight;

					mSkinNodeArray[4 * mSize + 1] = -1;
					mSkinWeightArray[4 * mSize + 1] = 0.0f;

					mSkinNodeArray[4 * mSize + 2] = -1;
					mSkinWeightArray[4 * mSize + 2] = 0.0f;

					mSkinNodeArray[4 * mSize + 3] = -1;
					mSkinWeightArray[4 * mSize + 3] = 0.0f;
				}
				break;
				case 2:
				{
					mSkinNodeArray[4 * mSize] = (*animated_vertex)->mSkinWeights[0]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize] = (*animated_vertex)->mSkinWeights[0]->mWeight;

					mSkinNodeArray[4 * mSize + 1] = (*animated_vertex)->mSkinWeights[1]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize + 1] = (*animated_vertex)->mSkinWeights[1]->mWeight;

					mSkinNodeArray[4 * mSize + 2] = -1;
					mSkinWeightArray[4 * mSize + 2] = 0.0f;

					mSkinNodeArray[4 * mSize + 3] = -1;
					mSkinWeightArray[4 * mSize + 3] = 0.0f;
				}
				break;
				case 3:
				{
					mSkinNodeArray[4 * mSize] = (*animated_vertex)->mSkinWeights[0]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize] = (*animated_vertex)->mSkinWeights[0]->mWeight;

					mSkinNodeArray[4 * mSize + 1] = (*animated_vertex)->mSkinWeights[1]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize + 1] = (*animated_vertex)->mSkinWeights[1]->mWeight;

					mSkinNodeArray[4 * mSize + 2] = (*animated_vertex)->mSkinWeights[2]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize + 2] = (*animated_vertex)->mSkinWeights[2]->mWeight;

					mSkinNodeArray[4 * mSize + 3] = -1;
					mSkinWeightArray[4 * mSize + 3] = 0.0f;
				}
				break;
				case 4:
				{
					mSkinNodeArray[4 * mSize] = (*animated_vertex)->mSkinWeights[0]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize] = (*animated_vertex)->mSkinWeights[0]->mWeight;

					mSkinNodeArray[4 * mSize + 1] = (*animated_vertex)->mSkinWeights[1]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize + 1] = (*animated_vertex)->mSkinWeights[1]->mWeight;

					mSkinNodeArray[4 * mSize + 2] = (*animated_vertex)->mSkinWeights[2]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize + 2] = (*animated_vertex)->mSkinWeights[2]->mWeight;

					mSkinNodeArray[4 * mSize + 3] = (*animated_vertex)->mSkinWeights[3]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize + 3] = (*animated_vertex)->mSkinWeights[3]->mWeight;
				}
				break;
				default:
				{
					float w0 = (*animated_vertex)->mSkinWeights[0]->mWeight;
					float w1 = (*animated_vertex)->mSkinWeights[1]->mWeight;
					float w2 = (*animated_vertex)->mSkinWeights[2]->mWeight;
					float w3 = (*animated_vertex)->mSkinWeights[2]->mWeight;
					float sum = w0 + w1 + w2 + w3;

					mSkinNodeArray[4 * mSize] = (*animated_vertex)->mSkinWeights[0]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize] = w0 / sum;

					mSkinNodeArray[4 * mSize + 1] = (*animated_vertex)->mSkinWeights[1]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize + 1] = w1 / sum;

					mSkinNodeArray[4 * mSize + 2] = (*animated_vertex)->mSkinWeights[2]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize + 2] = w2 / sum;

					mSkinNodeArray[4 * mSize + 3] = (*animated_vertex)->mSkinWeights[3]->mSkinWeights->mSkin->mIndex;
					mSkinWeightArray[4 * mSize + 3] = w3 / sum;
				}
				break;
				}

				++mSize;
			}
		}

		void MeshAnimatedVertices::RenderNormals(Camera* camera)
		{
			if (!mObject->mVisible)
			{
				return;
			}

			glDisable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glDisable(GL_COLOR_MATERIAL);
			glDisable(GL_TEXTURE_2D);

			glColor3f(0.0f, 0.0f, 1.0f);

			glBegin(GL_LINES);

			for (unsigned int i = 0; i < mSize; ++i)
			{
				glm::vec3 p(mFinalPositionArray[3 * i], mFinalPositionArray[3 * i + 1], mFinalPositionArray[3 * i + 2]);

				float s;
				if (camera->GetProjectionModel() == Camera::Ortho)
				{
					s = 2.0f * (50.0f / camera->GetViewport()[3]) * (1.0f / camera->GetZoom()) * (camera->GetDistance() / camera->GetUnitaryDistance());
				}
				else
				{
					s = (50.0f / camera->GetViewport()[3]) * (camera->GetDistanceTo(p) / camera->GetUnitaryDistance());
				}

				glVertex3f(mFinalPositionArray[3 * i], mFinalPositionArray[3 * i + 1], mFinalPositionArray[3 * i + 2]);
				glVertex3f(mFinalPositionArray[3 * i] + s * mFinalNormalArray[3 * i], mFinalPositionArray[3 * i + 1] + s * mFinalNormalArray[3 * i + 1], mFinalPositionArray[3 * i + 2] + s * mFinalNormalArray[3 * i + 2]);
			}

			glEnd();
		}

		void MeshAnimatedVertices::RenderTangents(Camera* camera)
		{
			if (!mObject->mVisible)
			{
				return;
			}

			glDisable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glDisable(GL_COLOR_MATERIAL);
			glDisable(GL_TEXTURE_2D);

			glBegin(GL_LINES);

			for (unsigned int i = 0; i < mSize; ++i)
			{
				glm::vec3 p(mFinalPositionArray[3 * i], mFinalPositionArray[3 * i + 1], mFinalPositionArray[3 * i + 2]);

				float s;
				if (camera->GetProjectionModel() == Camera::Ortho)
				{
					s = 2.0f * (50.0f / camera->GetViewport()[3]) * (1.0f / camera->GetZoom()) * (camera->GetDistance() / camera->GetUnitaryDistance());
				}
				else
				{
					s = (50.0f / camera->GetViewport()[3]) * (camera->GetDistanceTo(p) / camera->GetUnitaryDistance());
				}

				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(mFinalPositionArray[3 * i], mFinalPositionArray[3 * i + 1], mFinalPositionArray[3 * i + 2]);
				glVertex3f(mFinalPositionArray[3 * i] + s * mFinalTangentArray[3 * i], mFinalPositionArray[3 * i + 1] + s * mFinalTangentArray[3 * i + 1], mFinalPositionArray[3 * i + 2] + s * mFinalTangentArray[3 * i + 2]);

				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(mFinalPositionArray[3 * i], mFinalPositionArray[3 * i + 1], mFinalPositionArray[3 * i + 2]);
				glVertex3f(mFinalPositionArray[3 * i] + s * mFinalBitangentArray[3 * i], mFinalPositionArray[3 * i + 1] + s * mFinalBitangentArray[3 * i + 1], mFinalPositionArray[3 * i + 2] + s * mFinalBitangentArray[3 * i + 2]);
			}

			glEnd();
		}
	}
}
