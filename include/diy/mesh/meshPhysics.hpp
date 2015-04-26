#ifndef MESHPHYSICS_HPP
#define MESHPHYSICS_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <fstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#ifdef USE_ODE
#include <ode/ode.h>
#pragma link "ode_single.lib"

glm::mat4 ODEtoOGL(const float* R)
{
   return glm::mat4
   (
      glm::vec4(R[0], R[4], R[8],  0),
      glm::vec4(R[1], R[5], R[9],  0),
      glm::vec4(R[2], R[6], R[10], 0),
      glm::vec4(0,    0,    0,     1)
   );
}

glm::mat4 ODEtoOGL(const float* p, const float* R)
{
   return glm::mat4
   (
      glm::vec4(R[0], R[4], R[8],  0),
      glm::vec4(R[1], R[5], R[9],  0),
      glm::vec4(R[2], R[6], R[10], 0),
      glm::vec4(p[0], p[1], p[2],  1)
   );
}

__inline const float* GLMtoODE(glm::quat q)
{
   static float Q[4];
   Q[0] = q.w;
   Q[1] = q.x;
   Q[2] = q.y;
   Q[3] = q.z;
   return Q;
}

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
		class MeshPhysics
		{
		public:
			DIYLIB_API ~MeshPhysics(void);

			DIYLIB_API MeshFile* GetFile(void);

			DIYLIB_API MeshRigidBodies* GetRigidBodies(void);
			DIYLIB_API MeshJoints* GetJoints(void);

			DIYLIB_API void Clear(void);

			DIYLIB_API glm::vec3 GetGravity(void);
			DIYLIB_API void SetGravity(glm::vec3 gravity);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

			DIYLIB_API void Initialize(void);
			DIYLIB_API void Step(float step, int iterations = 1);
			DIYLIB_API void Step(anim::AnimKeyFrame* anim_key_frame, int iterations = 1);

		private:
			MESH_FRIENDS

			MeshPhysics(MeshFile* file);

			MeshFile* mFile;

			MeshRigidBodies* mRigidBodies;
			MeshJoints* mJoints;

			glm::vec3 mGravity;

#ifdef USE_ODE
			dWorldID mDWorldID;
			dSpaceID mDSpaceID;
			dJointGroupID mDJointGroupID;

			friend void nearCallback(void *data, dGeomID o1, dGeomID o2);
#endif

		};
	}
}

#endif

