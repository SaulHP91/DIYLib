#ifndef ANIMNODES_HPP
#define ANIMNODES_HPP

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
		class AnimNodes
		{
		public:
			DIYLIB_API ~AnimNodes(void);

			DIYLIB_API AnimKeyFrame* GetKeyFrame(void);

			DIYLIB_API AnimNode* New(std::string name);
			DIYLIB_API void Delete(AnimNode* node);
			DIYLIB_API void Delete(std::string name);
			DIYLIB_API void Clear(void);

			DIYLIB_API AnimNode* Find(std::string name, bool create = false);

			DIYLIB_API AnimNode* operator()(void);
			DIYLIB_API AnimNode* First(void);
			DIYLIB_API AnimNode* Next(void);
			DIYLIB_API AnimNode* Last(void);
			DIYLIB_API AnimNode* Previous(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

		private:
			ANIM_FRIENDS

			AnimNodes(AnimKeyFrame* key_frame);

			AnimKeyFrame* mKeyFrame;

			std::vector<AnimNode*> mNodes;
			std::vector<AnimNode*>::iterator mNode;
		};
	}
}

#endif
