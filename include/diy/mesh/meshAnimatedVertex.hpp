#ifndef MESHANIMATEDVERTEX_HPP
#define MESHANIMATEDVERTEX_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshVertex.hpp>

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
		class MeshAnimatedVertex
		{
		public:
			DIYLIB_API ~MeshAnimatedVertex(void);

			DIYLIB_API MeshAnimatedVertices* GetAnimatedVertices(void);

			DIYLIB_API int GetIndex(void);

			DIYLIB_API MeshVertex GetVertex(void);
			DIYLIB_API MeshVertex GetFinalVertex(void);
			DIYLIB_API void SetVertex(MeshVertex vertex);

			DIYLIB_API std::vector<MeshFace*> GetFaces(void);
			DIYLIB_API std::vector<MeshSkinWeight*> GetSkinWeights(void);
			DIYLIB_API std::vector<MeshMorphTargetVertex*> GetMorphTargetVertices(void);

			DIYLIB_API void AppendSkinWeight(MeshNode* node, float weight);
			DIYLIB_API void AppendSkinWeight(std::string nodeName, float weight);
			DIYLIB_API void AppendSkinWeight(int nodeIndex, float weight);

			DIYLIB_API void RemoveSkinWeight(MeshNode* node);
			DIYLIB_API void RemoveSkinWeight(std::string nodeName);
			DIYLIB_API void RemoveSkinWeight(int nodeIndex);

			DIYLIB_API void AppendMorphTargetVertex(MeshMorphTarget* morph_target, MeshVertex vertex);
			DIYLIB_API void AppendMorphTargetVertex(std::string morphTargetName, MeshVertex vertex);

			DIYLIB_API void RemoveMorphTargetVertex(MeshMorphTarget* morph_target);
			DIYLIB_API void RemoveMorphTargetVertex(std::string morphTargetName);

			DIYLIB_API void CalculateTangents(void);
			DIYLIB_API void CalculateNormal(void);
			DIYLIB_API void ReverseNormal(void);

			DIYLIB_API void LimitSkinWeights(unsigned int maxSkinWeightsPerVertex);
			DIYLIB_API void NormalizeSkinWeights(void);
			DIYLIB_API MeshAnimatedVertex* Duplicate(void);

		private:
			MESH_FRIENDS

			MeshAnimatedVertex(MeshAnimatedVertices* animated_vertices, int index, MeshVertex vertex = MeshVertex());
			MeshAnimatedVertices* mAnimatedVertices;

			int mIndex;
			MeshVertex mVertex;
			std::vector<MeshFace*> mFaces;
			std::vector<MeshSkinWeight*> mSkinWeights;
			std::vector<MeshMorphTargetVertex*> mMorphTargetVertices;

			MeshAnimatedVertex* mReserved;

			friend bool SortAnimatedVerticesRespectPosition(MeshAnimatedVertex* animated_vertex_a, MeshAnimatedVertex* animated_vertex_b);
			friend bool SortAnimatedVerticesRespectIndex(MeshAnimatedVertex* animated_vertex_a, MeshAnimatedVertex* animated_vertex_b);
		};

		bool SortAnimatedVerticesRespectPosition(MeshAnimatedVertex* animated_vertex_a, MeshAnimatedVertex* animated_vertex_b);
		bool SortAnimatedVerticesRespectIndex(MeshAnimatedVertex* animated_vertex_a, MeshAnimatedVertex* animated_vertex_b);
	}
}

#endif
