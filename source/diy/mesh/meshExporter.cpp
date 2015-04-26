#include <diy/diy.hpp>

#include <fstream>

namespace diy
{
	namespace mesh
	{
		void MeshExporter::ExportCCode(MeshFile* file, std::string name)
		{
			if (!file || name.empty())
			{
				return;
			}

			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR];
			char fname[_MAX_FNAME];
			char ext[_MAX_EXT];
			char path[_MAX_PATH];
			_splitpath(name.c_str(), drive, dir, fname, ext);
			_makepath(path, drive, dir, fname, "c");

			std::ofstream out(path);
			if (out.is_open())
			{
				out << "const int " << fname << "_vertices = " << file->mObjects->mAnimatedVertexCount << ";" << std::endl;
				out << std::endl;

				out << "const int " << fname << "_faces = " << file->mObjects->mFaceCount << ";" << std::endl;
				out << std::endl;

				out << "const float " << fname << "_v[] = " << std::endl;
				out << "{" << std::endl;
				for (std::vector<MeshObject*>::iterator object = file->mObjects->mObjects.begin(); object != file->mObjects->mObjects.end(); ++object)
				{
					for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = (*object)->mAnimatedVertices->mAnimatedVertices.begin(); animated_vertex != (*object)->mAnimatedVertices->mAnimatedVertices.end(); ++animated_vertex)
					{
						out << '\t' << (*animated_vertex)->mVertex.Position.x << ", " << (*animated_vertex)->mVertex.Position.y << ", " << (*animated_vertex)->mVertex.Position.z << ',' << std::endl;
					}
				}
				out << "};" << std::endl;
				out << std::endl;

				out << "const float " << fname << "_vt[] = " << std::endl;
				out << "{" << std::endl;
				for (std::vector<MeshObject*>::iterator object = file->mObjects->mObjects.begin(); object != file->mObjects->mObjects.end(); ++object)
				{
					for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = (*object)->mAnimatedVertices->mAnimatedVertices.begin(); animated_vertex != (*object)->mAnimatedVertices->mAnimatedVertices.end(); ++animated_vertex)
					{
						out << '\t' << (*animated_vertex)->mVertex.TexCoord.x << ", " << (*animated_vertex)->mVertex.TexCoord.y << ',' << std::endl;
					}
				}
				out << "};" << std::endl;
				out << std::endl;

				out << "const float " << fname << "_vtan[] = " << std::endl;
				out << "{" << std::endl;
				for (std::vector<MeshObject*>::iterator object = file->mObjects->mObjects.begin(); object != file->mObjects->mObjects.end(); ++object)
				{
					for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = (*object)->mAnimatedVertices->mAnimatedVertices.begin(); animated_vertex != (*object)->mAnimatedVertices->mAnimatedVertices.end(); ++animated_vertex)
					{
						out << '\t' << (*animated_vertex)->mVertex.Tangent.x << ", " << (*animated_vertex)->mVertex.Tangent.y << ", " << (*animated_vertex)->mVertex.Tangent.z << ',' << std::endl;
					}
				}
				out << "};" << std::endl;
				out << std::endl;

				out << "const float " << fname << "_vbi[] = " << std::endl;
				out << "{" << std::endl;
				for (std::vector<MeshObject*>::iterator object = file->mObjects->mObjects.begin(); object != file->mObjects->mObjects.end(); ++object)
				{
					for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = (*object)->mAnimatedVertices->mAnimatedVertices.begin(); animated_vertex != (*object)->mAnimatedVertices->mAnimatedVertices.end(); ++animated_vertex)
					{
						out << '\t' << (*animated_vertex)->mVertex.Bitangent.x << ", " << (*animated_vertex)->mVertex.Bitangent.y << ", " << (*animated_vertex)->mVertex.Bitangent.z << ',' << std::endl;
					}
				}
				out << "};" << std::endl;
				out << std::endl;

				out << "const float " << fname << "_vn[] = " << std::endl;
				out << "{" << std::endl;
				for (std::vector<MeshObject*>::iterator object = file->mObjects->mObjects.begin(); object != file->mObjects->mObjects.end(); ++object)
				{
					for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = (*object)->mAnimatedVertices->mAnimatedVertices.begin(); animated_vertex != (*object)->mAnimatedVertices->mAnimatedVertices.end(); ++animated_vertex)
					{
						out << '\t' << (*animated_vertex)->mVertex.Normal.x << ", " << (*animated_vertex)->mVertex.Normal.y << ", " << (*animated_vertex)->mVertex.Normal.z << ',' << std::endl;
					}
				}
				out << "};" << std::endl;
				out << std::endl;

				out << "const int " << fname << "_f[] = " << std::endl;
				out << "{" << std::endl;
				for (std::vector<MeshObject*>::iterator object = file->mObjects->mObjects.begin(); object != file->mObjects->mObjects.end(); ++object)
				{
					for (std::vector<MeshFace*>::iterator face = (*object)->mFaces->mFaces.begin(); face != (*object)->mFaces->mFaces.end(); ++face)
					{
						out << '\t' << (*face)->mAnimatedVertices[0]->mIndex << ", " << (*face)->mAnimatedVertices[1]->mIndex << ", " << (*face)->mAnimatedVertices[2]->mIndex << ',' << std::endl;
					}
				}
				out << "};" << std::endl;
				out << std::endl;

				out.close();
			}
		}
	}
}
 