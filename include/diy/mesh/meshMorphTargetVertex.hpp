#ifndef MESHMORPHTARGETVERTEX_HPP
#define MESHMORPHTARGETVERTEX_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>
#include <diy/mesh/meshVertex.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mesh
	{
		class MeshMorphTargetVertex
		{
		public:
			DIYLIB_API ~MeshMorphTargetVertex(void);

			DIYLIB_API MeshMorphTargetVertices* GetMorphTargetVertices(void);
			DIYLIB_API MeshAnimatedVertex* GetAnimatedVertex(void);

			DIYLIB_API MeshVertex GetVertex(void);
			DIYLIB_API void SetVertex(MeshVertex vertex);

		private:
			MESH_FRIENDS

			MeshMorphTargetVertex(MeshMorphTargetVertices* morph_target_vertices, MeshAnimatedVertex* animated_vertex, MeshVertex& vertex);

			MeshMorphTargetVertices* mMorphTargetVertices;
			MeshAnimatedVertex* mAnimatedVertex;

			MeshVertex mVertex;
			MeshVertex mDeltaVertex;
			MeshVertex mMorphedDeltaVertex;
		};
	}
}

#endif
