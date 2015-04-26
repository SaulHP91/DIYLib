#include <diy/pmd/pmd.hpp>

namespace diy
{
	namespace pmd
	{
		PmdVertex::PmdVertex(void) :
			Bone0Weight(0),
			EdgeFlag(0)
		{
			BoneIndices[1] = BoneIndices[0] = -1;
		}

		void PmdVertex::LoadFromFStream(std::ifstream& in)
		{
			Position.LoadFromFStream(in);
			Normal.LoadFromFStream(in);
			TexCoord.LoadFromFStream(in);
			in.read(reinterpret_cast<char*>(BoneIndices), 4);
			in.read(&Bone0Weight, 1);
			in.read(&EdgeFlag, 1);
		}
	}
}
