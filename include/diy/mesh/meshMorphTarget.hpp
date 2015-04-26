#ifndef MESHMORPHTARGET_HPP
#define MESHMORPHTARGET_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>
#include <diy/mesh/meshVertex.hpp>

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
		class MeshMorphTarget
		{
		public:
			DIYLIB_API ~MeshMorphTarget(void);

			DIYLIB_API MeshMorphTargets* MorphTargets(void);

			DIYLIB_API int GetIndex(void);
			DIYLIB_API std::string GetName(void);
			DIYLIB_API MeshMorphTargetVertices* GetMorphTargetVertices(void);
			DIYLIB_API void SetBlendFactor(float blendFactor);
			DIYLIB_API float GetBlendFactor(void);

			DIYLIB_API void AppendAnimatedVertex(MeshAnimatedVertex* animated_vertex, MeshVertex vertex);
			DIYLIB_API void AppendAnimatedVertex(int animatedVertexIndex, MeshVertex vertex);

			DIYLIB_API void RemoveAnimatedVertex(MeshAnimatedVertex* animated_vertex);
			DIYLIB_API void RemoveAnimatedVertex(int animatedVertexIndex);

			DIYLIB_API bool SetName(std::string name);

		private:
			MESH_FRIENDS

			MeshMorphTarget(MeshMorphTargets* morph_targets, int index, std::string name);

			MeshMorphTargets* mMorphTargets;

			int mIndex;
			std::string mName;
			MeshMorphTargetVertices* mMorphTargetVertices;
			float mBlendFactor;
		};
	}
}

#endif

