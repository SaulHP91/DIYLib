#ifndef MESHFACES_HPP
#define MESHFACES_HPP

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
	namespace mesh
	{
		class MeshFaces
		{
		public:
			DIYLIB_API ~MeshFaces(void);

			DIYLIB_API MeshObject* GetObject(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshFace* New(MeshGroup* group, std::vector<MeshAnimatedVertex*> animatedVertices);
			DIYLIB_API MeshFace* New(MeshGroup* group, std::vector<int> animatedVertexIndices);
			DIYLIB_API MeshFace* New(std::vector<MeshAnimatedVertex*> animatedVertices);
			DIYLIB_API MeshFace* New(std::vector<int> animatedVertexIndices);
			DIYLIB_API MeshFace* New(std::string materialName, std::vector<MeshAnimatedVertex*> animatedVertices);
			DIYLIB_API MeshFace* New(std::string materialName, std::vector<int> animatedVertexIndices);
			DIYLIB_API MeshFace* New(int materialIndex, std::vector<MeshAnimatedVertex*> animatedVertices);
			DIYLIB_API MeshFace* New(int materialIndex, std::vector<int> animatedVertexIndices);
			DIYLIB_API MeshFace* New(MeshMaterial* material, std::vector<MeshAnimatedVertex*> animatedVertices);
			DIYLIB_API MeshFace* New(MeshMaterial* material, std::vector<int> animatedVertexIndices);
			DIYLIB_API void Clear(void);
			DIYLIB_API void Delete(MeshFace* face);

			DIYLIB_API MeshFace* operator[](int index);
			DIYLIB_API MeshFace* Find(MeshGroup* group);
			DIYLIB_API MeshFace* Find(MeshMaterial* material);
			DIYLIB_API MeshFace* Find(std::string materialName);
			DIYLIB_API MeshFace* Find(int materialIndex);

			DIYLIB_API MeshFace* operator()(void);
			DIYLIB_API MeshFace* First(void);
			DIYLIB_API MeshFace* Next(void);
			DIYLIB_API MeshFace* Last(void);
			DIYLIB_API MeshFace* Previous(void);

			DIYLIB_API void Triangulate(void);
			DIYLIB_API void Reverse(void);

			DIYLIB_API int GetIndexCount(void);
			DIYLIB_API const void* GetIndexPointer(void);

		private:
			MESH_FRIENDS

			MeshFaces(MeshObject* scene);

			MeshObject* mObject;

			std::vector<MeshFace*> mFaces;
			std::vector<MeshFace*>::iterator mFace;

			unsigned int mCapacity;
			unsigned int mSize;
			unsigned int* mIndexArray;
			void Realloc(unsigned int size);
			void UpdateArray(void);
		};
	}
}

#endif
