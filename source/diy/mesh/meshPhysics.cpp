#include <diy/diy.hpp>

namespace diy
{
	namespace mesh
	{
		MeshPhysics::MeshPhysics(MeshFile* file) :
			mFile(file),
			mGravity(0.0f, -9.806f, 0.0f)
#ifdef USE_ODE
			,
			mDWorldID(0),
			mDSpaceID(0),
			mDJointGroupID(0)
#endif
		{
			mRigidBodies = new MeshRigidBodies(this);
			mJoints = new MeshJoints(this);
		}

		MeshPhysics::~MeshPhysics(void)
		{
			delete mRigidBodies;
			delete mJoints;

#ifdef USE_ODE
			if(mDJointGroupID)
			{
				dJointGroupDestroy(mDJointGroupID);
			}

			if(mDSpaceID)
			{
				dSpaceDestroy(mDSpaceID);
			}

			if(mDWorldID)
			{
				dWorldDestroy(mDWorldID);
			}
#endif
		}

		MeshFile* MeshPhysics::GetFile(void)
		{
			return mFile;
		}

		MeshRigidBodies* MeshPhysics::GetRigidBodies(void)
		{
			return mRigidBodies;
		}

		MeshJoints* MeshPhysics::GetJoints(void)
		{
			return mJoints;
		}

		void MeshPhysics::Clear(void)
		{
			mRigidBodies->Clear();
			mJoints->Clear();
		}

		glm::vec3 MeshPhysics::GetGravity(void)
		{
			return mGravity;
		}

		void MeshPhysics::SetGravity(glm::vec3 gravity)
		{
			mGravity = gravity;
		}

		bool MeshPhysics::LoadFromFStream(std::ifstream& in)
		{
			mRigidBodies->LoadFromFStream(in);
			mJoints->LoadFromFStream(in);

			return true;
		}

		void MeshPhysics::SaveToFStream(std::ofstream& out)
		{
			mRigidBodies->SaveToFStream(out);
			mJoints->SaveToFStream(out);
		}

#ifdef USE_ODE
#define MAX_CONTACTS 10

		void nearCallback(void *data, dGeomID o1, dGeomID o2)
		{
			MeshPhysics* physics = (MeshPhysics*)data;

			dBodyID b1, b2;

			b1 = dGeomGetBody(o1);
			b2 = dGeomGetBody(o2);

			if(b1 && b2)
			{
				if(((MeshRigidBody*)dBodyGetData(b1))->mType == RIGIDBODY_KINEMATIC && ((MeshRigidBody*)dBodyGetData(b2))->mType == RIGIDBODY_KINEMATIC)
				{
					return;
				}

				if(((MeshRigidBody*)dBodyGetData(b1))->mCollisionGroup == ((MeshRigidBody*)dBodyGetData(b2))->mCollisionGroup)
				{
					return;
				}
			}
			else if (b1 || b2)
			{
				if(b1)
				{
					if(((MeshRigidBody*)dBodyGetData(b1))->mType == RIGIDBODY_KINEMATIC)
					{
						return;
					}
				}
				else
				{
					if(((MeshRigidBody*)dBodyGetData(b2))->mType == RIGIDBODY_KINEMATIC)
					{
						return;
					}
				}
			}

			float friction = 0.0f;
			if(b1)
			{
				friction += ((MeshRigidBody*)dBodyGetData(b1))->mFriction;
			}
			if(b2)
			{
				friction += ((MeshRigidBody*)dBodyGetData(b2))->mFriction;
			}

			static dContact contact[MAX_CONTACTS];
			int n = dCollide(o1, o2, MAX_CONTACTS, &(contact[0].geom) ,sizeof(dContact));
			if(n > 0)
			{
				for(int i = 0; i < n; i++)
				{
					contact[i].surface.mode = dContactBounce | dContactSoftCFM;
					contact[i].surface.mu = friction;
					contact[i].surface.mu2 = 0;
					contact[i].surface.bounce = 0.01;
					contact[i].surface.bounce_vel = 0.1;
					contact[i].surface.soft_cfm = 0.01;
					dJointID c = dJointCreateContact(physics->mDWorldID, physics->mDJointGroupID, contact + i);
					dJointAttach(c, b1, b2);
				}
			}
		}
#endif

		void MeshPhysics::Initialize(void)
		{
#ifdef USE_ODE
			if(!mDWorldID)
			{
				mDWorldID = dWorldCreate();
			}

			if(!mDSpaceID)
			{
				mDSpaceID = dHashSpaceCreate(0);
			}

			if(!mDJointGroupID)
			{
				mDJointGroupID = dJointGroupCreate(0);
			}

			dWorldSetGravity(mDWorldID, mGravity.x, mGravity.y, mGravity.z);
#endif

			mRigidBodies->Initialize();
			mJoints->Initialize();
		}

		void MeshPhysics::Step(float step, int iterations)
		{
#ifdef USE_ODE
			if(step <= 0.0f || iterations <= 0)
			{
				return;
			}

			for(vector<MeshRigidBody*>::iterator rigid_body = mRigidBodies->mRigidBodies.begin(); rigid_body != mRigidBodies->mRigidBodies.end(); ++rigid_body)
			{
				if((*rigid_body)->mType == RIGIDBODY_KINEMATIC)
				{
					if((*rigid_body)->mNode)
					{
						glm::mat4 matrix = (*rigid_body)->mNode->mFinalMatrix * (*rigid_body)->mMatrix;

						dBodySetPosition((*rigid_body)->mDBodyID, matrix[3].x, matrix[3].y, matrix[3].z);
						dBodySetQuaternion((*rigid_body)->mDBodyID, GLMtoODE(glm::toQuat(glm::mat3(glm::normalize(glm::vec3(matrix[0])), glm::normalize(glm::vec3(matrix[1])), glm::normalize(glm::vec3(matrix[2]))))));
					}
					else
					{
						dBodySetPosition((*rigid_body)->mDBodyID, (*rigid_body)->mTranslation.x, (*rigid_body)->mTranslation.y, (*rigid_body)->mTranslation.z);
						dBodySetQuaternion((*rigid_body)->mDBodyID, GLMtoODE((*rigid_body)->mRotation));
					}

					dJointSetFixed((*rigid_body)->mDJointID);
				}
			}

			for(int i = 0; i < iterations; ++i)
			{
				dSpaceCollide(mDSpaceID, this, &nearCallback);

				dWorldStep(mDWorldID, step);

				dJointGroupEmpty(mDJointGroupID);
			}

			for(vector<MeshNode*>::iterator node = mFile->mNodes->mNodes.begin(); node != mFile->mNodes->mNodes.end(); ++node)
			{
				if((*node)->mRigidBody)
				{
					if((*node)->mRigidBody->mType == RIGIDBODY_KINEMATIC)
					{
						continue;
					}

					glm::mat4 fMOmega1 = ODEtoOGL(dBodyGetPosition((*node)->mRigidBody->mDBodyID), dBodyGetRotation((*node)->mRigidBody->mDBodyID));
					glm::mat4 fMOmega0 = (*node)->mFinalMatrix * (*node)->mRigidBody->mMatrix;
					glm::mat4 Delta = glm::inverse(fMOmega0) * fMOmega1;
					glm::mat4 Xi = (*node)->mRigidBody->mMatrix * Delta * glm::inverse((*node)->mRigidBody->mMatrix);
					glm::mat4 AMnu_ = (*node)->mAdditionalMatrix * Xi;
					(*node)->mAdditionalMatrix = AMnu_;
					mFile->mNodes->Update();
				}
			}

			for(vector<MeshRigidBody*>::iterator rigid_body = mRigidBodies->mRigidBodies.begin(); rigid_body != mRigidBodies->mRigidBodies.end(); ++rigid_body)
			{
				if((*rigid_body)->mType == RIGIDBODY_KINEMATIC)
				{
					continue;
				}

				if(!(*rigid_body)->mNode)
				{
					(*rigid_body)->SetMatrix(ODEtoOGL(dBodyGetPosition((*rigid_body)->mDBodyID), dBodyGetRotation((*rigid_body)->mDBodyID)));
				}
			}

			mFile->mObjects->Update();
#endif
		}

		void MeshPhysics::Step(anim::AnimKeyFrame* anim_key_frame, int iterations)
		{
			anim_key_frame->GetNodes()->First();
			anim::AnimNode* anim_node;
			while (anim_node = anim_key_frame->GetNodes()->operator()())
			{
				MeshNode* node = mFile->mNodes->Find(anim_node->GetName());
				if (node)
				{
					node->mAdditionalMatrix = anim_node->GetMatrix();
				}
				anim_key_frame->GetNodes()->Next();
			}
			mFile->mNodes->Update();

			anim_key_frame->GetObjects()->First();
			anim::AnimObject* anim_object;
			while (anim_object = anim_key_frame->GetObjects()->operator()())
			{
				MeshObject* object = mFile->mObjects->Find(anim_object->GetName());
				if (object)
				{
					object->mVisible = anim_object->Visible();

					anim_object->GetMorphTargets()->First();
					anim::AnimMorphTarget* anim_morph_target;
					while (anim_morph_target = anim_object->GetMorphTargets()->operator()())
					{
						MeshMorphTarget* morph_target = object->mMorphTargets->Find(anim_morph_target->GetName());
						if (morph_target)
						{
							morph_target->mBlendFactor = anim_morph_target->GetBlendFactor();
						}
						anim_object->GetMorphTargets()->Next();
					}
				}
				anim_key_frame->GetObjects()->Next();
			}

			Step(anim_key_frame->GetLastStep(), iterations);
		}
	}
}
