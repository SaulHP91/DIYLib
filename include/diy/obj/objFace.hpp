#ifndef OBJFACE_HPP
#define OBJFACE_HPP

#include <vector>
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
		class ObjIndexedVertex;
		class ObjFace
		{
		public:
			DIYLIB_API ObjFace(void);
			DIYLIB_API ~ObjFace(void);

			std::vector<ObjIndexedVertex> IndexedVertices;
			int GroupIndex;
			int MaterialIndex;

			DIYLIB_API bool ParseFromSStream(std::istringstream& in);
		};
	}
}

#endif
