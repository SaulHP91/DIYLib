#include <diy/pmd/pmd.hpp>

namespace diy
{
	namespace pmd
	{
		PmdMorphTargetVertex::PmdMorphTargetVertex(void) :
			Index(-1)
		{
			;
		}

		PmdMorphTargetVertex::~PmdMorphTargetVertex(void)
		{
			;
		}

		void PmdMorphTargetVertex::LoadFromFStream(std::ifstream& in)
		{
			in.read(reinterpret_cast<char*>(&Index), 4);
			Position.LoadFromFStream(in);
		}
	}
}
