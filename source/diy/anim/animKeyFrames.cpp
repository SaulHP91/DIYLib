#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace anim
	{
		AnimKeyFrames::AnimKeyFrames(AnimAnimation* animation) :
			mAnimation(animation)
		{
			mAnimatedKeyFrame = new AnimKeyFrame(this, -1, 0.0f);
		}

		AnimKeyFrames::~AnimKeyFrames(void)
		{
			Clear();

			delete mAnimatedKeyFrame;
		}

		AnimAnimation* AnimKeyFrames::GetAnimation(void)
		{
			return mAnimation;
		}

		AnimKeyFrame* AnimKeyFrames::New(float time)
		{
			mKeyFrames.push_back(new AnimKeyFrame(this, mKeyFrames.size(), time));
			return mKeyFrames.back();
		}

		AnimKeyFrame* AnimKeyFrames::Duplicate(AnimKeyFrame* key_frame)
		{
			std::vector<AnimKeyFrame*>::iterator iter = find(mKeyFrames.begin(), mKeyFrames.end(), key_frame);

			if (iter == mKeyFrames.end())
			{
				return 0;
			}

			AnimKeyFrame* new_key_frame = new AnimKeyFrame(this, (*iter)->mIndex + 1, (*iter)->mTime);
			new_key_frame->CopyFrom(key_frame);
			iter = mKeyFrames.insert(iter + 1, new_key_frame);

			for (iter = iter + 1; iter != mKeyFrames.end(); ++iter)
			{
				++(*iter)->mIndex;
			}

			return new_key_frame;
		}

		void AnimKeyFrames::Delete(AnimKeyFrame* key_frame)
		{
			std::vector<AnimKeyFrame*>::iterator iter = find(mKeyFrames.begin(), mKeyFrames.end(), key_frame);
			if (iter == mKeyFrames.end())
			{
				return;
			}

			iter = mKeyFrames.erase(iter);

			for (iter = iter; iter != mKeyFrames.end(); ++iter)
			{
				--(*iter)->mIndex;
			}
		}

		void AnimKeyFrames::Delete(int index)
		{
			Delete(operator[](index));
		}

		void AnimKeyFrames::Clear(void)
		{
			while (mKeyFrames.size())
			{
				delete mKeyFrames.back();
				mKeyFrames.pop_back();
			}
			mKeyFrames.swap(std::vector<AnimKeyFrame*>(mKeyFrames));

			mKeyFrame = mKeyFrames.end();
		}

		AnimKeyFrame* AnimKeyFrames::operator[](int index)
		{
			if (index < 0 || index >= static_cast<int>(mKeyFrames.size()))
			{
				return 0;
			}

			return mKeyFrames[index];
		}

		AnimKeyFrame* AnimKeyFrames::operator()(void)
		{
			if (mKeyFrame == mKeyFrames.end())
			{
				return 0;
			}

			return *mKeyFrame;
		}

		AnimKeyFrame* AnimKeyFrames::First(void)
		{
			if (mKeyFrames.size())
			{
				mKeyFrame = mKeyFrames.begin();
			}
			else
			{
				mKeyFrame = mKeyFrames.end();
			}

			return operator()();
		}

		AnimKeyFrame* AnimKeyFrames::Next(void)
		{
			if (mKeyFrame != mKeyFrames.end())
			{
				++mKeyFrame;
			}

			return operator()();
		}

		AnimKeyFrame* AnimKeyFrames::Last(void)
		{
			if (mKeyFrames.size())
			{
				mKeyFrame = mKeyFrames.end() - 1;
			}
			else
			{
				mKeyFrame = mKeyFrames.end();
			}

			return operator()();
		}

		AnimKeyFrame* AnimKeyFrames::Previous(void)
		{
			if (mKeyFrame == mKeyFrames.begin())
			{
				mKeyFrame = mKeyFrames.end();
			}
			else
			{
				--mKeyFrame;
			}

			return operator()();
		}

		int AnimKeyFrames::GetCount(void)
		{
			return mKeyFrames.size();
		}

		void AnimKeyFrames::Reserve(int count)
		{
			while (static_cast<int>(mKeyFrames.size()) < count)
			{
				New(0);
			}
		}

		AnimKeyFrame* AnimKeyFrames::GetAnimatedKeyFrame(void)
		{
			return mAnimatedKeyFrame;
		}

		AnimKeyFrame* AnimKeyFrames::mClipBoard = new AnimKeyFrame((AnimKeyFrames*)(0), -1, -1);

		void AnimKeyFrames::CopyToClipBoard(AnimKeyFrame* key_frame)
		{
			if (!key_frame)
			{
				return;
			}

			mClipBoard->CopyFrom(key_frame);
		}

		void AnimKeyFrames::CutToClipBoard(AnimKeyFrame* key_frame)
		{
			if (!key_frame)
			{
				return;
			}

			mClipBoard->CopyFrom(key_frame);

			Delete(key_frame);
		}

		void AnimKeyFrames::PasteFromClipBoard(AnimKeyFrame* key_frame)
		{
			if (!key_frame)
			{
				return;
			}

			key_frame->CopyFrom(mClipBoard);
		}

		void AnimKeyFrames::Interpolate(int index, float factor)
		{
			AnimKeyFrame* keyFrameA = operator [](index);
			AnimKeyFrame* keyFrameB = operator [](index + 1);

			if (!keyFrameA || !keyFrameB)
			{
				return;
			}

			mAnimatedKeyFrame->CopyFrom(keyFrameA);
			keyFrameA = mAnimatedKeyFrame;

			keyFrameA->mNodes->First();
			AnimNode* nodeA;
			while (nodeA = keyFrameA->mNodes->operator ()())
			{
				AnimNode* nodeB = keyFrameB->mNodes->Find(nodeA->mName);
				if (nodeB)
				{
					nodeA->Interpolate(nodeB, factor);
				}
				keyFrameA->mNodes->Next();
			}

			keyFrameA->mObjects->First();
			AnimObject* objectA;
			while (objectA = keyFrameA->mObjects->operator()())
			{
				AnimObject* objectB = keyFrameB->mObjects->Find(objectA->mName);
				if (objectB)
				{
					objectA->mMorphTargets->First();
					AnimMorphTarget* morphTargetA;
					while (morphTargetA = objectA->mMorphTargets->operator()())
					{
						AnimMorphTarget* morphTargetB = objectB->mMorphTargets->Find(morphTargetA->mName);
						if (morphTargetB)
						{
							morphTargetA->Interpolate(morphTargetB, factor);
						}
						objectA->mMorphTargets->Next();
					}
				}
				keyFrameA->mObjects->Next();
			}
		}

		bool AnimKeyFrames::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			Clear();
			for (int i = 0; i < count; ++i)
			{
				float time;
				in.read(reinterpret_cast<char*>(&time), 4);
				AnimKeyFrame* key_frame = New(time);

				key_frame->mNodes->LoadFromFStream(in);
				key_frame->mObjects->LoadFromFStream(in);
			}

			return true;
		}

		void AnimKeyFrames::SaveToFStream(std::ofstream& out)
		{
			int count = mKeyFrames.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<AnimKeyFrame*>::iterator key_frame = mKeyFrames.begin(); key_frame != mKeyFrames.end(); ++key_frame)
			{
				out.write(reinterpret_cast<char*>(&(*key_frame)->mTime), 4);

				(*key_frame)->mNodes->SaveToFStream(out);
				(*key_frame)->mObjects->SaveToFStream(out);
			}
		}

		void AnimKeyFrames::SetTimes(std::vector<float>& times)
		{
			std::vector<AnimKeyFrame*>::iterator key_frame;
			std::vector<float>::iterator time;

			for (key_frame = mKeyFrames.begin(), time = times.begin(); key_frame != mKeyFrames.end() && time != times.end(); ++key_frame, ++time)
			{
				(*key_frame)->mTime = *time;
			}
		}
	}
}