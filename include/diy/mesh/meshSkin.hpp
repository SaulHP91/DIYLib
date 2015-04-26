#ifndef MESHSKIN_HPP
#define MESHSKIN_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

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
		class MeshSkin
		{
		public:
			DIYLIB_API ~MeshSkin(void);

			DIYLIB_API MeshSkins* GetSkins(void);

			DIYLIB_API MeshNode* GetNode(void);

			DIYLIB_API int GetIndex(void);
			DIYLIB_API MeshSkinWeights* GetSkinWeights(void);

			glm::mat4 OffsetMatrix;

			DIYLIB_API void AppendAnimatedVertex(MeshAnimatedVertex* animated_vertex, float weight);
			DIYLIB_API void AppendAnimatedVertex(int animatedVertexIndex, float weight);

			DIYLIB_API void RemoveAnimatedVertex(MeshAnimatedVertex* animated_vertex);
			DIYLIB_API void RemoveAnimatedVertex(int animatedVertexIndex);

		private:
			MESH_FRIENDS

			MeshSkin(MeshSkins* skins, MeshNode* node, int index);

			MeshSkins* mSkins;

			MeshNode* mNode;

			int mIndex;
			MeshSkinWeights* mSkinWeights;

			glm::mat4 mMatrix;
			glm::mat3 mNormalMatrix;
			glm::vec3 mOrigin;
		};
	}
}

#endif

