#include <diy/diy.hpp>

namespace diy
{
	namespace mesh
	{
		MeshJoints::MeshJoints(MeshPhysics* physics) :
			mPhysics(physics)
		{
			mJoint = mJoints.end();
		}

		MeshJoints::~MeshJoints(void)
		{
			Clear();
		}

		MeshPhysics* MeshJoints::GetPhysics(void)
		{
			return mPhysics;
		}

		int MeshJoints::GetCount(void)
		{
			return mJoints.size();
		}

		MeshJoint* MeshJoints::New(void)
		{
			int index = 0;
			char name[256];

			MeshJoint* new_joint;
			do
			{
				sprintf(name, "Joint%i", index++);
				new_joint = New(name);
			} while (!new_joint);

			return new_joint;
		}

		MeshJoint* MeshJoints::New(std::string name)
		{
			if (name.empty())
			{
				return 0;
			}

			if (Find(name))
			{
				return 0;
			}

			mJoints.push_back(new MeshJoint(this, mJoints.size(), name));

			return mJoints.back();
		}

		void MeshJoints::Clear(void)
		{
			while (mJoints.size())
			{
				delete mJoints.back();
				mJoints.pop_back();
			}
			mJoints.swap(std::vector<MeshJoint*>(mJoints));

			mJoint = mJoints.end();
		}

		void MeshJoints::Delete(MeshJoint* joint)
		{
			std::vector<MeshJoint*>::iterator iterator = find(mJoints.begin(), mJoints.end(), joint);

			if (iterator == mJoints.end())
			{
				return;
			}

			delete *iterator;
			iterator = mJoints.erase(iterator);

			for (iterator = iterator; iterator != mJoints.end(); ++iterator)
			{
				--(*iterator)->mIndex;
			}
		}

		void MeshJoints::Delete(std::string name)
		{
			Delete(Find(name));
		}

		MeshJoint* MeshJoints::operator[](int index)
		{
			if (index < 0 || index >= int(mJoints.size()))
			{
				return 0;
			}

			return mJoints[index];
		}

		MeshJoint* MeshJoints::Find(std::string name, bool create)
		{
			for (std::vector<MeshJoint*>::iterator joint = mJoints.begin(); joint != mJoints.end(); ++joint)
			{
				if (!_stricmp((*joint)->mName.c_str(), name.c_str()))
				{
					return *joint;
				}
			}

			if (create)
			{
				return New(name);
			}

			return 0;
		}

		MeshJoint* MeshJoints::First(void)
		{
			if (mJoints.empty())
			{
				mJoint = mJoints.end();
			}
			else
			{
				mJoint = mJoints.begin();
			}

			return operator()();
		}

		MeshJoint* MeshJoints::Next(void)
		{
			if (mJoint != mJoints.end())
			{
				++mJoint;
			}

			return operator()();
		}

		MeshJoint* MeshJoints::Last(void)
		{
			if (mJoints.empty())
			{
				mJoint = mJoints.end();
			}
			else
			{
				mJoint = mJoints.end() - 1;
			}

			return operator()();
		}

		MeshJoint* MeshJoints::Previous(void)
		{
			if (mJoint == mJoints.begin())
			{
				mJoint = mJoints.end();
			}
			else if (mJoint != mJoints.end())
			{
				--mJoint;
			}

			return operator()();
		}

		MeshJoint* MeshJoints::operator()(void)
		{
			if (mJoint == mJoints.end())
			{
				return 0;
			}

			return *mJoint;
		}

		bool MeshJoints::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			for (int i = 0; i < count; ++i)
			{
				int length;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* name = new char[length];
				in.read(name, length);
				MeshJoint* joint = Find(name, true);
				delete[] name;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* comment = new char[length];
				in.read(comment, length);
				joint->mComment = std::string(comment);
				delete[] comment;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* node = new char[length];
				in.read(node, length);
				joint->SetNode(node);
				delete[] node;

				in.read(reinterpret_cast<char*>(&joint->mType), 4);

				in.read(reinterpret_cast<char*>(&length), 4);
				name = new char[length];
				in.read(name, length);
				joint->SetFirstBody(name);
				delete[] name;

				in.read(reinterpret_cast<char*>(&length), 4);
				name = new char[length];
				in.read(name, length);
				joint->SetSecondBody(name);
				delete[] name;

				in.read(reinterpret_cast<char*>(glm::value_ptr(joint->mTranslation)), 12);
				in.read(reinterpret_cast<char*>(const_cast<float*>(glm::value_ptr(joint->mRotation))), 16);
				in.read(reinterpret_cast<char*>(glm::value_ptr(joint->mLoStop)), 12);
				in.read(reinterpret_cast<char*>(glm::value_ptr(joint->mHiStop)), 12);
				in.read(reinterpret_cast<char*>(&joint->mBounce), 4);
				in.read(reinterpret_cast<char*>(&joint->mCFM), 4);
				in.read(reinterpret_cast<char*>(&joint->mStopERP), 4);
				in.read(reinterpret_cast<char*>(&joint->mStopCFM), 4);

				int endJoint;
				in.read(reinterpret_cast<char*>(&endJoint), 4);

				joint->Update();
			}

			int endJoints;
			in.read(reinterpret_cast<char*>(&endJoints), 4);

			return true;
		}

		void MeshJoints::SaveToFStream(std::ofstream& out)
		{
			int count = mJoints.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<MeshJoint*>::iterator joint = mJoints.begin(); joint != mJoints.end(); ++joint)
			{
				int length = strlen((*joint)->mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*joint)->mName.c_str(), length);
				length = strlen((*joint)->mComment.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*joint)->mComment.c_str(), length);

				if ((*joint)->mNode)
				{
					length = strlen((*joint)->mNode->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&length), 4);
					out.write((*joint)->mNode->mName.c_str(), length);
				}
				else
				{
					length = 0;
					out.write(reinterpret_cast<char*>(&length), 4);
				}

				out.write(reinterpret_cast<char*>(&(*joint)->mType), 4);

				if ((*joint)->mFirstBody)
				{
					length = strlen((*joint)->mFirstBody->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&length), 4);
					out.write((*joint)->mFirstBody->mName.c_str(), length);
				}
				else
				{
					length = 0;
					out.write(reinterpret_cast<char*>(&length), 4);
				}

				if ((*joint)->mSecondBody)
				{
					length = strlen((*joint)->mSecondBody->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&length), 4);
					out.write((*joint)->mSecondBody->mName.c_str(), length);
				}
				else
				{
					length = 0;
					out.write(reinterpret_cast<char*>(&length), 4);
				}

				out.write(reinterpret_cast<char*>(glm::value_ptr((*joint)->mTranslation)), 12);
				out.write(reinterpret_cast<char*>(const_cast<float*>(glm::value_ptr((*joint)->mRotation))), 16);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*joint)->mLoStop)), 12);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*joint)->mHiStop)), 12);
				out.write(reinterpret_cast<char*>(&(*joint)->mBounce), 4);
				out.write(reinterpret_cast<char*>(&(*joint)->mCFM), 4);
				out.write(reinterpret_cast<char*>(&(*joint)->mStopERP), 4);
				out.write(reinterpret_cast<char*>(&(*joint)->mStopCFM), 4);

				int endJoint = 0;
				out.write(reinterpret_cast<char*>(&endJoint), 4);
			}

			int endJoints = 0;
			out.write(reinterpret_cast<char*>(&endJoints), 4);
		}

		void MeshJoints::Render(Camera* camera)
		{
			for (std::vector<MeshJoint*>::iterator joint = mJoints.begin(); joint != mJoints.end(); ++joint)
			{
				(*joint)->Render(camera);
			}
		}

		void MeshJoints::Initialize(void)
		{
			for (std::vector<MeshJoint*>::iterator joint = mJoints.begin(); joint != mJoints.end(); ++joint)
			{
				(*joint)->Initialize();
			}
		}
	}
}
