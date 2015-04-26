#include <diy/pmd/pmd.hpp>

namespace diy
{
	namespace pmd
	{
		PmdBone::PmdBone(void) :
			ParentIndex(-1),
			ChildIndex(-1),
			Type(0),
			TargetBone(-1)
		{
			Name[0] = 0;
		}

		PmdBone::~PmdBone(void)
		{
			;
		}

		void PmdBone::LoadFromFStream(std::ifstream& in)
		{
			in.read(Name, 20);
			in.read(reinterpret_cast<char*>(&ParentIndex), 2);
			in.read(reinterpret_cast<char*>(&ChildIndex), 2);
			in.read(&Type, 1);
			in.read(reinterpret_cast<char*>(&TargetBone), 2);
			Translation.LoadFromFStream(in);
		}
	}
}
 