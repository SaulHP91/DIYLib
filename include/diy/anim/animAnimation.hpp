#ifndef ANIMANIMATION_HPP
#define ANIMANIMATION_HPP

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
		class AnimAnimation
		{
		public:
			DIYLIB_API ~AnimAnimation(void);

			DIYLIB_API AnimAnimations* Animations(void);

			DIYLIB_API int GetIndex(void);

			DIYLIB_API std::string GetName(void);
			DIYLIB_API bool SetName(std::string name);

			DIYLIB_API float GetTicksPerSecond(void);
			DIYLIB_API void SetTicksPerSecond(float ticksPerSecond);

			DIYLIB_API float GetWeight(void);
			DIYLIB_API void SetWeight(float weight);

			DIYLIB_API float GetDelay(void);
			DIYLIB_API void SetDelay(float delay);

			DIYLIB_API AnimKeyFrames* GetKeyFrames(void);

			DIYLIB_API bool GetLoop(void);
			DIYLIB_API void SetLoop(bool loop);
			DIYLIB_API int GetLoopCount(void);
			DIYLIB_API void SetLoopCount(int loopCount);

			DIYLIB_API bool GetBounce(void);
			DIYLIB_API void SetBounce(bool bounce);

			DIYLIB_API int GetMethod(void);
			DIYLIB_API void SetMethod(int method);

			DIYLIB_API void Play(void);
			DIYLIB_API void Pause(void);
			DIYLIB_API void Stop(void);

		private:
			ANIM_FRIENDS
				
			AnimAnimation(AnimAnimations* animations, std::string name, int index);

			AnimAnimations* mAnimations;

			int mIndex;
			std::string mName;

			float mTicksPerSecond;
			float mWeight;
			float mDelay;

			AnimKeyFrames* mKeyFrames;

			bool mPlay;
			bool mLoop;
			int mLoopCount;
			bool mBounce;
			int mMethod;

			float mTicks;
			void Step(float step);
		};
	}
}

#endif
