#include <diy/pmd/pmd.hpp>

#include <fstream>

namespace diy
{
	namespace pmd
	{
		PmdFile::PmdFile(void)
		{
			Name[0] = 0;
			Comment[0] = 0;
		}

		PmdFile::~PmdFile(void)
		{
			Clear();
		}

		void PmdFile::Clear(void)
		{
			Name[0] = 0;
			Comment[0] = 0;

			Vertices.clear();
			Vertices.swap(std::vector<PmdVertex>(Vertices));

			Faces.clear();
			Faces.swap(std::vector<PmdFace>(Faces));

			Materials.clear();
			Materials.swap(std::vector<PmdMaterial>(Materials));

			Bones.clear();
			Bones.swap(std::vector<PmdBone>(Bones));

			IKChains.clear();
			IKChains.swap(std::vector<PmdIKChain>(IKChains));

			MorphTargets.clear();
			MorphTargets.swap(std::vector<PmdMorphTarget>(MorphTargets));
		}

		bool PmdFile::LoadFromFile(std::string path)
		{
			Clear();

			bool result = false;
			std::ifstream in(path.c_str(), std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				char header[4];
				in.read(header, 3);
				header[3] = 0;
				if (!strcmp(header, "Pmd"))
				{
					float version;
					in.read(reinterpret_cast<char*>(&version), 4);
					if (version == 1.0f)
					{
						in.read(Name, 20);
						in.read(Comment, 256);

						unsigned int vertexCount;
						in.read(reinterpret_cast<char*>(&vertexCount), 4);
						Vertices.resize(vertexCount);
						for (std::vector<PmdVertex>::iterator pmd_vertex = Vertices.begin(); pmd_vertex != Vertices.end(); ++pmd_vertex)
						{
							pmd_vertex->LoadFromFStream(in);
						}

						unsigned int indexCount;
						in.read(reinterpret_cast<char*>(&indexCount), 4);
						int faceCount = indexCount / 3;
						Faces.resize(faceCount);
						for (std::vector<PmdFace>::iterator pmd_face = Faces.begin(); pmd_face != Faces.end(); ++pmd_face)
						{
							pmd_face->LoadFromFStream(in);
						}

						unsigned int materialCount;
						in.read(reinterpret_cast<char*>(&materialCount), 4);
						Materials.resize(materialCount);
						for (std::vector<PmdMaterial>::iterator pmd_material = Materials.begin(); pmd_material != Materials.end(); ++pmd_material)
						{
							pmd_material->LoadFromFStream(in);
						}

						unsigned short boneCount;
						in.read(reinterpret_cast<char*>(&boneCount), 2);
						Bones.resize(boneCount);
						for (std::vector<PmdBone>::iterator pmd_bone = Bones.begin(); pmd_bone != Bones.end(); ++pmd_bone)
						{
							pmd_bone->LoadFromFStream(in);
						}

						unsigned short iKChainCount;
						in.read(reinterpret_cast<char*>(&iKChainCount), 2);
						IKChains.resize(iKChainCount);
						for (std::vector<PmdIKChain>::iterator pmd_ik_chain = IKChains.begin(); pmd_ik_chain != IKChains.end(); ++pmd_ik_chain)
						{
							pmd_ik_chain->LoadFromFStream(in);
						}

						unsigned short morphTargetCount;
						in.read(reinterpret_cast<char*>(&morphTargetCount), 2);
						MorphTargets.resize(morphTargetCount);
						for (std::vector<PmdMorphTarget>::iterator pmd_morph_target = MorphTargets.begin(); pmd_morph_target != MorphTargets.end(); ++pmd_morph_target)
						{
							pmd_morph_target->LoadFromFStream(in);
						}

						result = true;
					}
				}
			}
			return result;
		}
	}
}
