#ifndef XMESHMATERIALLIST_HPP
#define XMESHMATERIALLIST_HPP

#include <diy/x/xDWord.hpp>
#include <diy/x/xArray.hpp>

#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		class XMeshMaterialList
		{
		public:
			DIYLIB_API XMeshMaterialList(void);
			DIYLIB_API ~XMeshMaterialList(void);

			XDWord NMaterials;
			XDWord NFaceIndexes;
			XArray<XDWord> FaceIndexes;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
