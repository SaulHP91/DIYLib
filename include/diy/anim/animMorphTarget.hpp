#ifndef ANIMMORPHTARGET_HPP
#define ANIMMORPHTARGET_HPP

#include <diy/diyClasses.hpp>
#include <diy/anim/animDefines.hpp>

#include <string>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace anim
	{
		class AnimMorphTarget
		{
		public:
			DIYLIB_API ~AnimMorphTarget(void);

			DIYLIB_API AnimMorphTargets* GetMorphTargets(void);

			DIYLIB_API std::string GetName(void);

			DIYLIB_API float GetBlendFactor(void);
			DIYLIB_API void SetBlendFactor(float blendFactor);

		private:
			ANIM_FRIENDS

			AnimMorphTarget(AnimMorphTargets* morph_targets, std::string name);

			AnimMorphTargets* mMorphTargets;

			std::string mName;
			float mBlendFactor;

			void Interpolate(AnimMorphTarget* morph_target, float factor);
		};
	}
}

#endif
 