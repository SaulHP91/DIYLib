#include <diy/tds/tds.hpp>

namespace diy
{
	namespace tds
	{
		TdsObject::TdsObject(void)
		{
			;
		}

		TdsObject::~TdsObject(void)
		{
			PointArray.clear();
			PointArray.swap(std::vector<TdsPoint>(PointArray));

			TexVerts.clear();
			TexVerts.swap(std::vector<TdsTexVert>(TexVerts));

			FaceArray.clear();
			FaceArray.swap(std::vector<TdsFace>(FaceArray));

			MshMatGroups.clear();
			MshMatGroups.swap(std::vector<TdsMshMatGroup>(MshMatGroups));
		}

		bool TdsObject::LoadNameFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_NAMED_OBJECT)
			{
				return false;
			}

			Name = chunk.GetString(in);

			if (chunk.OnString)
			{
				chunk.OnString(Name.c_str(), chunk.User);
			}

			return true;
		}

		bool TdsObject::LoadPointArrayFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_POINT_ARRAY)
			{
				return false;
			}

			unsigned short nPoints = chunk.GetShort(in);

			PointArray.resize(static_cast<unsigned int>(nPoints));
			for (std::vector<TdsPoint>::iterator point = PointArray.begin(); point != PointArray.end(); ++point)
			{
				point->LoadFromChunkAndFStream(chunk, in);
			}

			if (chunk.OnString)
			{
				char buffer[8];
				sprintf(buffer, "%i", PointArray.size());
				chunk.OnString(buffer, chunk.User);
			}

			return true;
		}

		bool TdsObject::LoadTexVertsFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_TEX_VERTS)
			{
				return false;
			}

			unsigned short nVerts = chunk.GetShort(in);

			TexVerts.resize(static_cast<unsigned int>(nVerts));
			for (std::vector<TdsTexVert>::iterator texVert = TexVerts.begin(); texVert != TexVerts.end(); ++texVert)
			{
				texVert->LoadFromChunkAndFStream(chunk, in);
			}

			if (chunk.OnString)
			{
				char buffer[8];
				sprintf(buffer, "%i", TexVerts.size());
				chunk.OnString(buffer, chunk.User);
			}

			return true;
		}

		bool TdsObject::LoadFaceArrayFromChunkAndFStream(TdsChunk& chunk, std::ifstream& in)
		{
			if (chunk.Type != TDS_FACE_ARRAY)
			{
				return false;
			}

			unsigned short nFaces = chunk.GetShort(in);

			FaceArray.resize(static_cast<unsigned int>(nFaces));
			for (std::vector<TdsFace>::iterator face = FaceArray.begin(); face != FaceArray.end(); ++face)
			{
				face->LoadFromChunkAndFStream(chunk, in);
			}

			if (chunk.OnString)
			{
				char buffer[8];
				sprintf(buffer, "%i", FaceArray.size());
				chunk.OnString(buffer, chunk.User);
			}

			return true;
		}
	}
}