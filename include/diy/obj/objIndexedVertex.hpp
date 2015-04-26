#ifndef OBJINDEXEDVERTEX_HPP
#define OBJINDEXEDVERTEX_HPP

#include <sstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace obj
	{
		class ObjIndexedVertex
		{
		public:
			DIYLIB_API ObjIndexedVertex(void);
			DIYLIB_API ~ObjIndexedVertex(void);

			int PositionIndex;
			int TexCoordIndex;
			int NormalIndex;
			static int PositionCount;
			static int TexCoordCount;
			static int NormalCount;

			DIYLIB_API bool ParseFromSStream(std::istringstream& in);
			DIYLIB_API bool operator==(ObjIndexedVertex objIndexedVertex);
		};
	}
}

#endif
