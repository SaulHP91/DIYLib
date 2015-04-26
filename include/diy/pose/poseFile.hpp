#ifndef POSEFILE_HPP
#define POSEFILE_HPP

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
		class PoseFile
		{
		public:
			DIYLIB_API PoseFile(void);
			DIYLIB_API ~PoseFile(void);

			std::string Name;
			std::string Comment;

			DIYLIB_API PosePoses* Poses(void);

			DIYLIB_API void Clear(void);

			DIYLIB_API bool LoadFromFile(std::string path);
			DIYLIB_API void SaveToFile(std::string path);

		private:
			POSE_FRIENDS

			PosePoses* mPoses;
		};
	}
}

#endif
