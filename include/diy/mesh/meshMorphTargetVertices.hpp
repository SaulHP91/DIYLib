#ifndef MESHMORPHTARGETVERTICES_HPP
#define MESHMORPHTARGETVERTICES_HPP

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
		class MeshMorphTargetVertices
		{
		public:
			DIYLIB_API ~MeshMorphTargetVertices(void);

			DIYLIB_API MeshMorphTarget* GetMorphTarget(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshMorphTargetVertex* New(MeshAnimatedVertex* animated_vertex, MeshVertex vertex);
			DIYLIB_API MeshMorphTargetVertex* New(int animatedVertexIndex, MeshVertex vertex);
			DIYLIB_API void Clear(void);
			DIYLIB_API void Delete(MeshMorphTargetVertex* morph_target_vertex);

			DIYLIB_API MeshMorphTargetVertex* Find(MeshAnimatedVertex* animated_vertex);
			DIYLIB_API MeshMorphTargetVertex* Find(int animatedVertexIndex);

			DIYLIB_API MeshMorphTargetVertex* operator()(void);
			DIYLIB_API MeshMorphTargetVertex* First(void);
			DIYLIB_API MeshMorphTargetVertex* Next(void);
			DIYLIB_API MeshMorphTargetVertex* Last(void);
			DIYLIB_API MeshMorphTargetVertex* Previous(void);

			DIYLIB_API void Reverse(void);

		private:
			MESH_FRIENDS

			MeshMorphTargetVertices(MeshMorphTarget* morph_target);

			MeshMorphTarget* mMorphTarget;

			std::vector<MeshMorphTargetVertex*> mMorphTargetVertices;
			std::vector<MeshMorphTargetVertex*>::iterator mMorphTargetVertex;

			unsigned int mCapacity;
			unsigned int mSize;
			unsigned int* mIndexArray;
			float* mDeltaPositionArray;
			void Realloc(unsigned int size);
			void UpdateArray(void);
		};
	}
}

#endif
