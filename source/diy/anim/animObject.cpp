#include <diy/diy.hpp>

namespace diy
{
	namespace anim
	{
		AnimObject::AnimObject(AnimObjects* objects, std::string name) :
			mObjects(objects),
			mName(name),
			mVisible(true)
		{
			mMorphTargets = new AnimMorphTargets(this);
		}

		AnimObject::~AnimObject(void)
		{
			delete mMorphTargets;
		}

		AnimObjects* AnimObject::GetObjects(void)
		{
			return mObjects;
		}

		std::string AnimObject::GetName(void)
		{
			return mName;
		}

		bool AnimObject::Visible(void)
		{
			return mVisible;
		}

		void AnimObject::Show(void)
		{
			mVisible = true;
		}

		void AnimObject::Hide(void)
		{
			mVisible = false;
		}

		AnimMorphTargets* AnimObject::GetMorphTargets(void)
		{
			return mMorphTargets;
		}
	}
}
