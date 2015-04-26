#ifndef MESHRIGIDBODY_HPP
#define MESHRIGIDBODY_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#ifdef USE_ODE
#include <ode/ode.h>
#pragma link "ode_single.lib"
#endif

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mesh
	{
		class MeshRigidBody
		{
		public:
			DIYLIB_API ~MeshRigidBody(void);

			DIYLIB_API MeshRigidBodies* GetRigidBodies(void);

			DIYLIB_API int GetIndex(void);
			DIYLIB_API std::string GetName(void);
			DIYLIB_API bool SetName(std::string name);

			DIYLIB_API MeshNode* GetNode(void);
			DIYLIB_API void SetNode(MeshNode* node);
			DIYLIB_API void SetNode(std::string nodeName);
			DIYLIB_API void SetNode(int nodeIndex);

			DIYLIB_API int GetCollisionGroup(void);
			DIYLIB_API void SetCollisionGroup(int collisionGroup);

			DIYLIB_API bool IsInUse(void);

			DIYLIB_API int GetShape(void);
			DIYLIB_API void SetShape(int shape);

			DIYLIB_API glm::vec3 GetSize(void);
			DIYLIB_API void SetSize(glm::vec3 size);
			DIYLIB_API void AutoSize(void);

			DIYLIB_API void SetIdentity(void);

			DIYLIB_API glm::mat4 GetMatrix(void);
			DIYLIB_API void SetMatrix(glm::mat4 matrix);

			DIYLIB_API glm::vec3 GetTranslation(void);
			DIYLIB_API void SetTranslation(glm::vec3 translation);

			DIYLIB_API glm::quat GetRotation(void);
			DIYLIB_API void SetRotation(glm::quat rotation);
			DIYLIB_API glm::vec3 GetEulerAngles(void);
			DIYLIB_API void SetEulerAngles(glm::vec3 eulerAngles);
			DIYLIB_API float GetAngle(void);
			DIYLIB_API glm::vec3 GetAxis(void);
			DIYLIB_API void SetAngleAxis(float angle, glm::vec3 axis);
			DIYLIB_API void AutoAlign(glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f));

			DIYLIB_API void LoadInitialConditions(void);
			DIYLIB_API void SaveInitialConditions(void);

			DIYLIB_API float GetMass(void);
			DIYLIB_API void SetMass(float mass);

			DIYLIB_API float GetLinearDamping(void);
			DIYLIB_API void SetLinearDamping(float linearDamping);

			DIYLIB_API float GetAngularDamping(void);
			DIYLIB_API void SetAngularDamping(float angularDamping);

			DIYLIB_API float GetBounce(void);
			DIYLIB_API void SetBounce(float bounce);

			DIYLIB_API float GetFriction(void);
			DIYLIB_API void SetFriction(float friction);

			DIYLIB_API int GetType(void);
			DIYLIB_API void SetType(int type);

			DIYLIB_API void Render(Camera* camera);

			DIYLIB_API void Initialize(void);

		private:
			MESH_FRIENDS

			MeshRigidBody(MeshRigidBodies* rigid_bodies, int index, std::string name);

			MeshRigidBodies* mRigidBodies;

			int mIndex;
			std::string mName;
			std::string mComment;
			MeshNode* mNode;
			int mCollisionGroup;
			int mShape;
			glm::vec3 mSize;
			glm::mat4 mMatrix;
			glm::vec3 mTranslation;
			glm::vec3 mInitialTranslation;
			glm::quat mRotation;
			glm::quat mInitialRotation;
			float mMass;
			float mLinearDamping;
			float mAngularDamping;
			float mBounce;
			float mFriction;
			int mType;

#ifdef USE_ODE
			dBodyID mDBodyID;
			dGeomID mDGeomID;
			dJointID mDJointID;
			dMass mDMass;

			friend void nearCallback(void *data, dGeomID o1, dGeomID o2);
#endif

			void Update(void);

			static unsigned int mSpherePositionBuffer;
			static unsigned int mSphereNormalBuffer;
			static unsigned int mSphereFaceBuffer;

			static unsigned int mBoxPositionBuffer;
			static unsigned int mBoxNormalBuffer;
			static unsigned int mBoxFaceBuffer;

			static unsigned int mCapsuleLowerPositionBuffer;
			static unsigned int mCapsuleLowerNormalBuffer;
			static unsigned int mCapsuleLowerFaceBuffer;

			static unsigned int mCapsuleMiddlePositionBuffer;
			static unsigned int mCapsuleMiddleNormalBuffer;
			static unsigned int mCapsuleMiddleFaceBuffer;

			static unsigned int mCapsuleUpperPositionBuffer;
			static unsigned int mCapsuleUpperNormalBuffer;
			static unsigned int mCapsuleUpperFaceBuffer;

			static unsigned int mCylinderPositionBuffer;
			static unsigned int mCylinderNormalBuffer;
			static unsigned int mCylinderFaceBuffer;

			static Shader mShader;

			static bool mInitializedBuffersAndShaders;
			void InitializeBuffersAndShaders(void);
		};
	}
}

#endif

