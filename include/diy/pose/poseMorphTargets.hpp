#ifndef POSEMORPHTARGETS_HPP
#define POSEMORPHTARGETS_HPP

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
		class PoseMorphTargets
		{
		public:
			DIYLIB_API ~PoseMorphTargets(void);

			DIYLIB_API PoseObject* Object(void);

			DIYLIB_API PoseMorphTarget* New(std::string name);
			DIYLIB_API void Delete(PoseMorphTarget* object);
			DIYLIB_API void Delete(std::string name);
			DIYLIB_API void Clear(void);

			DIYLIB_API PoseMorphTarget* Find(std::string name, bool create = false);

			DIYLIB_API PoseMorphTarget* operator()(void);
			DIYLIB_API PoseMorphTarget* First(void);
			DIYLIB_API PoseMorphTarget* Next(void);
			DIYLIB_API PoseMorphTarget* Last(void);
			DIYLIB_API PoseMorphTarget* Previous(void);

			DIYLIB_API int Count(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

		private:
			POSE_FRIENDS

			PoseMorphTargets(PoseObject* object);

			PoseObject* mObject;

			std::vector<PoseMorphTarget*> mMorphTargets;
			std::vector<PoseMorphTarget*>::iterator mMorphTarget;
		};
	}
}

#endif

