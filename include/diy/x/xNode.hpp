#ifndef XOBJECT_HPP
#define XOBJECT_HPP

#include <diy/x/xName.hpp>

#include <vector>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		class XNode
		{
		public:
			DIYLIB_API XNode(void);
			DIYLIB_API ~XNode(void);

			XName Name;
			enum NodeType
			{
				Null,
				AnimTicksPerSecond,
				Frame,
				FrameTransformMatrix,
				Mesh,
				MeshNormals,
				MeshTextureCoords,
				MeshMaterialList,
				Material,
				SkinMeshHeader,
				SkinWeights,
				TextureFileName,
				AnimationSet,
				Animation,
				AnimationKey
			} Type;
			int ID;
			int ParentID;
			int Index;
			bool Instance;
			int Indentation;

			DIYLIB_API std::string GetString(void);
		};
	}
}

#endif
