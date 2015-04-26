#ifndef MESHJOINT_HPP
#define MESHJOINT_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#ifdef USE_ODE
#include <ode/ode.h>
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
		class MeshJoint
		{
		public:
			DIYLIB_API ~MeshJoint(void);

			DIYLIB_API MeshJoints* GetJoints(void);

			DIYLIB_API int GetIndex(void);
			DIYLIB_API std::string GetName(void);
			DIYLIB_API bool SetName(std::string name);

			DIYLIB_API MeshNode* GetNode(void);
			DIYLIB_API void SetNode(MeshNode* node);
			DIYLIB_API void SetNode(std::string nodeName);
			DIYLIB_API void SetNode(int nodeIndex);

			DIYLIB_API int GetType(void);
			DIYLIB_API void SetType(int type);

			DIYLIB_API MeshRigidBody* GetFirstBody(void);
			DIYLIB_API void SetFirstBody(MeshRigidBody* rigid_body);
			DIYLIB_API void SetFirstBody(std::string rigidBodyName);
			DIYLIB_API void SetFirstBody(int rigidBodyIndex);

			DIYLIB_API MeshRigidBody* GetSecondBody(void);
			DIYLIB_API void SetSecondBody(MeshRigidBody* rigid_body);
			DIYLIB_API void SetSecondBody(std::string rigidBodyName);
			DIYLIB_API void SetSecondBody(int rigidBodyIndex);

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

			DIYLIB_API glm::vec3 GetLoStop(void);
			DIYLIB_API void SetLoStop(glm::vec3 loStop);

			DIYLIB_API glm::vec3 GetHiStop(void);
			DIYLIB_API void SetHiStop(glm::vec3 hiStop);

			DIYLIB_API float GetBounce(void);
			DIYLIB_API void SetBounce(float bounce);

			DIYLIB_API float GetCFM(void);
			DIYLIB_API void SetCFM(float cFM);

			DIYLIB_API float GetStopERP(void);
			DIYLIB_API void SetStopERP(float stopERP);

			DIYLIB_API float GetStopCFM(void);
			DIYLIB_API void SetStopCFM(float stopCFM);

			DIYLIB_API void Render(Camera* camera);

			DIYLIB_API void Initialize(void);

		private:
			MESH_FRIENDS

			MeshJoint(MeshJoints* joints, int index, std::string name);

			MeshJoints* mJoints;

			int mIndex;
			std::string mName;
			std::string mComment;
			MeshNode* mNode;
			int mType;
			MeshRigidBody* mFirstBody;
			MeshRigidBody* mSecondBody;
			glm::mat4 mMatrix;
			glm::vec3 mTranslation;
			glm::quat mRotation;
			glm::vec3 mLoStop;
			glm::vec3 mHiStop;
			float mBounce;
			float mCFM;
			float mStopERP;
			float mStopCFM;

#ifdef USE_ODE
			dJointID mDJointID;
#endif      

			void Update(void);
		};
	}
}

#endif


 