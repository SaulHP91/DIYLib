#ifndef POSEPOSES_HPP
#define POSEPOSES_HPP

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
		class PosePoses
		{
		public:
			DIYLIB_API ~PosePoses(void);

			DIYLIB_API PoseFile* File(void);

			DIYLIB_API PosePose* New(void);
			DIYLIB_API PosePose* New(std::string name);
			DIYLIB_API void Delete(PosePose* pose);
			DIYLIB_API void Delete(int index);
			DIYLIB_API void Delete(std::string name);
			DIYLIB_API void Clear(void);

			DIYLIB_API PosePose* Find(std::string name, bool create = false);

			DIYLIB_API PosePose* operator[](int index);
			DIYLIB_API PosePose* operator()(void);
			DIYLIB_API PosePose* First(void);
			DIYLIB_API PosePose* Next(void);
			DIYLIB_API PosePose* Last(void);
			DIYLIB_API PosePose* Previous(void);

			DIYLIB_API int Count(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

		private:
			POSE_FRIENDS

			PosePoses(PoseFile* file);

			PoseFile* mFile;

			std::vector<PosePose*> mPoses;
			std::vector<PosePose*>::iterator mPose;
		};
	}
}

#endif

