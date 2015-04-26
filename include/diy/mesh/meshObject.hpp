#ifndef MESHOBJECT_HPP
#define MESHOBJECT_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <string>
#include <vector>

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
		class MeshObject
		{
		public:
			DIYLIB_API ~MeshObject(void);

			DIYLIB_API MeshObjects* GetObjects(void);
			DIYLIB_API MeshNode* GetNode(void);

			DIYLIB_API int GetIndex(void);
			DIYLIB_API std::string GetName(void);

			DIYLIB_API MeshAnimatedVertices* GetAnimatedVertices(void);
			DIYLIB_API MeshFaces* GetFaces(void);
			DIYLIB_API MeshGroups* GetGroups(void);
			DIYLIB_API MeshSkins* GetSkins(void);
			DIYLIB_API MeshMorphTargets* GetMorphTargets(void);

			DIYLIB_API bool SetName(std::string newName);

			DIYLIB_API void SetNode(MeshNode* node);
			DIYLIB_API void SetNode(std::string nodeName);
			DIYLIB_API void SetNode(int nodeIndex);
			DIYLIB_API void Update(void);
			DIYLIB_API void Freeze(void);
			DIYLIB_API void Mirror(int mirror_axis);

			DIYLIB_API bool operator==(MeshObject object);

			DIYLIB_API glm::vec3 GetMinLimits(void);
			DIYLIB_API glm::vec3 GetMaxLimits(void);
			DIYLIB_API glm::vec3 GetCenter(void);

			DIYLIB_API void Render(MeshMaterial* material);
			DIYLIB_API void RenderOpaque(void);
			DIYLIB_API void RenderTransparent(void);
			DIYLIB_API void Render(Camera* camera);

			DIYLIB_API bool IsVisible(void);
			DIYLIB_API void Show(void);
			DIYLIB_API void Hide(void);
			DIYLIB_API void ToggleVisible(void);

		private:
			MESH_FRIENDS

			MeshObject(MeshObjects* objects, int index, std::string name);

			MeshObjects* mObjects;
			MeshNode* mNode;

			glm::mat4 mOffsetMatrix;
			glm::mat4 mMatrix;
			glm::mat3 mNormalMatrix;
			glm::vec3 mOrigin;

			int mIndex;
			std::string mName;
			std::string mComment;

			MeshAnimatedVertices* mAnimatedVertices;
			MeshFaces* mFaces;
			MeshGroups* mGroups;
			MeshSkins* mSkins;
			MeshMorphTargets* mMorphTargets;

			glm::vec3 mMinLimits;
			glm::vec3 mMaxLimits;
			glm::vec3 mCenter;
			void UpdateLimits(glm::vec3 reference);

			bool mVisible;

			void UpdateArrays(void);

			friend bool SortObjectsIncreasingRespectName(MeshObject* object_a, MeshObject* object_b);
		};

		bool SortObjectsIncreasingRespectName(MeshObject* object_a, MeshObject* object_b);
	}
}

#endif
