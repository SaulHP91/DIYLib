#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace mesh
	{
		MeshFaces::MeshFaces(MeshObject* object) :
			mObject(object),
			mCapacity(256),
			mSize(0)
		{
			mFace = mFaces.end();

			mIndexArray = new unsigned int[3 * mCapacity];
		}

		MeshFaces::~MeshFaces(void)
		{
			Clear();

			delete mIndexArray;
		}

		MeshObject* MeshFaces::GetObject(void)
		{
			return mObject;
		}

		int MeshFaces::GetCount(void)
		{
			return mFaces.size();
		}

		MeshFace* MeshFaces::New(MeshGroup* group, std::vector<class MeshAnimatedVertex*> animatedVertices)
		{
			mFaces.push_back(new MeshFace(this, group, mFaces.size(), animatedVertices));

			return mFaces.back();
		}

		MeshFace* MeshFaces::New(MeshGroup* group, std::vector<int> animatedVertexIndices)
		{
			mFaces.push_back(new MeshFace(this, group, mFaces.size(), animatedVertexIndices));

			return mFaces.back();
		}

		MeshFace* MeshFaces::New(std::vector<class MeshAnimatedVertex*> animatedVertices)
		{
			return New((MeshGroup*)(0), animatedVertices);
		}

		MeshFace* MeshFaces::New(std::vector<int> animatedVertexIndices)
		{
			return New((MeshGroup*)(0), animatedVertexIndices);
		}

		MeshFace* MeshFaces::New(std::string materialName, std::vector<class MeshAnimatedVertex*> animatedVertices)
		{
			return New(mObject->mGroups->Find(materialName, true), animatedVertices);
		}

		MeshFace* MeshFaces::New(std::string materialName, std::vector<int> animatedVertexIndices)
		{
			return New(mObject->mGroups->Find(materialName, true), animatedVertexIndices);
		}

		MeshFace* MeshFaces::New(int materialIndex, std::vector<class MeshAnimatedVertex*> animatedVertices)
		{
			return New(mObject->mGroups->Find(materialIndex, true), animatedVertices);
		}

		MeshFace* MeshFaces::New(int materialIndex, std::vector<int> animatedVertexIndices)
		{
			return New(mObject->mGroups->Find(materialIndex, true), animatedVertexIndices);
		}

		MeshFace* MeshFaces::New(MeshMaterial* material, std::vector<class MeshAnimatedVertex*> animatedVertices)
		{
			return New(mObject->mGroups->Find(material, true), animatedVertices);
		}

		MeshFace* MeshFaces::New(MeshMaterial* material, std::vector<int> animatedVertexIndices)
		{
			return New(mObject->mGroups->Find(material, true), animatedVertexIndices);
		}

		void MeshFaces::Clear(void)
		{
			while (mFaces.size())
			{
				delete mFaces.back();
				mFaces.pop_back();
			}
			mFaces.swap(std::vector<MeshFace*>(mFaces));

			mFace = mFaces.end();

			mSize = 0;
		}

		void MeshFaces::Delete(MeshFace* face)
		{
			std::vector<MeshFace*>::iterator iterator = find(mFaces.begin(), mFaces.end(), face);

			if (iterator == mFaces.end())
			{
				return;
			}

			delete *iterator;
			iterator = mFaces.erase(mFace);

			for (iterator = iterator; iterator != mFaces.end(); ++iterator)
			{
				--(*iterator)->mIndex;
			}
		}

		MeshFace* MeshFaces::operator[](int index)
		{
			if (index < 0 || index >= int(mFaces.size()))
			{
				return 0;
			}

			return mFaces[index];
		}

		MeshFace* MeshFaces::Find(class MeshGroup* group)
		{
			for (std::vector<MeshFace*>::iterator face = mFaces.begin(); face != mFaces.end(); ++face)
			{
				if ((*face)->mGroup == group)
				{
					return *face;
				}
			}

			return 0;
		}

		MeshFace* MeshFaces::Find(class MeshMaterial* material)
		{
			for (std::vector<MeshFace*>::iterator face = mFaces.begin(); face != mFaces.end(); ++face)
			{
				if ((*face)->mGroup->mMaterial == material)
				{
					return *face;
				}
			}

			return 0;
		}

		MeshFace* MeshFaces::Find(std::string materialName)
		{
			return Find(mObject->mObjects->mFile->mMaterials->Find(materialName));
		}

		MeshFace* MeshFaces::Find(int materialIndex)
		{
			return Find(mObject->mObjects->mFile->mMaterials->operator [](materialIndex));
		}

		MeshFace* MeshFaces::operator()(void)
		{
			if (mFace == mFaces.end())
			{
				return 0;
			}

			return *mFace;
		}

		MeshFace* MeshFaces::First(void)
		{
			if (mFaces.empty())
			{
				mFace = mFaces.end();
			}
			else
			{
				mFace = mFaces.begin();
			}

			return operator()();
		}

		MeshFace* MeshFaces::Next(void)
		{
			if (mFace != mFaces.end())
			{
				++mFace;
			}

			return operator()();
		}

		MeshFace* MeshFaces::Last(void)
		{
			if (mFaces.empty())
			{
				mFace = mFaces.end();
			}
			else
			{
				mFace = mFaces.end() - 1;
			}

			return operator()();
		}

		MeshFace* MeshFaces::Previous(void)
		{
			if (mFace == mFaces.begin())
			{
				mFace = mFaces.end();
			}
			else if (mFace != mFaces.end())
			{
				--mFace;
			}

			return operator()();
		}

		void MeshFaces::Triangulate(void)
		{
			for (std::vector<MeshFace*>::iterator face = mFaces.begin(); face != mFaces.end(); ++face)
			{
				if ((*face)->mAnimatedVertices.size() > 3)
				{
					MeshFace* polygon = *face;

					std::vector<std::vector<MeshAnimatedVertex*> > triangles;
					polygon->Triangulate(triangles);

					MeshGroup* polygonGroup = polygon->mGroup;

					face = mFaces.erase(face);

					for (std::vector<std::vector<MeshAnimatedVertex*> >::iterator triangle = triangles.begin(); triangle != triangles.end(); ++triangle)
					{
						face = mFaces.insert(face, new MeshFace(this, polygonGroup, 0, *triangle));
					}

					delete polygon;
				}
			}

			int index = 0;
			for (std::vector<MeshFace*>::iterator face = mFaces.begin(); face != mFaces.end(); ++face)
			{
				(*face)->mIndex = index++;
			}

			UpdateArray();
		}

		void MeshFaces::Reverse(void)
		{
			for (std::vector<MeshFace*>::iterator face = mFaces.begin(); face != mFaces.end(); ++face)
			{
				(*face)->Reverse();
			}

			UpdateArray();
		}

		void MeshFaces::Realloc(unsigned int size)
		{
			while (mCapacity < size)
			{
				mCapacity *= 2;
			}

			delete[] mIndexArray;
			mIndexArray = new unsigned int[3 * mCapacity];
		}

		void MeshFaces::UpdateArray(void)
		{
			unsigned int faceCount = 0;
			for (std::vector<MeshGroup*>::iterator group = mObject->mGroups->mGroups.begin(); group != mObject->mGroups->mGroups.end(); ++group)
			{
				faceCount += (*group)->mFaces.size();
			}

			if (faceCount > mCapacity)
			{
				Realloc(faceCount);
			}

			mSize = 0;
			for (std::vector<MeshGroup*>::iterator group = mObject->mGroups->mGroups.begin(); group != mObject->mGroups->mGroups.end(); ++group)
			{
				(*group)->mFirstFace = mSize;
				for (std::vector<MeshFace*>::iterator face = (*group)->mFaces.begin(); face != (*group)->mFaces.end(); ++face)
				{
					mIndexArray[3 * mSize] = (*face)->mAnimatedVertices[0]->mIndex;
					mIndexArray[3 * mSize + 1] = (*face)->mAnimatedVertices[1]->mIndex;
					mIndexArray[3 * mSize + 2] = (*face)->mAnimatedVertices[2]->mIndex;
					++mSize;
				}
			}

			if (!mSize)
			{
				faceCount = mFaces.size();

				if (faceCount > mCapacity)
				{
					Realloc(faceCount);
				}

				for (std::vector<MeshFace*>::iterator face = mFaces.begin(); face != mFaces.end(); ++face)
				{
					mIndexArray[3 * mSize] = (*face)->mAnimatedVertices[0]->mIndex;
					mIndexArray[3 * mSize + 1] = (*face)->mAnimatedVertices[1]->mIndex;
					mIndexArray[3 * mSize + 2] = (*face)->mAnimatedVertices[2]->mIndex;
					++mSize;
				}
			}
		}

		int MeshFaces::GetIndexCount(void)
		{
			return 3 * mSize;
		}

		const void* MeshFaces::GetIndexPointer(void)
		{
			return mIndexArray;
		}
	}
}
