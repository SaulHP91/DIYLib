#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XNode::XNode(void) :
			Type(Null),
			ID(-1),
			ParentID(-1),
			Index(-1),
			Instance(false),
			Indentation(0)
		{
			;
		}

		XNode::~XNode(void)
		{
			;
		}

		std::string XNode::GetString(void)
		{
			std::string indentation(Indentation, '+'), type;
			switch (Type)
			{
			case Null:
				type = "Null";
				break;
			case AnimTicksPerSecond:
				type = "AnimTicksPerSecond";
				break;
			case Frame:
				type = "Frame";
				break;
			case FrameTransformMatrix:
				type = "FrameTransformMatrix";
				break;
			case Mesh:
				type = "Mesh";
				break;
			case MeshNormals:
				type = "MeshNormals";
				break;
			case MeshTextureCoords:
				type = "MeshTextureCoords";
				break;
			case MeshMaterialList:
				type = "MeshMaterialList";
				break;
			case Material:
				type = "Material";
				break;
			case SkinMeshHeader:
				type = "SkinMeshHeader";
				break;
			case SkinWeights:
				type = "SkinWeights";
				break;
			case TextureFileName:
				type = "TextureFileName";
				break;
			case AnimationSet:
				type = "AnimationSet";
				break;
			case Animation:
				type = "Animation";
				break;
			case AnimationKey:
				type = "AnimationKey";
				break;
			default:
				type = "Unknown";
			}

			static char buffer[256];
			sprintf(buffer, "%s%s %s%s", indentation.c_str(), type.c_str(), Name.Text.c_str(), Instance ? "*" : "");
			return std::string(buffer);
		}
	}
}
