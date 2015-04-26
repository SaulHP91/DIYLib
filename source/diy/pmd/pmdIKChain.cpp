#include <diy/pmd/pmd.hpp>

namespace diy
{
	namespace pmd
	{
		PmdIKChain::PmdIKChain(void)
		{
			;
		}

		PmdIKChain::~PmdIKChain(void)
		{
			Links.clear();
			Links.swap(std::vector<short>(Links));
		}

		void PmdIKChain::LoadFromFStream(std::ifstream& in)
		{
			in.read(reinterpret_cast<char*>(&Target), 2);
			in.read(reinterpret_cast<char*>(&EndEffector), 2);
			unsigned char linkCount;
			in.read(reinterpret_cast<char*>(&linkCount), 1);
			in.read(reinterpret_cast<char*>(&MaxIterations), 2);
			in.read(reinterpret_cast<char*>(&MaxAngle), 4);
			Links.clear();
			Links.swap(std::vector<short>(Links));
			Links.reserve(linkCount);
			short link;
			for (int i = 0; i < linkCount; ++i)
			{
				in.read(reinterpret_cast<char*>(&link), 2);
				Links.push_back(link);
			}
		}
	}
}
