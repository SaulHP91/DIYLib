#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace anim
	{
		AnimAnimations::AnimAnimations(AnimFile* file) :
			mFile(file),
			mTime(0.0f)
		{
			mAnimatedKeyFrame = new AnimKeyFrame(0, -1, 0.0f);
		}

		AnimAnimations::~AnimAnimations(void)
		{
			Clear();
		}

		AnimFile* AnimAnimations::GetFile(void)
		{
			return mFile;
		}

		AnimAnimation* AnimAnimations::New(void)
		{
			int index = 0;
			char name[256];

			AnimAnimation* new_animation;
			do
			{
				sprintf(name, "Animation%i", index++);
				new_animation = New(name);
			} while (!new_animation);

			return new_animation;
		}

		AnimAnimation* AnimAnimations::New(std::string name)
		{
			if (!Find(name))
			{
				mAnimations.push_back(new AnimAnimation(this, name, mAnimations.size()));
				return mAnimations.back();
			}

			return 0;
		}

		void AnimAnimations::Delete(AnimAnimation* animation)
		{
			std::vector<AnimAnimation*>::iterator iter = find(mAnimations.begin(), mAnimations.end(), animation);
			if (iter == mAnimations.end())
			{
				return;
			}

			mAnimations.erase(iter);
		}

		void AnimAnimations::Delete(int index)
		{
			Delete(operator[](index));
		}


		void AnimAnimations::Delete(std::string name)
		{
			Delete(Find(name));
		}

		void AnimAnimations::Clear(void)
		{
			while (mAnimations.size())
			{
				delete mAnimations.back();
				mAnimations.pop_back();
			}
			mAnimations.swap(std::vector<AnimAnimation*>(mAnimations));

			mAnimation = mAnimations.end();
		}

		AnimAnimation* AnimAnimations::Find(std::string name, bool create)
		{
			for (std::vector<AnimAnimation*>::iterator animation_iterator = mAnimations.begin(); animation_iterator != mAnimations.end(); ++animation_iterator)
			{
				AnimAnimation* animation = *animation_iterator;
				if (!_stricmp(animation->mName.c_str(), name.c_str()))
				{
					return animation;
				}
			}

			if (create)
			{
				return New(name);
			}

			return 0;
		}

		AnimAnimation* AnimAnimations::operator[](int index)
		{
			if (index < 0 || index >= static_cast<int>(mAnimations.size()))
			{
				return 0;
			}

			return mAnimations[index];
		}

		AnimAnimation* AnimAnimations::operator()(void)
		{
			if (mAnimation == mAnimations.end())
			{
				return 0;
			}

			return *mAnimation;
		}

		AnimAnimation* AnimAnimations::First(void)
		{
			if (mAnimations.size())
			{
				mAnimation = mAnimations.begin();
			}
			else
			{
				mAnimation = mAnimations.end();
			}

			return operator()();
		}

		AnimAnimation* AnimAnimations::Next(void)
		{
			if (mAnimation != mAnimations.end())
			{
				++mAnimation;
			}

			return operator()();
		}

		AnimAnimation* AnimAnimations::Last(void)
		{
			if (mAnimations.size())
			{
				mAnimation = mAnimations.end() - 1;
			}
			else
			{
				mAnimation = mAnimations.end();
			}

			return operator()();
		}

		AnimAnimation* AnimAnimations::Previous(void)
		{
			if (mAnimation == mAnimations.begin())
			{
				mAnimation = mAnimations.end();
			}
			else
			{
				--mAnimation;
			}

			return operator()();
		}

		int AnimAnimations::GetCount(void)
		{
			return mAnimations.size();
		}

		void AnimAnimations::SetTime(float time)
		{
			mTime = time;
		}

		float AnimAnimations::GetTime(void)
		{
			return mTime;
		}

		void AnimAnimations::Step(float step)
		{
			mTime += step;

			for (std::vector<AnimAnimation*>::iterator animation_iterator = mAnimations.begin(); animation_iterator != mAnimations.end(); ++animation_iterator)
			{
				AnimAnimation* animation = *animation_iterator;
				animation->Step(step);
			}

			mAnimatedKeyFrame->Reset();
			for (std::vector<AnimAnimation*>::iterator animation_iterator = mAnimations.begin(); animation_iterator != mAnimations.end(); ++animation_iterator)
			{
				if ((*animation_iterator)->mWeight > 0.0f)
				{
					mAnimatedKeyFrame->Accumulate((*animation_iterator)->mKeyFrames->mAnimatedKeyFrame, (*animation_iterator)->mWeight);
				}
			}

			mAnimatedKeyFrame->mTime = mTime;
			mAnimatedKeyFrame->mLastStep = step;
		}

		bool AnimAnimations::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			for (int i = 0; i < count; ++i)
			{
				int length;
				in.read(reinterpret_cast<char*>(&length), 4);

				char* name = new char[length];
				in.read(name, length);

				AnimAnimation* animation = Find(name, true);
				delete[] name;

				in.read(reinterpret_cast<char*>(&animation->mLoop), 4);
				in.read(reinterpret_cast<char*>(&animation->mLoopCount), 4);
				in.read(reinterpret_cast<char*>(&animation->mBounce), 4);
				in.read(reinterpret_cast<char*>(&animation->mTicksPerSecond), 4);
				in.read(reinterpret_cast<char*>(&animation->mDelay), 4);
				in.read(reinterpret_cast<char*>(&animation->mMethod), 4);

				animation->mKeyFrames->LoadFromFStream(in);
			}

			return true;
		}

		void AnimAnimations::SaveToFStream(std::ofstream& out)
		{
			int count = mAnimations.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<AnimAnimation*>::iterator animation = mAnimations.begin(); animation != mAnimations.end(); ++animation)
			{
				int length = strlen((*animation)->mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*animation)->mName.c_str(), length);

				out.write(reinterpret_cast<char*>(&(*animation)->mLoop), 4);
				out.write(reinterpret_cast<char*>(&(*animation)->mLoopCount), 4);
				out.write(reinterpret_cast<char*>(&(*animation)->mBounce), 4);
				out.write(reinterpret_cast<char*>(&(*animation)->mTicksPerSecond), 4);
				out.write(reinterpret_cast<char*>(&(*animation)->mDelay), 4);
				out.write(reinterpret_cast<char*>(&(*animation)->mMethod), 4);

				(*animation)->mKeyFrames->SaveToFStream(out);
			}
		}

		AnimKeyFrame* AnimAnimations::GetAnimatedKeyFrame(void)
		{
			return mAnimatedKeyFrame;
		}
	}
}
