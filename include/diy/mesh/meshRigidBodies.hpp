#ifndef MESHRIGIDBODIES_HPP
#define MESHRIGIDBODIES_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <string>
#include <fstream>
#include <vector>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mesh
	{
		class MeshRigidBodies
		{
		public:
			DIYLIB_API ~MeshRigidBodies(void);

			DIYLIB_API MeshPhysics* GetPhysics(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshRigidBody* New(void);
			DIYLIB_API MeshRigidBody* New(std::string name);
			DIYLIB_API void Clear(void);
			DIYLIB_API void Delete(MeshRigidBody* rigid_body);
			DIYLIB_API void Delete(std::string name);
			DIYLIB_API void DeleteUnused(void);

			DIYLIB_API MeshRigidBody* operator[](int index);
			DIYLIB_API MeshRigidBody* Find(std::string name, bool create = false);
			DIYLIB_API MeshRigidBody* First(void);
			DIYLIB_API MeshRigidBody* Next(void);
			DIYLIB_API MeshRigidBody* Last(void);
			DIYLIB_API MeshRigidBody* Previous(void);
			DIYLIB_API MeshRigidBody* operator()(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

			DIYLIB_API void Render(Camera* camera);
			DIYLIB_API void Render(Camera* camera, int collisionGroup);
			DIYLIB_API void Render(Camera* camera, std::vector<int>& collisionGroups);

			DIYLIB_API void Initialize(void);

		private:
			MESH_FRIENDS

			MeshRigidBodies(MeshPhysics* physics);

			MeshPhysics* mPhysics;

			std::vector<MeshRigidBody*> mRigidBodies;
			std::vector<MeshRigidBody*>::iterator mRigidBody;
		};
	}
}

#endif

 