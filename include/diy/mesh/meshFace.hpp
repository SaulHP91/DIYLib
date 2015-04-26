#ifndef MESHFACE_HPP
#define MESHFACE_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <vector>
#include <string>

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
		class MeshFace
		{
		public:
			DIYLIB_API ~MeshFace(void);

			DIYLIB_API MeshFaces* GetFaces(void);
			DIYLIB_API MeshGroup* GetGroup(void);

			DIYLIB_API int GetIndex(void);
			DIYLIB_API std::vector<MeshAnimatedVertex*> GetAnimatedVertices(void);

			DIYLIB_API glm::vec3 GetCentroid(void);
			DIYLIB_API glm::vec3 GetTangent(void);
			DIYLIB_API glm::vec3 GetBitangent(void);
			DIYLIB_API glm::vec3 GetNormal(void);

			DIYLIB_API void AppendAnimatedVertex(MeshAnimatedVertex* animated_vertex);
			DIYLIB_API void AppendAnimatedVertex(int animatedVertexIndex);
			DIYLIB_API void RemoveAnimatedVertex(MeshAnimatedVertex* animated_vertex);
			DIYLIB_API void RemoveAnimatedVertex(int animatedVertexIndex);
			DIYLIB_API void SetAnimatedVertices(std::vector<MeshAnimatedVertex*> animatedVertices);
			DIYLIB_API void SetAnimatedVertices(std::vector<int> animatedVertexIndices);
			DIYLIB_API void ReplaceAnimatedVertex(MeshAnimatedVertex* destiny, MeshAnimatedVertex* source);

			DIYLIB_API void SetGroup(MeshGroup* group);
			DIYLIB_API void SetGroup(int groupIndex);

			DIYLIB_API void SetMaterial(MeshMaterial* material);
			DIYLIB_API void SetMaterial(std::string materialName);
			DIYLIB_API void SetMaterial(int materialIndex);

			DIYLIB_API void Triangulate(std::vector<std::vector<MeshAnimatedVertex*> >& triangles);
			DIYLIB_API void Reverse(void);

		private:
			MESH_FRIENDS

			MeshFace(MeshFaces* faces, MeshGroup* group, int index, std::vector<MeshAnimatedVertex*> animatedVertices);
			MeshFace(MeshFaces* faces, MeshGroup* group, int index, std::vector<int> animatedVertexIndices);

			MeshFaces* mFaces;
			MeshGroup* mGroup;

			int mIndex;
			std::vector<MeshAnimatedVertex*> mAnimatedVertices;

			glm::vec3 mCentroid;
			glm::vec3 mTangent;
			glm::vec3 mBitangent;
			glm::vec3 mNormal;

			void Update(void);
		};
	}
}

#endif
