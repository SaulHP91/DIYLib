#ifndef ANIMKEYFRAMES_HPP
#define ANIMKEYFRAMES_HPP

#include <diy/diyClasses.hpp>
#include <diy/anim/animDefines.hpp>

#include <fstream>
#include <vector>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace anim
	{
		class AnimKeyFrames
		{
		public:
			DIYLIB_API ~AnimKeyFrames(void);

			DIYLIB_API AnimAnimation* GetAnimation(void);

			DIYLIB_API AnimKeyFrame* New(float time);
			DIYLIB_API AnimKeyFrame* Duplicate(AnimKeyFrame* key_frame);
			DIYLIB_API void Delete(AnimKeyFrame* key_frame);
			DIYLIB_API void Delete(int index);
			DIYLIB_API void Clear(void);

			DIYLIB_API void CopyToClipBoard(AnimKeyFrame* key_frame);
			DIYLIB_API void CutToClipBoard(AnimKeyFrame* key_frame);
			DIYLIB_API void PasteFromClipBoard(AnimKeyFrame* key_frame);

			DIYLIB_API AnimKeyFrame* operator[](int index);
			DIYLIB_API AnimKeyFrame* operator()(void);
			DIYLIB_API AnimKeyFrame* First(void);
			DIYLIB_API AnimKeyFrame* Next(void);
			DIYLIB_API AnimKeyFrame* Last(void);
			DIYLIB_API AnimKeyFrame* Previous(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API void Reserve(int count);

			DIYLIB_API AnimKeyFrame* GetAnimatedKeyFrame(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

			DIYLIB_API void SetTimes(std::vector<float>& times);

		private:
			ANIM_FRIENDS

			AnimKeyFrames(AnimAnimation* animation);

			AnimAnimation* mAnimation;

			std::vector<AnimKeyFrame*> mKeyFrames;
			std::vector<AnimKeyFrame*>::iterator mKeyFrame;

			static AnimKeyFrame* mClipBoard;

			AnimKeyFrame* mAnimatedKeyFrame;

			void Interpolate(int index, float factor);
		};
	}
}

#endif
 