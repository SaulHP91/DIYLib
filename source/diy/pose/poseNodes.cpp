#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace pose
	{
		PoseNodes::PoseNodes(PosePose* pose) :
			mPose(pose)
		{
			;
		}

		PoseNodes::~PoseNodes(void)
		{
			Clear();
		}

		PosePose* PoseNodes::Pose(void)
		{
			return mPose;
		}

		PoseNode* PoseNodes::New(std::string name)
		{
			if (!Find(name))
			{
				mNodes.push_back(new PoseNode(this, name));
				return mNodes.back();
			}

			return 0;
		}

		void PoseNodes::Delete(PoseNode* node)
		{
			std::vector<PoseNode*>::iterator iter = find(mNodes.begin(), mNodes.end(), node);
			if (iter == mNodes.end())
			{
				return;
			}

			mNodes.erase(iter);
		}

		void PoseNodes::Delete(std::string name)
		{
			Delete(Find(name));
		}

		void PoseNodes::Clear(void)
		{
			while (mNodes.size())
			{
				delete mNodes.back();
				mNodes.pop_back();
			}
			mNodes.swap(std::vector<PoseNode*>(mNodes));

			mNode = mNodes.end();
		}

		PoseNode* PoseNodes::Find(std::string name, bool create)
		{
			for (std::vector<PoseNode*>::iterator node_iterator = mNodes.begin(); node_iterator != mNodes.end(); ++node_iterator)
			{
				PoseNode* node = *node_iterator;
				if (!_stricmp(node->mName.c_str(), name.c_str()))
				{
					return node;
				}
			}

			if (create)
			{
				return New(name);
			}

			return 0;
		}

		PoseNode* PoseNodes::operator()(void)
		{
			if (mNode == mNodes.end())
			{
				return 0;
			}

			return *mNode;
		}

		PoseNode* PoseNodes::First(void)
		{
			if (mNodes.size())
			{
				mNode = mNodes.begin();
			}
			else
			{
				mNode = mNodes.end();
			}

			return operator()();
		}

		PoseNode* PoseNodes::Next(void)
		{
			if (mNode != mNodes.end())
			{
				++mNode;
			}

			return operator()();
		}

		PoseNode* PoseNodes::Last(void)
		{
			if (mNodes.size())
			{
				mNode = mNodes.end() - 1;
			}
			else
			{
				mNode = mNodes.end();
			}

			return operator()();
		}

		PoseNode* PoseNodes::Previous(void)
		{
			if (mNode == mNodes.begin())
			{
				mNode = mNodes.end();
			}
			else
			{
				--mNode;
			}

			return operator()();
		}

		int PoseNodes::Count(void)
		{
			return mNodes.size();
		}

		bool PoseNodes::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			for (int i = 0; i < count; ++i)
			{
				int length;
				in.read(reinterpret_cast<char*>(&length), 4);

				char* name = new char[length];
				in.read(name, length);

				PoseNode* node = Find(name, true);
				delete[] name;

				in.read(reinterpret_cast<char*>(glm::value_ptr(node->mTranslation)), 12);
				in.read(reinterpret_cast<char*>(const_cast<float*>(glm::value_ptr(node->mRotation))), 16);
				in.read(reinterpret_cast<char*>(glm::value_ptr(node->mScaling)), 12);

				node->Update();
			}

			return true;
		}

		void PoseNodes::SaveToFStream(std::ofstream& out)
		{
			int count = mNodes.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<PoseNode*>::iterator node = mNodes.begin(); node != mNodes.end(); ++node)
			{
				int length = strlen((*node)->mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*node)->mName.c_str(), length);

				out.write(reinterpret_cast<char*>(glm::value_ptr((*node)->mTranslation)), 12);
				out.write(reinterpret_cast<char*>(const_cast<float*>(glm::value_ptr((*node)->mRotation))), 16);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*node)->mScaling)), 12);
			}
		}
	}
}
