#ifndef MESHANIMATEDVERTICES_HPP
#define MESHANIMATEDVERTICES_HPP

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
		class MeshAnimatedVertices
		{
		public:
			DIYLIB_API ~MeshAnimatedVertices(void);

			DIYLIB_API MeshObject* GetObject(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshAnimatedVertex* New(MeshVertex vertex = MeshVertex());
			DIYLIB_API MeshAnimatedVertex* Duplicate(MeshAnimatedVertex* animated_vertex);
			DIYLIB_API MeshAnimatedVertex* Duplicate(int animatedVertexIndex);
			DIYLIB_API void Clear(void);
			DIYLIB_API void Delete(MeshAnimatedVertex* animated_vertex);

			DIYLIB_API MeshAnimatedVertex* operator[](int index);

			DIYLIB_API MeshAnimatedVertex* operator()(void);
			DIYLIB_API MeshAnimatedVertex* First(void);
			DIYLIB_API MeshAnimatedVertex* Next(void);
			DIYLIB_API MeshAnimatedVertex* Last(void);
			DIYLIB_API MeshAnimatedVertex* Previous(void);

			DIYLIB_API void CalculateTangents(void);
			DIYLIB_API void CalculateNormals(void);
			DIYLIB_API void ReverseNormals(void);

			DIYLIB_API void LimitSkinWeights(unsigned int maxSkinWeightsPerVertex);
			DIYLIB_API void NormalizeSkinWeights(void);

			DIYLIB_API void Merge(void);
			DIYLIB_API void AverageNormals(void);

			DIYLIB_API void TransformTexCoords(glm::mat3 transform);

			DIYLIB_API const void* GetPositionPointer(void);
			DIYLIB_API const void* GetTexCoordPointer(void);
			DIYLIB_API const void* GetTangentPointer(void);
			DIYLIB_API const void* GetBitangentPointer(void);
			DIYLIB_API const void* GetNormalPointer(void);

			DIYLIB_API void RenderNormals(Camera* camera);
			DIYLIB_API void RenderTangents(Camera* camera);

		private:
			MESH_FRIENDS

			MeshAnimatedVertices(MeshObject* object);

			MeshObject* mObject;

			std::vector<MeshAnimatedVertex*> mAnimatedVertices;
			std::vector<MeshAnimatedVertex*>::iterator mAnimatedVertex;

			unsigned int mCapacity;
			unsigned int mSize;
			float* mPositionArray;
			float* mMorphedPositionArray;
			float* mFinalPositionArray;
			float* mTexCoordArray;
			float* mTangentArray;
			float* mFinalTangentArray;
			float* mBitangentArray;
			float* mFinalBitangentArray;
			float* mNormalArray;
			float* mFinalNormalArray;
			int* mSkinNodeArray;
			float* mSkinWeightArray;
			void Realloc(unsigned int size);
			void UpdateArray(void);
		};
	}
}

#endif
