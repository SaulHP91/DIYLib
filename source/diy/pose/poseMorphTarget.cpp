#include <diy/diy.hpp>

namespace diy
{
	namespace pose
	{
		PoseMorphTarget::PoseMorphTarget(PoseMorphTargets* morph_targets, std::string name) :
			mMorphTargets(morph_targets),
			mName(name),
			mBlendFactor(0.0f)
		{
			;
		}

		PoseMorphTarget::~PoseMorphTarget(void)
		{
			;
		}

		PoseMorphTargets* PoseMorphTarget::MorphTargets(void)
		{
			return mMorphTargets;
		}

		std::string PoseMorphTarget::Name(void)
		{
			return mName;
		}

		float PoseMorphTarget::GetBlendFactor(void)
		{
			return mBlendFactor;
		}

		void PoseMorphTarget::SetBlendFactor(float blendFactor)
		{
			mBlendFactor = blendFactor;
		}
	}
}
