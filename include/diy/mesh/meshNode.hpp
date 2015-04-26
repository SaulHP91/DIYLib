#ifndef MESHNODE_HPP
#define MESHNODE_HPP

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
		class MeshNode
		{
		public:
			DIYLIB_API ~MeshNode(void);

			DIYLIB_API MeshNodes* Nodes(void);

			DIYLIB_API int GetIndex(void);
			DIYLIB_API std::string GetName(void);
			DIYLIB_API bool SetName(std::string name);

			DIYLIB_API MeshNode* GetParent(void);
			DIYLIB_API std::vector<MeshNode*> GetChildren(void);
			DIYLIB_API std::vector<MeshObject*> GetObjects(void);
			DIYLIB_API std::vector<MeshSkin*> GetSkins(void);

			DIYLIB_API MeshRigidBody* GetRigidBody(void);
			DIYLIB_API void SetRigidBody(MeshRigidBody* rigid_body);

			DIYLIB_API void SetMatrix(glm::mat4 matrix, bool linked = true, bool update = true);
			DIYLIB_API void SetAdditionalMatrix(glm::mat4 additionalMatrix, bool linked = true, bool update = true);
			DIYLIB_API glm::mat4 GetMatrix(void);
			DIYLIB_API glm::mat4 GetAdditionalMatrix(void);
			DIYLIB_API glm::mat4 GetFinalMatrix(void);
			DIYLIB_API glm::mat4 GetOffsetMatrix(void);

			DIYLIB_API void SetParent(MeshNode* parentNode, bool update = true);
			DIYLIB_API void SetParent(std::string parentNodeName, bool update = true);
			DIYLIB_API void SetParent(int parentNodeIndex, bool update = true);

			DIYLIB_API void AppendChild(MeshNode* childNode, bool update = true);
			DIYLIB_API void AppendChild(std::string childNodeName, bool update = true);
			DIYLIB_API void AppendChild(int childNodeIndex, bool update = true);

			DIYLIB_API void RemoveChild(MeshNode* childNode, bool update = true);
			DIYLIB_API void RemoveChild(std::string childNodeName, bool update = true);
			DIYLIB_API void RemoveChild(int childNodeIndex, bool update = true);

			DIYLIB_API void AppendObject(MeshObject* object);
			DIYLIB_API void AppendObject(std::string objectName);
			DIYLIB_API void AppendObject(int objectIndex);

			DIYLIB_API void RemoveObject(MeshObject* object);
			DIYLIB_API void RemoveObject(std::string objectName);
			DIYLIB_API void RemoveObject(int objectIndex);

			DIYLIB_API void AppendSkin(MeshSkin* skin);
			DIYLIB_API void RemoveSkin(MeshSkin* skin);

			DIYLIB_API bool IsInUse(void);

			DIYLIB_API void SaveDefaultPose(void);
			DIYLIB_API void LoadDefaultPose(void);

			DIYLIB_API int Depth(void);

			DIYLIB_API static int SymmetryMode;
			DIYLIB_API static float SymmetryXAxis;
			DIYLIB_API static float SymmetryYAxis;
			DIYLIB_API static float SymmetryZAxis;

			DIYLIB_API void GetAncestorList(std::vector<MeshNode*>& ancestors);
			DIYLIB_API void GetDescendantList(std::vector<MeshNode*>& descendants);

			DIYLIB_API void AutoAlign(glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f));
			DIYLIB_API void ResetAlign(void);

			DIYLIB_API void MoveFirst(void);
			DIYLIB_API void MoveUp(void);
			DIYLIB_API void MoveDown(void);
			DIYLIB_API void MoveLast(void);

		private:
			MESH_FRIENDS

			MeshNode(MeshNodes* nodes, int index, std::string name, glm::mat4 matrix);

			MeshNodes* mNodes;

			int mIndex;
			std::string mName;
			std::string mComment;
			MeshNode* mParent;
			MeshRigidBody* mRigidBody;
			std::vector<MeshObject*> mObjects;
			std::vector<MeshNode*> mChildren;
			std::vector<MeshSkin*> mSkins;

			glm::mat4 mMatrix;
			glm::mat4 mAdditionalMatrix;
			glm::mat4 mOldFinalMatrix;
			glm::mat4 mFinalMatrix;
			glm::mat4 mModelMatrix;
			glm::mat4 mCombinedMatrix;
			glm::mat4 mOffsetMatrix;
			glm::mat3 mNormalMatrix;
			glm::vec3 mOrigin;
			glm::vec3 mPivot;

			bool mFocused;
			void Render(Camera* camera);
			void UpdateSelectable(Camera* camera);

			int mDepth;

			static unsigned int mJointPositionBuffer;
			static unsigned int mJointNormalBuffer;
			static unsigned int mJointFaceBuffer;

			static unsigned int mLinkPositionBuffer;
			static unsigned int mLinkNormalBuffer;
			static unsigned int mLinkFaceBuffer;

			static Shader mShader;

			static bool mInitializedBuffersAndShaders;
			void InitializeBuffersAndShaders(void);

			SelectableMesh* mSelectableJoint;
			SelectableMesh* mSelectableLink;
		};
	}
}

#endif
