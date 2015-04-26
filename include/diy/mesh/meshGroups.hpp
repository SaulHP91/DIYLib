#ifndef MESHGROUPS_HPP
#define MESHGROUPS_HPP

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
		class MeshGroups
		{
		public:
			DIYLIB_API ~MeshGroups(void);

			DIYLIB_API MeshObject* GetObject(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshGroup* New(MeshMaterial* material);
			DIYLIB_API MeshGroup* New(std::string materialName);
			DIYLIB_API MeshGroup* New(int materialIndex);
			DIYLIB_API void Clear(void);
			DIYLIB_API void Delete(MeshGroup* group);

			DIYLIB_API MeshGroup* operator[](int index);
			DIYLIB_API MeshGroup* Find(MeshMaterial* material, bool create = false);
			DIYLIB_API MeshGroup* Find(std::string materialName, bool create = false);
			DIYLIB_API MeshGroup* Find(int materialIndex, bool create = false);

			DIYLIB_API MeshGroup* operator()(void);
			DIYLIB_API MeshGroup* First(void);
			DIYLIB_API MeshGroup* Next(void);
			DIYLIB_API MeshGroup* Last(void);
			DIYLIB_API MeshGroup* Previous(void);

		private:
			MESH_FRIENDS

			MeshGroups(MeshObject* scene);

			MeshObject* mObject;

			std::vector<MeshGroup*> mGroups;
			std::vector<MeshGroup*>::iterator mGroup;
		};
	}
}

#endif
