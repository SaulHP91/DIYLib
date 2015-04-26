#ifndef MESHJOINTS_HPP
#define MESHJOINTS_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <vector>
#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mesh
	{
		class MeshJoints
		{
		public:
			DIYLIB_API ~MeshJoints(void);

			DIYLIB_API MeshPhysics* GetPhysics(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshJoint* New(void);
			DIYLIB_API MeshJoint* New(std::string name);
			DIYLIB_API void Clear(void);
			DIYLIB_API void Delete(MeshJoint* joint);
			DIYLIB_API void Delete(std::string name);

			DIYLIB_API MeshJoint* operator[](int index);
			DIYLIB_API MeshJoint* Find(std::string name, bool create = false);
			DIYLIB_API MeshJoint* First(void);
			DIYLIB_API MeshJoint* Next(void);
			DIYLIB_API MeshJoint* Last(void);
			DIYLIB_API MeshJoint* Previous(void);
			DIYLIB_API MeshJoint* operator()(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

			DIYLIB_API void Render(Camera* camera);

			DIYLIB_API void Initialize(void);

		private:
			MESH_FRIENDS

			MeshJoints(MeshPhysics* physics);

			MeshPhysics* mPhysics;

			std::vector<MeshJoint*> mJoints;
			std::vector<MeshJoint*>::iterator mJoint;
		};
	}
}

#endif


