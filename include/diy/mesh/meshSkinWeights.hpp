#ifndef MESHSKINWEIGHTS_HPP
#define MESHSKINWEIGHTS_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

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
		class MeshSkinWeights
		{
		public:
			DIYLIB_API ~MeshSkinWeights(void);

			DIYLIB_API MeshSkin* GetSkin(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshSkinWeight* New(MeshAnimatedVertex* animated_vertex, float weight = 1.0f);
			DIYLIB_API MeshSkinWeight* New(int animatedVertexIndex, float weight = 1.0f);
			DIYLIB_API void Clear(void);
			DIYLIB_API void Delete(MeshSkinWeight* skin_weight);

			DIYLIB_API MeshSkinWeight* Find(MeshAnimatedVertex* animated_vertex);
			DIYLIB_API MeshSkinWeight* Find(int animatedVertexIndex);

			DIYLIB_API MeshSkinWeight* operator()(void);
			DIYLIB_API MeshSkinWeight* First(void);
			DIYLIB_API MeshSkinWeight* Next(void);
			DIYLIB_API MeshSkinWeight* Last(void);
			DIYLIB_API MeshSkinWeight* Previous(void);

		private:
			MESH_FRIENDS

			MeshSkinWeights(MeshSkin* skin);

			MeshSkin* mSkin;

			std::vector<MeshSkinWeight*> mSkinWeights;
			std::vector<MeshSkinWeight*>::iterator mSkinWeight;

			unsigned int mCapacity;
			unsigned int mSize;
			unsigned int* mIndexArray;
			float* mWeightArray;
			void Realloc(unsigned int size);
			void UpdateArray(void);
		};
	}
}

#endif
