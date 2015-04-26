#ifndef ANIMOBJECT_HPP
#define ANIMOBJECT_HPP

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
		class AnimObject
		{
		public:
			DIYLIB_API ~AnimObject(void);

			DIYLIB_API AnimObjects* GetObjects(void);
			DIYLIB_API std::string GetName(void);

			DIYLIB_API bool Visible(void);
			DIYLIB_API void Show(void);
			DIYLIB_API void Hide(void);

			DIYLIB_API AnimMorphTargets* GetMorphTargets(void);

		private:
			ANIM_FRIENDS

			AnimObject(AnimObjects* objects, std::string name);

			AnimObjects* mObjects;

			std::string mName;
			bool mVisible;
			AnimMorphTargets* mMorphTargets;
		};
	}
}

#endif
