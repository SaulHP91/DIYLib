#ifndef MESHOBJECTS_HPP
#define MESHOBJECTS_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <string>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mesh
	{
		class MeshObjects
		{
		public:
			DIYLIB_API ~MeshObjects(void);

			DIYLIB_API MeshFile* GetFile(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshObject* New(void);
			DIYLIB_API MeshObject* New(std::string name);
			DIYLIB_API void Clear(void);
			DIYLIB_API void Delete(MeshObject* object);
			DIYLIB_API MeshObject* Duplicate(MeshObject* object);
			DIYLIB_API MeshObject* Duplicate(std::string name);
			DIYLIB_API MeshObject* Duplicate(int index);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

			DIYLIB_API MeshObject* operator[](int index);
			DIYLIB_API MeshObject* Find(std::string name, bool create = false);

			DIYLIB_API MeshObject* operator()(void);
			DIYLIB_API MeshObject* First(void);
			DIYLIB_API MeshObject* Next(void);
			DIYLIB_API MeshObject* Last(void);
			DIYLIB_API MeshObject* Previous(void);

			DIYLIB_API glm::vec3 GetMinLimits(void);
			DIYLIB_API glm::vec3 GetMaxLimits(void);
			DIYLIB_API glm::vec3 GetCenter(void);

			DIYLIB_API void Update(void);

			DIYLIB_API void Render(MeshMaterial* material);
			DIYLIB_API void Render(void);
			DIYLIB_API void Render(Camera* camera);

			DIYLIB_API int GetAnimatedVertexCount(void);
			DIYLIB_API int GetFaceCount(void);

			DIYLIB_API void Merge(MeshObject* destiny_object, MeshObject* source_object);
			DIYLIB_API void MergeAll(void);

			DIYLIB_API void FreezeAll(void);

			DIYLIB_API void Explode(MeshObject* object);

			DIYLIB_API bool MakeMorphTarget(MeshObject* base_object, MeshObject* target_object);
			DIYLIB_API void SetBlendFactor(std::string morphTargetName, float blendFactor);

			DIYLIB_API void Sort(void);
			DIYLIB_API void Reverse(void);

		private:
			MESH_FRIENDS

			MeshObjects(MeshFile* scene);

			MeshFile* mFile;

			std::vector<MeshObject*> mObjects;
			std::vector<MeshObject*>::iterator mObject;

			glm::vec3 mMinLimits;
			glm::vec3 mMaxLimits;
			glm::vec3 mCenter;
			void UpdateLimits(glm::vec3 reference);

			int mAnimatedVertexCount;
			int mFaceCount;
		};
	}
}

#endif
