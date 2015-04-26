#ifndef POSEOBJECT_HPP
#define POSEOBJECT_HPP

#include <diy/diyClasses.hpp>
#include <diy/pose/poseDefines.hpp>

#include <string>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace pose
	{
		class PoseObject
		{
		public:
			DIYLIB_API ~PoseObject(void);

			DIYLIB_API PoseObjects* Objects(void);
			DIYLIB_API std::string Name(void);

			DIYLIB_API bool Visible(void);
			DIYLIB_API void Show(void);
			DIYLIB_API void Hide(void);

			DIYLIB_API PoseMorphTargets* MorphTargets(void);

		private:
			POSE_FRIENDS

			PoseObject(PoseObjects* objects, std::string name);

			PoseObjects* mObjects;

			std::string mName;
			bool mVisible;
			PoseMorphTargets* mMorphTargets;
		};
	}
}

#endif

