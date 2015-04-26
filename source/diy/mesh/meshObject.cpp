#include <diy/diy.hpp>

#include <glm/ext.hpp>

#include <gl/glew.h>

GLint inflateProgram;
float factor;

namespace diy
{
	namespace mesh
	{
		MeshObject::MeshObject(MeshObjects* objects, int index, std::string name) :
			mObjects(objects),
			mNode(0),
			mOffsetMatrix(1.0f),
			mMatrix(1.0f),
			mNormalMatrix(1.0f),
			mOrigin(0.0f),
			mIndex(index),
			mName(name),
			mComment(),
			mMinLimits(0.0f),
			mMaxLimits(0.0f),
			mCenter(0.0f),
			mVisible(true)
		{
			mAnimatedVertices = new MeshAnimatedVertices(this);
			mFaces = new MeshFaces(this);
			mGroups = new MeshGroups(this);
			mSkins = new MeshSkins(this);
			mMorphTargets = new MeshMorphTargets(this);
		}

		MeshObject::~MeshObject(void)
		{
			if (mNode)
			{
				mNode->RemoveObject(this);
			}

			delete mMorphTargets;
			delete mSkins;
			delete mGroups;
			delete mFaces;
			delete mAnimatedVertices;
		}

		MeshObjects* MeshObject::GetObjects(void)
		{
			return mObjects;
		}

		MeshNode* MeshObject::GetNode(void)
		{
			return mNode;
		}

		MeshAnimatedVertices* MeshObject::GetAnimatedVertices(void)
		{
			return mAnimatedVertices;
		}

		int MeshObject::GetIndex(void)
		{
			return mIndex;
		}

		std::string MeshObject::GetName(void)
		{
			return mName;
		}

		MeshFaces* MeshObject::GetFaces(void)
		{
			return mFaces;
		}

		MeshGroups* MeshObject::GetGroups(void)
		{
			return mGroups;
		}

		MeshSkins* MeshObject::GetSkins(void)
		{
			return mSkins;
		}

		MeshMorphTargets* MeshObject::GetMorphTargets(void)
		{
			return mMorphTargets;
		}

		bool MeshObject::SetName(std::string name)
		{
			if (mObjects->Find(name))
			{
				return false;
			}

			mName = name;

			return true;
		}

		void MeshObject::SetNode(MeshNode* node)
		{
			if (mNode)
			{
				mNode->RemoveObject(this);
			}

			if (node)
			{
				node->AppendObject(this);
			}
		}

		void MeshObject::SetNode(std::string nodeName)
		{
			SetNode(mObjects->mFile->mNodes->Find(nodeName));
		}

		void MeshObject::SetNode(int nodeIndex)
		{
			SetNode(mObjects->mFile->mNodes->operator[](nodeIndex));
		}

		void MeshObject::Update(void)
		{
			if (!mVisible)
			{
				return;
			}

			if (mNode)
			{
				mMatrix = mNode->mModelMatrix * mOffsetMatrix;
				mNormalMatrix = glm::mat3(mMatrix);
				mOrigin = glm::vec3(mMatrix[3]);
			}

			for (std::vector<MeshSkin*>::iterator skin_iterator = mSkins->mSkins.begin(); skin_iterator != mSkins->mSkins.end(); ++skin_iterator)
			{
				MeshSkin* skin = *skin_iterator;
				if (skin->mNode)
				{
					skin->mMatrix = skin->mNode->mModelMatrix * skin->OffsetMatrix;
					skin->mNormalMatrix = glm::mat3(skin->mMatrix);
					skin->mOrigin = glm::vec3(skin->mMatrix[3]);
				}
				else
				{
					skin->mMatrix = glm::mat4(1.0f);
					skin->mNormalMatrix = glm::mat3(1.0f);
					skin->mOrigin = glm::vec3(0.0f);
				}
			}

			memcpy(mAnimatedVertices->mMorphedPositionArray, mAnimatedVertices->mPositionArray, 12 * mAnimatedVertices->mSize);
			for (std::vector<MeshMorphTarget*>::iterator morph_target = mMorphTargets->mMorphTargets.begin(); morph_target != mMorphTargets->mMorphTargets.end(); ++morph_target)
			{
				float blendFactor = (*morph_target)->mBlendFactor;

				for (std::vector<MeshMorphTargetVertex*>::iterator morph_target_vertex = (*morph_target)->mMorphTargetVertices->mMorphTargetVertices.begin(); morph_target_vertex != (*morph_target)->mMorphTargetVertices->mMorphTargetVertices.end(); ++morph_target_vertex)
				{
					float* morphed_position = mAnimatedVertices->mMorphedPositionArray + 3 * (*morph_target_vertex)->mAnimatedVertex->mIndex;
					float* delta_position = glm::value_ptr((*morph_target_vertex)->mDeltaVertex.Position);
					add_eq_3fv_3fv_f(morphed_position, delta_position, blendFactor);
				}
			}

			if (mSkins->mSkins.size())
			{
				memset(mAnimatedVertices->mFinalPositionArray, 0, 12 * mAnimatedVertices->mSize);
				memset(mAnimatedVertices->mFinalTangentArray, 0, 12 * mAnimatedVertices->mSize);
				memset(mAnimatedVertices->mFinalBitangentArray, 0, 12 * mAnimatedVertices->mSize);
				memset(mAnimatedVertices->mFinalNormalArray, 0, 12 * mAnimatedVertices->mSize);
				for (std::vector<MeshSkin*>::iterator skin = mSkins->mSkins.begin(); skin != mSkins->mSkins.end(); ++skin)
				{
					float* matrix = glm::value_ptr((*skin)->mMatrix);
					float* normal_matrix = glm::value_ptr((*skin)->mNormalMatrix);
					for (std::vector<MeshSkinWeight*>::iterator skin_weight = (*skin)->mSkinWeights->mSkinWeights.begin(); skin_weight != (*skin)->mSkinWeights->mSkinWeights.end(); ++skin_weight)
					{
						float* final_position = mAnimatedVertices->mFinalPositionArray + 3 * (*skin_weight)->mAnimatedVertex->mIndex;
						float weight = (*skin_weight)->mWeight;
						float* morphed_position = mAnimatedVertices->mMorphedPositionArray + 3 * (*skin_weight)->mAnimatedVertex->mIndex;

						add_eq_3fv_mul_1f_mul_16fv_3fv(final_position, weight, matrix, morphed_position);

						float* final_tangent = mAnimatedVertices->mFinalTangentArray + 3 * (*skin_weight)->mAnimatedVertex->mIndex;
						float* tangent = mAnimatedVertices->mTangentArray + 3 * (*skin_weight)->mAnimatedVertex->mIndex;

						add_eq_3fv_mul_1f_mul_9fv_3fv(final_tangent, weight, normal_matrix, tangent);

						float* final_bitangent = mAnimatedVertices->mFinalBitangentArray + 3 * (*skin_weight)->mAnimatedVertex->mIndex;
						float* bitangent = mAnimatedVertices->mBitangentArray + 3 * (*skin_weight)->mAnimatedVertex->mIndex;

						add_eq_3fv_mul_1f_mul_9fv_3fv(final_bitangent, weight, normal_matrix, bitangent);

						float* final_normal = mAnimatedVertices->mFinalNormalArray + 3 * (*skin_weight)->mAnimatedVertex->mIndex;
						float* normal = mAnimatedVertices->mNormalArray + 3 * (*skin_weight)->mAnimatedVertex->mIndex;

						add_eq_3fv_mul_1f_mul_9fv_3fv(final_normal, weight, normal_matrix, normal);
					}
				}
			}
			else if (mNode)
			{
				float* matrix = glm::value_ptr(mMatrix);
				float* normal_matrix = glm::value_ptr(mNormalMatrix);
				for (unsigned int i = 0; i < mAnimatedVertices->mSize; ++i)
				{
					float* final_position = mAnimatedVertices->mFinalPositionArray + 3 * i;
					float* morphed_position = mAnimatedVertices->mMorphedPositionArray + 3 * i;

					eq_3fv_mul_16fv_3fv(final_position, matrix, morphed_position);

					float* final_tangent = mAnimatedVertices->mFinalTangentArray + 3 * i;
					float* tangent = mAnimatedVertices->mTangentArray + 3 * i;

					eq_3fv_mul_9fv_3fv(final_tangent, normal_matrix, tangent);

					float* final_bitangent = mAnimatedVertices->mFinalBitangentArray + 3 * i;
					float* bitangent = mAnimatedVertices->mBitangentArray + 3 * i;

					eq_3fv_mul_9fv_3fv(final_bitangent, normal_matrix, bitangent);

					float* final_normal = mAnimatedVertices->mFinalNormalArray + 3 * i;
					float* normal = mAnimatedVertices->mNormalArray + 3 * i;

					eq_3fv_mul_9fv_3fv(final_normal, normal_matrix, normal);
				}
			}
			else
			{
				if (mMorphTargets->mMorphTargets.size())
				{
					memcpy(mAnimatedVertices->mFinalPositionArray, mAnimatedVertices->mMorphedPositionArray, 12 * mAnimatedVertices->mSize);
				}
				else
				{
					memcpy(mAnimatedVertices->mFinalPositionArray, mAnimatedVertices->mPositionArray, 12 * mAnimatedVertices->mSize);
				}
				memcpy(mAnimatedVertices->mFinalTangentArray, mAnimatedVertices->mTangentArray, 12 * mAnimatedVertices->mSize);
				memcpy(mAnimatedVertices->mFinalBitangentArray, mAnimatedVertices->mBitangentArray, 12 * mAnimatedVertices->mSize);
				memcpy(mAnimatedVertices->mFinalNormalArray, mAnimatedVertices->mNormalArray, 12 * mAnimatedVertices->mSize);
			}
		}

		void MeshObject::Freeze(void)
		{
			Update();

			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = mAnimatedVertices->mAnimatedVertices.begin(); animated_vertex != mAnimatedVertices->mAnimatedVertices.end(); ++animated_vertex)
			{
				(*animated_vertex)->mVertex = (*animated_vertex)->GetFinalVertex();
			}
			mAnimatedVertices->UpdateArray();

			mSkins->Clear();
			mMorphTargets->Clear();

			if (mNode)
			{
				mNode->RemoveObject(this);
			}
		}

		void MeshObject::Mirror(int mirror_axis)
		{
			glm::vec3 scale;
			switch (mirror_axis)
			{
			case OBJECT_MIRROR_X: scale = glm::vec3(-1.0f, 1.0f, 1.0f); break;
			case OBJECT_MIRROR_Y: scale = glm::vec3(1.0f, -1.0f, 1.0f); break;
			case OBJECT_MIRROR_Z: scale = glm::vec3(1.0f, 1.0f, -1.0f); break;
			default: return;
			}

			int vertexCount = mAnimatedVertices->mAnimatedVertices.size();
			for (int i = 0; i < vertexCount; ++i)
			{
				MeshVertex vertex = mAnimatedVertices->mAnimatedVertices[i]->mVertex;
				vertex.Position *= scale;
				mAnimatedVertices->New(vertex);
			}

			int faceCount = mFaces->mFaces.size();
			for (int i = 0; i < faceCount; ++i)
			{
				std::vector<int> vertexIndices;
				MeshFace* face = mFaces->mFaces[i];
				for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = face->mAnimatedVertices.begin(); animated_vertex != face->mAnimatedVertices.end(); ++animated_vertex)
				{
					vertexIndices.push_back((*animated_vertex)->mIndex + vertexCount);
				}
				face = mFaces->New(face->mGroup, vertexIndices);
				face->Reverse();
			}

			UpdateArrays();
		}

		bool MeshObject::operator==(MeshObject object)
		{
			return !_stricmp(mName.c_str(), object.mName.c_str());
		}

		glm::vec3 MeshObject::GetMinLimits(void)
		{
			return mMinLimits;
		}

		glm::vec3 MeshObject::GetMaxLimits(void)
		{
			return mMaxLimits;
		}

		glm::vec3 MeshObject::GetCenter(void)
		{
			return mCenter;
		}

		void MeshObject::UpdateLimits(glm::vec3 reference)
		{
			for (int i = 0; i < 3; ++i)
			{
				if (reference[i] < mMinLimits[i])
				{
					mMinLimits[i] = reference[i];
				}
				else if (reference[i] > mMaxLimits[i])
				{
					mMaxLimits[i] = reference[i];
				}
			}

			mCenter = 0.5f * (mMinLimits + mMaxLimits);

			mObjects->UpdateLimits(reference);
		}

		void MeshObject::Render(MeshMaterial* material)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, mAnimatedVertices->mFinalPositionArray);
			glTexCoordPointer(2, GL_FLOAT, 0, mAnimatedVertices->mTexCoordArray);
			glNormalPointer(GL_FLOAT, 0, mAnimatedVertices->mFinalNormalArray);

			if (material)
			{
				material->Apply();

				if (material->IsTransparent())
				{
					glFrontFace(GL_CW);
					glDrawElements(GL_TRIANGLES, 3 * mFaces->mSize, GL_UNSIGNED_INT, mFaces->mIndexArray);
					glFrontFace(GL_CCW);
				}

				if (material->Contour)
				{
					glUseProgram(inflateProgram);
					glFrontFace(GL_CW);

					glUniform1f(glGetUniformLocation(inflateProgram, "factor"), material->ContourThickness / factor);
					glUniform4fv(glGetUniformLocation(inflateProgram, "color"), 1, glm::value_ptr(material->ContourColor));

					glDrawElements(GL_TRIANGLES, 3 * mFaces->mSize, GL_UNSIGNED_INT, mFaces->mIndexArray);

					glFrontFace(GL_CCW);
					glUseProgram(0);
				}
			}
			glDrawElements(GL_TRIANGLES, 3 * mFaces->mSize, GL_UNSIGNED_INT, mFaces->mIndexArray);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);

			glDisable(GL_LIGHTING);
			glDisable(GL_COLOR_MATERIAL);
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
		}

		void MeshObject::RenderOpaque(void)
		{
			if (!mVisible)
			{
				return;
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, mAnimatedVertices->mFinalPositionArray);
			glTexCoordPointer(2, GL_FLOAT, 0, mAnimatedVertices->mTexCoordArray);
			glNormalPointer(GL_FLOAT, 0, mAnimatedVertices->mFinalNormalArray);

			for (std::vector<MeshGroup*>::iterator group = mGroups->mGroups.begin(); group != mGroups->mGroups.end(); ++group)
			{
				if ((*group)->mMaterial)
				{
					if ((*group)->mMaterial->IsTransparent())
					{
						continue;
					}

					(*group)->mMaterial->Apply();
				}
				else
				{
					glDisable(GL_COLOR_MATERIAL);
					glDisable(GL_LIGHTING);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);

					glColor3f(1.0f, 1.0f, 1.0f);
				}

				glDrawElements(GL_TRIANGLES, 3 * (*group)->mFaces.size(), GL_UNSIGNED_INT, mFaces->mIndexArray + 3 * (*group)->mFirstFace);

				if ((*group)->mMaterial)
				{
					if ((*group)->mMaterial->Contour)
					{
						glUseProgram(inflateProgram);
						glFrontFace(GL_CW);

						glUniform1f(glGetUniformLocation(inflateProgram, "factor"), (*group)->mMaterial->ContourThickness / factor);
						glUniform4fv(glGetUniformLocation(inflateProgram, "color"), 1, glm::value_ptr((*group)->mMaterial->ContourColor));

						glDrawElements(GL_TRIANGLES, 3 * (*group)->mFaces.size(), GL_UNSIGNED_INT, mFaces->mIndexArray + 3 * (*group)->mFirstFace);

						glFrontFace(GL_CCW);
						glUseProgram(0);
					}
				}
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);

			glDisable(GL_LIGHTING);
			glDisable(GL_COLOR_MATERIAL);
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
		}

		void MeshObject::RenderTransparent(void)
		{
			if (!mVisible)
			{
				return;
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, mAnimatedVertices->mFinalPositionArray);
			glTexCoordPointer(2, GL_FLOAT, 0, mAnimatedVertices->mTexCoordArray);
			glNormalPointer(GL_FLOAT, 0, mAnimatedVertices->mFinalNormalArray);

			for (std::vector<MeshGroup*>::iterator group = mGroups->mGroups.begin(); group != mGroups->mGroups.end(); ++group)
			{
				if ((*group)->mMaterial)
				{
					if ((*group)->mMaterial->IsOpaque())
					{
						continue;
					}

					(*group)->mMaterial->Apply();
				}
				else
				{
					continue;
				}

				glDrawElements(GL_TRIANGLES, 3 * (*group)->mFaces.size(), GL_UNSIGNED_INT, mFaces->mIndexArray + 3 * (*group)->mFirstFace);

				if ((*group)->mMaterial)
				{
					if ((*group)->mMaterial->mMode != MeshMaterial::MODE_NORMAL)
					{
						glDepthMask(GL_FALSE);
					}

					if ((*group)->mMaterial->Contour)
					{
						glUseProgram(inflateProgram);
						glFrontFace(GL_CW);

						glUniform1f(glGetUniformLocation(inflateProgram, "factor"), (*group)->mMaterial->ContourThickness / factor);
						glUniform4fv(glGetUniformLocation(inflateProgram, "color"), 1, glm::value_ptr((*group)->mMaterial->ContourColor));

						glDrawElements(GL_TRIANGLES, 3 * (*group)->mFaces.size(), GL_UNSIGNED_INT, mFaces->mIndexArray + 3 * (*group)->mFirstFace);

						glFrontFace(GL_CCW);
						glUseProgram(0);
					}

					if ((*group)->mMaterial->mMode != MeshMaterial::MODE_NORMAL)
					{
						glDepthMask(GL_TRUE);
					}
				}
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);

			glDisable(GL_LIGHTING);
			glDisable(GL_COLOR_MATERIAL);
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
		}

		void MeshObject::Render(class Camera* camera)
		{
			if (!mVisible)
			{
				return;
			}

			for (std::vector<MeshGroup*>::iterator group = mGroups->mGroups.begin(); group != mGroups->mGroups.end(); ++group)
			{
				(*group)->Render(camera);
			}
		}

		bool MeshObject::IsVisible(void)
		{
			return mVisible;
		}

		void MeshObject::Show(void)
		{
			mVisible = true;

			Update();
		}

		void MeshObject::Hide(void)
		{
			mVisible = false;

			Update();
		}

		void MeshObject::ToggleVisible(void)
		{
			mVisible = !mVisible;

			Update();
		}

		void MeshObject::UpdateArrays(void)
		{
			mAnimatedVertices->UpdateArray();
			mFaces->UpdateArray();
			mSkins->UpdateArrays();
			mMorphTargets->UpdateArrays();

			Update();
		}

		bool SortObjectsIncreasingRespectName(class MeshObject* object_a, class MeshObject* object_b)
		{
			return strcmp(object_a->mName.c_str(), object_b->mName.c_str()) < 0;
		}
	}
}