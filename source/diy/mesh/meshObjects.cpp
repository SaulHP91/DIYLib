#include <diy/diy.hpp>

#include <glm/ext.hpp>

#include <algorithm>

namespace diy
{
	namespace mesh
	{
		MeshObjects::MeshObjects(MeshFile* file) :
			mFile(file),
			mMinLimits(0.0f),
			mMaxLimits(0.0f),
			mCenter(0.0f),
			mAnimatedVertexCount(0),
			mFaceCount(0)
		{
			mObject = mObjects.end();
		}

		MeshObjects::~MeshObjects(void)
		{
			Clear();
		}

		MeshFile* MeshObjects::GetFile(void)
		{
			return mFile;
		}

		int MeshObjects::GetCount(void)
		{
			return mObjects.size();
		}

		MeshObject* MeshObjects::New(void)
		{
			int index = 0;
			char name[256];

			MeshObject* new_object;
			do
			{
				sprintf(name, "Object%i", index++);
				new_object = New(name);
			} while (!new_object);

			return new_object;
		}

		MeshObject* MeshObjects::New(std::string name)
		{
			if (name.empty())
			{
				return 0;
			}

			if (Find(name))
			{
				return 0;
			}

			mObjects.push_back(new MeshObject(this, mObjects.size(), name));

			return mObjects.back();
		}

		void MeshObjects::Clear(void)
		{
			while (mObjects.size())
			{
				delete mObjects.back();
				mObjects.pop_back();
			}
			mObjects.swap(std::vector<MeshObject*>(mObjects));

			mObject = mObjects.end();

			mMinLimits = glm::vec3(0.0f);
			mMaxLimits = glm::vec3(0.0f);
			mCenter = glm::vec3(0.0f);
		}

		void MeshObjects::Delete(MeshObject* object)
		{
			std::vector<MeshObject*>::iterator iterator = find(mObjects.begin(), mObjects.end(), object);

			if (iterator == mObjects.end())
			{
				return;
			}

			delete *iterator;
			iterator = mObjects.erase(iterator);

			for (iterator = iterator; iterator != mObjects.end(); ++iterator)
			{
				--(*iterator)->mIndex;
			}
		}

		MeshObject* MeshObjects::Duplicate(MeshObject* object)
		{
			if (!object)
			{
				return 0;
			}

			MeshObject* new_object;
			char suffix[256] = "";
			do
			{
				char name[256];
				strcat(suffix, "copyOf");
				sprintf(name, "%s%s", suffix, object->mName.c_str());
				new_object = New(name);
			} while (!new_object);

			new_object->SetNode(object->mNode);

			object->mAnimatedVertices->First();
			MeshAnimatedVertex* animated_vertex;
			while (animated_vertex = object->mAnimatedVertices->operator ()())
			{
				MeshAnimatedVertex* new_animated_vertex = new_object->mAnimatedVertices->New(animated_vertex->mVertex);
				for (std::vector<MeshSkinWeight*>::iterator skin_weight = animated_vertex->mSkinWeights.begin(); skin_weight != animated_vertex->mSkinWeights.end(); ++skin_weight)
				{
					new_animated_vertex->AppendSkinWeight((*skin_weight)->mSkinWeights->mSkin->mNode->mName, (*skin_weight)->mWeight);
				}
				for (std::vector<MeshMorphTargetVertex*>::iterator morph_target_vertex = animated_vertex->mMorphTargetVertices.begin(); morph_target_vertex != animated_vertex->mMorphTargetVertices.end(); ++morph_target_vertex)
				{
					new_animated_vertex->AppendMorphTargetVertex((*morph_target_vertex)->mMorphTargetVertices->mMorphTarget->mName, (*morph_target_vertex)->mVertex);
				}
				object->mAnimatedVertices->Next();
			}

			MeshFace* face;
			object->mFaces->First();
			while (face = object->mFaces->operator ()())
			{
				std::vector<int> animatedVertexIndices;
				for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = face->mAnimatedVertices.begin(); animated_vertex != face->mAnimatedVertices.end(); ++animated_vertex)
				{
					animatedVertexIndices.push_back((*animated_vertex)->mIndex);
				}

				if (face->mGroup)
				{
					new_object->mFaces->New(face->mGroup->mMaterial, animatedVertexIndices);
				}
				else
				{
					new_object->mFaces->New(animatedVertexIndices);
				}

				object->mFaces->Next();
			}

			new_object->UpdateArrays();

			return new_object;
		}

		MeshObject* MeshObjects::Duplicate(std::string name)
		{
			return Duplicate(Find(name));
		}

		MeshObject* MeshObjects::Duplicate(int index)
		{
			return Duplicate(operator[](index));
		}

		bool MeshObjects::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			mObjects.reserve(count);

			for (int i = 0; i < count; ++i)
			{
				int length;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* name = new char[length];
				in.read(name, length);
				MeshObject* object = Find(name, true);
				delete[] name;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* comment = new char[length];
				in.read(comment, length);
				object->mComment = std::string(comment);
				delete[] comment;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* node = new char[length];
				in.read(node, length);
				object->SetNode(node);
				delete[] node;

				in.read(reinterpret_cast<char*>(glm::value_ptr(object->mOffsetMatrix)), 64);
				in.read(reinterpret_cast<char*>(glm::value_ptr(object->mMatrix)), 64);
				in.read(reinterpret_cast<char*>(glm::value_ptr(object->mMinLimits)), 12);
				in.read(reinterpret_cast<char*>(glm::value_ptr(object->mMaxLimits)), 12);

				int animatedVertexOffset = object->mAnimatedVertices->mAnimatedVertices.size();
				int animatedVertexCount;
				int texCoordChannels;
				in.read(reinterpret_cast<char*>(&animatedVertexCount), 4);
				in.read(reinterpret_cast<char*>(&texCoordChannels), 4);
				object->mAnimatedVertices->mAnimatedVertices.reserve(animatedVertexCount);
				for (int i = 0; i < animatedVertexCount; ++i)
				{
					MeshVertex vertex;
					vertex.LoadFromFStream(in);
					object->mAnimatedVertices->New(vertex);
				}

				int faceOffset = object->mFaces->mFaces.size();
				int faceCount;
				in.read(reinterpret_cast<char*>(&faceCount), 4);
				object->mFaces->mFaces.reserve(faceCount);
				std::vector<int> vertexIndices(3);
				for (int i = 0; i < faceCount; ++i)
				{
					in.read(reinterpret_cast<char*>(&vertexIndices[0]), 12);
					object->mFaces->New(vertexIndices);
				}

				int groupCount;
				in.read(reinterpret_cast<char*>(&groupCount), 4);
				object->mGroups->mGroups.reserve(groupCount);
				for (int i = 0; i < groupCount; ++i)
				{
					int length;
					in.read(reinterpret_cast<char*>(&length), 4);
					char* name = new char[length];
					in.read(name, length);
					MeshGroup* group = object->mGroups->Find(name, true);
					delete[] name;
					int faceCount;
					in.read(reinterpret_cast<char*>(&faceCount), 4);
					group->mFaces.reserve(faceCount);
					for (int j = 0; j < faceCount; ++j)
					{
						int faceIndex;
						in.read(reinterpret_cast<char*>(&faceIndex), 4);
						group->AppendFace(faceIndex + faceOffset);
					}
				}

				int skinCount;
				in.read(reinterpret_cast<char*>(&skinCount), 4);
				object->mSkins->mSkins.reserve(skinCount);
				for (int i = 0; i < skinCount; ++i)
				{
					int length;
					in.read(reinterpret_cast<char*>(&length), 4);
					char* name = new char[length];
					in.read(name, length);
					MeshSkin* skin = object->mSkins->Find(name, true);
					int vertexCount;
					in.read(reinterpret_cast<char*>(&vertexCount), 4);
					skin->mSkinWeights->mSkinWeights.reserve(vertexCount);
					for (int j = 0; j < vertexCount; ++j)
					{
						int vertexIndex;
						in.read(reinterpret_cast<char*>(&vertexIndex), 4);
						float weight;
						in.read(reinterpret_cast<char*>(&weight), 4);
						skin->mSkinWeights->New(vertexIndex + animatedVertexOffset, weight);
					}
					in.read(reinterpret_cast<char*>(glm::value_ptr(skin->OffsetMatrix)), 64);
				}

				int morphTargetCount;
				in.read(reinterpret_cast<char*>(&morphTargetCount), 4);
				object->mMorphTargets->mMorphTargets.reserve(morphTargetCount);
				for (int i = 0; i < morphTargetCount; ++i)
				{
					int length;
					in.read(reinterpret_cast<char*>(&length), 4);
					char* name = new char[length];
					in.read(name, length);
					MeshMorphTarget* morph_target = object->mMorphTargets->Find(name, true);
					delete[] name;
					int vertexCount;
					in.read(reinterpret_cast<char*>(&vertexCount), 4);
					morph_target->mMorphTargetVertices->mMorphTargetVertices.reserve(vertexCount);
					for (int j = 0; j < vertexCount; ++j)
					{
						int vertexIndex;
						in.read(reinterpret_cast<char*>(&vertexIndex), 4);
						MeshVertex vertex;
						vertex.LoadFromFStream(in);
						morph_target->mMorphTargetVertices->New(vertexIndex + animatedVertexOffset, vertex);
					}
				}

				int endObject;
				in.read(reinterpret_cast<char*>(&endObject), 4);

				object->UpdateArrays();
			}

			int endObjects;
			in.read(reinterpret_cast<char*>(&endObjects), 4);

			return true;
		}

		void MeshObjects::SaveToFStream(std::ofstream& out)
		{
			int count = mObjects.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				int length = strlen((*object)->mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*object)->mName.c_str(), length);
				length = strlen((*object)->mComment.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*object)->mComment.c_str(), length);
				if ((*object)->mNode)
				{
					length = strlen((*object)->mNode->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&length), 4);
					out.write((*object)->mNode->mName.c_str(), length);
				}
				else
				{
					length = 0;
					out.write(reinterpret_cast<char*>(&length), 4);
				}

				out.write(reinterpret_cast<char*>(glm::value_ptr((*object)->mOffsetMatrix)), 64);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*object)->mMatrix)), 64);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*object)->mMinLimits)), 12);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*object)->mMaxLimits)), 12);

				int animatedVertexCount = (*object)->mAnimatedVertices->mAnimatedVertices.size();
				out.write(reinterpret_cast<char*>(&animatedVertexCount), 4);
				int texCoordsChannels = 1;
				out.write(reinterpret_cast<char*>(&texCoordsChannels), 4);
				for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = (*object)->mAnimatedVertices->mAnimatedVertices.begin(); animated_vertex != (*object)->mAnimatedVertices->mAnimatedVertices.end(); ++animated_vertex)
				{
					(*animated_vertex)->mVertex.SaveToFStream(out);
				}

				int faceCount = (*object)->mFaces->mFaces.size();
				out.write(reinterpret_cast<char*>(&faceCount), 4);
				for (std::vector<MeshFace*>::iterator face = (*object)->mFaces->mFaces.begin(); face != (*object)->mFaces->mFaces.end(); ++face)
				{
					int vertexCount = 0;
					for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = (*face)->mAnimatedVertices.begin(); animated_vertex != (*face)->mAnimatedVertices.end(); ++animated_vertex)
					{
						int vertexIndex = (*animated_vertex)->mIndex;
						out.write(reinterpret_cast<char*>(&vertexIndex), 4);
						if (++vertexCount == 3)
						{
							break;
						}
					}
				}

				int groupCount = (*object)->mGroups->mGroups.size();
				out.write(reinterpret_cast<char*>(&groupCount), 4);
				for (std::vector<MeshGroup*>::iterator group = (*object)->mGroups->mGroups.begin(); group != (*object)->mGroups->mGroups.end(); ++group)
				{
					if ((*group)->mMaterial)
					{
						int length = strlen((*group)->mMaterial->mName.c_str()) + 1;
						out.write(reinterpret_cast<char*>(&length), 4);
						out.write((*group)->mMaterial->mName.c_str(), length);
					}
					else
					{
						int length = 0;
						out.write(reinterpret_cast<char*>(&length), 4);
					}
					int faceCount = (*group)->mFaces.size();
					out.write(reinterpret_cast<char*>(&faceCount), 4);
					for (std::vector<MeshFace*>::iterator face = (*group)->mFaces.begin(); face != (*group)->mFaces.end(); ++face)
					{
						int faceIndex = (*face)->mIndex;
						out.write(reinterpret_cast<char*>(&faceIndex), 4);
					}
				}

				int skinCount = (*object)->mSkins->mSkins.size();
				out.write(reinterpret_cast<char*>(&skinCount), 4);
				for (std::vector<MeshSkin*>::iterator skin = (*object)->mSkins->mSkins.begin(); skin != (*object)->mSkins->mSkins.end(); ++skin)
				{
					int length = strlen((*skin)->mNode->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&length), 4);
					out.write((*skin)->mNode->mName.c_str(), length);
					int vertexCount = (*skin)->mSkinWeights->mSkinWeights.size();
					out.write(reinterpret_cast<char*>(&vertexCount), 4);
					for (std::vector<MeshSkinWeight*>::iterator skin_weight = (*skin)->mSkinWeights->mSkinWeights.begin(); skin_weight != (*skin)->mSkinWeights->mSkinWeights.end(); ++skin_weight)
					{
						int vertexIndex = (*skin_weight)->mAnimatedVertex->mIndex;
						out.write(reinterpret_cast<char*>(&vertexIndex), 4);
						float weight = (*skin_weight)->mWeight;
						out.write(reinterpret_cast<char*>(&weight), 4);
					}
					out.write(reinterpret_cast<char*>(glm::value_ptr((*skin)->OffsetMatrix)), 64);
				}

				int morphTargetCount = (*object)->mMorphTargets->mMorphTargets.size();
				out.write(reinterpret_cast<char*>(&morphTargetCount), 4);
				for (std::vector<MeshMorphTarget*>::iterator morph_target = (*object)->mMorphTargets->mMorphTargets.begin(); morph_target != (*object)->mMorphTargets->mMorphTargets.end(); ++morph_target)
				{
					int length = strlen((*morph_target)->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&length), 4);
					out.write((*morph_target)->mName.c_str(), length);
					int vertexCount = (*morph_target)->mMorphTargetVertices->mMorphTargetVertices.size();
					out.write(reinterpret_cast<char*>(&vertexCount), 4);
					for (std::vector<MeshMorphTargetVertex*>::iterator morph_target_vertex = (*morph_target)->mMorphTargetVertices->mMorphTargetVertices.begin(); morph_target_vertex != (*morph_target)->mMorphTargetVertices->mMorphTargetVertices.end(); ++morph_target_vertex)
					{
						int vertexIndex = (*morph_target_vertex)->mAnimatedVertex->mIndex;
						out.write(reinterpret_cast<char*>(&vertexIndex), 4);
						(*morph_target_vertex)->mVertex.SaveToFStream(out);
					}
				}

				int endObject = 0;
				out.write(reinterpret_cast<char*>(&endObject), 4);
			}

			int endObjects = 0;
			out.write(reinterpret_cast<char*>(&endObjects), 4);
		}

		MeshObject* MeshObjects::operator[](int index)
		{
			if (index < 0 || index >= int(mObjects.size()))
			{
				return 0;
			}

			return mObjects[index];
		}

		MeshObject* MeshObjects::Find(std::string name, bool create)
		{
			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				if (!_stricmp((*object)->mName.c_str(), name.c_str()))
				{
					return *object;
				}
			}

			if (create)
			{
				return New(name);
			}

			return 0;
		}

		MeshObject* MeshObjects::operator()(void)
		{
			if (mObject == mObjects.end())
			{
				return 0;
			}

			return *mObject;
		}

		MeshObject* MeshObjects::First(void)
		{
			if (mObjects.empty())
			{
				mObject = mObjects.end();
			}
			else
			{
				mObject = mObjects.begin();
			}

			return operator()();
		}

		MeshObject* MeshObjects::Next(void)
		{
			if (mObject != mObjects.end())
			{
				++mObject;
			}

			return operator()();
		}

		MeshObject* MeshObjects::Last(void)
		{
			if (mObjects.empty())
			{
				mObject = mObjects.end();
			}
			else
			{
				mObject = mObjects.end() - 1;
			}

			return operator()();
		}

		MeshObject* MeshObjects::Previous(void)
		{
			if (mObject == mObjects.begin())
			{
				mObject = mObjects.end();
			}
			else if (mObject != mObjects.end())
			{
				--mObject;
			}

			return operator()();
		}

		glm::vec3 MeshObjects::GetMinLimits(void)
		{
			return mMinLimits;
		}

		glm::vec3 MeshObjects::GetMaxLimits(void)
		{
			return mMaxLimits;
		}

		glm::vec3 MeshObjects::GetCenter(void)
		{
			return mCenter;
		}

		void MeshObjects::UpdateLimits(glm::vec3 reference)
		{
			for (int i = 0; i < 3; ++i)
			{
				if (reference[i] < mMinLimits[i])
				{
					mMinLimits[i] = reference[i];
				}
				else if (reference[i] > mMaxLimits[i])
				{
					mMaxLimits[i] = reference[i];
				}
			}

			mCenter = 0.5f * (mMinLimits + mMaxLimits);
		}

		void MeshObjects::Render(MeshMaterial* material)
		{
			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				(*object)->Render(material);
			}
		}

		void MeshObjects::Render(void)
		{
			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				(*object)->RenderOpaque();
			}

			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				(*object)->RenderTransparent();
			}
		}

		void MeshObjects::Render(class Camera* camera)
		{
			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				(*object)->Render(camera);
			}
		}

		int MeshObjects::GetAnimatedVertexCount(void)
		{
			int count = 0;

			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				count += (*object)->mAnimatedVertices->mAnimatedVertices.size();
			}

			return count;
		}

		int MeshObjects::GetFaceCount(void)
		{
			int count = 0;

			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				count += (*object)->mFaces->mFaces.size();
			}

			return count;
		}

		void MeshObjects::Update(void)
		{
			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				(*object)->Update();
			}
		}

		void MeshObjects::Merge(MeshObject* destiny_object, MeshObject* source_object)
		{
			if (!destiny_object || !source_object || source_object == destiny_object)
			{
				return;
			}

			int vertexOffset = destiny_object->mAnimatedVertices->mAnimatedVertices.size();
			source_object->mAnimatedVertices->First();
			MeshAnimatedVertex* animated_vertex;
			while (animated_vertex = source_object->mAnimatedVertices->operator ()())
			{
				MeshAnimatedVertex* new_animated_vertex = destiny_object->mAnimatedVertices->New(animated_vertex->mVertex);

				for (std::vector<MeshSkinWeight*>::iterator skin_weight = animated_vertex->mSkinWeights.begin(); skin_weight != animated_vertex->mSkinWeights.end(); ++skin_weight)
				{
					new_animated_vertex->AppendSkinWeight((*skin_weight)->mSkinWeights->mSkin->mNode->mName, (*skin_weight)->mWeight);
				}

				for (std::vector<MeshMorphTargetVertex*>::iterator morph_target_vertex = animated_vertex->mMorphTargetVertices.begin(); morph_target_vertex != animated_vertex->mMorphTargetVertices.end(); ++morph_target_vertex)
				{
					new_animated_vertex->AppendMorphTargetVertex((*morph_target_vertex)->mMorphTargetVertices->mMorphTarget->mName, (*morph_target_vertex)->mVertex);
				}

				source_object->mAnimatedVertices->Next();
			}

			MeshFace* face;
			source_object->mFaces->First();
			while (face = source_object->mFaces->operator ()())
			{
				std::vector<int> vertexIndices;
				for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = face->mAnimatedVertices.begin(); animated_vertex != face->mAnimatedVertices.end(); ++animated_vertex)
				{
					vertexIndices.push_back((*animated_vertex)->mIndex + vertexOffset);
				}
				if (face->mGroup)
				{
					destiny_object->mFaces->New(face->mGroup->mMaterial, vertexIndices);
				}
				else
				{
					destiny_object->mFaces->New(vertexIndices);
				}
				source_object->mFaces->Next();
			}

			destiny_object->UpdateArrays();

			Delete(source_object);
		}

		void MeshObjects::MergeAll(void)
		{
			while (mObjects.size() > 1)
			{
				Merge(mObjects.front(), *(mObjects.begin() + 1));
			}
		}

		void MeshObjects::FreezeAll(void)
		{
			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				(*object)->Freeze();
			}
		}

		bool MeshObjects::MakeMorphTarget(class MeshObject* base_object, class MeshObject* target_object)
		{
			if (base_object == target_object || !base_object || !target_object)
			{
				return false;
			}

			if (base_object->mFaces->mFaces.size() != target_object->mFaces->mFaces.size())
			{
				return false;
			}

			for (std::vector<MeshFace*>::iterator base_face = base_object->mFaces->mFaces.begin(), target_face = target_object->mFaces->mFaces.begin(); base_face != base_object->mFaces->mFaces.end(); ++base_face, ++target_face)
			{
				if ((*base_face)->mAnimatedVertices.size() != (*target_face)->mAnimatedVertices.size())
				{
					return false;
				}
			}

			MeshMorphTarget* morph_target = base_object->mMorphTargets->New(target_object->mName);

			if (!morph_target)
			{
				return false;
			}

			for (std::vector<MeshAnimatedVertex*>::iterator base_vertex = base_object->mAnimatedVertices->mAnimatedVertices.begin(); base_vertex != base_object->mAnimatedVertices->mAnimatedVertices.end(); ++base_vertex)
			{
				(*base_vertex)->mReserved = 0;
			}

			for (std::vector<MeshAnimatedVertex*>::iterator target_vertex = target_object->mAnimatedVertices->mAnimatedVertices.begin(); target_vertex != target_object->mAnimatedVertices->mAnimatedVertices.end(); ++target_vertex)
			{
				(*target_vertex)->mReserved = 0;
			}

			for (std::vector<MeshFace*>::iterator base_face = base_object->mFaces->mFaces.begin(), target_face = target_object->mFaces->mFaces.begin(); base_face != base_object->mFaces->mFaces.end(); ++base_face, ++target_face)
			{
				for (std::vector<MeshAnimatedVertex*>::iterator base_animated_vertex = (*base_face)->mAnimatedVertices.begin(), target_animated_vertex = (*target_face)->mAnimatedVertices.begin(); base_animated_vertex != (*base_face)->mAnimatedVertices.end(); ++base_animated_vertex, ++target_animated_vertex)
				{
					if (!(*base_animated_vertex)->mReserved || !(*target_animated_vertex)->mReserved)
					{
						if (glm::length((*base_animated_vertex)->mVertex.Position - (*target_animated_vertex)->mVertex.Position) < 1e-4f)
						{
							(*base_animated_vertex)->mReserved = *target_animated_vertex;
							(*target_animated_vertex)->mReserved = *base_animated_vertex;
							continue;
						}
						if (!(*base_animated_vertex)->mReserved)
						{
							(*base_animated_vertex)->mReserved = *target_animated_vertex;
							(*target_animated_vertex)->mReserved = *base_animated_vertex;
							(*base_animated_vertex)->AppendMorphTargetVertex(morph_target, (*target_animated_vertex)->mVertex);
						}
						else if (!(*target_animated_vertex)->mReserved)
						{
							(*target_animated_vertex)->mReserved = *base_animated_vertex;
							MeshAnimatedVertex* duplicated_animated_vertex = base_object->mAnimatedVertices->Duplicate(*base_animated_vertex);
							duplicated_animated_vertex->mReserved = *target_animated_vertex;
							duplicated_animated_vertex->RemoveMorphTargetVertex(morph_target);
							duplicated_animated_vertex->AppendMorphTargetVertex(morph_target, (*target_animated_vertex)->mVertex);
							(*base_face)->ReplaceAnimatedVertex(*base_animated_vertex, duplicated_animated_vertex);
						}
					}
				}
			}

			Delete(target_object);

			base_object->UpdateArrays();

			return true;
		}

		void MeshObjects::Explode(MeshObject* object)
		{
			for (std::vector<MeshGroup*>::iterator group = object->mGroups->mGroups.begin(); group != object->mGroups->mGroups.end(); ++group)
			{
				char new_name[256] = "";
				strcpy(new_name, object->mName.c_str());
				strcat(new_name, "_");
				if ((*group)->mMaterial)
				{
					strcat(new_name, (*group)->mMaterial->mName.c_str());
				}

				MeshObject* new_object = Find(new_name, true);

				new_object->SetNode(object->mNode);
				new_object->mOffsetMatrix = object->mOffsetMatrix;

				for (std::vector<MeshSkin*>::iterator skin = object->mSkins->mSkins.begin(); skin != object->mSkins->mSkins.end(); ++skin)
				{
					MeshSkin* new_skin = new_object->mSkins->New((*skin)->mNode);
					new_skin->OffsetMatrix = (*skin)->OffsetMatrix;
				}

				for (std::vector<MeshFace*>::iterator face = (*group)->mFaces.begin(); face != (*group)->mFaces.end(); ++face)
				{
					std::vector<MeshAnimatedVertex*> animatedVertices;
					for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = (*face)->mAnimatedVertices.begin(); animated_vertex != (*face)->mAnimatedVertices.end(); ++animated_vertex)
					{
						MeshAnimatedVertex* new_animated_vertex = (*animated_vertex)->mReserved;
						if (!new_animated_vertex)
						{
							new_animated_vertex = new_object->mAnimatedVertices->New((*animated_vertex)->mVertex);
							(*animated_vertex)->mReserved = new_animated_vertex;

							for (std::vector<MeshSkinWeight*>::iterator skin_weight = (*animated_vertex)->mSkinWeights.begin(); skin_weight != (*animated_vertex)->mSkinWeights.end(); ++skin_weight)
							{
								new_animated_vertex->AppendSkinWeight((*skin_weight)->mSkinWeights->mSkin->mNode, (*skin_weight)->mWeight);
							}

							for (std::vector<MeshMorphTargetVertex*>::iterator morph_target_vertex = (*animated_vertex)->mMorphTargetVertices.begin(); morph_target_vertex != (*animated_vertex)->mMorphTargetVertices.end(); ++morph_target_vertex)
							{
								new_animated_vertex->AppendMorphTargetVertex((*morph_target_vertex)->mMorphTargetVertices->mMorphTarget->mName, (*morph_target_vertex)->mVertex);
							}
						}
						animatedVertices.push_back(new_animated_vertex);
					}
					new_object->mFaces->New((*group)->mMaterial, animatedVertices);
				}

				new_object->UpdateArrays();

				for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = object->mAnimatedVertices->mAnimatedVertices.begin(); animated_vertex != object->mAnimatedVertices->mAnimatedVertices.end(); ++animated_vertex)
				{
					(*animated_vertex)->mReserved = 0;
				}
			}

			Delete(object);

			Update();
		}

		void MeshObjects::SetBlendFactor(std::string morphTargetName, float blendFactor)
		{
			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				MeshMorphTarget* morph_target = (*object)->mMorphTargets->Find(morphTargetName);
				if (morph_target)
				{
					morph_target->mBlendFactor = blendFactor;
					(*object)->Update();
				}
			}
		}

		void MeshObjects::Sort(void)
		{
			sort(mObjects.begin(), mObjects.end(), SortObjectsIncreasingRespectName);

			int index = 0;
			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				(*object)->mIndex = index++;
			}
		}

		void MeshObjects::Reverse(void)
		{
			reverse(mObjects.begin(), mObjects.end());

			int index = 0;
			for (std::vector<MeshObject*>::iterator object = mObjects.begin(); object != mObjects.end(); ++object)
			{
				(*object)->mIndex = index++;
			}
		}
	}
}