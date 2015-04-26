#include <diy/diy.hpp>

#include <gl/glew.h>
#include <glm/ext.hpp>
#include <glm/own.hpp>

namespace diy
{
	namespace mesh
	{
		MeshNodes::MeshNodes(MeshFile* file) :
			mFile(file),
			mFocused(0),
			mSelected(0)
		{
			mNode = mNodes.end();

			mSelector = new Selector();
		}

		MeshNodes::~MeshNodes(void)
		{
			delete mSelector;

			Clear();
		}

		MeshFile* MeshNodes::GetFile(void)
		{
			return mFile;
		}

		int MeshNodes::GetCount(void)
		{
			return mNodes.size();
		}

		MeshNode* MeshNodes::New(glm::mat4 matrix)
		{
			int index = 0;
			char name[256];

			MeshNode* new_node;
			do
			{
				sprintf(name, "Node%i", index++);
				new_node = New(name, matrix);
			} while (!new_node);

			return new_node;
		}

		MeshNode* MeshNodes::New(std::string name, glm::mat4 matrix)
		{
			if (name.empty())
			{
				return 0;
			}

			if (Find(name))
			{
				return 0;
			}

			mNodes.push_back(new MeshNode(this, mNodes.size(), name, matrix));

			return mNodes.back();
		}

		void MeshNodes::Clear(void)
		{
			while (mNodes.size())
			{
				delete mNodes.back();
				mNodes.pop_back();
			}
			mNodes.swap(std::vector<MeshNode*>(mNodes));

			mNode = mNodes.end();
		}

		void MeshNodes::Delete(MeshNode* node)
		{
			std::vector<MeshNode*>::iterator iterator = find(mNodes.begin(), mNodes.end(), node);

			if (iterator == mNodes.end())
			{
				return;
			}

			delete *iterator;
			iterator = mNodes.erase(iterator);

			for (iterator = iterator; iterator != mNodes.end(); ++iterator)
			{
				--(*iterator)->mIndex;
			}

			Update();
		}

		void MeshNodes::Delete(std::string name)
		{
			Delete(Find(name));
		}

		void MeshNodes::DeleteUnused(void)
		{
			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end();)
			{
				if (!(*node)->IsInUse())
				{
					if (*node == mSelected)
					{
						ClearSelection();
					}
					delete *node;
					mNodes.erase(node);
					node = mNodes.begin();
					continue;
				}
				++node;
			}
			int index = 0;
			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				(*node)->mIndex = index++;
			}
		}

		bool MeshNodes::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			mNodes.reserve(count);

			for (int i = 0; i < count; ++i)
			{
				int length;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* name = new char[length];
				in.read(name, length);

				in.read(reinterpret_cast<char*>(&length), 4);
				char* comment = new char[length];
				in.read(comment, length);

				glm::mat4 matrix;
				in.read(reinterpret_cast<char*>(glm::value_ptr(matrix)), 64);

				glm::mat4 additionalMatrix;
				in.read(reinterpret_cast<char*>(glm::value_ptr(additionalMatrix)), 64);

				MeshNode* node = Find(name, true);
				node->mComment = std::string(comment);
				node->mAdditionalMatrix = additionalMatrix;
				node->SetMatrix(matrix);

				delete[] name;
				delete[] comment;

				int parentIndex;
				in.read(reinterpret_cast<char*>(&parentIndex), 4);
				node->SetParent(parentIndex);

				int endNode;
				in.read(reinterpret_cast<char*>(&endNode), 4);
			}

			int endNodes;
			in.read(reinterpret_cast<char*>(&endNodes), 4);

			return true;
		}

		void MeshNodes::SaveToFStream(std::ofstream& out)
		{
			int count = mNodes.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				int length = strlen((*node)->mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*node)->mName.c_str(), length);
				length = strlen((*node)->mComment.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*node)->mComment.c_str(), length);

				out.write(reinterpret_cast<const char*>(glm::value_ptr((*node)->mMatrix)), 64);
				out.write(reinterpret_cast<const char*>(glm::value_ptr((*node)->mAdditionalMatrix)), 64);

				if ((*node)->mParent)
				{
					int parentIndex = (*node)->mParent->mIndex;
					out.write(reinterpret_cast<char*>(&parentIndex), 4);
				}
				else
				{
					int parentIndex = -1;
					out.write(reinterpret_cast<char*>(&parentIndex), 4);
				}

				int endNode = 0;
				out.write(reinterpret_cast<char*>(&endNode), 4);
			}

			int endNodes = 0;
			out.write(reinterpret_cast<char*>(&endNodes), 4);
		}

		MeshNode* MeshNodes::operator[](int index)
		{
			if (index < 0 || index >= int(mNodes.size()))
			{
				return 0;
			}

			return mNodes[index];
		}

		MeshNode* MeshNodes::Find(std::string name, bool create)
		{
			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				if (!_stricmp((*node)->mName.c_str(), name.c_str()))
				{
					return *node;
				}
			}

			if (create)
			{
				return New(name);
			}

			return 0;
		}

		MeshNode* MeshNodes::operator()(void)
		{
			if (mNode == mNodes.end())
			{
				return 0;
			}

			return *mNode;
		}

		MeshNode* MeshNodes::First(void)
		{
			if (mNodes.empty())
			{
				mNode = mNodes.end();
			}
			else
			{
				mNode = mNodes.begin();
			}

			return operator()();
		}

		MeshNode* MeshNodes::Next(void)
		{
			if (mNode != mNodes.end())
			{
				++mNode;
			}

			return operator()();
		}

		MeshNode* MeshNodes::Last(void)
		{
			if (mNodes.empty())
			{
				mNode = mNodes.end();
			}
			else
			{
				mNode = mNodes.end() - 1;
			}

			return operator()();
		}

		MeshNode* MeshNodes::Previous(void)
		{
			if (mNode == mNodes.begin())
			{
				mNode = mNodes.end();
			}
			else if (mNode != mNodes.end())
			{
				--mNode;
			}

			return operator()();
		}

		void MeshNodes::Update(void)
		{
			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				(*node)->mOldFinalMatrix = (*node)->mFinalMatrix;
				if ((*node)->mParent)
				{
					(*node)->mDepth = (*node)->mParent->mDepth + 1;
					(*node)->mFinalMatrix = (*node)->mParent->mFinalMatrix * (*node)->mMatrix * (*node)->mAdditionalMatrix;
					(*node)->mCombinedMatrix = (*node)->mParent->mCombinedMatrix * (*node)->mMatrix;
				}
				else
				{
					(*node)->mDepth = 0;
					(*node)->mFinalMatrix = (*node)->mMatrix * (*node)->mAdditionalMatrix;
					(*node)->mCombinedMatrix = (*node)->mMatrix;
				}

				(*node)->mModelMatrix = (*node)->mFinalMatrix;
				(*node)->mOffsetMatrix = glm::inverse((*node)->mCombinedMatrix);
				(*node)->mNormalMatrix = glm::mat3((*node)->mFinalMatrix);
				(*node)->mOrigin = glm::vec3((*node)->mFinalMatrix[3]);
			}
		}

		void MeshNodes::Render(Camera* camera)
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_COLOR_MATERIAL);

			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				(*node)->Render(camera);
			}

			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			glDisable(GL_COLOR_MATERIAL);
		}

		void MeshNodes::MouseMove(Camera* camera, glm::vec2 mousePos)
		{
			if (mSelected)
			{
				return;
			}

			mSelector->SetCamera(camera);

			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				(*node)->UpdateSelectable(camera);
			}

			mSelector->MouseMove(mousePos);

			if (mSelector->GetSelected())
			{
				mFocused = reinterpret_cast<MeshNode*>(mSelector->GetSelected()->GetData());

				return;
			}

			mFocused = 0;
		}

		void MeshNodes::Select(MeshNode* node)
		{
			mFocused = node;

			mSelected = mFocused;
		}

		MeshNode* MeshNodes::Select(void)
		{
			mSelected = mFocused;

			return mSelected;
		}

		MeshNode* MeshNodes::Selected(void)
		{
			return mSelected;
		}

		void MeshNodes::ClearSelection(void)
		{
			mFocused = 0;
			mSelected = 0;
		}

		void MeshNodes::LoadBindPose(void)
		{
			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				(*node)->mAdditionalMatrix = glm::mat4(1.0f);
			}

			Update();

			mFile->mObjects->Update();
		}

		void MeshNodes::AssumeBindPose(void)
		{
			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				(*node)->mMatrix = (*node)->mMatrix * (*node)->mAdditionalMatrix;

				(*node)->mAdditionalMatrix = glm::mat4(1.0f);
			}

			Update();

			mFile->mObjects->Update();
		}

		void MeshNodes::LoadBindPose(std::vector<MeshNode*>& range)
		{
			for (std::vector<MeshNode*>::iterator node = range.begin(); node != range.end(); ++node)
			{
				(*node)->mAdditionalMatrix = glm::mat4(1.0f);
			}

			Update();

			mFile->mObjects->Update();
		}

		void MeshNodes::AssumeBindPose(std::vector<MeshNode*>& range)
		{
			for (std::vector<MeshNode*>::iterator node = range.begin(); node != range.end(); ++node)
			{
				(*node)->mMatrix = (*node)->mMatrix * (*node)->mAdditionalMatrix;

				(*node)->mAdditionalMatrix = glm::mat4(1.0f);
			}

			Update();

			mFile->mObjects->Update();
		}

		MeshNode* MeshNodes::FindBySymmetry(MeshNode* node)
		{
			for (std::vector<MeshNode*>::iterator iter = mNodes.begin(); iter != mNodes.end(); ++iter)
			{
				if (*iter != node && (*iter)->mDepth == node->mDepth)
				{
					glm::vec3 originA = glm::vec3((*iter)->mCombinedMatrix[3]);
					glm::vec3 originB = glm::vec3(node->mCombinedMatrix[3]);
					if (fabs(originA.x + originB.x + originA.y - originB.y) < 0.05f && fabs(originA.z - originB.z) < 0.05f)
					{
						return *iter;
					}
				}
			}

			return 0;
		}

		void MeshNodes::MirrorAdditionalMatrices(void)
		{
			if (MeshNode::SymmetryMode == NODE_SYMMETRY_NONE)
			{
				return;
			}

			for (std::vector<MeshNode*>::iterator iter = mNodes.begin(); iter != mNodes.end(); ++iter)
			{
				glm::vec3 originA = glm::vec3((*iter)->mCombinedMatrix[3]);
				if (originA.x < 0.0f)
				{
					MeshNode* node = FindBySymmetry(*iter);
					if (node)
					{
						glm::vec3 AT = glm::matrixToTranslation((*iter)->mAdditionalMatrix);
						glm::vec3 AR = glm::matrixToRotation((*iter)->mAdditionalMatrix);
						glm::vec3 AS = glm::matrixToScaling((*iter)->mAdditionalMatrix);
						glm::vec3 factorA(MeshNode::SymmetryXAxis, MeshNode::SymmetryYAxis, MeshNode::SymmetryZAxis);
						if (MeshNode::SymmetryMode == NODE_SYMMETRY_MIRROR)
						{
							factorA *= -1.0f;
						}

						glm::vec3 BT = glm::matrixToTranslation(node->mAdditionalMatrix);
						glm::vec3 BR = glm::matrixToRotation(node->mAdditionalMatrix);
						glm::vec3 BS = glm::matrixToScaling(node->mAdditionalMatrix);
						glm::vec3 factorB;
						factorB.x = factorA.x == 0.0f ? 1.0f : 0.0f;
						factorB.y = factorA.y == 0.0f ? 1.0f : 0.0f;
						factorB.z = factorA.z == 0.0f ? 1.0f : 0.0f;

						(*iter)->mAdditionalMatrix = glm::compose(factorA * BT + factorB * AT, factorA * BR + factorB * AR, BS);
						node->mAdditionalMatrix = glm::compose(factorA * AT + factorB * BT, factorA * AR + factorB * BR, AS);
					}
				}
			}

			Update();

			mFile->mObjects->Update();
		}

		void MeshNodes::MirrorAdditionalMatricesLeftToRight(void)
		{
			if (MeshNode::SymmetryMode == NODE_SYMMETRY_NONE)
			{
				return;
			}

			for (std::vector<MeshNode*>::iterator iter = mNodes.begin(); iter != mNodes.end(); ++iter)
			{
				glm::vec3 originA = glm::vec3((*iter)->mCombinedMatrix[3]);
				if (originA.x > 0.0f)
				{
					MeshNode* node = FindBySymmetry(*iter);
					if (node)
					{
						glm::vec3 AT = glm::matrixToTranslation((*iter)->mAdditionalMatrix);
						glm::vec3 AR = glm::matrixToRotation((*iter)->mAdditionalMatrix);
						glm::vec3 AS = glm::matrixToScaling((*iter)->mAdditionalMatrix);
						glm::vec3 factorA(MeshNode::SymmetryXAxis, MeshNode::SymmetryYAxis, MeshNode::SymmetryZAxis);
						if (MeshNode::SymmetryMode == NODE_SYMMETRY_MIRROR)
						{
							factorA *= -1.0f;
						}

						glm::vec3 BT = glm::matrixToTranslation(node->mAdditionalMatrix);
						glm::vec3 BR = glm::matrixToRotation(node->mAdditionalMatrix);
						glm::vec3 BS = glm::matrixToScaling(node->mAdditionalMatrix);
						glm::vec3 factorB;
						factorB.x = factorA.x == 0.0f ? 1.0f : 0.0f;
						factorB.y = factorA.y == 0.0f ? 1.0f : 0.0f;
						factorB.z = factorA.z == 0.0f ? 1.0f : 0.0f;

						node->mAdditionalMatrix = glm::compose(factorA * AT + factorB * BT, factorA * AR + factorB * BR, AS);
					}
				}
			}

			Update();

			mFile->mObjects->Update();
		}

		void MeshNodes::MirrorAdditionalMatricesRightToLeft(void)
		{
			if (MeshNode::SymmetryMode == NODE_SYMMETRY_NONE)
			{
				return;
			}

			for (std::vector<MeshNode*>::iterator iter = mNodes.begin(); iter != mNodes.end(); ++iter)
			{
				glm::vec3 originA = glm::vec3((*iter)->mCombinedMatrix[3]);
				if (originA.x < 0.0f)
				{
					MeshNode* node = FindBySymmetry(*iter);
					if (node)
					{
						glm::vec3 AT = glm::matrixToTranslation((*iter)->mAdditionalMatrix);
						glm::vec3 AR = glm::matrixToRotation((*iter)->mAdditionalMatrix);
						glm::vec3 AS = glm::matrixToScaling((*iter)->mAdditionalMatrix);
						glm::vec3 factorA(MeshNode::SymmetryXAxis, MeshNode::SymmetryYAxis, MeshNode::SymmetryZAxis);
						if (MeshNode::SymmetryMode == NODE_SYMMETRY_MIRROR)
						{
							factorA *= -1.0f;
						}

						glm::vec3 BT = glm::matrixToTranslation(node->mAdditionalMatrix);
						glm::vec3 BR = glm::matrixToRotation(node->mAdditionalMatrix);
						glm::vec3 BS = glm::matrixToScaling(node->mAdditionalMatrix);
						glm::vec3 factorB;
						factorB.x = factorA.x == 0.0f ? 1.0f : 0.0f;
						factorB.y = factorA.y == 0.0f ? 1.0f : 0.0f;
						factorB.z = factorA.z == 0.0f ? 1.0f : 0.0f;

						node->mAdditionalMatrix = glm::compose(factorA * AT + factorB * BT, factorA * AR + factorB * BR, AS);
					}
				}
			}

			Update();

			mFile->mObjects->Update();
		}

		void MeshNodes::MirrorAdditionalMatrices(std::vector<class MeshNode*>& range)
		{
			if (MeshNode::SymmetryMode == NODE_SYMMETRY_NONE)
			{
				return;
			}

			for (std::vector<MeshNode*>::iterator iter = range.begin(); iter != range.end(); ++iter)
			{
				glm::vec3 originA = glm::vec3((*iter)->mCombinedMatrix[3]);

				MeshNode* node = FindBySymmetry(*iter);
				if (node)
				{
					glm::vec3 AT = glm::matrixToTranslation((*iter)->mAdditionalMatrix);
					glm::vec3 AR = glm::matrixToRotation((*iter)->mAdditionalMatrix);
					glm::vec3 AS = glm::matrixToScaling((*iter)->mAdditionalMatrix);
					glm::vec3 factorA(MeshNode::SymmetryXAxis, MeshNode::SymmetryYAxis, MeshNode::SymmetryZAxis);
					if (MeshNode::SymmetryMode == NODE_SYMMETRY_MIRROR)
					{
						factorA *= -1.0f;
					}

					glm::vec3 BT = glm::matrixToTranslation(node->mAdditionalMatrix);
					glm::vec3 BR = glm::matrixToRotation(node->mAdditionalMatrix);
					glm::vec3 BS = glm::matrixToScaling(node->mAdditionalMatrix);
					glm::vec3 factorB;
					factorB.x = factorA.x == 0.0f ? 1.0f : 0.0f;
					factorB.y = factorA.y == 0.0f ? 1.0f : 0.0f;
					factorB.z = factorA.z == 0.0f ? 1.0f : 0.0f;

					(*iter)->mAdditionalMatrix = glm::compose(factorA * BT + factorB * AT, factorA * BR + factorB * AR, BS);
					node->mAdditionalMatrix = glm::compose(factorA * AT + factorB * BT, factorA * AR + factorB * BR, AS);
				}
			}

			Update();

			mFile->mObjects->Update();
		}

		void MeshNodes::MirrorAdditionalMatricesLeftToRight(std::vector<class MeshNode*>& range)
		{
			if (MeshNode::SymmetryMode == NODE_SYMMETRY_NONE)
			{
				return;
			}

			for (std::vector<MeshNode*>::iterator iter = range.begin(); iter != range.end(); ++iter)
			{
				glm::vec3 originA = glm::vec3((*iter)->mCombinedMatrix[3]);

				MeshNode* node = FindBySymmetry(*iter);
				if (node)
				{
					glm::vec3 AT = glm::matrixToTranslation((*iter)->mAdditionalMatrix);
					glm::vec3 AR = glm::matrixToRotation((*iter)->mAdditionalMatrix);
					glm::vec3 AS = glm::matrixToScaling((*iter)->mAdditionalMatrix);
					glm::vec3 factorA(MeshNode::SymmetryXAxis, MeshNode::SymmetryYAxis, MeshNode::SymmetryZAxis);
					if (MeshNode::SymmetryMode == NODE_SYMMETRY_MIRROR)
					{
						factorA *= -1.0f;
					}

					glm::vec3 BT = glm::matrixToTranslation(node->mAdditionalMatrix);
					glm::vec3 BR = glm::matrixToRotation(node->mAdditionalMatrix);
					glm::vec3 BS = glm::matrixToScaling(node->mAdditionalMatrix);
					glm::vec3 factorB;
					factorB.x = factorA.x == 0.0f ? 1.0f : 0.0f;
					factorB.y = factorA.y == 0.0f ? 1.0f : 0.0f;
					factorB.z = factorA.z == 0.0f ? 1.0f : 0.0f;

					if (originA.x < 0.0f)
					{
						(*iter)->mAdditionalMatrix = glm::compose(factorA * BT + factorB * AT, factorA * BR + factorB * AR, BS);
					}
					else
					{
						node->mAdditionalMatrix = glm::compose(factorA * AT + factorB * BT, factorA * AR + factorB * BR, AS);
					}
				}
			}

			Update();

			mFile->mObjects->Update();
		}

		void MeshNodes::MirrorAdditionalMatricesRightToLeft(std::vector<class MeshNode*>& range)
		{
			if (MeshNode::SymmetryMode == NODE_SYMMETRY_NONE)
			{
				return;
			}

			for (std::vector<MeshNode*>::iterator iter = range.begin(); iter != range.end(); ++iter)
			{
				glm::vec3 originA = glm::vec3((*iter)->mCombinedMatrix[3]);

				MeshNode* node = FindBySymmetry(*iter);
				if (node)
				{
					glm::vec3 AT = glm::matrixToTranslation((*iter)->mAdditionalMatrix);
					glm::vec3 AR = glm::matrixToRotation((*iter)->mAdditionalMatrix);
					glm::vec3 AS = glm::matrixToScaling((*iter)->mAdditionalMatrix);
					glm::vec3 factorA(MeshNode::SymmetryXAxis, MeshNode::SymmetryYAxis, MeshNode::SymmetryZAxis);
					if (MeshNode::SymmetryMode == NODE_SYMMETRY_MIRROR)
					{
						factorA *= -1.0f;
					}

					glm::vec3 BT = glm::matrixToTranslation(node->mAdditionalMatrix);
					glm::vec3 BR = glm::matrixToRotation(node->mAdditionalMatrix);
					glm::vec3 BS = glm::matrixToScaling(node->mAdditionalMatrix);
					glm::vec3 factorB;
					factorB.x = factorA.x == 0.0f ? 1.0f : 0.0f;
					factorB.y = factorA.y == 0.0f ? 1.0f : 0.0f;
					factorB.z = factorA.z == 0.0f ? 1.0f : 0.0f;

					if (originA.x > 0.0f)
					{
						(*iter)->mAdditionalMatrix = glm::compose(factorA * BT + factorB * AT, factorA * BR + factorB * AR, BS);
					}
					else
					{
						node->mAdditionalMatrix = glm::compose(factorA * AT + factorB * BT, factorA * AR + factorB * BR, AS);
					}
				}
			}

			Update();

			mFile->mObjects->Update();
		}

		int MeshNodes::GetIKChain(int* node_indices, glm::mat4* node_matrices, int max_nodes, class MeshNode* end_effector)
		{
			MeshNode* ancestor = end_effector;

			int length = 1;
			while (true)
			{
				if (length == max_nodes)
				{
					break;
				}

				if (!ancestor->mParent)
				{
					break;
				}

				if (ancestor->mParent->mChildren.size() > 1)
				{
					break;
				}

				ancestor = ancestor->mParent;
				++length;
			}

			node_indices[0] = ancestor->mIndex;
			node_matrices[0] = ancestor->mFinalMatrix;
			ancestor = end_effector;

			for (int i = length - 1; i > 0; --i)
			{
				node_indices[i] = ancestor->mIndex;
				node_matrices[i] = ancestor->mMatrix * ancestor->mAdditionalMatrix;
				ancestor = ancestor->mParent;
			}

			return length;
		}

		void MeshNodes::SetIKChain(int* node_indices, glm::mat4* node_matrices, int length, bool update)
		{
			MeshNode* root = operator[](*node_indices);
			if (root)
			{
				if (root->mParent)
				{
					root->mAdditionalMatrix = glm::inverse(root->mParent->mFinalMatrix * root->mMatrix) * *node_matrices;
				}
				else
				{
					root->mAdditionalMatrix = glm::inverse(root->mMatrix) * *node_matrices;
				}

				if (MeshNode::SymmetryMode != NODE_SYMMETRY_NONE)
				{
					MeshNode* sym = FindBySymmetry(root);
					if (sym)
					{
						glm::vec3 AT = glm::matrixToTranslation(root->mAdditionalMatrix);
						glm::vec3 AR = glm::matrixToRotation(root->mAdditionalMatrix);
						glm::vec3 AS = glm::matrixToScaling(root->mAdditionalMatrix);
						glm::vec3 factorA(MeshNode::SymmetryXAxis, MeshNode::SymmetryYAxis, MeshNode::SymmetryZAxis);
						if (MeshNode::SymmetryMode == NODE_SYMMETRY_MIRROR)
						{
							factorA *= -1.0f;
						}

						glm::vec3 BT = glm::matrixToTranslation(sym->mAdditionalMatrix);
						glm::vec3 BR = glm::matrixToRotation(sym->mAdditionalMatrix);
						glm::vec3 BS = glm::matrixToScaling(sym->mAdditionalMatrix);
						glm::vec3 factorB;
						factorB.x = factorA.x == 0.0f ? 1.0f : 0.0f;
						factorB.y = factorA.y == 0.0f ? 1.0f : 0.0f;
						factorB.z = factorA.z == 0.0f ? 1.0f : 0.0f;

						sym->mAdditionalMatrix = glm::compose(factorA * AT + factorB * BT, factorA * AR + factorB * BR, AS);
					}
				}
			}

			for (int i = 1; i < length; ++i)
			{
				MeshNode* node = operator[](node_indices[i]);
				if (node)
				{
					node->mAdditionalMatrix = glm::inverse(node->mMatrix) * node_matrices[i];

					if (MeshNode::SymmetryMode != NODE_SYMMETRY_NONE)
					{
						MeshNode* sym = FindBySymmetry(node);
						if (sym)
						{
							glm::vec3 AT = glm::matrixToTranslation(node->mAdditionalMatrix);
							glm::vec3 AR = glm::matrixToRotation(node->mAdditionalMatrix);
							glm::vec3 AS = glm::matrixToScaling(node->mAdditionalMatrix);
							glm::vec3 factorA(MeshNode::SymmetryXAxis, MeshNode::SymmetryYAxis, MeshNode::SymmetryZAxis);
							if (MeshNode::SymmetryMode == NODE_SYMMETRY_MIRROR)
							{
								factorA *= -1.0f;
							}

							glm::vec3 BT = glm::matrixToTranslation(sym->mAdditionalMatrix);
							glm::vec3 BR = glm::matrixToRotation(sym->mAdditionalMatrix);
							glm::vec3 BS = glm::matrixToScaling(sym->mAdditionalMatrix);
							glm::vec3 factorB;
							factorB.x = factorA.x == 0.0f ? 1.0f : 0.0f;
							factorB.y = factorA.y == 0.0f ? 1.0f : 0.0f;
							factorB.z = factorA.z == 0.0f ? 1.0f : 0.0f;

							sym->mAdditionalMatrix = glm::compose(factorA * AT + factorB * BT, factorA * AR + factorB * BR, AS);
						}
					}
				}
			}

			Update();

			if (update)
			{
				mFile->mObjects->Update();
			}
		}

		void MeshNodes::ReverseAdditionalMatrices(void)
		{
			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				glm::vec3 T = glm::matrixToTranslation((*node)->mAdditionalMatrix);
				glm::vec3 R = glm::matrixToRotation((*node)->mAdditionalMatrix);
				glm::vec3 S = glm::matrixToScaling((*node)->mAdditionalMatrix);
				(*node)->mAdditionalMatrix = glm::compose(T, -R, S);
			}

			Update();

			mFile->mObjects->Update();
		}

		void MeshNodes::ReverseAdditionalMatrices(std::vector<MeshNode*>& range)
		{
			for (std::vector<MeshNode*>::iterator node = range.begin(); node != range.end(); ++node)
			{
				glm::vec3 T = glm::matrixToTranslation((*node)->mAdditionalMatrix);
				glm::vec3 R = glm::matrixToRotation((*node)->mAdditionalMatrix);
				glm::vec3 S = glm::matrixToScaling((*node)->mAdditionalMatrix);
				(*node)->mAdditionalMatrix = glm::compose(T, -R, S);
			}

			Update();

			mFile->mObjects->Update();
		}

		void MeshNodes::AutoAlign(void)
		{
			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				(*node)->AutoAlign();
			}
		}

		void MeshNodes::AutoAlign(std::vector<MeshNode*>& range)
		{
			for (std::vector<MeshNode*>::iterator node = range.begin(); node != range.end(); ++node)
			{
				(*node)->AutoAlign();
			}
		}

		void MeshNodes::ResetAlign(void)
		{
			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				(*node)->ResetAlign();
			}
		}

		void MeshNodes::ResetAlign(std::vector<MeshNode*>& range)
		{
			for (std::vector<MeshNode*>::iterator node = range.begin(); node != range.end(); ++node)
			{
				(*node)->ResetAlign();
			}
		}

		void MeshNodes::UpdateIndices(void)
		{
			int index = 0;

			for (std::vector<MeshNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				(*node)->mIndex = index++;
			}
		}
	}
}
