#ifndef POSEPOSE_HPP
#define POSEPOSE_HPP

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
		class PosePose
		{
		public:
			DIYLIB_API ~PosePose(void);

			DIYLIB_API PosePoses* Poses(void);

			DIYLIB_API PoseNodes* Nodes(void);
			DIYLIB_API PoseObjects* Objects(void);

			DIYLIB_API int Index(void);

			DIYLIB_API std::string Name(void);
			DIYLIB_API bool Rename(std::string name);

			std::string Comment;

			DIYLIB_API void CopyFrom(PosePose* pose);
			DIYLIB_API void Record(mesh::MeshFile* mesh_file);
			DIYLIB_API void Reset(void);

		private:
			POSE_FRIENDS

			PosePose(PosePoses* poses, std::string name, int index);

			PosePoses* mPoses;

			PoseNodes* mNodes;
			PoseObjects* mObjects;

			int mIndex;
			std::string mName;
		};
	}
}

#endif

