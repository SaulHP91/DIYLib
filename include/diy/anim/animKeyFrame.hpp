#ifndef ANIMKEYFRAME_HPP
#define ANIMKEYFRAME_HPP

#include <diy/diyClasses.hpp>
#include <diy/anim/animDefines.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace anim
	{
		class AnimKeyFrame
		{
		public:
			DIYLIB_API ~AnimKeyFrame(void);

			DIYLIB_API AnimKeyFrames* GetKeyFrames(void);

			DIYLIB_API AnimNodes* GetNodes(void);
			DIYLIB_API AnimObjects* GetObjects(void);

			DIYLIB_API int GetIndex(void);

			DIYLIB_API void SetTime(float time);
			DIYLIB_API float GetTime(void);

			DIYLIB_API float GetLastStep(void);

			DIYLIB_API void CopyFrom(AnimKeyFrame* key_frame);
			DIYLIB_API void Record(mesh::MeshFile* mesh_file);
			DIYLIB_API void Accumulate(AnimKeyFrame* key_frame, float weight);
			DIYLIB_API void Reset(void);

		private:
			ANIM_FRIENDS

			AnimKeyFrame(AnimKeyFrames* key_frames, int index, float time);

			AnimKeyFrames* mKeyFrames;

			AnimNodes* mNodes;
			AnimObjects* mObjects;

			int mIndex;
			float mTime;
			float mLastStep;
		};
	}
}

#endif
 