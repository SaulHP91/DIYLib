#ifndef MESHMATERIALS_HPP
#define MESHMATERIALS_HPP

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
		class MeshMaterials
		{
		public:
			DIYLIB_API ~MeshMaterials(void);

			DIYLIB_API MeshFile* GetFile(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshMaterial* New(void);
			DIYLIB_API MeshMaterial* New(std::string name);
			DIYLIB_API void Clear(void);
			DIYLIB_API bool Delete(MeshMaterial* material);
			DIYLIB_API void DeleteUnused(void);
			DIYLIB_API void MergeDuplicated(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

			DIYLIB_API MeshMaterial* operator[](int index);
			DIYLIB_API MeshMaterial* Find(std::string name, bool create = false);

			DIYLIB_API MeshMaterial* operator()(void);
			DIYLIB_API MeshMaterial* First(void);
			DIYLIB_API MeshMaterial* Next(void);
			DIYLIB_API MeshMaterial* Last(void);
			DIYLIB_API MeshMaterial* Previous(void);

			DIYLIB_API void Sort(void);
			DIYLIB_API void Reverse(void);

		private:
			MESH_FRIENDS

			MeshMaterials(MeshFile* file);

			MeshFile* mFile;

			std::vector<MeshMaterial*> mMaterials;
			std::vector<MeshMaterial*>::iterator mMaterial;
		};
	}
}

#endif
