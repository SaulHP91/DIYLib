#include <diy/tds/tds.hpp>

#include <fstream>

namespace diy
{
	namespace tds
	{
		TdsFile::TdsFile(void) :
			OnChunk(0),
			OnString(0)
		{
			;
		}

		TdsFile::~TdsFile(void)
		{
			Clear();
		}

		void TdsFile::Clear(void)
		{
			Name.clear();

			Materials.clear();
			Materials.swap(std::vector<TdsMaterial>(Materials));

			Objects.clear();
			Objects.swap(std::vector<TdsObject>(Objects));
		}

		bool TdsFile::LoadFromFile(std::string file)
		{
			char name[_MAX_FNAME];
			char ext[_MAX_EXT];
			_splitpath(file.c_str(), 0, 0, name, ext);

			if (_stricmp(ext, TDS_EXT))
			{
				return false;
			}

			Name = name;

			std::ifstream in(file.c_str(), std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				TdsChunks chunks;
				chunks.OnChunk = OnChunk;
				chunks.OnString = OnString;

				chunks.LoadFirstChunk(in);

				if (chunks.GetChunk().Type != TDS_M3DMAGIC)
				{
					in.close();
					return false;
				}

				while (chunks.LoadNextChunk(in))
				{
					switch (chunks.GetChunk().Type)
					{
					case TDS_M3DMAGIC:
					{
						break;
					}
					case TDS_MDATA:
					{
						break;
					}
					case TDS_KFDATA:
					{
						break;
					}
					case TDS_NAMED_OBJECT:
					{
						Objects.push_back(TdsObject());
						Objects.back().LoadNameFromChunkAndFStream(chunks.GetChunk(), in);
						break;
					}
					case TDS_N_TRI_OBJECT:
					{
						break;
					}
					case TDS_POINT_ARRAY:
					{
						Objects.back().LoadPointArrayFromChunkAndFStream(chunks.GetChunk(), in);
						break;
					}
					case TDS_FACE_ARRAY:
					{
						Objects.back().LoadFaceArrayFromChunkAndFStream(chunks.GetChunk(), in);
						break;
					}
					case TDS_MSH_MAT_GROUP:
					{
						Objects.back().MshMatGroups.push_back(TdsMshMatGroup());
						Objects.back().MshMatGroups.back().LoadFromChunkAndFStream(chunks.GetChunk(), in);
						break;
					}
					case TDS_TEX_VERTS:
					{
						Objects.back().LoadTexVertsFromChunkAndFStream(chunks.GetChunk(), in);
						break;
					}
					case TDS_MESH_MATRIX:
					{
						Objects.back().Matrix.LoadFromChunkAndFStream(chunks.GetChunk(), in);
						break;
					}
					case TDS_MAT_ENTRY:
					{
						Materials.push_back(TdsMaterial());
						Materials.back().LoadFromChunksAndFStream(chunks, in);
						break;
					}
					case TDS_OBJECT_NODE_TAG:
					{
						ObjectNodes.push_back(TdsObjectNode());
						break;
					}
					case TDS_NODE_ID:
					{
						ObjectNodes.back().LoadIDFromFStreamAndChunk(chunks.GetChunk(), in);
						break;
					}
					case TDS_NODE_HDR:
					{
						ObjectNodes.back().Header.LoadFromChunkAndFStream(chunks.GetChunk(), in);
						break;
					}
					case TDS_PIVOT:
					{
						ObjectNodes.back().LoadPivotFromFStreamAndChunk(chunks.GetChunk(), in);
						break;
					}
					default:
					{
						chunks.GetChunk().Finish(in);
					}
					}
				}

				in.close();
			}

			return true;
		}
	}
}
