#include <diy/diy.hpp>

#include <cmath>

namespace diy
{
	namespace anim
	{
		AnimAnimation::AnimAnimation(class AnimAnimations* animations, std::string name, int index) :
			mAnimations(animations),
			mName(name),
			mIndex(index),
			mTicksPerSecond(60.0f),
			mWeight(1.0f),
			mDelay(0.0f),
			mTicks(0.0f),
			mPlay(false),
			mLoop(true),
			mLoopCount(-1),
			mBounce(false),
			mMethod(ANIMATION_SINUSOID)
		{
			mKeyFrames = new AnimKeyFrames(this);
		}

		AnimAnimation::~AnimAnimation(void)
		{
			delete mKeyFrames;
		}

		AnimAnimations* AnimAnimation::Animations(void)
		{
			return mAnimations;
		}

		int AnimAnimation::GetIndex(void)
		{
			return mIndex;
		}

		std::string AnimAnimation::GetName(void)
		{
			return mName;
		}

		bool AnimAnimation::SetName(std::string name)
		{
			if (mAnimations->Find(name))
			{
				return false;
			}

			mName = name;

			return true;
		}

		float AnimAnimation::GetTicksPerSecond(void)
		{
			return mTicksPerSecond;
		}

		void AnimAnimation::SetTicksPerSecond(float ticksPerSecond)
		{
			mTicksPerSecond = ticksPerSecond;
		}

		float AnimAnimation::GetWeight(void)
		{
			return mWeight;
		}

		void AnimAnimation::SetWeight(float weight)
		{
			mWeight = weight;
		}

		float AnimAnimation::GetDelay(void)
		{
			return mDelay;
		}

		void AnimAnimation::SetDelay(float delay)
		{
			mDelay = delay;
		}

		AnimKeyFrames* AnimAnimation::GetKeyFrames(void)
		{
			return mKeyFrames;
		}

		bool AnimAnimation::GetLoop(void)
		{
			return mLoop;
		}

		void AnimAnimation::SetLoop(bool loop)
		{
			mLoop = loop;
		}

		int AnimAnimation::GetLoopCount(void)
		{
			return mLoopCount;
		}

		void AnimAnimation::SetLoopCount(int loopCount)
		{
			mLoopCount = loopCount;
		}

		bool AnimAnimation::GetBounce(void)
		{
			return mBounce;
		}

		void AnimAnimation::SetBounce(bool bounce)
		{
			mBounce = bounce;
		}

		int AnimAnimation::GetMethod(void)
		{
			return mMethod;
		}

		void AnimAnimation::SetMethod(int method)
		{
			mMethod = method;
		}

		void AnimAnimation::Play(void)
		{
			mTicks = -mDelay;
			mPlay = true;
		}

		void AnimAnimation::Pause(void)
		{
			mPlay = false;
		}

		void AnimAnimation::Stop(void)
		{
			mPlay = false;
			mTicks = 0.0f;
		}

		void AnimAnimation::Step(float step)
		{
			if (!mKeyFrames->mKeyFrames.size() || !mPlay)
			{
				return;
			}

			mTicks += step * mTicksPerSecond;

			if (mTicks < 0)
			{
				mKeyFrames->mAnimatedKeyFrame->Reset();
				return;
			}

			float tMax = mKeyFrames->mKeyFrames.back()->mTime;
			if (!mLoop && mTicks > (mBounce ? 2.0f * tMax : tMax))
			{
				mKeyFrames->mAnimatedKeyFrame->CopyFrom(mBounce ? mKeyFrames->mKeyFrames.front() : mKeyFrames->mKeyFrames.back());
				return;
			}

			if (tMax > 0)
			{
				if (mTicks / (mBounce ? 2.0f * tMax : tMax) > mLoopCount && mLoopCount >= 0)
				{
					mKeyFrames->mAnimatedKeyFrame->CopyFrom(mBounce ? mKeyFrames->mKeyFrames.front() : mKeyFrames->mKeyFrames.back());
					return;
				}

				float t = 0.0f;
				if (mBounce)
				{
					t = mTicks - 2.0f * tMax * glm::floor(mTicks / (2.0f * tMax));
					if (t > tMax)
					{
						t = 2.0f * tMax - t;
					}
				}
				else
				{
					t = mTicks - tMax * glm::floor(mTicks / tMax);
				}
				int index = 0;
				float factor = 0.0f;
				for (std::vector<AnimKeyFrame*>::iterator key_frame = mKeyFrames->mKeyFrames.begin(); key_frame != mKeyFrames->mKeyFrames.end(); ++key_frame, ++index)
				{
					if ((*key_frame)->mTime >= t)
					{
						if ((*key_frame)->mTime > t)
						{
							if (!index)
							{
								mKeyFrames->mAnimatedKeyFrame->Reset();
								return;
							}
							switch (mMethod)
							{
							case ANIMATION_RAMP:
							{
								factor = 1.0f - ((*key_frame)->mTime - t) / ((*key_frame)->mTime - (*(key_frame - 1))->mTime);
							}
							break;
							case ANIMATION_SINUSOID:
							{
								factor = 1.0f - ((*key_frame)->mTime - t) / ((*key_frame)->mTime - (*(key_frame - 1))->mTime);
								factor = (sin(factor * M_PI - M_PI_2) + 1.0f) / 2.0f;
							}
							break;
							case ANIMATION_EXPONENTIAL:
							{
								factor = 1.0f - ((*key_frame)->mTime - t) / ((*key_frame)->mTime - (*(key_frame - 1))->mTime);
								factor = (exp(5.0f * factor) - 1.0f) / (exp(5.0f) - 1.0f);
							}
							break;
							default:
							{
								;
							}
							}
						}
						--index;
						break;
					}
				}
				mKeyFrames->Interpolate(index, factor);
			}

			mKeyFrames->mAnimatedKeyFrame->mTime = mAnimations->mTime;
			mKeyFrames->mAnimatedKeyFrame->mLastStep = step;
		}
	}
}
