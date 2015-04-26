#ifndef MESHSKINS_HPP
#define MESHSKINS_HPP

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
		class MeshSkins
		{
		public:
			DIYLIB_API ~MeshSkins(void);

			DIYLIB_API MeshObject* GetObject(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshSkin* New(MeshNode* node);
			DIYLIB_API MeshSkin* New(std::string nodeName);
			DIYLIB_API MeshSkin* New(int nodeIndex);
			DIYLIB_API void Clear(void);
			DIYLIB_API void Delete(MeshSkin* skin);

			DIYLIB_API MeshSkin* operator[](int index);
			DIYLIB_API MeshSkin* Find(MeshNode* node, bool create = false);
			DIYLIB_API MeshSkin* Find(std::string nodeName, bool create = false);
			DIYLIB_API MeshSkin* Find(int nodeIndex, bool create = false);

			DIYLIB_API MeshSkin* operator()(void);
			DIYLIB_API MeshSkin* First(void);
			DIYLIB_API MeshSkin* Next(void);
			DIYLIB_API MeshSkin* Last(void);
			DIYLIB_API MeshSkin* Previous(void);

			DIYLIB_API unsigned int GetMaxSkinWeightsPerVertex(void);

		private:
			MESH_FRIENDS

			MeshSkins(MeshObject* scene);

			MeshObject* mObject;

			std::vector<MeshSkin*> mSkins;
			std::vector<MeshSkin*>::iterator mSkin;

			unsigned int mMaxSkinWeightsPerVertex;

			void UpdateArrays(void);
		};
	}
}

#endif
