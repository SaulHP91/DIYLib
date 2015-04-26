#ifndef MESHGROUP_HPP
#define MESHGROUP_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <vector>
#include <string>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	class Shader;

	namespace mesh
	{
		class MeshGroup
		{
		public:
			DIYLIB_API ~MeshGroup(void);

			DIYLIB_API MeshGroups* GetGroups(void);
			DIYLIB_API MeshMaterial* GetMaterial(void);

			DIYLIB_API int GetIndex(void);
			DIYLIB_API std::vector<MeshFace*>& GetFaces(void);

			DIYLIB_API void AppendFace(MeshFace* face);
			DIYLIB_API void AppendFace(int faceIndex);

			DIYLIB_API void RemoveFace(MeshFace* face);
			DIYLIB_API void RemoveFace(int faceIndex);

			DIYLIB_API void SetMaterial(MeshMaterial* material);
			DIYLIB_API void SetMaterial(int index);
			DIYLIB_API void SetMaterial(std::string name);

			DIYLIB_API void RenderUV(void);

			DIYLIB_API int GetIndexCount(void);
			DIYLIB_API const void* GetIndexPointer(void);

			DIYLIB_API void Render(Camera* camera);

		private:
			MESH_FRIENDS

			MeshGroup(MeshGroups* groups, MeshMaterial* material, int index);

			MeshGroups* mGroups;
			MeshMaterial* mMaterial;

			int mIndex;
			std::vector<MeshFace*> mFaces;

			unsigned int mFirstFace;

			Shader* mShader;
			void Update(void);
		};
	}
}

#endif
