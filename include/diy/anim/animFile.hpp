#ifndef ANIMFILE_HPP
#define ANIMFILE_HPP

#include <diy/diyClasses.hpp>
#include <diy/anim/animDefines.hpp>

#include <string>
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
		class AnimFile
		{
		public:
			DIYLIB_API AnimFile(void);
			DIYLIB_API ~AnimFile(void);

			std::string Name;
			std::string Comment;

			DIYLIB_API AnimAnimations* GetAnimations(void);

			DIYLIB_API void Clear(void);

			DIYLIB_API bool LoadFromFile(std::string path);
			DIYLIB_API void SaveToFile(std::string path);

		private:
			ANIM_FRIENDS

			AnimAnimations* mAnimations;
		};
	}
}

#endif
