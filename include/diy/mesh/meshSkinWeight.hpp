#ifndef MESHSKINWEIGHT_HPP
#define MESHSKINWEIGHT_HPP

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
		class MeshSkinWeight
		{
		public:
			DIYLIB_API ~MeshSkinWeight(void);

			DIYLIB_API MeshSkinWeights* GetSkinWeights(void);
			DIYLIB_API MeshAnimatedVertex* GetAnimatedVertex(void);

			DIYLIB_API void SetWeight(float weight);
			DIYLIB_API float GetWeight(void);

		private:
			MESH_FRIENDS

			MeshSkinWeight(MeshSkinWeights* skin_weights, MeshAnimatedVertex* animated_vertex, float weight);

			MeshSkinWeights* mSkinWeights;

			MeshAnimatedVertex* mAnimatedVertex;
			float mWeight;
			MeshVertex mDeltaVertex;

			void SetAnimatedVertex(MeshAnimatedVertex* animated_vertex);

			friend bool SortDecreasing(MeshSkinWeight* skin_weight_a, MeshSkinWeight* skin_weight_b);
		};

		bool SortDecreasing(MeshSkinWeight* skin_weight_a, MeshSkinWeight* skin_weight_b);
	}
}

#endif
