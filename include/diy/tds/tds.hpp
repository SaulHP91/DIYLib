#ifndef TDS_HPP
#define TDS_HPP

#include <diy/tds/tdsChunk.hpp>
#include <diy/tds/tdsChunks.hpp>
#include <diy/tds/tdsColor.hpp>
#include <diy/tds/tdsDefines.hpp>
#include <diy/tds/tdsFace.hpp>
#include <diy/tds/tdsFile.hpp>
#include <diy/tds/tdsMap.hpp>
#include <diy/tds/tdsMaterial.hpp>
#include <diy/tds/tdsMatrix.hpp>
#include <diy/tds/tdsMshMatGroup.hpp>
#include <diy/tds/tdsNodeHeader.hpp>
#include <diy/tds/tdsObject.hpp>
#include <diy/tds/tdsObjectNode.hpp>
#include <diy/tds/tdsPercentage.hpp>
#include <diy/tds/tdsPoint.hpp>
#include <diy/tds/tdsTexVert.hpp>

namespace diy
{
	namespace tds
	{
		__inline const char* TdsChunkTypeToString(unsigned short id)
		{
			switch (id)
			{
			case TDS_NULL_CHUNK:          return "NULL_CHUNK";
			case TDS_M3DMAGIC:            return "M3DMAGIC";
			case TDS_MDATA:               return "MDATA";
			case TDS_KFDATA:              return "KFDATA";
			case TDS_M3D_VERSION:         return "M3D_VERSION";
			case TDS_MESH_VERSION:        return "MESH_VERSION";
			case TDS_M3D_KFVERSION:       return "M3D_KFVERSION";
			case TDS_COLOR_F:             return "COLOR_F";
			case TDS_COLOR_24:            return "COLOR_24";
			case TDS_LIN_COLOR_24:        return "LIN_COLOR_24";
			case TDS_LIN_COLOR_F:         return "LIN_COLOR_F";
			case TDS_INT_PERCENTAGE:      return "INT_PERCENTAGE";
			case TDS_FLOAT_PERCENTAGE:    return "FLOAT_PERCENTAGE";
			case TDS_MASTER_SCALE:        return "MASTER_SCALE";
			case TDS_BIT_MAP:             return "BIT_MAP";
			case TDS_AMBIENT_LIGHT:       return "AMBIENT_LIGHT";
			case TDS_NAMED_OBJECT:        return "NAMED_OBJECT";
			case TDS_N_TRI_OBJECT:        return "N_TRI_OBJECT";
			case TDS_POINT_ARRAY:         return "POINT_ARRAY";
			case TDS_POINT_FLAG_ARRAY:    return "POINT_FLAG_ARRAY";
			case TDS_FACE_ARRAY:          return "FACE_ARRAY";
			case TDS_MSH_MAT_GROUP:       return "MSH_MAT_GROUP";
			case TDS_TEX_VERTS:           return "TEX_VERTS";
			case TDS_MESH_MATRIX:         return "MESH_MATRIX";
			case TDS_MESH_COLOR:          return "MESH_COLOR";
			case TDS_MESH_TEXTURE_INFO:   return "MESH_TEXTURE_INFO";
			case TDS_HIERARCHY:           return "HIERARCHY";
			case TDS_MAT_ENTRY:           return "MAT_ENTRY";
			case TDS_MAT_NAME:            return "MAT_NAME";
			case TDS_MAT_AMBIENT:         return "MAT_AMBIENT";
			case TDS_MAT_DIFFUSE:         return "MAT_DIFFUSE";
			case TDS_MAT_SPECULAR:        return "MAT_SPECULAR";
			case TDS_MAT_SHININESS:       return "MAT_SHININESS";
			case TDS_MAT_SHIN2PCT:        return "MAT_SHIN2PCT";
			case TDS_MAT_TRANSPARENCY:    return "MAT_TRANSPARENCY";
			case TDS_MAT_XPFALL:          return "MAT_XPFALL";
			case TDS_MAT_REFBLUR:         return "MAT_REFBLUR";
			case TDS_MAT_EMISSIVE:        return "MAT_EMISSIVE";
			case TDS_MAT_TWO_SIDE:        return "MAT_TWO_SIDE";
			case TDS_MAT_SELF_ILPCT:      return "MAT_SELF_ILPCT";
			case TDS_MAT_WIRESIZE:        return "MAT_WIRESIZE";
			case TDS_MAT_SHADING:         return "MAT_SHADING";
			case TDS_MAT_TEXMAP:          return "MAT_TEXMAP";
			case TDS_MAT_OPACMAP:         return "MAT_OPACMAP";
			case TDS_MAT_MAPNAME:         return "MAT_MAPNAME";
			case TDS_MAT_MAP_TILING:      return "MAT_MAP_TILING";
			case TDS_MAT_MAP_USCALE:      return "MAT_MAP_USCALE";
			case TDS_MAT_MAP_VSCALE:      return "MAT_MAP_VSCALE";
			case TDS_MAT_MAP_UOFFSET:     return "MAT_MAP_UOFFSET";
			case TDS_MAT_MAP_VOFFSET:     return "MAT_MAP_VOFFSET";
			case TDS_N_DIRECT_LIGHT:      return "N_DIRECT_LIGHT";
			case TDS_N_CAMERA:            return "N_CAMERA";
			case TDS_KFHDR:               return "KFHDR";
			case TDS_OBJECT_NODE_TAG:     return "OBJECT_NODE_TAG";
			case TDS_L_TARGET_NODE_TAG:   return "L_TARGET_NODE_TAG";
			case TDS_SPOTLIGHT_NODE_TAG:  return "SPOTLIGHT_NODE_TAG";
			case TDS_KFSEG:               return "KFSEG";
			case TDS_KFCURTIME:           return "KFCURTIME";
			case TDS_NODE_HDR:            return "NODE_HDR";
			case TDS_INSTANCE_NAME:       return "INSTANCE_NAME";
			case TDS_PIVOT:               return "PIVOT";
			case TDS_BOUNDBOX:            return "BOUNDBOX";
			case TDS_POS_TRACK_TAG:       return "POS_TRACK_TAG";
			case TDS_ROT_TRACK_TAG:       return "ROT_TRACK_TAG";
			case TDS_SCL_TRACK_TAG:       return "SCL_TRACK_TAG";
			case TDS_NODE_ID:             return "NODE_ID";
			}

			return "???";
		}
	}
}

#endif
