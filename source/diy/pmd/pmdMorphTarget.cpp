#include <diy/pmd/pmd.hpp>

namespace diy
{
	namespace pmd
	{
		PmdMorphTarget::PmdMorphTarget(void) :
			Type(0)
		{
			Name[0] = 0;
		}

		PmdMorphTarget::~PmdMorphTarget(void)
		{
			Vertices.clear();
			Vertices.swap(std::vector<PmdMorphTargetVertex>(Vertices));
		}

		void PmdMorphTarget::LoadFromFStream(std::ifstream& in)
		{
			in.read(Name, 20);
			unsigned int vertexCount;
			in.read(reinterpret_cast<char*>(&vertexCount), 4);
			in.read(reinterpret_cast<char*>(&Type), 1);
			Vertices.resize(vertexCount);
			for (std::vector<PmdMorphTargetVertex>::iterator pmd_morph_target_vertex = Vertices.begin(); pmd_morph_target_vertex != Vertices.end(); ++pmd_morph_target_vertex)
			{
				pmd_morph_target_vertex->LoadFromFStream(in);
			}
		}
	}
}