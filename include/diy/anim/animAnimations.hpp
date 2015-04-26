#ifndef ANIMANIMATIONS_HPP
#define ANIMANIMATIONS_HPP

#include <diy/diyClasses.hpp>
#include <diy/anim/animDefines.hpp>

#include <vector>
#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace anim
	{
		class AnimAnimations
		{
		public:
			DIYLIB_API ~AnimAnimations(void);

			DIYLIB_API AnimFile* GetFile(void);

			DIYLIB_API AnimAnimation* New(void);
			DIYLIB_API AnimAnimation* New(std::string name);
			DIYLIB_API void Delete(AnimAnimation* animation);
			DIYLIB_API void Delete(int index);
			DIYLIB_API void Delete(std::string name);
			DIYLIB_API void Clear(void);

			DIYLIB_API AnimAnimation* Find(std::string name, bool create = false);

			DIYLIB_API AnimAnimation* operator[](int index);
			DIYLIB_API AnimAnimation* operator()(void);
			DIYLIB_API AnimAnimation* First(void);
			DIYLIB_API AnimAnimation* Next(void);
			DIYLIB_API AnimAnimation* Last(void);
			DIYLIB_API AnimAnimation* Previous(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API void SetTime(float time);
			DIYLIB_API float GetTime(void);
			DIYLIB_API void Step(float step);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

			DIYLIB_API AnimKeyFrame* GetAnimatedKeyFrame(void);

		private:
			ANIM_FRIENDS

			AnimAnimations(AnimFile* file);

			AnimFile* mFile;

			std::vector<AnimAnimation*> mAnimations;
			std::vector<AnimAnimation*>::iterator mAnimation;

			float mTime;

			AnimKeyFrame* mAnimatedKeyFrame;
		};
	}
}

#endif
 