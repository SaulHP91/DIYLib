#include <diy/diy.hpp>

#include "joint.c"
#include "link.c"

#include <cmath>

#include <gl/glew.h>
#include <glm/own.hpp>

namespace diy
{
	namespace mesh
	{

		unsigned int MeshNode::mJointPositionBuffer;
		unsigned int MeshNode::mJointNormalBuffer;
		unsigned int MeshNode::mJointFaceBuffer;

		unsigned int MeshNode::mLinkPositionBuffer;
		unsigned int MeshNode::mLinkNormalBuffer;
		unsigned int MeshNode::mLinkFaceBuffer;

		Shader MeshNode::mShader;

		bool MeshNode::mInitializedBuffersAndShaders = false;

		MeshNode::MeshNode(MeshNodes* nodes, int index, std::string name, glm::mat4 matrix) :
			mNodes(nodes),
			mIndex(index),
			mName(name),
			mComment(),
			mParent(0),
			mRigidBody(0),
			mMatrix(matrix),
			mAdditionalMatrix(1.0f),
			mPivot(0.0f),
			mFocused(false),
			mDepth(0)
		{
			mMatrix = matrix;
			mOldFinalMatrix = mMatrix;
			mFinalMatrix = mMatrix;
			mCombinedMatrix = mMatrix;
			mOffsetMatrix = glm::inverse(mMatrix);
			mNormalMatrix = glm::mat3(glm::transpose(glm::inverse(mMatrix)));
			mPivot = glm::vec3(mMatrix[3]);

			mSelectableJoint = mNodes->mSelector->New<SelectableMesh>();
			mSelectableJoint->SetVertexCount(joint_vertices);
			mSelectableJoint->SetVerticesPointer(joint_v);
			mSelectableJoint->SetFaceCount(joint_faces);
			mSelectableJoint->SetFacesPointer(joint_f);
			mSelectableJoint->SetData(this);

			mSelectableLink = mNodes->mSelector->New<SelectableMesh>();
			mSelectableLink->SetVertexCount(link_vertices);
			mSelectableLink->SetVerticesPointer(link_v);
			mSelectableLink->SetFaceCount(link_faces);
			mSelectableLink->SetFacesPointer(link_f);
		}

		MeshNode::~MeshNode(void)
		{
			if (mParent)
			{
				mParent->RemoveChild(this);
			}
			if (mRigidBody)
			{
				mRigidBody->SetNode((MeshNode*)0);
			}

			while (mChildren.size())
			{
				RemoveChild(mChildren.front());
			}
			mChildren.swap(std::vector<MeshNode*>(mChildren));

			while (mObjects.size())
			{
				RemoveObject(mObjects.front());
			}
			mObjects.swap(std::vector<MeshObject*>(mObjects));

			while (mSkins.size())
			{
				RemoveSkin(mSkins.front());
			}
			mSkins.swap(std::vector<MeshSkin*>(mSkins));
		}

		MeshNodes* MeshNode::Nodes(void)
		{
			return mNodes;
		}

		MeshNode* MeshNode::GetParent(void)
		{
			return mParent;
		}

		std::vector<MeshNode*> MeshNode::GetChildren(void)
		{
			return mChildren;
		}

		std::vector<MeshObject*> MeshNode::GetObjects(void)
		{
			return mObjects;
		}

		void MeshNode::SetMatrix(glm::mat4 matrix, bool linked, bool update)
		{
			if (!linked)
			{
				glm::mat4 M = glm::inverse(mAdditionalMatrix) * glm::inverse(matrix) * mMatrix * mAdditionalMatrix;
				for (std::vector<MeshNode*>::iterator node = mChildren.begin(); node != mChildren.end(); ++node)
				{
					(*node)->mMatrix = M * (*node)->mMatrix;
				}
			}

			glm::mat4 A = glm::inverse(mMatrix) * matrix;
			mMatrix = matrix;

			if (SymmetryMode != NODE_SYMMETRY_NONE)
			{
				MeshNode* sym = mNodes->FindBySymmetry(this);
				if (sym)
				{
					glm::vec3 T = glm::matrixToTranslation(A);
					glm::vec3 R = glm::matrixToRotation(A);
					glm::vec3 S = glm::matrixToScaling(A);

					glm::vec3 factor(SymmetryXAxis, SymmetryYAxis, SymmetryZAxis);
					if (SymmetryMode == NODE_SYMMETRY_MIRROR)
					{
						factor *= -1.0f;
					}
					T *= factor;
					R *= factor;

					A = glm::compose(T, R, S);
					matrix = sym->mMatrix * A;

					if (!linked)
					{
						glm::mat4 M = glm::inverse(sym->mAdditionalMatrix) * glm::inverse(matrix) * sym->mMatrix * sym->mAdditionalMatrix;
						for (std::vector<MeshNode*>::iterator node = sym->mChildren.begin(); node != sym->mChildren.end(); ++node)
						{
							(*node)->mMatrix = M * (*node)->mMatrix;
						}
					}

					sym->mMatrix = matrix;
				}
			}

			mNodes->Update();

			for (std::vector<MeshNode*>::iterator node = mNodes->mNodes.begin(); node != mNodes->mNodes.end(); ++node)
			{
				glm::mat4 M = glm::inverse((*node)->mFinalMatrix) * (*node)->mOldFinalMatrix;

				for (std::vector<MeshObject*>::iterator object = (*node)->mObjects.begin(); object != (*node)->mObjects.end(); ++object)
				{
					(*object)->mOffsetMatrix = M * (*object)->mOffsetMatrix;
				}

				for (std::vector<MeshSkin*>::iterator skin = (*node)->mSkins.begin(); skin != (*node)->mSkins.end(); ++skin)
				{
					(*skin)->OffsetMatrix = M * (*skin)->OffsetMatrix;
				}
			}

			if (update)
			{
				mNodes->mFile->mObjects->Update();
			}
		}

		void MeshNode::SetAdditionalMatrix(glm::mat4 additionalMatrix, bool linked, bool update)
		{
			if (!linked)
			{
				glm::mat4 M = glm::inverse(additionalMatrix) * mAdditionalMatrix;
				for (std::vector<MeshNode*>::iterator node = mChildren.begin(); node != mChildren.end(); ++node)
				{
					(*node)->mAdditionalMatrix = glm::inverse((*node)->mMatrix) * M * (*node)->mMatrix * (*node)->mAdditionalMatrix;
				}
			}

			glm::mat4 A = glm::inverse(mAdditionalMatrix) * additionalMatrix;
			mAdditionalMatrix = additionalMatrix;

			if (SymmetryMode != NODE_SYMMETRY_NONE)
			{
				MeshNode* sym = mNodes->FindBySymmetry(this);
				if (sym)
				{
					glm::vec3 T = glm::matrixToTranslation(A);
					glm::vec3 R = glm::matrixToRotation(A);
					glm::vec3 S = glm::matrixToScaling(A);

					glm::vec3 factor(SymmetryXAxis, SymmetryYAxis, SymmetryZAxis);
					if (SymmetryMode == NODE_SYMMETRY_MIRROR)
					{
						factor *= -1.0f;
					}
					T *= factor;
					R *= factor;

					A = glm::compose(T, R, S);
					additionalMatrix = sym->mAdditionalMatrix * A;

					if (!linked)
					{
						glm::mat4 M = glm::inverse(additionalMatrix) * sym->mAdditionalMatrix;
						for (std::vector<MeshNode*>::iterator node = sym->mChildren.begin(); node != sym->mChildren.end(); ++node)
						{
							(*node)->mAdditionalMatrix = glm::inverse((*node)->mMatrix) * M * (*node)->mMatrix * (*node)->mAdditionalMatrix;
						}
					}

					sym->mAdditionalMatrix = additionalMatrix;
				}
			}

			mNodes->Update();

			if (update)
			{
				mNodes->mFile->mObjects->Update();
			}
		}

		glm::mat4 MeshNode::GetMatrix(void)
		{
			return mMatrix;
		}

		glm::mat4 MeshNode::GetAdditionalMatrix(void)
		{
			return mAdditionalMatrix;
		}

		glm::mat4 MeshNode::GetFinalMatrix(void)
		{
			return mFinalMatrix;
		}

		glm::mat4 MeshNode::GetOffsetMatrix(void)
		{
			return mOffsetMatrix;
		}

		int MeshNode::GetIndex(void)
		{
			return mIndex;
		}

		std::string MeshNode::GetName(void)
		{
			return mName;
		}

		bool MeshNode::SetName(std::string name)
		{
			if (mNodes->Find(name))
			{
				return false;
			}

			mName = name;

			return true;
		}

		void MeshNode::SetParent(MeshNode* parentNode, bool update)
		{
			if (parentNode)
			{
				if (parentNode->mIndex >= mIndex)
				{
					return;
				}
			}

			if (mParent)
			{
				mParent->RemoveChild(this);
			}

			mParent = parentNode;

			if (mParent)
			{
				mParent->mChildren.push_back(this);
			}

			mNodes->Update();

			if (update)
			{
				mNodes->mFile->mObjects->Update();
			}

			mSelectableLink->SetData(mParent);
		}

		void MeshNode::SetParent(std::string parentNodeName, bool update)
		{
			SetParent(mNodes->Find(parentNodeName), update);
		}

		void MeshNode::SetParent(int parentNodeIndex, bool update)
		{
			SetParent(mNodes->operator[](parentNodeIndex), update);
		}

		void MeshNode::AppendChild(MeshNode* childNode, bool update)
		{
			if (childNode)
			{
				childNode->SetParent(this, update);
			}
		}

		void MeshNode::AppendChild(std::string childNodeName, bool update)
		{
			AppendChild(mNodes->Find(childNodeName), update);
		}

		void MeshNode::AppendChild(int childNodeIndex, bool update)
		{
			AppendChild(mNodes->operator [](childNodeIndex), update);
		}

		void MeshNode::RemoveChild(MeshNode* childNode, bool update)
		{
			std::vector<MeshNode*>::iterator node = find(mChildren.begin(), mChildren.end(), childNode);

			if (node == mChildren.end())
			{
				return;

			}

			(*node)->mParent = 0;

			mChildren.erase(node);

			mNodes->Update();

			if (update)
			{
				mNodes->mFile->mObjects->Update();
			}
		}

		void MeshNode::RemoveChild(std::string childNodeName, bool update)
		{
			RemoveChild(mNodes->Find(childNodeName), update);
		}

		void MeshNode::RemoveChild(int childNodeIndex, bool update)
		{
			RemoveChild(mNodes->operator [](childNodeIndex), update);
		}

		void MeshNode::AppendObject(MeshObject* object)
		{
			if (!object)
			{
				return;
			}

			if (object->mNode)
			{
				object->mNode->RemoveObject(object);
			}

			object->mNode = this;

			mObjects.push_back(object);

			object->Update();
		}

		void MeshNode::AppendObject(std::string objectName)
		{
			AppendObject(mNodes->mFile->mObjects->Find(objectName));
		}

		void MeshNode::AppendObject(int objectIndex)
		{
			AppendObject(mNodes->mFile->mObjects->operator [](objectIndex));
		}

		void MeshNode::RemoveObject(MeshObject* object)
		{
			std::vector<MeshObject*>::iterator iterator = find(mObjects.begin(), mObjects.end(), object);

			if (iterator == mObjects.end())
			{
				return;
			}

			(*iterator)->mNode = 0;
			(*iterator)->mOffsetMatrix = glm::mat4(1.0f);
			(*iterator)->Update();

			mObjects.erase(iterator);
		}

		void MeshNode::RemoveObject(std::string objectName)
		{
			RemoveObject(mNodes->mFile->mObjects->Find(objectName));
		}

		void MeshNode::RemoveObject(int objectIndex)
		{
			RemoveObject(mNodes->mFile->mObjects->operator [](objectIndex));
		}

		std::vector<class MeshSkin*> MeshNode::GetSkins(void)
		{
			return mSkins;
		}

		void MeshNode::AppendSkin(MeshSkin* skin)
		{
			if (!skin)
			{
				return;
			}

			if (skin->mNode)
			{
				skin->mNode->RemoveSkin(skin);
			}

			skin->mNode = this;
			mSkins.push_back(skin);
		}

		void MeshNode::RemoveSkin(MeshSkin* skin)
		{
			std::vector<MeshSkin*>::iterator iterator = find(mSkins.begin(), mSkins.end(), skin);

			if (iterator == mSkins.end())
			{
				return;
			}

			(*iterator)->mNode = 0;
			mSkins.erase(iterator);
		}

		bool MeshNode::IsInUse(void)
		{
			if (mChildren.size() || mObjects.size() || mSkins.size() || mRigidBody)
			{
				return true;
			}

			for (std::vector<MeshJoint*>::iterator joint = mNodes->mFile->mPhysics->mJoints->mJoints.begin(); joint != mNodes->mFile->mPhysics->mJoints->mJoints.end(); ++joint)
			{
				if ((*joint)->mNode == this)
				{
					return true;
				}
			}

			return false;
		}

		void MeshNode::Render(Camera* camera)
		{
			if (!mInitializedBuffersAndShaders)
			{
				InitializeBuffersAndShaders();
			}
			if (!mInitializedBuffersAndShaders)
			{
				return;
			}

			glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
			glm::mat4 modelViewMatrix = camera->GetFinalMatrix();

			glm::vec3 i = glm::vec3(mFinalMatrix[3]);

			float s = camera->GetScaleFactor(i, 20.0f);

			glm::mat4 jointModelViewMatrix = modelViewMatrix * glm::translate(i) * glm::scale(glm::vec3(s));

			mShader.Begin();

			mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(projectionMatrix * jointModelViewMatrix));
			mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(jointModelViewMatrix)))));
			if (mNodes->mFocused == this)
			{
				mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.0f)));
			}
			else
			{
				mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
			}

			mShader.EnableVertexAttribArray("aPosition");
			mShader.EnableVertexAttribArray("aNormal");

			glBindBuffer(GL_ARRAY_BUFFER, mJointPositionBuffer);
			mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

			glBindBuffer(GL_ARRAY_BUFFER, mJointNormalBuffer);
			mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mJointFaceBuffer);
			glDrawElements(GL_TRIANGLES, 3 * joint_faces, GL_UNSIGNED_INT, 0);

			mShader.DisableVertexAttribArray("aPosition");
			mShader.DisableVertexAttribArray("aNormal");

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			mShader.End();

			if (mParent)
			{
				glm::vec3 ip = glm::vec3(mParent->mFinalMatrix[3]);

				glm::vec3 L = i - ip;

				float l = glm::length(L);

				if (l == 0.0f)
				{
					return;
				}

				s = camera->GetScaleFactor(ip, 20.0f);

				glm::vec2 R = glm::vec2(L.x, L.z);

				float alpha = 0.0f;
				if (L.x == 0.0f)
				{
					if (L.z > 0.0f)
					{
						alpha = 90.0f;
					}
					else if (L.z < 0.0f)
					{
						alpha = -90.0f;
					}
				}
				else
				{
					alpha = glm::degrees(atan2(L.z, L.x));
				}

				float beta = 0.0f;
				if (glm::length(R) == 0.0f)
				{
					if (L.y > 0.0f)
					{
						beta = 90.0f;
					}
					else if (L.y < 0.0f)
					{
						beta = -90.0f;
					}
				}
				else
				{
					beta = glm::degrees(atan2(L.y, glm::length(R)));
				}

				glm::mat4 linkModelViewMatrix = modelViewMatrix * glm::translate(ip) * glm::rotate(glm::radians(alpha), glm::vec3(0.0f, -1.0f, 0.0f)) * glm::rotate(glm::radians(beta), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::vec3(l, s, s));

				mShader.Begin();

				mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(projectionMatrix * linkModelViewMatrix));
				mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(linkModelViewMatrix)))));
				if (mNodes->mFocused == mParent)
				{
					mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.0f)));
				}
				else
				{
					mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
				}

				mShader.EnableVertexAttribArray("aPosition");
				mShader.EnableVertexAttribArray("aNormal");

				glBindBuffer(GL_ARRAY_BUFFER, mLinkPositionBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

				glBindBuffer(GL_ARRAY_BUFFER, mLinkNormalBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mLinkFaceBuffer);
				glDrawElements(GL_TRIANGLES, 3 * link_faces, GL_UNSIGNED_INT, 0);

				mShader.DisableVertexAttribArray("aPosition");
				mShader.DisableVertexAttribArray("aNormal");

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				mShader.End();
			}
		}

		void MeshNode::UpdateSelectable(Camera* camera)
		{
			glm::vec3 i = glm::vec3(mFinalMatrix[3]);

			float s = camera->GetScaleFactor(i, 20.0f);

			mSelectableJoint->SetMatrix(glm::translate(i) * glm::scale(glm::vec3(s)));

			mSelectableLink->SetEnabled(false);
			if (mParent)
			{
				glm::vec3 ip = glm::vec3(mParent->mFinalMatrix[3]);

				glm::vec3 L = i - ip;

				float l = glm::length(L);

				if (l == 0.0f)
				{
					return;
				}

				s = camera->GetScaleFactor(ip, 20.0f);

				glm::vec2 R = glm::vec2(L.x, L.z);

				float alpha = 0.0f;
				if (L.x == 0.0f)
				{
					if (L.z > 0.0f)
					{
						alpha = 90.0f;
					}
					else if (L.z < 0.0f)
					{
						alpha = -90.0f;
					}
				}
				else
				{
					alpha = glm::degrees(atan2(L.z, L.x));
				}

				float beta = 0.0f;
				if (glm::length(R) == 0.0f)
				{
					if (L.y > 0.0f)
					{
						beta = 90.0f;
					}
					else if (L.y < 0.0f)
					{
						beta = -90.0f;
					}
				}
				else
				{
					beta = glm::degrees(atan2(L.y, glm::length(R)));
				}

				mSelectableLink->SetMatrix(glm::translate(ip) * glm::rotate(glm::radians(alpha), glm::vec3(0.0f, -1.0f, 0.0f)) * glm::rotate(glm::radians(beta), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::vec3(l, s, s)));
				mSelectableLink->SetEnabled(true);
			}
		}

		int MeshNode::Depth(void)
		{
			return mDepth;
		}

		void MeshNode::GetAncestorList(std::vector<MeshNode*>& ancestors)
		{
			if (mParent)
			{
				ancestors.push_back(mParent);
				mParent->GetAncestorList(ancestors);
			}
		}

		void MeshNode::GetDescendantList(std::vector<MeshNode*>& descendants)
		{
			for (std::vector<MeshNode*>::iterator node = mChildren.begin(); node != mChildren.end(); ++node)
			{
				descendants.push_back(*node);
				(*node)->GetDescendantList(descendants);
			}
		}

		int MeshNode::SymmetryMode = NODE_SYMMETRY_NONE;
		float MeshNode::SymmetryXAxis = NODE_SYMMETRY_NEGATIVE;
		float MeshNode::SymmetryYAxis = NODE_SYMMETRY_POSITIVE;
		float MeshNode::SymmetryZAxis = NODE_SYMMETRY_POSITIVE;

		void MeshNode::AutoAlign(glm::vec3 normal)
		{
			glm::mat4 matrix;

			if (mChildren.size())
			{
				glm::vec3 i(0.0f);
				for (std::vector<MeshNode*>::iterator child = mChildren.begin(); child != mChildren.end(); ++child)
				{
					i += glm::vec3((*child)->mFinalMatrix[3]);
				}
				i /= mChildren.size();

				glm::vec3 ip = glm::vec3(mFinalMatrix[3]);

				glm::vec3 L = i - ip;

				if (glm::length(L) > 0.0f)
				{
					glm::vec3 y = glm::normalize(L);
					glm::vec3 z = glm::length(normal) > 0.0f ? glm::normalize(normal) : glm::vec3(0.0f, 0.0f, 1.0f);
					glm::vec3 x = glm::cross(y, z);
					if (glm::length(x) > 0.0f)
					{
						x = glm::normalize(x);
						z = glm::normalize(glm::cross(x, y));
						matrix = mMatrix * glm::inverse(glm::matrixToRotate(mFinalMatrix)) *
							glm::mat4(
							glm::vec4(x, 0.0f),
							glm::vec4(y, 0.0f),
							glm::vec4(z, 0.0f),
							glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
							);
					}
					else
					{
						matrix = glm::translate(glm::vec3(mMatrix[3]));
					}
				}
				else
				{
					matrix = glm::translate(glm::vec3(mMatrix[3]));
				}
			}
			else
			{
				matrix = glm::translate(glm::vec3(mMatrix[3]));
			}

			glm::mat4 M = glm::inverse(mAdditionalMatrix) * glm::inverse(matrix) * mMatrix * mAdditionalMatrix;
			for (std::vector<MeshNode*>::iterator node = mChildren.begin(); node != mChildren.end(); ++node)
			{
				(*node)->mMatrix = M * (*node)->mMatrix;
			}

			glm::mat4 A = glm::inverse(mMatrix) * matrix;
			mMatrix = matrix;

			if (SymmetryMode != NODE_SYMMETRY_NONE)
			{
				MeshNode* sym = mNodes->FindBySymmetry(this);
				if (sym)
				{
					glm::vec3 T = glm::matrixToTranslation(A);
					glm::vec3 R = glm::matrixToRotation(A);
					glm::vec3 S = glm::matrixToScaling(A);

					glm::vec3 factor(SymmetryXAxis, SymmetryYAxis, SymmetryZAxis);
					if (SymmetryMode == NODE_SYMMETRY_MIRROR)
					{
						factor *= -1.0f;
					}
					T *= factor;
					R *= factor;

					A = glm::compose(T, R, S);
					matrix = sym->mMatrix * A;

					glm::mat4 M = glm::inverse(sym->mAdditionalMatrix) * glm::inverse(matrix) * sym->mMatrix * sym->mAdditionalMatrix;
					for (std::vector<MeshNode*>::iterator node = sym->mChildren.begin(); node != sym->mChildren.end(); ++node)
					{
						(*node)->mMatrix = M * (*node)->mMatrix;
					}

					sym->mMatrix = matrix;
				}
			}

			mNodes->Update();

			for (std::vector<MeshNode*>::iterator node = mNodes->mNodes.begin(); node != mNodes->mNodes.end(); ++node)
			{
				for (std::vector<MeshObject*>::iterator object = (*node)->mObjects.begin(); object != (*node)->mObjects.end(); ++object)
				{
					glm::mat4 M = glm::inverse((*node)->mFinalMatrix) * (*node)->mOldFinalMatrix;
					(*object)->mOffsetMatrix = M * (*object)->mOffsetMatrix;
				}

				for (std::vector<MeshSkin*>::iterator skin = (*node)->mSkins.begin(); skin != (*node)->mSkins.end(); ++skin)
				{
					glm::mat4 M = glm::inverse((*node)->mFinalMatrix) * (*node)->mOldFinalMatrix;
					(*skin)->OffsetMatrix = M * (*skin)->OffsetMatrix;
				}
			}
		}

		void MeshNode::ResetAlign(void)
		{
			glm::mat4 matrix = mMatrix * glm::inverse(glm::matrixToRotate(mFinalMatrix));

			glm::mat4 M = glm::inverse(mAdditionalMatrix) * glm::inverse(matrix) * mMatrix * mAdditionalMatrix;
			for (std::vector<MeshNode*>::iterator node = mChildren.begin(); node != mChildren.end(); ++node)
			{
				(*node)->mMatrix = M * (*node)->mMatrix;
			}

			mMatrix = matrix;

			if (SymmetryMode != NODE_SYMMETRY_NONE)
			{
				MeshNode* sym = mNodes->FindBySymmetry(this);
				if (sym)
				{
					matrix = sym->mMatrix * glm::inverse(glm::matrixToRotate(sym->mFinalMatrix));

					glm::mat4 M = glm::inverse(sym->mAdditionalMatrix) * glm::inverse(matrix) * sym->mMatrix * sym->mAdditionalMatrix;
					for (std::vector<MeshNode*>::iterator node = sym->mChildren.begin(); node != sym->mChildren.end(); ++node)
					{
						(*node)->mMatrix = M * (*node)->mMatrix;
					}
					sym->mMatrix = matrix;
				}
			}

			mNodes->Update();

			for (std::vector<MeshNode*>::iterator node = mNodes->mNodes.begin(); node != mNodes->mNodes.end(); ++node)
			{
				for (std::vector<MeshObject*>::iterator object = (*node)->mObjects.begin(); object != (*node)->mObjects.end(); ++object)
				{
					glm::mat4 M = glm::inverse((*node)->mFinalMatrix) * (*node)->mOldFinalMatrix;
					(*object)->mOffsetMatrix = M * (*object)->mOffsetMatrix;
				}

				for (std::vector<MeshSkin*>::iterator skin = (*node)->mSkins.begin(); skin != (*node)->mSkins.end(); ++skin)
				{
					glm::mat4 M = glm::inverse((*node)->mFinalMatrix) * (*node)->mOldFinalMatrix;
					(*skin)->OffsetMatrix = M * (*skin)->OffsetMatrix;
				}
			}
		}

		void MeshNode::MoveFirst(void)
		{
			if (mParent)
			{
				return;
			}

			std::vector<MeshNode*>::iterator node = find(mNodes->mNodes.begin(), mNodes->mNodes.end(), this);

			if (node == mNodes->mNodes.begin())
			{
				return;
			}

			mNodes->mNodes.erase(node);
			mNodes->mNodes.insert(mNodes->mNodes.begin(), this);

			mNodes->UpdateIndices();
		}

		void MeshNode::MoveUp(void)
		{
			if (mParent)
			{
				return;
			}

			std::vector<MeshNode*>::iterator node = find(mNodes->mNodes.begin(), mNodes->mNodes.end(), this);

			if (node == mNodes->mNodes.begin())
			{
				return;
			}

			iter_swap(node, node - 1);

			mNodes->UpdateIndices();
		}

		void MeshNode::MoveDown(void)
		{
			if (mChildren.size())
			{
				return;
			}

			std::vector<MeshNode*>::iterator node = find(mNodes->mNodes.begin(), mNodes->mNodes.end(), this);

			if (node == mNodes->mNodes.end() - 1)
			{
				return;
			}

			iter_swap(node, node + 1);

			mNodes->UpdateIndices();
		}

		void MeshNode::MoveLast(void)
		{
			if (mChildren.size())
			{
				return;
			}

			std::vector<MeshNode*>::iterator node = find(mNodes->mNodes.begin(), mNodes->mNodes.end(), this);

			if (node == mNodes->mNodes.end() - 1)
			{
				return;
			}

			mNodes->mNodes.erase(node);
			mNodes->mNodes.push_back(this);

			mNodes->UpdateIndices();
		}

		MeshRigidBody* MeshNode::GetRigidBody(void)
		{
			return mRigidBody;
		}

		void MeshNode::SetRigidBody(MeshRigidBody* rigid_body)
		{
			if (mRigidBody == rigid_body)
			{
				return;
			}

			if (mRigidBody)
			{
				mRigidBody->SetNode((MeshNode*)0);
			}

			if (rigid_body)
			{
				rigid_body->SetNode(this);
			}
		}

		void MeshNode::InitializeBuffersAndShaders(void)
		{
			if (mInitializedBuffersAndShaders)
			{
				return;
			}

			glGenBuffers(1, &mJointPositionBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mJointPositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, joint_vertices * 3 * sizeof(float), joint_v, GL_STATIC_DRAW);

			glGenBuffers(1, &mJointNormalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mJointNormalBuffer);
			glBufferData(GL_ARRAY_BUFFER, joint_vertices * 3 * sizeof(float), joint_vn, GL_STATIC_DRAW);

			glGenBuffers(1, &mJointFaceBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mJointFaceBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, joint_faces * 3 * sizeof(unsigned int), joint_f, GL_STATIC_DRAW);

			glGenBuffers(1, &mLinkPositionBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mLinkPositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, link_vertices * 3 * sizeof(float), link_v, GL_STATIC_DRAW);

			glGenBuffers(1, &mLinkNormalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mLinkNormalBuffer);
			glBufferData(GL_ARRAY_BUFFER, link_vertices * 3 * sizeof(float), link_vn, GL_STATIC_DRAW);

			glGenBuffers(1, &mLinkFaceBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mLinkFaceBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, link_faces * 3 * sizeof(unsigned int), link_f, GL_STATIC_DRAW);

			mShader.CompileSource("uniform mat4 uFTransform;uniform mat3 uNormalMatrix;attribute vec3 aPosition;attribute vec3 aNormal;varying vec3 vNormal;void main(){vNormal = uNormalMatrix * aNormal;gl_Position = uFTransform * vec4(aPosition, 1.0);}", GL_VERTEX_SHADER);

			mShader.CompileSource("varying vec3 vNormal;uniform vec3 uColor;void main(){vec3 normal = normalize(vNormal);vec3 light = vec3(0.0, 0.0, 1.0);float diffuse = max(0.0, dot(normal, light));gl_FragColor = vec4(diffuse * uColor, 1.0);}", GL_FRAGMENT_SHADER);

			mShader.Link();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			mInitializedBuffersAndShaders = true;
		}
	}
}
