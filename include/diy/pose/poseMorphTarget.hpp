#ifndef POSEMORPHTARGET_HPP
#define POSEMORPHTARGET_HPP

#include <diy/diyClasses.hpp>
#include <diy/pose/poseDefines.hpp>

#include <string>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace pose
	{
		class PoseMorphTarget
		{
		public:
			DIYLIB_API ~PoseMorphTarget(void);

			DIYLIB_API PoseMorphTargets* MorphTargets(void);

			std::string Name(void);

			DIYLIB_API float GetBlendFactor(void);
			DIYLIB_API void SetBlendFactor(float blendFactor);

		private:
			POSE_FRIENDS

			PoseMorphTarget(PoseMorphTargets* morph_targets, std::string name);

			PoseMorphTargets* mMorphTargets;

			std::string mName;
			float mBlendFactor;
		};
	}
}

#endif

