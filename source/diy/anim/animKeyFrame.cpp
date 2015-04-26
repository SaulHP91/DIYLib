#include <diy/diy.hpp>

namespace diy
{
	namespace anim
	{
		AnimKeyFrame::AnimKeyFrame(class AnimKeyFrames* key_frames, int index, float time) :
			mKeyFrames(key_frames),
			mIndex(index),
			mTime(time),
			mLastStep(0.0f)
		{
			mNodes = new AnimNodes(this);
			mObjects = new AnimObjects(this);
		}

		AnimKeyFrame::~AnimKeyFrame(void)
		{
			delete mNodes;
			delete mObjects;
		}

		AnimKeyFrames* AnimKeyFrame::GetKeyFrames(void)
		{
			return mKeyFrames;
		}

		int AnimKeyFrame::GetIndex(void)
		{
			return mIndex;
		}

		void AnimKeyFrame::SetTime(float time)
		{
			mTime = time;
		}

		float AnimKeyFrame::GetTime(void)
		{
			return mTime;
		}

		float AnimKeyFrame::GetLastStep(void)
		{
			return mLastStep;
		}

		AnimNodes* AnimKeyFrame::GetNodes(void)
		{
			return mNodes;
		}

		AnimObjects* AnimKeyFrame::GetObjects(void)
		{
			return mObjects;
		}

		void AnimKeyFrame::CopyFrom(AnimKeyFrame* key_frame)
		{
			if (!key_frame)
			{
				return;
			}

			mNodes->Clear();
			for (std::vector<AnimNode*>::iterator node = key_frame->mNodes->mNodes.begin(); node != key_frame->mNodes->mNodes.end(); ++node)
			{
				AnimNode* new_node = mNodes->New((*node)->mName);
				new_node->mMatrix = (*node)->mMatrix;
				new_node->mTranslation = (*node)->mTranslation;
				new_node->mRotation = (*node)->mRotation;
				new_node->mScaling = (*node)->mScaling;
			}

			mObjects->Clear();
			for (std::vector<AnimObject*>::iterator object = key_frame->mObjects->mObjects.begin(); object != key_frame->mObjects->mObjects.end(); ++object)
			{
				AnimObject* new_object = mObjects->New((*object)->mName);
				new_object->mVisible = (*object)->mVisible;

				for (std::vector<AnimMorphTarget*>::iterator morph_target = (*object)->mMorphTargets->mMorphTargets.begin(); morph_target != (*object)->mMorphTargets->mMorphTargets.end(); ++morph_target)
				{
					AnimMorphTarget* new_morph_target = new_object->mMorphTargets->New((*morph_target)->mName);
					new_morph_target->mBlendFactor = (*morph_target)->mBlendFactor;
				}
			}
		}

		void AnimKeyFrame::Record(mesh::MeshFile* mesh_file)
		{
			mesh::MeshNodes* mesh_nodes = mesh_file->GetNodes();
			mesh_nodes->First();
			mesh::MeshNode* mesh_node;
			while (mesh_node = mesh_nodes->operator ()())
			{
				AnimNode* node = mNodes->Find(mesh_node->GetName(), true);
				node->SetMatrix(mesh_node->GetAdditionalMatrix());

				mesh_nodes->Next();
			}

			mesh::MeshObjects* mesh_objects = mesh_file->GetObjects();
			mesh_objects->First();
			mesh::MeshObject* mesh_object;
			while (mesh_object = mesh_objects->operator()())
			{
				AnimObject* object = mObjects->Find(mesh_object->GetName(), true);
				object->mVisible = mesh_object->IsVisible();

				mesh::MeshMorphTargets* mesh_morph_targets = mesh_object->GetMorphTargets();
				mesh_morph_targets->First();
				mesh::MeshMorphTarget* mesh_morph_target;
				while (mesh_morph_target = mesh_morph_targets->operator()())
				{
					AnimMorphTarget* morph_target = object->mMorphTargets->Find(mesh_morph_target->GetName(), true);
					morph_target->mBlendFactor = mesh_morph_target->GetBlendFactor();

					mesh_morph_targets->Next();
				}

				mesh_objects->Next();
			}
		}

		void AnimKeyFrame::Accumulate(AnimKeyFrame* key_frame, float weight)
		{
			if (!key_frame)
			{
				return;
			}

			for (std::vector<AnimNode*>::iterator node = key_frame->mNodes->mNodes.begin(); node != key_frame->mNodes->mNodes.end(); ++node)
			{
				AnimNode* new_node = mNodes->Find((*node)->mName, true);
				new_node->mTranslation += (*node)->mTranslation;

				glm::mat4 rotA = glm::toMat4(new_node->mRotation);
				glm::mat4 rotB = glm::toMat4((*node)->mRotation);

				new_node->mRotation = glm::toQuat(rotB * rotA);
				new_node->mScaling *= (*node)->mScaling;
				new_node->Update();
			}

			for (std::vector<AnimObject*>::iterator object = key_frame->mObjects->mObjects.begin(); object != key_frame->mObjects->mObjects.end(); ++object)
			{
				AnimObject* new_object = mObjects->Find((*object)->mName, true);
				new_object->mVisible |= (*object)->mVisible;

				for (std::vector<AnimMorphTarget*>::iterator morph_target = (*object)->mMorphTargets->mMorphTargets.begin(); morph_target != (*object)->mMorphTargets->mMorphTargets.end(); ++morph_target)
				{
					AnimMorphTarget* new_morph_target = new_object->mMorphTargets->Find((*morph_target)->mName, true);
					new_morph_target->mBlendFactor += (*morph_target)->mBlendFactor;
				}
			}
		}

		void AnimKeyFrame::Reset(void)
		{
			for (std::vector<AnimNode*>::iterator node = mNodes->mNodes.begin(); node != mNodes->mNodes.end(); ++node)
			{
				(*node)->SetIdentity();
			}

			for (std::vector<AnimObject*>::iterator object = mObjects->mObjects.begin(); object != mObjects->mObjects.end(); ++object)
			{
				(*object)->mVisible = true;

				for (std::vector<AnimMorphTarget*>::iterator morph_target = (*object)->mMorphTargets->mMorphTargets.begin(); morph_target != (*object)->mMorphTargets->mMorphTargets.end(); ++morph_target)
				{
					(*morph_target)->mBlendFactor = 0.0f;
				}
			}
		}
	}
}
