#include <diy/pmd/pmd.hpp>

namespace diy
{
	namespace pmd
	{
		PmdTexCoord::PmdTexCoord(void) :
			U(0.0f),
			V(0.0f)
		{
			;
		}

		PmdTexCoord::~PmdTexCoord(void)
		{
			;
		}

		bool PmdTexCoord::LoadFromFStream(std::ifstream& in)
		{
			in.read(reinterpret_cast<char*>(&U), 4);
			in.read(reinterpret_cast<char*>(&V), 4);

			return true;
		}

		PmdTexCoord::operator glm::vec2(void)
		{
			return glm::vec2(U, -V);
		}
	}
}
