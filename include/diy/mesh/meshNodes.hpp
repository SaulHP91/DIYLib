#ifndef MESHNODES_HPP
#define MESHNODES_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <string>
#include <fstream>
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
		class MeshNodes
		{
		public:
			DIYLIB_API ~MeshNodes(void);

			DIYLIB_API MeshFile* GetFile(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshNode* New(glm::mat4 matrix = glm::mat4(1.0f));
			DIYLIB_API MeshNode* New(std::string name, glm::mat4 matrix = glm::mat4(1.0f));
			DIYLIB_API void Clear(void);
			DIYLIB_API void Delete(MeshNode* node);
			DIYLIB_API void Delete(std::string name);
			DIYLIB_API void DeleteUnused(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

			DIYLIB_API MeshNode* operator[](int index);
			DIYLIB_API MeshNode* Find(std::string name, bool create = false);

			DIYLIB_API MeshNode* operator()(void);
			DIYLIB_API MeshNode* First(void);
			DIYLIB_API MeshNode* Next(void);
			DIYLIB_API MeshNode* Last(void);
			DIYLIB_API MeshNode* Previous(void);

			DIYLIB_API void Render(Camera* camera);

			DIYLIB_API void MouseMove(Camera* camera, glm::vec2 mousePos);
			DIYLIB_API void Select(MeshNode* node);
			DIYLIB_API MeshNode* Select(void);
			DIYLIB_API MeshNode* Selected(void);
			DIYLIB_API void ClearSelection(void);

			DIYLIB_API void LoadBindPose(void);
			DIYLIB_API void AssumeBindPose(void);

			DIYLIB_API void LoadBindPose(std::vector<MeshNode*>& range);
			DIYLIB_API void AssumeBindPose(std::vector<MeshNode*>& range);

			DIYLIB_API MeshNode* FindBySymmetry(MeshNode* node);

			DIYLIB_API void MirrorAdditionalMatrices(void);
			DIYLIB_API void MirrorAdditionalMatricesLeftToRight(void);
			DIYLIB_API void MirrorAdditionalMatricesRightToLeft(void);

			DIYLIB_API void MirrorAdditionalMatrices(std::vector<MeshNode*>& range);
			DIYLIB_API void MirrorAdditionalMatricesLeftToRight(std::vector<MeshNode*>& range);
			DIYLIB_API void MirrorAdditionalMatricesRightToLeft(std::vector<MeshNode*>& range);

			DIYLIB_API int GetIKChain(int* node_indices, glm::mat4* node_matrices, int max_nodes, MeshNode* final_effector);
			DIYLIB_API void SetIKChain(int* node_indices, glm::mat4* node_matrices, int length, bool update = true);

			DIYLIB_API void ReverseAdditionalMatrices(void);
			DIYLIB_API void ReverseAdditionalMatrices(std::vector<MeshNode*>& range);

			DIYLIB_API void AutoAlign(void);
			DIYLIB_API void AutoAlign(std::vector<MeshNode*>& range);
			DIYLIB_API void ResetAlign(void);
			DIYLIB_API void ResetAlign(std::vector<MeshNode*>& range);

		private:
			MESH_FRIENDS

			MeshNodes(MeshFile* scene);

			MeshFile* mFile;

			std::vector<MeshNode*> mNodes;
			std::vector<MeshNode*>::iterator mNode;

			void Update(void);

			MeshNode* mFocused;
			MeshNode* mSelected;

			void UpdateIndices(void);

			Selector* mSelector;
		};
	}
}

#endif
