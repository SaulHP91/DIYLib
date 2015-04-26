#include <diy/pmd/pmd.hpp>

namespace diy
{
	namespace pmd
	{
		PmdFace::PmdFace(void)
		{
			VertexIndices[2] = VertexIndices[1] = VertexIndices[0] = -1;
		}

		PmdFace::~PmdFace(void)
		{
			;
		}

		void PmdFace::LoadFromFStream(std::ifstream& in)
		{
			in.read(reinterpret_cast<char*>(VertexIndices), 6);
		}
	}
}
