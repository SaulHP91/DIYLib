#ifndef SMDFILE_HPP
#define SMDFILE_HPP

#include <string>
#include <vector>
#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace smd
	{
		class SmdBone;
		class SmdFrame;
		class SmdFace;		
		class SmdFile
		{
		public:
			DIYLIB_API SmdFile(void);
			DIYLIB_API ~SmdFile(void);

			std::string Name;
			std::vector<SmdBone> Bones;
			std::vector<SmdFrame> Frames;
			std::vector<SmdFace> Faces;

			DIYLIB_API void Clear(void);
			DIYLIB_API bool ParseFromFile(std::string path);

		private:
			bool ParseBonesFromFStream(std::ifstream& in);
			bool ParseFramesFromFStream(std::ifstream& in);
			bool ParseFacesFromFStream(std::ifstream& in);
		};
	}
}

#endif
