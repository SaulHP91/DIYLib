#include <diy/diy.hpp>

namespace diy
{
	namespace pose
	{
		PosePose::PosePose(class PosePoses* poses, std::string name, int index) :
			mPoses(poses),
			mName(name),
			mIndex(index)
		{
			mNodes = new PoseNodes(this);
			mObjects = new PoseObjects(this);
		}

		PosePose::~PosePose(void)
		{
			delete mNodes;
			delete mObjects;
		}

		PosePoses* PosePose::Poses(void)
		{
			return mPoses;
		}

		int PosePose::Index(void)
		{
			return mIndex;
		}

		std::string PosePose::Name(void)
		{
			return mName;
		}

		bool PosePose::Rename(std::string name)
		{
			if (mPoses->Find(name))
			{
				return false;
			}

			mName = name;

			return true;
		}

		PoseNodes* PosePose::Nodes(void)
		{
			return mNodes;
		}

		PoseObjects* PosePose::Objects(void)
		{
			return mObjects;
		}

		void PosePose::CopyFrom(PosePose* pose)
		{
			if (!pose)
			{
				return;
			}

			mNodes->Clear();
			for (std::vector<PoseNode*>::iterator node = pose->mNodes->mNodes.begin(); node != pose->mNodes->mNodes.end(); ++node)
			{
				PoseNode* new_node = mNodes->New((*node)->mName);
				new_node->mMatrix = (*node)->mMatrix;
				new_node->mTranslation = (*node)->mTranslation;
				new_node->mRotation = (*node)->mRotation;
				new_node->mScaling = (*node)->mScaling;
			}

			mObjects->Clear();
			for (std::vector<PoseObject*>::iterator object = pose->mObjects->mObjects.begin(); object != pose->mObjects->mObjects.end(); ++object)
			{
				PoseObject* new_object = mObjects->New((*object)->mName);
				new_object->mVisible = (*object)->mVisible;

				for (std::vector<PoseMorphTarget*>::iterator morph_target = (*object)->mMorphTargets->mMorphTargets.begin(); morph_target != (*object)->mMorphTargets->mMorphTargets.end(); ++morph_target)
				{
					PoseMorphTarget* new_morph_target = new_object->mMorphTargets->New((*morph_target)->mName);
					new_morph_target->mBlendFactor = (*morph_target)->mBlendFactor;
				}
			}
		}

		void PosePose::Record(mesh::MeshFile* mesh_file)
		{
			mesh::MeshNodes* mesh_nodes = mesh_file->GetNodes();
			mesh_nodes->First();
			mesh::MeshNode* mesh_node;
			while (mesh_node = mesh_nodes->operator ()())
			{
				PoseNode* node = mNodes->Find(mesh_node->GetName(), true);
				node->SetMatrix(mesh_node->GetAdditionalMatrix());

				mesh_nodes->Next();
			}

			mesh::MeshObjects* mesh_objects = mesh_file->GetObjects();
			mesh_objects->First();
			mesh::MeshObject* mesh_object;
			while (mesh_object = mesh_objects->operator()())
			{
				PoseObject* object = mObjects->Find(mesh_object->GetName(), true);
				object->mVisible = mesh_object->IsVisible();

				mesh::MeshMorphTargets* mesh_morph_targets = mesh_object->GetMorphTargets();
				mesh_morph_targets->First();
				mesh::MeshMorphTarget* mesh_morph_target;
				while (mesh_morph_target = mesh_morph_targets->operator()())
				{
					PoseMorphTarget* morph_target = object->mMorphTargets->Find(mesh_morph_target->GetName(), true);
					morph_target->mBlendFactor = mesh_morph_target->GetBlendFactor();

					mesh_morph_targets->Next();
				}

				mesh_objects->Next();
			}
		}

		void PosePose::Reset(void)
		{
			for (std::vector<PoseNode*>::iterator node = mNodes->mNodes.begin(); node != mNodes->mNodes.end(); ++node)
			{
				(*node)->SetIdentity();
			}

			for (std::vector<PoseObject*>::iterator object = mObjects->mObjects.begin(); object != mObjects->mObjects.end(); ++object)
			{
				(*object)->mVisible = true;

				for (std::vector<PoseMorphTarget*>::iterator morph_target = (*object)->mMorphTargets->mMorphTargets.begin(); morph_target != (*object)->mMorphTargets->mMorphTargets.end(); ++morph_target)
				{
					(*morph_target)->mBlendFactor = 0.0f;
				}
			}
		}
	}
}
