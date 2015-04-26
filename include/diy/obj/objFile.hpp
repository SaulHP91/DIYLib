#ifndef OBJFILE_HPP
#define OBJFILE_HPP

#include <string>
#include <vector>

#include <diy/obj/mtlFile.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace obj
	{
		class ObjVector;
		class ObjTexCoord;
		class ObjFace;
		class ObjFile
		{
		public:
			DIYLIB_API ObjFile(void);
			DIYLIB_API ~ObjFile(void);

			std::string Name;
			std::vector<ObjVector> VertexPositions;
			std::vector<ObjTexCoord> VertexTexCoords;
			std::vector<ObjVector> VertexNormals;
			std::vector<std::string> Groups;
			std::vector<ObjFace> Faces;
			MtlFile MaterialLibrary;

			DIYLIB_API void Clear(void);
			DIYLIB_API bool ParseFromFile(std::string path);
			DIYLIB_API int IndexOf(std::string groupName);
		};
	}
}

#endif
