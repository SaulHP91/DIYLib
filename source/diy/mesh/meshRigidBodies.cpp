#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace mesh
	{
		MeshRigidBodies::MeshRigidBodies(MeshPhysics* physics) :
			mPhysics(physics)
		{
			mRigidBody = mRigidBodies.end();
		}

		MeshRigidBodies::~MeshRigidBodies(void)
		{
			Clear();
		}

		MeshPhysics* MeshRigidBodies::GetPhysics(void)
		{
			return mPhysics;
		}

		int MeshRigidBodies::GetCount(void)
		{
			return mRigidBodies.size();
		}

		MeshRigidBody* MeshRigidBodies::New(void)
		{
			int index = 0;
			char name[256];

			MeshRigidBody* new_rigid_body;
			do
			{
				sprintf(name, "RigidBody%i", index++);
				new_rigid_body = New(name);
			} while (!new_rigid_body);

			return new_rigid_body;
		}

		MeshRigidBody* MeshRigidBodies::New(std::string name)
		{
			if (name.empty())
			{
				return 0;
			}

			if (Find(name))
			{
				return 0;
			}

			mRigidBodies.push_back(new MeshRigidBody(this, mRigidBodies.size(), name));

			return mRigidBodies.back();
		}

		void MeshRigidBodies::Clear(void)
		{
			while (mRigidBodies.size())
			{
				delete mRigidBodies.back();
				mRigidBodies.pop_back();
			}
			mRigidBodies.swap(std::vector<MeshRigidBody*>(mRigidBodies));

			mRigidBody = mRigidBodies.end();
		}

		void MeshRigidBodies::Delete(MeshRigidBody* rigid_body)
		{
			std::vector<MeshRigidBody*>::iterator iterator = find(mRigidBodies.begin(), mRigidBodies.end(), rigid_body);

			if (iterator == mRigidBodies.end())
			{
				return;
			}

			delete *iterator;
			iterator = mRigidBodies.erase(iterator);

			for (iterator = iterator; iterator != mRigidBodies.end(); ++iterator)
			{
				--(*iterator)->mIndex;
			}
		}

		void MeshRigidBodies::Delete(std::string name)
		{
			Delete(Find(name));
		}

		void MeshRigidBodies::DeleteUnused(void)
		{
			for (std::vector<MeshRigidBody*>::iterator rigid_body = mRigidBodies.begin(); rigid_body != mRigidBodies.end();)
			{
				if (!(*rigid_body)->IsInUse())
				{
					delete *rigid_body;
					mRigidBodies.erase(rigid_body);
					rigid_body = mRigidBodies.begin();
					continue;
				}
				++rigid_body;
			}
			int index = 0;
			for (std::vector<MeshRigidBody*>::iterator rigid_body = mRigidBodies.begin(); rigid_body != mRigidBodies.end(); ++rigid_body)
			{
				(*rigid_body)->mIndex = index++;
			}
		}

		MeshRigidBody* MeshRigidBodies::operator[](int index)
		{
			if (index < 0 || index >= int(mRigidBodies.size()))
			{
				return 0;
			}

			return mRigidBodies[index];
		}

		MeshRigidBody* MeshRigidBodies::Find(std::string name, bool create)
		{
			for (std::vector<MeshRigidBody*>::iterator rigid_body = mRigidBodies.begin(); rigid_body != mRigidBodies.end(); ++rigid_body)
			{
				if (!_stricmp((*rigid_body)->mName.c_str(), name.c_str()))
				{
					return *rigid_body;
				}
			}

			if (create)
			{
				return New(name);
			}

			return 0;
		}

		MeshRigidBody* MeshRigidBodies::First(void)
		{
			if (mRigidBodies.empty())
			{
				mRigidBody = mRigidBodies.end();
			}
			else
			{
				mRigidBody = mRigidBodies.begin();
			}

			return operator()();
		}

		MeshRigidBody* MeshRigidBodies::Next(void)
		{
			if (mRigidBody != mRigidBodies.end())
			{
				++mRigidBody;
			}

			return operator()();
		}

		MeshRigidBody* MeshRigidBodies::Last(void)
		{
			if (mRigidBodies.empty())
			{
				mRigidBody = mRigidBodies.end();
			}
			else
			{
				mRigidBody = mRigidBodies.end() - 1;
			}

			return operator()();
		}

		MeshRigidBody* MeshRigidBodies::Previous(void)
		{
			if (mRigidBody == mRigidBodies.begin())
			{
				mRigidBody = mRigidBodies.end();
			}
			else if (mRigidBody != mRigidBodies.end())
			{
				--mRigidBody;
			}

			return operator()();
		}

		MeshRigidBody* MeshRigidBodies::operator()(void)
		{
			if (mRigidBody == mRigidBodies.end())
			{
				return 0;
			}

			return *mRigidBody;
		}

		bool MeshRigidBodies::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			for (int i = 0; i < count; ++i)
			{
				int length;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* name = new char[length];
				in.read(name, length);
				MeshRigidBody* rigid_body = Find(name, true);
				delete[] name;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* comment = new char[length];
				in.read(comment, length);
				rigid_body->mComment = std::string(comment);
				delete[] comment;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* node = new char[length];
				in.read(node, length);
				rigid_body->SetNode(node);
				delete[] node;

				in.read(reinterpret_cast<char*>(&rigid_body->mCollisionGroup), 4);
				in.read(reinterpret_cast<char*>(&rigid_body->mShape), 4);
				in.read(reinterpret_cast<char*>(glm::value_ptr(rigid_body->mSize)), 12);
				in.read(reinterpret_cast<char*>(glm::value_ptr(rigid_body->mInitialTranslation)), 12);
				in.read(reinterpret_cast<char*>(const_cast<float*>(glm::value_ptr(rigid_body->mInitialRotation))), 16);
				in.read(reinterpret_cast<char*>(&rigid_body->mMass), 4);
				in.read(reinterpret_cast<char*>(&rigid_body->mLinearDamping), 4);
				in.read(reinterpret_cast<char*>(&rigid_body->mAngularDamping), 4);
				in.read(reinterpret_cast<char*>(&rigid_body->mBounce), 4);
				in.read(reinterpret_cast<char*>(&rigid_body->mFriction), 4);
				in.read(reinterpret_cast<char*>(&rigid_body->mType), 4);

				int endRigidBody;
				in.read(reinterpret_cast<char*>(&endRigidBody), 4);

				rigid_body->LoadInitialConditions();
			}

			int endRigidBodies;
			in.read(reinterpret_cast<char*>(&endRigidBodies), 4);

			return true;
		}

		void MeshRigidBodies::SaveToFStream(std::ofstream& out)
		{
			int count = mRigidBodies.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<MeshRigidBody*>::iterator rigid_body = mRigidBodies.begin(); rigid_body != mRigidBodies.end(); ++rigid_body)
			{
				int length = strlen((*rigid_body)->mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*rigid_body)->mName.c_str(), length);
				length = strlen((*rigid_body)->mComment.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*rigid_body)->mComment.c_str(), length);

				if ((*rigid_body)->mNode)
				{
					length = strlen((*rigid_body)->mNode->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&length), 4);
					out.write((*rigid_body)->mNode->mName.c_str(), length);
				}
				else
				{
					length = 0;
					out.write(reinterpret_cast<char*>(&length), 4);
				}

				out.write(reinterpret_cast<char*>(&(*rigid_body)->mCollisionGroup), 4);
				out.write(reinterpret_cast<char*>(&(*rigid_body)->mShape), 4);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*rigid_body)->mSize)), 12);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*rigid_body)->mInitialTranslation)), 12);
				out.write(reinterpret_cast<char*>(const_cast<float*>(glm::value_ptr((*rigid_body)->mInitialRotation))), 16);
				out.write(reinterpret_cast<char*>(&(*rigid_body)->mMass), 4);
				out.write(reinterpret_cast<char*>(&(*rigid_body)->mLinearDamping), 4);
				out.write(reinterpret_cast<char*>(&(*rigid_body)->mAngularDamping), 4);
				out.write(reinterpret_cast<char*>(&(*rigid_body)->mBounce), 4);
				out.write(reinterpret_cast<char*>(&(*rigid_body)->mFriction), 4);
				out.write(reinterpret_cast<char*>(&(*rigid_body)->mType), 4);

				int endRigidBody = 0;
				out.write(reinterpret_cast<char*>(&endRigidBody), 4);
			}

			int endRigidBodies = 0;
			out.write(reinterpret_cast<char*>(&endRigidBodies), 4);
		}

		void MeshRigidBodies::Render(class Camera* camera)
		{
			for (std::vector<MeshRigidBody*>::iterator rigid_body = mRigidBodies.begin(); rigid_body != mRigidBodies.end(); ++rigid_body)
			{
				(*rigid_body)->Render(camera);
			}
		}

		void MeshRigidBodies::Render(class Camera* camera, int collisionGroup)
		{
			for (std::vector<MeshRigidBody*>::iterator rigid_body = mRigidBodies.begin(); rigid_body != mRigidBodies.end(); ++rigid_body)
			{
				if ((*rigid_body)->mCollisionGroup == collisionGroup)
				{
					(*rigid_body)->Render(camera);
				}
			}
		}

		void MeshRigidBodies::Render(class Camera* camera, std::vector<int>& collisionGroups)
		{
			for (std::vector<MeshRigidBody*>::iterator rigid_body = mRigidBodies.begin(); rigid_body != mRigidBodies.end(); ++rigid_body)
			{
				if (std::find(collisionGroups.begin(), collisionGroups.end(), (*rigid_body)->mCollisionGroup) != collisionGroups.end())
				{
					(*rigid_body)->Render(camera);
				}
			}
		}

		void MeshRigidBodies::Initialize(void)
		{
			for (std::vector<MeshRigidBody*>::iterator rigid_body = mRigidBodies.begin(); rigid_body != mRigidBodies.end(); ++rigid_body)
			{
				(*rigid_body)->Initialize();
			}
		}
	}
}