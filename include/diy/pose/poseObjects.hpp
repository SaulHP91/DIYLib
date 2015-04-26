#ifndef POSEOBJECTS_HPP
#define POSEOBJECTS_HPP

#include <diy/diyClasses.hpp>
#include <diy/pose/poseDefines.hpp>

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
	namespace pose
	{
		class PoseObjects
		{
		public:
			~PoseObjects(void);

			DIYLIB_API PosePose* Pose(void);

			DIYLIB_API PoseObject* New(std::string name);
			DIYLIB_API void Delete(PoseObject* object);
			DIYLIB_API void Delete(std::string name);
			DIYLIB_API void Clear(void);

			DIYLIB_API PoseObject* Find(std::string name, bool create = false);

			DIYLIB_API PoseObject* operator()(void);
			DIYLIB_API PoseObject* First(void);
			DIYLIB_API PoseObject* Next(void);
			DIYLIB_API PoseObject* Last(void);
			DIYLIB_API PoseObject* Previous(void);

			DIYLIB_API int Count(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

		private:
			POSE_FRIENDS

			PoseObjects(PosePose* pose);

			PosePose* mPose;

			std::vector<PoseObject*> mObjects;
			std::vector<PoseObject*>::iterator mObject;
		};
	}
}

#endif

