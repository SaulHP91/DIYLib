#include <diy/diy.hpp>

namespace diy
{
	namespace pose
	{
		PoseObject::PoseObject(PoseObjects* objects, std::string name) :
			mObjects(objects),
			mName(name),
			mVisible(true)
		{
			mMorphTargets = new PoseMorphTargets(this);
		}

		PoseObject::~PoseObject(void)
		{
			delete mMorphTargets;
		}

		PoseObjects* PoseObject::Objects(void)
		{
			return mObjects;
		}

		std::string PoseObject::Name(void)
		{
			return mName;
		}

		bool PoseObject::Visible(void)
		{
			return mVisible;
		}

		void PoseObject::Show(void)
		{
			mVisible = true;
		}

		void PoseObject::Hide(void)
		{
			mVisible = false;
		}

		PoseMorphTargets* PoseObject::MorphTargets(void)
		{
			return mMorphTargets;
		}
	}
}
