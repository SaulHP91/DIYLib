#include <diy/pmd/pmd.hpp>

namespace diy
{
	namespace pmd
	{
		PmdVector::PmdVector(void) :
			X(0.0f),
			Y(0.0f),
			Z(0.0f)
		{
			;
		}

		PmdVector::~PmdVector(void)
		{
			;
		}

		bool PmdVector::LoadFromFStream(std::ifstream& in)
		{
			in.read(reinterpret_cast<char*>(&X), 4);
			in.read(reinterpret_cast<char*>(&Y), 4);
			in.read(reinterpret_cast<char*>(&Z), 4);

			return true;
		}

		PmdVector::operator glm::vec3(void)
		{
			return glm::vec3(X, Y, -Z);
		}
	}
}
