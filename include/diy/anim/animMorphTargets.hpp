#ifndef ANIMMORPHTARGETS_HPP
#define ANIMMORPHTARGETS_HPP

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
		class AnimMorphTargets
		{
		public:
			DIYLIB_API ~AnimMorphTargets(void);

			DIYLIB_API AnimObject* GetObject(void);

			DIYLIB_API AnimMorphTarget* New(std::string name);
			DIYLIB_API void Delete(AnimMorphTarget* object);
			DIYLIB_API void Delete(std::string name);
			DIYLIB_API void Clear(void);

			DIYLIB_API AnimMorphTarget* Find(std::string name, bool create = false);

			DIYLIB_API AnimMorphTarget* operator()(void);
			DIYLIB_API AnimMorphTarget* First(void);
			DIYLIB_API AnimMorphTarget* Next(void);
			DIYLIB_API AnimMorphTarget* Last(void);
			DIYLIB_API AnimMorphTarget* Previous(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

		private:
			ANIM_FRIENDS

			AnimMorphTargets(AnimObject* object);

			AnimObject* mObject;

			std::vector<AnimMorphTarget*> mMorphTargets;
			std::vector<AnimMorphTarget*>::iterator mMorphTarget;
		};
	}
}

#endif
 