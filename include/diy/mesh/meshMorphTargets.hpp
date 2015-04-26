#ifndef MESHMORPHTARGETS_HPP
#define MESHMORPHTARGETS_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <string>
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
		class MeshMorphTargets
		{
		public:
			DIYLIB_API ~MeshMorphTargets(void);

			DIYLIB_API MeshObject* GetObject(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshMorphTarget* New(void);
			DIYLIB_API MeshMorphTarget* New(std::string name);
			DIYLIB_API void Clear(void);
			DIYLIB_API void Delete(MeshMorphTarget* object);

			DIYLIB_API MeshMorphTarget* operator[](int index);
			DIYLIB_API MeshMorphTarget* Find(std::string name, bool create = false);

			DIYLIB_API MeshMorphTarget* operator()(void);
			DIYLIB_API MeshMorphTarget* First(void);
			DIYLIB_API MeshMorphTarget* Next(void);
			DIYLIB_API MeshMorphTarget* Last(void);
			DIYLIB_API MeshMorphTarget* Previous(void);

		private:
			MESH_FRIENDS

			MeshMorphTargets(MeshObject* object);

			MeshObject* mObject;

			std::vector<MeshMorphTarget*> mMorphTargets;
			std::vector<MeshMorphTarget*>::iterator mMorphTarget;

			void UpdateArrays(void);
		};
	}
}

#endif
