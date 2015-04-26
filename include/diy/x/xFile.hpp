#ifndef XFILE_HPP
#define XFILE_HPP

#include <string>
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
		class XNode;
		class XAnimTicksPerSecond;
		class XFrame;
		class XFrameTransformMatrix;
		class XMesh;
		class XMeshNormals;
		class XMeshTextureCoords;
		class XMeshMaterialList;
		class XMaterial;
		class XSkinMeshHeader;
		class XSkinWeights;
		class XTextureFileName;
		class XAnimationSet;
		class XAnimation;
		class XAnimationKey;
		class XFile
		{
		public:
			DIYLIB_API XFile(void);
			DIYLIB_API ~XFile(void);

			std::string Name;
			std::vector<XNode> Nodes;
			std::vector<XAnimTicksPerSecond> AnimTicksPerSecond;
			std::vector<XFrame> Frames;
			std::vector<XFrameTransformMatrix> FrameTransformMatrices;
			std::vector<XMesh> Meshes;
			std::vector<XMeshNormals> MeshNormals;
			std::vector<XMeshTextureCoords> MeshTextureCoords;
			std::vector<XMeshMaterialList> MeshMaterialList;
			std::vector<XMaterial> Materials;
			std::vector<XSkinMeshHeader> SkinMeshHeaders;
			std::vector<XSkinWeights> SkinWeights;
			std::vector<XTextureFileName> TextureFileNames;
			std::vector<XAnimationSet> AnimationSets;
			std::vector<XAnimation> Animations;
			std::vector<XAnimationKey> AnimationKeys;

			DIYLIB_API void Clear();
			DIYLIB_API bool ParseFromFile(std::string filename);
		};
	}
}

#endif
