#ifndef POSENODES_HPP
#define POSENODES_HPP

#include <diy/diyClasses.hpp>
#include <diy/pose/poseDefines.hpp>

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
		class PoseNodes
		{
		public:
			DIYLIB_API ~PoseNodes(void);

			DIYLIB_API PosePose* Pose(void);

			DIYLIB_API PoseNode* New(std::string name);
			DIYLIB_API void Delete(PoseNode* object);
			DIYLIB_API void Delete(std::string name);
			DIYLIB_API void Clear(void);

			DIYLIB_API PoseNode* Find(std::string name, bool create = false);

			DIYLIB_API PoseNode* operator()(void);
			DIYLIB_API PoseNode* First(void);
			DIYLIB_API PoseNode* Next(void);
			DIYLIB_API PoseNode* Last(void);
			DIYLIB_API PoseNode* Previous(void);

			DIYLIB_API int Count(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

		private:
			POSE_FRIENDS

			PoseNodes(PosePose* pose);

			PosePose* mPose;

			std::vector<PoseNode*> mNodes;
			std::vector<PoseNode*>::iterator mNode;
		};
	}
}

#endif

