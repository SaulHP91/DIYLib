#ifndef ANIMOBJECTS_HPP
#define ANIMOBJECTS_HPP

#include <diy/diyClasses.hpp>
#include <diy/anim/animDefines.hpp>

#include <string>
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
		class AnimObjects
		{
		public:
			DIYLIB_API ~AnimObjects(void);

			DIYLIB_API AnimKeyFrame* GetKeyFrame(void);

			DIYLIB_API AnimObject* New(std::string name);
			DIYLIB_API void Delete(AnimObject* object);
			DIYLIB_API void Delete(std::string name);
			DIYLIB_API void Clear(void);

			DIYLIB_API AnimObject* Find(std::string name, bool create = false);

			DIYLIB_API AnimObject* operator()(void);
			DIYLIB_API AnimObject* First(void);
			DIYLIB_API AnimObject* Next(void);
			DIYLIB_API AnimObject* Last(void);
			DIYLIB_API AnimObject* Previous(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

		private:
			ANIM_FRIENDS

			AnimObjects(AnimKeyFrame* key_frame);

			AnimKeyFrame* mKeyFrame;

			std::vector<AnimObject*> mObjects;
			std::vector<AnimObject*>::iterator mObject;
		};
	}
}

#endif
 