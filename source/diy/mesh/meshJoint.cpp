#include <diy/diy.hpp>

#include <cmath>

#include <gl/glew.h>
#include <glm/own.hpp>

namespace diy
{
	namespace mesh
	{
		MeshJoint::MeshJoint(MeshJoints* joints, int index, std::string name) :
			mJoints(joints),
			mIndex(index),
			mName(name),
			mComment(),
			mNode(0),
			mType(JOINT_BALL),
			mFirstBody(0),
			mSecondBody(0),
			mMatrix(1.0f),
			mTranslation(0.0f),
			mRotation(1.0f, 0.0f, 0.0f, 0.0f),
			mLoStop(-180.0f),
			mHiStop(180.0f),
			mBounce(0.0f),
			mCFM(1.0e-5f),
			mStopERP(0.2f),
			mStopCFM(1.0e-5f)
#ifdef USE_ODE
			,
			mDJointID(0)
#endif
		{
			;
		}

		MeshJoint::~MeshJoint(void)
		{
			SetNode((MeshNode*)0);

			SetFirstBody((MeshRigidBody*)0);
			SetSecondBody((MeshRigidBody*)0);

#ifdef USE_ODE
			if(mDJointID)
			{
				dJointDestroy(mDJointID);
			}
#endif
		}

		MeshJoints* MeshJoint::GetJoints(void)
		{
			return mJoints;
		}

		int MeshJoint::GetIndex(void)
		{
			return mIndex;
		}

		std::string MeshJoint::GetName(void)
		{
			return mName;
		}

		bool MeshJoint::SetName(std::string name)
		{
			if (mJoints->Find(name))
			{
				return false;
			}

			mName = name;

			return true;
		}

		MeshNode* MeshJoint::GetNode(void)
		{
			return mNode;
		}

		void MeshJoint::SetNode(MeshNode* node)
		{
			mNode = node;
		}

		void MeshJoint::SetNode(std::string nodeName)
		{
			SetNode(mJoints->mPhysics->mFile->mNodes->Find(nodeName));
		}

		void MeshJoint::SetNode(int nodeIndex)
		{
			SetNode(mJoints->mPhysics->mFile->mNodes->operator[](nodeIndex));
		}

		int MeshJoint::GetType(void)
		{
			return mType;
		}

		void MeshJoint::SetType(int type)
		{
			if (mType == type)
			{
				return;
			}

			switch (type)
			{
			case JOINT_BALL:
			case JOINT_HINGE:
			case JOINT_UNIVERSAL:
			{
				mType = type;
			}
			default:
			{
				;
			}
			}
		}

		MeshRigidBody* MeshJoint::GetFirstBody(void)
		{
			return mFirstBody;
		}

		void MeshJoint::SetFirstBody(MeshRigidBody* rigid_body)
		{
			mFirstBody = rigid_body;
		}

		void MeshJoint::SetFirstBody(std::string rigidBodyName)
		{
			SetFirstBody(mJoints->mPhysics->mRigidBodies->Find(rigidBodyName));
		}

		void MeshJoint::SetFirstBody(int rigidBodyIndex)
		{
			SetFirstBody(mJoints->mPhysics->mRigidBodies->operator[](rigidBodyIndex));
		}

		MeshRigidBody* MeshJoint::GetSecondBody(void)
		{
			return mSecondBody;
		}

		void MeshJoint::SetSecondBody(MeshRigidBody* rigid_body)
		{
			mSecondBody = rigid_body;
		}

		void MeshJoint::SetSecondBody(std::string rigidBodyName)
		{
			SetSecondBody(mJoints->mPhysics->mRigidBodies->Find(rigidBodyName));
		}

		void MeshJoint::SetSecondBody(int rigidBodyIndex)
		{
			SetSecondBody(mJoints->mPhysics->mRigidBodies->operator[](rigidBodyIndex));
		}

		void MeshJoint::SetIdentity(void)
		{
			mTranslation = glm::vec3(0.0f);
			mRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

			mMatrix = glm::mat4(1.0f);
		}

		glm::mat4 MeshJoint::GetMatrix(void)
		{
			return mMatrix;
		}

		void MeshJoint::SetMatrix(glm::mat4 matrix)
		{
			mRotation = glm::toQuat(glm::mat3(glm::normalize(glm::vec3(matrix[0])), glm::normalize(glm::vec3(matrix[1])), glm::normalize(glm::vec3(matrix[2]))));
			mTranslation = glm::vec3(matrix[3]);

			Update();
		}

		glm::vec3 MeshJoint::GetTranslation(void)
		{
			return mTranslation;
		}

		void MeshJoint::SetTranslation(glm::vec3 translation)
		{
			mTranslation = translation;

			Update();
		}

		glm::quat MeshJoint::GetRotation(void)
		{
			return mRotation;
		}

		void MeshJoint::SetRotation(glm::quat rotation)
		{
			mRotation = rotation;

			Update();
		}

		glm::vec3 MeshJoint::GetEulerAngles(void)
		{
			return glm::matrixToRotation(mMatrix);
		}

		void MeshJoint::SetEulerAngles(glm::vec3 eulerAngles)
		{
			SetRotation(glm::quat(glm::radians(eulerAngles)));
		}

		float MeshJoint::GetAngle(void)
		{
			return glm::angle(mRotation);
		}

		glm::vec3 MeshJoint::GetAxis(void)
		{
			return glm::axis(mRotation);
		}

		void MeshJoint::SetAngleAxis(float angle, glm::vec3 axis)
		{
			SetRotation(glm::angleAxis(angle, axis));
		}

		void MeshJoint::AutoAlign(glm::vec3 normal)
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

				if (glm::length(L) > 0.0f)
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

						SetRotation(glm::toQuat(matrix));

						return;
					}
				}
			}

			SetRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		}

		void MeshJoint::Update(void)
		{
			mMatrix = glm::translate(mTranslation) * glm::toMat4(mRotation);
		}

		glm::vec3 MeshJoint::GetLoStop(void)
		{
			return mLoStop;
		}

		void MeshJoint::SetLoStop(glm::vec3 loStop)
		{
			if (loStop.x < -180 || loStop.y < -180 || loStop.z < -180 || loStop.x > mHiStop.x || loStop.y > mHiStop.y || loStop.z > mHiStop.z)
			{
				return;
			}

			mLoStop = loStop;
		}

		glm::vec3 MeshJoint::GetHiStop(void)
		{
			return mHiStop;
		}

		void MeshJoint::SetHiStop(glm::vec3 hiStop)
		{
			if (hiStop.x > 180 || hiStop.y > 180 || hiStop.z > 180 || hiStop.x < mLoStop.x || hiStop.y < mLoStop.y || hiStop.z < mLoStop.z)
			{
				return;
			}

			mHiStop = hiStop;
		}

		float MeshJoint::GetBounce(void)
		{
			return mBounce;
		}

		void MeshJoint::SetBounce(float bounce)
		{
			if (bounce > 0.0f)
			{
				mBounce = bounce;
			}
		}

		float MeshJoint::GetCFM(void)
		{
			return mCFM;
		}

		void MeshJoint::SetCFM(float cFM)
		{
			if (cFM < 0.0f || cFM > 1.0f)
			{
				return;
			}

			mCFM = cFM;
		}

		float MeshJoint::GetStopERP(void)
		{
			return mStopERP;
		}

		void MeshJoint::SetStopERP(float stopERP)
		{
			if (stopERP < 0.0f || stopERP > 1.0f)
			{
				return;
			}

			mStopERP = stopERP;
		}

		float MeshJoint::GetStopCFM(void)
		{
			return mStopCFM;
		}

		void MeshJoint::SetStopCFM(float stopCFM)
		{
			if (stopCFM < 0.0f || stopCFM > 1.0f)
			{
				return;
			}

			mStopCFM = stopCFM;
		}

#include "joint_ball_axis.c"
#include "joint_hinge_axis.c"
#include "joint_universal_axis.c"

		void MeshJoint::Render(Camera* camera)
		{
			glPushMatrix();
			camera->ApplyFinalMatrix();

			glm::vec3 pivot(0.0f);
			if (mNode)
			{
				glm::mat4 matrix = mNode->GetFinalMatrix();
				matrix[0] = glm::normalize(matrix[0]);
				matrix[1] = glm::normalize(matrix[1]);
				matrix[2] = glm::normalize(matrix[2]);
				glMultMatrixf(glm::value_ptr(matrix));
				pivot = glm::vec3(mNode->GetFinalMatrix()[3]);
			}
			glMultMatrixf(glm::value_ptr(mMatrix));
			pivot += glm::vec3(mMatrix[3]);

			float s;
			if (camera->GetProjectionModel() == Camera::Ortho)
			{
				s = 2.0f * (50.0f / camera->GetViewport()[3]) * (1.0f / camera->GetZoom()) * (camera->GetDistance() / camera->GetUnitaryDistance());
			}
			else
			{
				s = (50.0f / camera->GetViewport()[3]) * (camera->GetDistanceTo(pivot) / camera->GetUnitaryDistance());
			}
			glScalef(s, s, s);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			glEnable(GL_LIGHTING);
			glEnable(GL_COLOR_MATERIAL);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);

			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			glColor3f(0.0f, 0.125f, 0.0f);
			glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
			glColor3f(0.0f, 0.0f, 0.0f);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
			glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
			glColor3f(0.0f, 0.0f, 0.0f);

			switch (mType)
			{
			case JOINT_BALL:
			{
				glVertexPointer(3, GL_FLOAT, 0, joint_ball_axis_v);
				glNormalPointer(GL_FLOAT, 0, joint_ball_axis_vn);
				glDrawElements(GL_TRIANGLES, 3 * joint_ball_axis_faces, GL_UNSIGNED_INT, joint_ball_axis_f);
			}
			break;
			case JOINT_HINGE:
			{
				glVertexPointer(3, GL_FLOAT, 0, joint_hinge_axis_v);
				glNormalPointer(GL_FLOAT, 0, joint_hinge_axis_vn);
				glDrawElements(GL_TRIANGLES, 3 * joint_hinge_axis_faces, GL_UNSIGNED_INT, joint_hinge_axis_f);
			}
			break;
			case JOINT_UNIVERSAL:
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0);
				glVertexPointer(3, GL_FLOAT, 0, joint_universal_axis_v);
				glNormalPointer(GL_FLOAT, 0, joint_universal_axis_vn);
				glDrawElements(GL_TRIANGLES, 3 * joint_universal_axis_faces, GL_UNSIGNED_INT, joint_universal_axis_f);
			}
			break;
			default:;
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);

			glPopMatrix();
		}

		void MeshJoint::Initialize(void)
		{
#ifdef USE_ODE
			if(mDJointID)
			{
				dJointDestroy(mDJointID);
			}

			dBodyID body1 = 0;
			dBodyID body2 = 0;

			if(mFirstBody)
			{
				body1 = mFirstBody->mDBodyID;
			}
			if(mSecondBody)
			{
				body2 = mSecondBody->mDBodyID;
			}

			switch(mType)
			{
			case JOINT_BALL:
			{
				mDJointID = dJointCreateBall(mJoints->mPhysics->mDWorldID, 0);
				dJointSetData(mDJointID, this);

				dJointAttach(mDJointID, body1, body2);

				if(mNode)
				{
					glm::mat4 matrix = mNode->mFinalMatrix * mMatrix;
					dJointSetBallAnchor(mDJointID, matrix[3].x, matrix[3].y, matrix[3].z);
				}
				else
				{
					dJointSetBallAnchor(mDJointID, mTranslation.x, mTranslation.y, mTranslation.z);
				}

				dJointSetBallParam(mDJointID, dParamLoStop1, glm::radians(mLoStop[0]));
				dJointSetBallParam(mDJointID, dParamLoStop2, glm::radians(mLoStop[1]));
				dJointSetBallParam(mDJointID, dParamLoStop3, glm::radians(mLoStop[2]));
				dJointSetBallParam(mDJointID, dParamHiStop1, glm::radians(mHiStop[0]));
				dJointSetBallParam(mDJointID, dParamHiStop2, glm::radians(mHiStop[1]));
				dJointSetBallParam(mDJointID, dParamHiStop3, glm::radians(mHiStop[2]));
				dJointSetBallParam(mDJointID, dParamBounce, mBounce);
				dJointSetBallParam(mDJointID, dParamCFM, mCFM);
				dJointSetBallParam(mDJointID, dParamStopERP, mStopERP);
				dJointSetBallParam(mDJointID, dParamStopCFM, mStopCFM);
			}
			break;
			case JOINT_HINGE:
			{
				mDJointID = dJointCreateHinge(mJoints->mPhysics->mDWorldID, 0);
				dJointSetData(mDJointID, this);

				dJointAttach(mDJointID, body1, body2);

				if(mNode)
				{
					glm::mat4 matrix = mNode->mFinalMatrix * mMatrix;
					dJointSetHingeAnchor(mDJointID, matrix[3].x, matrix[3].y, matrix[3].z);
					glm::vec3 axis = glm::vec3(glm::matrixToRotate(mNode->mFinalMatrix * mMatrix) * glm::vec4(1.0f, 0.0f, 0.0, 0.0f));
					dJointSetHingeAxis(mDJointID, axis.x, axis.y, axis.z);
				}
				else
				{
					dJointSetHingeAnchor(mDJointID, mTranslation.x, mTranslation.y, mTranslation.z);
					glm::vec3 axis = glm::vec3(glm::toMat4(mRotation) * glm::vec4(1.0f, 0.0f, 0.0, 0.0f));
					dJointSetHingeAxis(mDJointID, axis.x, axis.y, axis.z);
				}

				dJointSetHingeParam(mDJointID, dParamLoStop1, glm::radians(mLoStop[0]));
				dJointSetHingeParam(mDJointID, dParamHiStop1, glm::radians(mHiStop[0]));
				dJointSetHingeParam(mDJointID, dParamBounce, mBounce);
				dJointSetHingeParam(mDJointID, dParamCFM, mCFM);
				dJointSetHingeParam(mDJointID, dParamStopERP, mStopERP);
				dJointSetHingeParam(mDJointID, dParamStopCFM, mStopCFM);
			}
			break;
			case JOINT_UNIVERSAL:
			{
				mDJointID = dJointCreateUniversal(mJoints->mPhysics->mDWorldID, 0);
				dJointSetData(mDJointID, this);

				dJointAttach(mDJointID, body1, body2);

				if(mNode)
				{
					glm::mat4 matrix = mNode->mFinalMatrix * mMatrix;
					dJointSetUniversalAnchor(mDJointID, matrix[3].x, matrix[3].y, matrix[3].z);
					glm::vec3 axis1 = glm::vec3(glm::matrixToRotate(mNode->mFinalMatrix * mMatrix) * glm::vec4(1.0f, 0.0f, 0.0, 0.0f));
					glm::vec3 axis2 = glm::vec3(glm::matrixToRotate(mNode->mFinalMatrix * mMatrix) * glm::vec4(0.0f, 0.0f, 1.0, 0.0f));
					dJointSetUniversalAxis1(mDJointID, axis1.x, axis1.y, axis1.z);
					dJointSetUniversalAxis2(mDJointID, axis2.x, axis2.y, axis2.z);
				}
				else
				{
					dJointSetUniversalAnchor(mDJointID, mTranslation.x, mTranslation.y, mTranslation.z);
					glm::vec3 axis1 = glm::vec3(glm::toMat4(mRotation) * glm::vec4(1.0f, 0.0f, 0.0, 0.0f));
					glm::vec3 axis2 = glm::vec3(glm::toMat4(mRotation) * glm::vec4(0.0f, 0.0f, 1.0, 0.0f));
					dJointSetUniversalAxis1(mDJointID, axis1.x, axis1.y, axis1.z);
					dJointSetUniversalAxis2(mDJointID, axis2.x, axis2.y, axis2.z);
				}

				dJointSetUniversalParam(mDJointID, dParamLoStop1, glm::radians(mLoStop[0]));
				dJointSetUniversalParam(mDJointID, dParamLoStop2, glm::radians(mLoStop[1]));
				dJointSetUniversalParam(mDJointID, dParamHiStop1, glm::radians(mHiStop[0]));
				dJointSetUniversalParam(mDJointID, dParamHiStop2, glm::radians(mHiStop[1]));
				dJointSetUniversalParam(mDJointID, dParamBounce, mBounce);
				dJointSetUniversalParam(mDJointID, dParamCFM, mCFM);
				dJointSetUniversalParam(mDJointID, dParamStopERP, mStopERP);
				dJointSetUniversalParam(mDJointID, dParamStopCFM, mStopCFM);
			}
			break;
			default:
			{
				;
			}
			}
#endif
		}
	}
}
