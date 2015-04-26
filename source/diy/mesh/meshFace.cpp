#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace mesh
	{
		MeshFace::MeshFace(MeshFaces* faces, MeshGroup* group, int index, std::vector<MeshAnimatedVertex*> animatedVertices) :
			mFaces(faces),
			mGroup(0),
			mIndex(index)
		{
			mFaces->mObject->mObjects->mFaceCount++;

			SetGroup(group);

			SetAnimatedVertices(animatedVertices);
		}

		MeshFace::MeshFace(MeshFaces* faces, MeshGroup* group, int index, std::vector<int> animatedVertexIndices) :
			mFaces(faces),
			mGroup(0),
			mIndex(index)
		{
			mFaces->mObject->mObjects->mFaceCount++;

			SetGroup(group);

			SetAnimatedVertices(animatedVertexIndices);
		}

		MeshFace::~MeshFace(void)
		{
			while (mAnimatedVertices.size())
			{
				RemoveAnimatedVertex(mAnimatedVertices.back());
			}
			mAnimatedVertices.swap(std::vector<MeshAnimatedVertex*>(mAnimatedVertices));

			if (mGroup)
			{
				mGroup->RemoveFace(this);
			}

			mFaces->mObject->mObjects->mFaceCount--;
		}

		MeshFaces* MeshFace::GetFaces(void)
		{
			return mFaces;
		}

		MeshGroup* MeshFace::GetGroup(void)
		{
			return mGroup;
		}

		int MeshFace::GetIndex(void)
		{
			return mIndex;
		}

		std::vector<MeshAnimatedVertex*> MeshFace::GetAnimatedVertices(void)
		{
			return mAnimatedVertices;
		}

		glm::vec3 MeshFace::GetCentroid(void)
		{
			return mCentroid;
		}

		glm::vec3 MeshFace::GetTangent(void)
		{
			return mTangent;
		}

		glm::vec3 MeshFace::GetBitangent(void)
		{
			return mBitangent;
		}

		glm::vec3 MeshFace::GetNormal(void)
		{
			return mNormal;
		}

		void MeshFace::Update(void)
		{
			mCentroid = glm::vec3(0.0f);
			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = mAnimatedVertices.begin(); animated_vertex != mAnimatedVertices.end(); ++animated_vertex)
			{
				mCentroid += (*animated_vertex)->mVertex.Position;
			}
			if (mAnimatedVertices.size())
			{
				mCentroid /= mAnimatedVertices.size();
			}

			if (mAnimatedVertices.size() < 3)
			{
				return;
			}

			glm::vec3 xyz1 = mAnimatedVertices[1]->mVertex.Position - mAnimatedVertices[0]->mVertex.Position;
			glm::vec3 xyz2 = mAnimatedVertices[2]->mVertex.Position - mAnimatedVertices[0]->mVertex.Position;

			glm::vec2 st1 = mAnimatedVertices[1]->mVertex.TexCoord - mAnimatedVertices[0]->mVertex.TexCoord;
			glm::vec2 st2 = mAnimatedVertices[2]->mVertex.TexCoord - mAnimatedVertices[0]->mVertex.TexCoord;

			float d = st1.x * st2.y - st2.x * st1.y;

			if (d)
			{
				float r = 1.0f / d;

				mTangent = glm::vec3((st2.y * xyz1.x - st1.y * xyz2.x) * r, (st2.y * xyz1.y - st1.y * xyz2.y) * r, (st2.y * xyz1.z - st1.y * xyz2.z) * r);
				mBitangent = glm::vec3((st1.x * xyz2.x - st2.x * xyz1.x) * r, (st1.x * xyz2.y - st2.x * xyz1.y) * r, (st1.x * xyz2.z - st2.x * xyz1.z) * r);

				float tangentLength = mTangent.length();
				if (tangentLength > 0.0f)
				{
					mTangent /= tangentLength;
				}

				float bitangentLength = mBitangent.length();
				if (bitangentLength > 0.0f)
				{
					mBitangent /= bitangentLength;
				}
			}

			mNormal = glm::cross(xyz1, xyz2);

			float normalLength = mNormal.length();
			if (normalLength > 0.0f)
			{
				mNormal /= normalLength;
			}
		}

		void MeshFace::Triangulate(std::vector<std::vector<MeshAnimatedVertex*> >& triangles)
		{
			triangles.clear();
			triangles.swap(std::vector<std::vector<MeshAnimatedVertex*> >(triangles));

			std::vector<MeshAnimatedVertex*> polygonVertices = mAnimatedVertices;
			while (polygonVertices.size() > 3)
			{
				std::vector<MeshAnimatedVertex*> triangleVertices;
				triangleVertices.insert(triangleVertices.end(), polygonVertices.begin(), polygonVertices.begin() + 3);
				triangles.push_back(triangleVertices);
				polygonVertices.erase(polygonVertices.begin() + 1);
			}
			triangles.push_back(polygonVertices);
		}

		void MeshFace::Reverse(void)
		{
			reverse(mAnimatedVertices.begin(), mAnimatedVertices.end());

			mNormal *= -1.0f;
		}

		void MeshFace::AppendAnimatedVertex(MeshAnimatedVertex* animated_vertex)
		{
			if (!animated_vertex)
			{
				return;
			}

			animated_vertex->mFaces.push_back(this);

			mAnimatedVertices.push_back(animated_vertex);

			Update();
		}

		void MeshFace::AppendAnimatedVertex(int animatedVertexIndex)
		{
			AppendAnimatedVertex(mFaces->mObject->mAnimatedVertices->operator [](animatedVertexIndex));
		}

		void MeshFace::RemoveAnimatedVertex(MeshAnimatedVertex* animated_vertex)
		{
			std::vector<MeshAnimatedVertex*>::iterator iterator = find(mAnimatedVertices.begin(), mAnimatedVertices.end(), animated_vertex);

			if (iterator == mAnimatedVertices.end())
			{
				return;
			}

			(*iterator)->mFaces.erase(find((*iterator)->mFaces.begin(), (*iterator)->mFaces.end(), this));

			mAnimatedVertices.erase(iterator);

			Update();
		}

		void MeshFace::RemoveAnimatedVertex(int animatedVertexIndex)
		{
			RemoveAnimatedVertex(mFaces->mObject->mAnimatedVertices->operator [](animatedVertexIndex));
		}

		void MeshFace::SetAnimatedVertices(std::vector<MeshAnimatedVertex*> animatedVertices)
		{
			while (mAnimatedVertices.size())
			{
				RemoveAnimatedVertex(mAnimatedVertices.back());
			}

			for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = animatedVertices.begin(); animated_vertex != animatedVertices.end(); ++animated_vertex)
			{
				AppendAnimatedVertex(*animated_vertex);
			}
		}

		void MeshFace::SetAnimatedVertices(std::vector<int> animatedVertexIndices)
		{
			std::vector<MeshAnimatedVertex*> animatedVertices;
			for (std::vector<int>::iterator animated_vertex_index = animatedVertexIndices.begin(); animated_vertex_index != animatedVertexIndices.end(); ++animated_vertex_index)
			{
				animatedVertices.push_back(mFaces->mObject->mAnimatedVertices->mAnimatedVertices[*animated_vertex_index]);
			}

			SetAnimatedVertices(animatedVertices);
		}

		void MeshFace::ReplaceAnimatedVertex(MeshAnimatedVertex* destiny, MeshAnimatedVertex* source)
		{
			std::vector<MeshAnimatedVertex*>::iterator animated_vertex = find(mAnimatedVertices.begin(), mAnimatedVertices.end(), destiny);

			if (animated_vertex == mAnimatedVertices.end())
			{
				return;
			}

			std::vector<MeshFace*>::iterator face = find((*animated_vertex)->mFaces.begin(), (*animated_vertex)->mFaces.end(), this);

			if (face == (*animated_vertex)->mFaces.end())
			{
				return;
			}

			(*animated_vertex)->mFaces.erase(face);
			*animated_vertex = source;
			(*animated_vertex)->mFaces.push_back(this);

			Update();
		}

		void MeshFace::SetGroup(class MeshGroup* group)
		{
			if (mGroup)
			{
				mGroup->RemoveFace(this);
			}

			if (group)
			{
				group->AppendFace(this);
			}
		}

		void MeshFace::SetGroup(int groupIndex)
		{
			SetGroup(mFaces->mObject->mGroups->operator[](groupIndex));
		}

		void MeshFace::SetMaterial(MeshMaterial* material)
		{
			SetGroup(mFaces->mObject->mGroups->Find(material, true));
		}

		void MeshFace::SetMaterial(std::string materialName)
		{
			SetMaterial(mFaces->mObject->mObjects->mFile->mMaterials->Find(materialName, true));
		}

		void MeshFace::SetMaterial(int materialIndex)
		{
			SetMaterial(mFaces->mObject->mObjects->mFile->mMaterials->operator[](materialIndex));
		}
	}
}
