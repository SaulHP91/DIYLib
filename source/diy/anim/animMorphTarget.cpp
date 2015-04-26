#include <diy/diy.hpp>

namespace diy
{
	namespace anim
	{
		AnimMorphTarget::AnimMorphTarget(AnimMorphTargets* morph_targets, std::string name) :
			mMorphTargets(morph_targets),
			mName(name),
			mBlendFactor(0.0f)
		{
			;
		}

		AnimMorphTarget::~AnimMorphTarget(void)
		{
			;
		}

		AnimMorphTargets* AnimMorphTarget::GetMorphTargets(void)
		{
			return mMorphTargets;
		}

		std::string AnimMorphTarget::GetName(void)
		{
			return mName;
		}

		float AnimMorphTarget::GetBlendFactor(void)
		{
			return mBlendFactor;
		}

		void AnimMorphTarget::SetBlendFactor(float blendFactor)
		{
			mBlendFactor = blendFactor;
		}

		void AnimMorphTarget::Interpolate(AnimMorphTarget* morph_target, float factor)
		{
			mBlendFactor = glm::lerp(mBlendFactor, morph_target->mBlendFactor, factor);
		}
	}
}