#include <diy/diy.hpp>

#include "sphere.c"
#include "box.c"
#include "capsule_lower.c"
#include "capsule_middle.c"
#include "capsule_upper.c"
#include "cylinder.c"

#include <gl/glew.h>
#include <glm/own.hpp>

namespace diy
{
	namespace mesh
	{

		unsigned int MeshRigidBody::mSpherePositionBuffer;
		unsigned int MeshRigidBody::mSphereNormalBuffer;
		unsigned int MeshRigidBody::mSphereFaceBuffer;

		unsigned int MeshRigidBody::mBoxPositionBuffer;
		unsigned int MeshRigidBody::mBoxNormalBuffer;
		unsigned int MeshRigidBody::mBoxFaceBuffer;

		unsigned int MeshRigidBody::mCapsuleLowerPositionBuffer;
		unsigned int MeshRigidBody::mCapsuleLowerNormalBuffer;
		unsigned int MeshRigidBody::mCapsuleLowerFaceBuffer;

		unsigned int MeshRigidBody::mCapsuleMiddlePositionBuffer;
		unsigned int MeshRigidBody::mCapsuleMiddleNormalBuffer;
		unsigned int MeshRigidBody::mCapsuleMiddleFaceBuffer;

		unsigned int MeshRigidBody::mCapsuleUpperPositionBuffer;
		unsigned int MeshRigidBody::mCapsuleUpperNormalBuffer;
		unsigned int MeshRigidBody::mCapsuleUpperFaceBuffer;

		unsigned int MeshRigidBody::mCylinderPositionBuffer;
		unsigned int MeshRigidBody::mCylinderNormalBuffer;
		unsigned int MeshRigidBody::mCylinderFaceBuffer;

		Shader MeshRigidBody::mShader;

		bool MeshRigidBody::mInitializedBuffersAndShaders = false;

		MeshRigidBody::MeshRigidBody(MeshRigidBodies* rigid_bodies, int index, std::string name) :
			mRigidBodies(rigid_bodies),
			mIndex(index),
			mName(name),
			mComment(),
			mNode(0),
			mCollisionGroup(0),
			mShape(RIGIDBODY_BOX),
			mSize(0.5f),
			mMatrix(1.0f),
			mTranslation(0.0f),
			mInitialTranslation(0.0f),
			mRotation(1.0f, 0.0f, 0.0f, 0.0f),
			mInitialRotation(1.0f, 0.0f, 0.0f, 0.0f),
			mMass(1.0f),
			mLinearDamping(1.0e-2f),
			mAngularDamping(1.0e-2f),
			mBounce(0.0f),
			mFriction(0.0f),
			mType(RIGIDBODY_KINEMATIC)
#ifdef USE_ODE
			,
			mDBodyID(0),
			mDGeomID(0),
			mDJointID(0)
#endif
		{
			;
		}

		MeshRigidBody::~MeshRigidBody(void)
		{
			SetNode((MeshNode*)0);

#ifdef USE_ODE
			if(mDJointID)
			{
				dJointDestroy(mDJointID);
			}

			if(mDGeomID)
			{
				dGeomDestroy(mDGeomID);
			}   

			if(mDBodyID)
			{
				dBodyDestroy(mDBodyID);
			}
#endif
		}

		MeshRigidBodies* MeshRigidBody::GetRigidBodies(void)
		{
			return mRigidBodies;
		}

		int MeshRigidBody::GetIndex(void)
		{
			return mIndex;
		}

		std::string MeshRigidBody::GetName(void)
		{
			return mName;
		}

		bool MeshRigidBody::SetName(std::string name)
		{
			if (mRigidBodies->Find(name))
			{
				return false;
			}

			mName = name;

			return true;
		}

		MeshNode* MeshRigidBody::GetNode(void)
		{
			return mNode;
		}

		void MeshRigidBody::SetNode(MeshNode* node)
		{
			if (mNode == node)
			{
				return;
			}

			if (mNode)
			{
				mNode->mRigidBody = 0;
			}

			mNode = node;

			if (mNode)
			{
				if (node->mRigidBody)
				{
					node->mRigidBody->SetNode((MeshNode*)0);
				}

				mNode->mRigidBody = this;
			}
		}

		void MeshRigidBody::SetNode(std::string nodeName)
		{
			SetNode(mRigidBodies->mPhysics->mFile->mNodes->Find(nodeName));
		}

		void MeshRigidBody::SetNode(int nodeIndex)
		{
			SetNode(mRigidBodies->mPhysics->mFile->mNodes->operator[](nodeIndex));
		}

		int MeshRigidBody::GetCollisionGroup(void)
		{
			return mCollisionGroup;
		}

		void MeshRigidBody::SetCollisionGroup(int collisionGroup)
		{
			mCollisionGroup = collisionGroup;
		}

		bool MeshRigidBody::IsInUse(void)
		{
			for (std::vector<MeshJoint*>::iterator constraint = mRigidBodies->mPhysics->mJoints->mJoints.begin(); constraint != mRigidBodies->mPhysics->mJoints->mJoints.end(); ++constraint)
			{
				if ((*constraint)->mFirstBody == this || (*constraint)->mSecondBody == this)
				{
					return true;
				}
			}

			return false;
		}

		int MeshRigidBody::GetShape(void)
		{
			return mShape;
		}

		void MeshRigidBody::SetShape(int shape)
		{
			if (mShape == shape)
			{
				return;
			}

			switch (shape)
			{
			case RIGIDBODY_SPHERE:
			case RIGIDBODY_BOX:
			case RIGIDBODY_CAPSULE:
			case RIGIDBODY_CYLINDER:
			{
				mShape = shape;
			}
			break;
			default:
			{
				;
			}
			}
		}

		glm::vec3 MeshRigidBody::GetSize(void)
		{
			return mSize;
		}

		void MeshRigidBody::SetSize(glm::vec3 size)
		{
			if (size.x > 0.0f)
			{
				mSize.x = size.x;
			}

			if (size.y > 0.0f)
			{
				mSize.y = size.y;
			}

			if (size.z > 0.0f)
			{
				mSize.z = size.z;
			}
		}

		void MeshRigidBody::AutoSize(void)
		{
			if (!mNode)
			{
				SetSize(glm::vec3(1.0f));

				return;
			}

			if (mNode->mSkins.size())
			{
				glm::vec3 min;
				glm::vec3 max;
				bool first = true;

				for (std::vector<MeshSkin*>::iterator skin = mNode->mSkins.begin(); skin != mNode->mSkins.end(); ++skin)
				{
					glm::mat4 bind = glm::inverse(glm::inverse((*skin)->OffsetMatrix) * glm::toMat4(mRotation));
					for (std::vector<MeshSkinWeight*>::iterator skin_weight = (*skin)->mSkinWeights->mSkinWeights.begin(); skin_weight != (*skin)->mSkinWeights->mSkinWeights.end(); ++skin_weight)
					{
						glm::vec3 vertex = glm::vec3
							(
							bind *
							glm::vec4
							(
							(*skin_weight)->mAnimatedVertex->mVertex.Position, 1.0f
							)
							);

						if (first)
						{
							min = vertex;
							max = min;
							first = false;
							continue;
						}

						if (vertex.x < min.x)
						{
							min.x = vertex.x;
						}
						if (vertex.y < min.y)
						{
							min.y = vertex.y;
						}
						if (vertex.z < min.z)
						{
							min.z = vertex.z;
						}

						if (vertex.x > max.x)
						{
							max.x = vertex.x;
						}
						if (vertex.y > max.y)
						{
							max.y = vertex.y;
						}
						if (vertex.z > max.z)
						{
							max.z = vertex.z;
						}
					}
				}

				SetSize((max - min) / 2.0f);
				SetTranslation(glm::vec3(glm::toMat4(mRotation) * glm::vec4((min + max) / 2.0f, 0.0f)));
			}
			else
			{
				if (mNode->mObjects.size())
				{
					glm::vec3 min;
					glm::vec3 max;
					bool first = true;

					for (std::vector<MeshObject*>::iterator object = mNode->mObjects.begin(); object != mNode->mObjects.end(); ++object)
					{
						glm::mat4 bind = glm::inverse(glm::inverse((*object)->mOffsetMatrix) * glm::toMat4(mRotation));
						for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = (*object)->mAnimatedVertices->mAnimatedVertices.begin(); animated_vertex != (*object)->mAnimatedVertices->mAnimatedVertices.end(); ++animated_vertex)
						{
							glm::vec3 vertex = glm::vec3
								(
								bind *
								glm::vec4
								(
								(*animated_vertex)->mVertex.Position, 1.0f
								)
								);

							if (first)
							{
								min = vertex;
								max = min;
								first = false;
								continue;
							}

							if (vertex.x < min.x)
							{
								min.x = vertex.x;
							}
							if (vertex.y < min.y)
							{
								min.y = vertex.y;
							}
							if (vertex.z < min.z)
							{
								min.z = vertex.z;
							}

							if (vertex.x > max.x)
							{
								max.x = vertex.x;
							}
							if (vertex.y > max.y)
							{
								max.y = vertex.y;
							}
							if (vertex.z > max.z)
							{
								max.z = vertex.z;
							}
						}
					}

					SetSize((max - min) / 2.0f);
					SetTranslation(glm::vec3(glm::toMat4(mRotation) * glm::vec4((min + max) / 2.0f, 0.0f)));
				}
			}
		}

		void MeshRigidBody::SetIdentity(void)
		{
			mTranslation = glm::vec3(0.0f);
			mRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

			mMatrix = glm::mat4(1.0f);
		}

		glm::mat4 MeshRigidBody::GetMatrix(void)
		{
			return mMatrix;
		}

		void MeshRigidBody::SetMatrix(glm::mat4 matrix)
		{
			mTranslation = glm::vec3(matrix[3]);
			mRotation = glm::toQuat(glm::mat3(glm::normalize(glm::vec3(matrix[0])), glm::normalize(glm::vec3(matrix[1])), glm::normalize(glm::vec3(matrix[2]))));

			Update();
		}

		glm::vec3 MeshRigidBody::GetTranslation(void)
		{
			return mTranslation;
		}

		void MeshRigidBody::SetTranslation(glm::vec3 translation)
		{
			mTranslation = translation;

			Update();
		}

		glm::quat MeshRigidBody::GetRotation(void)
		{
			return mRotation;
		}

		void MeshRigidBody::SetRotation(glm::quat rotation)
		{
			mRotation = rotation;

			Update();
		}

		glm::vec3 MeshRigidBody::GetEulerAngles(void)
		{
			return glm::matrixToRotation(mMatrix);
		}

		void MeshRigidBody::SetEulerAngles(glm::vec3 eulerAngles)
		{
			SetRotation(glm::quat(glm::radians(eulerAngles)));
		}

		float MeshRigidBody::GetAngle(void)
		{
			return glm::angle(mRotation);
		}

		glm::vec3 MeshRigidBody::GetAxis(void)
		{
			return glm::axis(mRotation);
		}

		void MeshRigidBody::SetAngleAxis(float angle, glm::vec3 axis)
		{
			SetRotation(glm::angleAxis(angle, axis));
		}

		void MeshRigidBody::AutoAlign(glm::vec3 normal)
		{
			if (mNode)
			{
				glm::vec3 ip = glm::vec3(mNode->mCombinedMatrix[3]);
				glm::vec3 i(0.0f);
				if (mNode->mChildren.size())
				{
					for (std::vector<MeshNode*>::iterator child = mNode->mChildren.begin(); child != mNode->mChildren.end(); ++child)
					{
						i += glm::vec3((*child)->mCombinedMatrix[3]);
					}
					i /= mNode->mChildren.size();
				}
				else if (mNode->mParent)
				{
					i = ip;
					ip = glm::vec3(mNode->mParent->mCombinedMatrix[3]);
				}

				glm::vec3 L = i - ip;

				float l = glm::length(L);
				if (l > 0.0f)
				{
					glm::vec3 y = glm::normalize(L);
					glm::vec3 z = glm::length(normal) > 0.0f ? glm::normalize(normal) : glm::vec3(0.0f, 0.0f, 1.0f);
					glm::vec3 x = glm::cross(y, z);
					if (glm::length(x) > 0.0f)
					{
						x = glm::normalize(x);
						z = glm::normalize(glm::cross(x, y));
						glm::mat4 matrix = glm::inverse(glm::matrixToRotate(mNode->mCombinedMatrix)) *
							glm::mat4(
							glm::vec4(x, 0.0f),
							glm::vec4(y, 0.0f),
							glm::vec4(z, 0.0f),
							glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
							);

						SetSize(glm::vec3(mSize.x, l / 2.0f, mSize.z));
						SetTranslation(glm::vec3(matrix * glm::vec4(0.0f, l / 2.0f, 0.0f, 0.0f)));
						SetRotation(glm::toQuat(matrix));

						return;
					}
				}
			}

			SetTranslation(glm::vec3(0.0f, mSize.y, 0.0f));
			SetRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		}

		void MeshRigidBody::LoadInitialConditions(void)
		{
			mTranslation = mInitialTranslation;
			mRotation = mInitialRotation;

			Update();
		}

		void MeshRigidBody::SaveInitialConditions(void)
		{
			mInitialTranslation = mTranslation;
			mInitialRotation = mRotation;
		}

		void MeshRigidBody::Update(void)
		{
			mMatrix = glm::translate(mTranslation) * glm::toMat4(mRotation);
		}

		float MeshRigidBody::GetMass(void)
		{
			return mMass;
		}

		void MeshRigidBody::SetMass(float mass)
		{
			if (mass < 0.0f)
			{
				return;
			}

			mMass = mass;
		}

		float MeshRigidBody::GetLinearDamping(void)
		{
			return mLinearDamping;
		}

		void MeshRigidBody::SetLinearDamping(float linearDamping)
		{
			if (linearDamping < 0.0f || linearDamping > 1.0f)
			{
				return;
			}

			mLinearDamping = linearDamping;
		}

		float MeshRigidBody::GetAngularDamping(void)
		{
			return mAngularDamping;
		}

		void MeshRigidBody::SetAngularDamping(float angularDamping)
		{
			if (angularDamping < 0.0f || angularDamping > 1.0f)
			{
				return;
			}

			mAngularDamping = angularDamping;
		}

		float MeshRigidBody::GetBounce(void)
		{
			return mBounce;
		}

		void MeshRigidBody::SetBounce(float bounce)
		{
			if (bounce >= 0.0f)
			{
				mBounce = bounce;
			}
		}

		float MeshRigidBody::GetFriction(void)
		{
			return mFriction;
		}

		void MeshRigidBody::SetFriction(float friction)
		{
			if (friction >= 0.0f)
			{
				mFriction = friction;
			}
		}

		int MeshRigidBody::GetType(void)
		{
			return mType;
		}

		void MeshRigidBody::SetType(int type)
		{
			switch (type)
			{
			case RIGIDBODY_KINEMATIC:
			case RIGIDBODY_SIMULATED:
			case RIGIDBODY_ALIGNED:
			{
				mType = type;
			}
			default:
			{
				;
			}
			}
		}

		void MeshRigidBody::Render(Camera* camera)
		{
			if (!mInitializedBuffersAndShaders)
			{
				InitializeBuffersAndShaders();
			}
			if (!mInitializedBuffersAndShaders)
			{
				return;
			}

			srand(mCollisionGroup);
			glm::vec3 color((float)(rand() % 9) / 8.0f, (float)(rand() % 17) / 16.0f, (float)(rand() % 33) / 32.0f);

			glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
			glm::mat4 modelViewMatrix = camera->GetFinalMatrix();

			if (mNode)
			{
				glm::mat4 matrix = mNode->GetFinalMatrix();
				matrix[0] = glm::normalize(matrix[0]);
				matrix[1] = glm::normalize(matrix[1]);
				matrix[2] = glm::normalize(matrix[2]);
				modelViewMatrix *= matrix;
			}
			modelViewMatrix *= mMatrix;

			switch (mShape)
			{
			case RIGIDBODY_SPHERE:
			{
				mShader.Begin();

				modelViewMatrix *= glm::scale(glm::vec3(mSize[0]));
				mShader.Uniform(GL_FLOAT_MAT4, mShader.GetUniformLocation("uFTransform"), 1, glm::value_ptr(projectionMatrix * modelViewMatrix));
				mShader.Uniform(GL_FLOAT_MAT3, mShader.GetUniformLocation("uNormalMatrix"), 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(modelViewMatrix)))));
				mShader.Uniform(GL_FLOAT_VEC3, mShader.GetUniformLocation("uColor"), 1, glm::value_ptr(color));

				mShader.EnableVertexAttribArray(mShader.GetAttribLocation("aPosition"));
				mShader.EnableVertexAttribArray(mShader.GetAttribLocation("aNormal"));

				glBindBuffer(GL_ARRAY_BUFFER, mSpherePositionBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, mShader.GetAttribLocation("aPosition"), 0);

				glBindBuffer(GL_ARRAY_BUFFER, mSphereNormalBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, mShader.GetAttribLocation("aNormal"), 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSphereFaceBuffer);
				glDrawElements(GL_TRIANGLES, 3 * sphere_faces, GL_UNSIGNED_INT, 0);

				mShader.DisableVertexAttribArray(mShader.GetAttribLocation("aPosition"));
				mShader.DisableVertexAttribArray(mShader.GetAttribLocation("aNormal"));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				mShader.End();
			}
			break;
			case RIGIDBODY_BOX:
			{
				mShader.Begin();

				modelViewMatrix *= glm::scale(glm::vec3(2.0f * mSize[0], 2.0f * mSize[1], 2.0f * mSize[2]));
				mShader.Uniform(GL_FLOAT_MAT4, mShader.GetUniformLocation("uFTransform"), 1, glm::value_ptr(projectionMatrix * modelViewMatrix));
				mShader.Uniform(GL_FLOAT_MAT3, mShader.GetUniformLocation("uNormalMatrix"), 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(modelViewMatrix)))));
				mShader.Uniform(GL_FLOAT_VEC3, mShader.GetUniformLocation("uColor"), 1, glm::value_ptr(color));

				mShader.EnableVertexAttribArray(mShader.GetAttribLocation("aPosition"));
				mShader.EnableVertexAttribArray(mShader.GetAttribLocation("aNormal"));

				glBindBuffer(GL_ARRAY_BUFFER, mBoxPositionBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, mShader.GetAttribLocation("aPosition"), 0);

				glBindBuffer(GL_ARRAY_BUFFER, mBoxNormalBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, mShader.GetAttribLocation("aNormal"), 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBoxFaceBuffer);
				glDrawElements(GL_TRIANGLES, 3 * box_faces, GL_UNSIGNED_INT, 0);

				mShader.DisableVertexAttribArray(mShader.GetAttribLocation("aPosition"));
				mShader.DisableVertexAttribArray(mShader.GetAttribLocation("aNormal"));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				mShader.End();
			}
			break;
			case RIGIDBODY_CAPSULE:
			{
				mShader.Begin();

				glm::mat4 lowerModelViewMatrix = modelViewMatrix * glm::translate(glm::vec3(0.0f, -mSize[1], 0.0f)) * glm::scale(glm::vec3(mSize[0]));
				mShader.Uniform(GL_FLOAT_MAT4, mShader.GetUniformLocation("uFTransform"), 1, glm::value_ptr(projectionMatrix * lowerModelViewMatrix));
				mShader.Uniform(GL_FLOAT_MAT3, mShader.GetUniformLocation("uNormalMatrix"), 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(lowerModelViewMatrix)))));
				mShader.Uniform(GL_FLOAT_VEC3, mShader.GetUniformLocation("uColor"), 1, glm::value_ptr(color));

				mShader.EnableVertexAttribArray(mShader.GetAttribLocation("aPosition"));
				mShader.EnableVertexAttribArray(mShader.GetAttribLocation("aNormal"));

				glBindBuffer(GL_ARRAY_BUFFER, mCapsuleLowerPositionBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, mShader.GetAttribLocation("aPosition"), 0);

				glBindBuffer(GL_ARRAY_BUFFER, mCapsuleLowerNormalBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, mShader.GetAttribLocation("aNormal"), 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCapsuleLowerFaceBuffer);
				glDrawElements(GL_TRIANGLES, 3 * capsule_lower_faces, GL_UNSIGNED_INT, 0);

				mShader.DisableVertexAttribArray(mShader.GetAttribLocation("aPosition"));
				mShader.DisableVertexAttribArray(mShader.GetAttribLocation("aNormal"));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				mShader.End();

				mShader.Begin();

				glm::mat4 middleModelViewMatrix = modelViewMatrix * glm::scale(glm::vec3(mSize[0], 2.0f * mSize[1], mSize[0]));
				mShader.Uniform(GL_FLOAT_MAT4, mShader.GetUniformLocation("uFTransform"), 1, glm::value_ptr(projectionMatrix * middleModelViewMatrix));
				mShader.Uniform(GL_FLOAT_MAT3, mShader.GetUniformLocation("uNormalMatrix"), 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(middleModelViewMatrix)))));
				mShader.Uniform(GL_FLOAT_VEC3, mShader.GetUniformLocation("uColor"), 1, glm::value_ptr(color));

				mShader.EnableVertexAttribArray(mShader.GetAttribLocation("aPosition"));
				mShader.EnableVertexAttribArray(mShader.GetAttribLocation("aNormal"));

				glBindBuffer(GL_ARRAY_BUFFER, mCapsuleMiddlePositionBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, mShader.GetAttribLocation("aPosition"), 0);

				glBindBuffer(GL_ARRAY_BUFFER, mCapsuleMiddleNormalBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, mShader.GetAttribLocation("aNormal"), 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCapsuleMiddleFaceBuffer);
				glDrawElements(GL_TRIANGLES, 3 * capsule_middle_faces, GL_UNSIGNED_INT, 0);

				mShader.DisableVertexAttribArray(mShader.GetAttribLocation("aPosition"));
				mShader.DisableVertexAttribArray(mShader.GetAttribLocation("aNormal"));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				mShader.End();

				mShader.Begin();

				glm::mat4 upperModelViewMatrix = modelViewMatrix * glm::translate(glm::vec3(0.0f, mSize[1], 0.0f)) * glm::scale(glm::vec3(mSize[0]));
				mShader.Uniform(GL_FLOAT_MAT4, mShader.GetUniformLocation("uFTransform"), 1, glm::value_ptr(projectionMatrix * upperModelViewMatrix));
				mShader.Uniform(GL_FLOAT_MAT3, mShader.GetUniformLocation("uNormalMatrix"), 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(upperModelViewMatrix)))));
				mShader.Uniform(GL_FLOAT_VEC3, mShader.GetUniformLocation("uColor"), 1, glm::value_ptr(color));

				mShader.EnableVertexAttribArray(mShader.GetAttribLocation("aPosition"));
				mShader.EnableVertexAttribArray(mShader.GetAttribLocation("aNormal"));

				glBindBuffer(GL_ARRAY_BUFFER, mCapsuleUpperPositionBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, mShader.GetAttribLocation("aPosition"), 0);

				glBindBuffer(GL_ARRAY_BUFFER, mCapsuleUpperNormalBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, mShader.GetAttribLocation("aNormal"), 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCapsuleUpperFaceBuffer);
				glDrawElements(GL_TRIANGLES, 3 * capsule_upper_faces, GL_UNSIGNED_INT, 0);

				mShader.DisableVertexAttribArray(mShader.GetAttribLocation("aPosition"));
				mShader.DisableVertexAttribArray(mShader.GetAttribLocation("aNormal"));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				mShader.End();
			}
			break;
			case RIGIDBODY_CYLINDER:
			{
				mShader.Begin();

				modelViewMatrix *= glm::scale(glm::vec3(mSize[0], 2.0f * mSize[1], mSize[0]));
				mShader.Uniform(GL_FLOAT_MAT4, mShader.GetUniformLocation("uFTransform"), 1, glm::value_ptr(projectionMatrix * modelViewMatrix));
				mShader.Uniform(GL_FLOAT_MAT3, mShader.GetUniformLocation("uNormalMatrix"), 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(modelViewMatrix)))));
				mShader.Uniform(GL_FLOAT_VEC3, mShader.GetUniformLocation("uColor"), 1, glm::value_ptr(color));

				mShader.EnableVertexAttribArray(mShader.GetAttribLocation("aPosition"));
				mShader.EnableVertexAttribArray(mShader.GetAttribLocation("aNormal"));

				glBindBuffer(GL_ARRAY_BUFFER, mCylinderPositionBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, mShader.GetAttribLocation("aPosition"), 0);

				glBindBuffer(GL_ARRAY_BUFFER, mCylinderNormalBuffer);
				mShader.VertexAttribPointer(GL_FLOAT_VEC3, mShader.GetAttribLocation("aNormal"), 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCylinderFaceBuffer);
				glDrawElements(GL_TRIANGLES, 3 * cylinder_faces, GL_UNSIGNED_INT, 0);

				mShader.DisableVertexAttribArray(mShader.GetAttribLocation("aPosition"));
				mShader.DisableVertexAttribArray(mShader.GetAttribLocation("aNormal"));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				mShader.End();
			}
			break;
			default:
			{
				;
			}
			}
		}

		void MeshRigidBody::Initialize(void)
		{
#ifdef USE_ODE
			if(!mDBodyID)
			{
				mDBodyID = dBodyCreate(mRigidBodies->mPhysics->mDWorldID);
				dBodySetData(mDBodyID, this);
			}

			if(mNode)
			{
				glm::mat4 matrix = mNode->mFinalMatrix * mMatrix;

				dBodySetPosition(mDBodyID, matrix[3].x, matrix[3].y, matrix[3].z);
				dBodySetQuaternion(mDBodyID, GLMtoODE(glm::toQuat(glm::mat3(glm::normalize(glm::vec3(matrix[0])), glm::normalize(glm::vec3(matrix[1])), glm::normalize(glm::vec3(matrix[2]))))));
			}
			else
			{
				dBodySetPosition(mDBodyID, mTranslation.x, mTranslation.y, mTranslation.z);
				dBodySetQuaternion(mDBodyID, GLMtoODE(mRotation));
			}

			if(mType == RIGIDBODY_KINEMATIC)
			{
				if(!mDJointID)
				{
					mDJointID = dJointCreateFixed(mRigidBodies->mPhysics->mDWorldID, 0);
				}

				dJointAttach(mDJointID, 0, mDBodyID);
				dJointSetFixed(mDJointID);
			}
			else
			{
				if(mDJointID)
				{
					dJointAttach(mDJointID, 0, 0);
				}
			}

			if(mDGeomID)
			{
				dGeomDestroy(mDGeomID);
			}

			switch(mShape)
			{
			case RIGIDBODY_SPHERE:
			{
				dMassSetSphereTotal(&mDMass, mMass, mSize[0]);
				mDGeomID = dCreateSphere(mRigidBodies->mPhysics->mDSpaceID, mSize[0]);
			}
			break;
			case RIGIDBODY_BOX:
			{
				dMassSetBoxTotal(&mDMass, mMass, 2.0f * mSize[0], 2.0f * mSize[1], 2.0f * mSize[2]);
				mDGeomID = dCreateBox(mRigidBodies->mPhysics->mDSpaceID, 2.0f * mSize[0], 2.0f * mSize[1], 2.0f * mSize[2]);
			}
			break;
			case RIGIDBODY_CAPSULE:
			{
				dMassSetCapsuleTotal(&mDMass, mMass, 3, mSize[0], 2.0f * mSize[1]);
				mDGeomID = dCreateCapsule(mRigidBodies->mPhysics->mDSpaceID, mSize[0], 2.0f * mSize[1]);
			}
			break;
			case RIGIDBODY_CYLINDER:
			{
				dMassSetCylinderTotal(&mDMass, mMass, 3, mSize[0], 2.0f * mSize[1]);
				mDGeomID = dCreateCylinder(mRigidBodies->mPhysics->mDSpaceID, mSize[0], 2.0f * mSize[1]);
			}
			break;
			default:
			{
				;
			}
			}
			dGeomSetData(mDGeomID, this);

			dBodySetMass(mDBodyID, &mDMass);
			dGeomSetBody(mDGeomID, mDBodyID);

			if(mShape == RIGIDBODY_CAPSULE || mShape == RIGIDBODY_CYLINDER)
			{
				dGeomSetOffsetQuaternion(mDGeomID, GLMtoODE(glm::toQuat(glm::rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f)))));
			}

			dBodySetLinearDamping(mDBodyID, mLinearDamping);
			dBodySetAngularDamping(mDBodyID, mAngularDamping);

			dBodySetLinearVel(mDBodyID, 0.0f, 0.0f, 0.0f);
			dBodySetAngularVel(mDBodyID, 0.0f, 0.0f, 0.0f);
			dBodySetForce(mDBodyID, 0.0f, 0.0f, 0.0f);
			dBodySetTorque(mDBodyID, 0.0f, 0.0f, 0.0f);
#endif
		}

		void MeshRigidBody::InitializeBuffersAndShaders(void)
		{
			if (mInitializedBuffersAndShaders)
			{
				return;
			}

			glGenBuffers(1, &mSpherePositionBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mSpherePositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, sphere_vertices * 3 * sizeof(float), sphere_v, GL_STATIC_DRAW);

			glGenBuffers(1, &mSphereNormalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mSphereNormalBuffer);
			glBufferData(GL_ARRAY_BUFFER, sphere_vertices * 3 * sizeof(float), sphere_vn, GL_STATIC_DRAW);

			glGenBuffers(1, &mSphereFaceBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSphereFaceBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere_faces * 3 * sizeof(unsigned int), sphere_f, GL_STATIC_DRAW);

			glGenBuffers(1, &mBoxPositionBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mBoxPositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, box_vertices * 3 * sizeof(float), box_v, GL_STATIC_DRAW);

			glGenBuffers(1, &mBoxNormalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mBoxNormalBuffer);
			glBufferData(GL_ARRAY_BUFFER, box_vertices * 3 * sizeof(float), box_vn, GL_STATIC_DRAW);

			glGenBuffers(1, &mBoxFaceBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBoxFaceBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, box_faces * 3 * sizeof(unsigned int), box_f, GL_STATIC_DRAW);

			glGenBuffers(1, &mCapsuleLowerPositionBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mCapsuleLowerPositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, capsule_lower_vertices * 3 * sizeof(float), capsule_lower_v, GL_STATIC_DRAW);

			glGenBuffers(1, &mCapsuleLowerNormalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mCapsuleLowerNormalBuffer);
			glBufferData(GL_ARRAY_BUFFER, capsule_lower_vertices * 3 * sizeof(float), capsule_lower_vn, GL_STATIC_DRAW);

			glGenBuffers(1, &mCapsuleLowerFaceBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCapsuleLowerFaceBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, capsule_lower_faces * 3 * sizeof(unsigned int), capsule_lower_f, GL_STATIC_DRAW);

			glGenBuffers(1, &mCapsuleMiddlePositionBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mCapsuleMiddlePositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, capsule_middle_vertices * 3 * sizeof(float), capsule_middle_v, GL_STATIC_DRAW);

			glGenBuffers(1, &mCapsuleMiddleNormalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mCapsuleMiddleNormalBuffer);
			glBufferData(GL_ARRAY_BUFFER, capsule_middle_vertices * 3 * sizeof(float), capsule_middle_vn, GL_STATIC_DRAW);

			glGenBuffers(1, &mCapsuleMiddleFaceBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCapsuleMiddleFaceBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, capsule_middle_faces * 3 * sizeof(unsigned int), capsule_middle_f, GL_STATIC_DRAW);

			glGenBuffers(1, &mCapsuleUpperPositionBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mCapsuleUpperPositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, capsule_upper_vertices * 3 * sizeof(float), capsule_upper_v, GL_STATIC_DRAW);

			glGenBuffers(1, &mCapsuleUpperNormalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mCapsuleUpperNormalBuffer);
			glBufferData(GL_ARRAY_BUFFER, capsule_upper_vertices * 3 * sizeof(float), capsule_upper_vn, GL_STATIC_DRAW);

			glGenBuffers(1, &mCapsuleUpperFaceBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCapsuleUpperFaceBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, capsule_upper_faces * 3 * sizeof(unsigned int), capsule_upper_f, GL_STATIC_DRAW);

			glGenBuffers(1, &mCylinderPositionBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mCylinderPositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, cylinder_vertices * 3 * sizeof(float), cylinder_v, GL_STATIC_DRAW);

			glGenBuffers(1, &mCylinderNormalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mCylinderNormalBuffer);
			glBufferData(GL_ARRAY_BUFFER, cylinder_vertices * 3 * sizeof(float), cylinder_vn, GL_STATIC_DRAW);

			glGenBuffers(1, &mCylinderFaceBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCylinderFaceBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinder_faces * 3 * sizeof(unsigned int), cylinder_f, GL_STATIC_DRAW);

			mShader.CompileSource("uniform mat4 uFTransform;uniform mat3 uNormalMatrix;attribute vec3 aPosition;attribute vec3 aNormal;varying vec3 vNormal;void main(){vNormal = uNormalMatrix * aNormal;gl_Position = uFTransform * vec4(aPosition, 1.0);}", GL_VERTEX_SHADER);

			mShader.CompileSource("varying vec3 vNormal;uniform vec3 uColor;void main(){vec3 normal = normalize(vNormal);vec3 light = vec3(0.0, 0.0, 1.0);float diffuse = max(0.0, dot(normal, light));gl_FragColor = vec4(diffuse * uColor, 1.0);}", GL_FRAGMENT_SHADER);

			mShader.Link();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			mInitializedBuffersAndShaders = true;
		}
	}
}
