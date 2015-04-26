#include <diy/smd/smd.hpp>

namespace diy
{
	namespace smd
	{
		SmdVector::SmdVector(void) :
			X(0.0f),
			Y(0.0f),
			Z(0.0f)
		{
			;
		}

		SmdVector::~SmdVector(void)
		{
			;
		}

		bool SmdVector::ParseFromSStream(std::istringstream& in)
		{
			std::streampos pos = in.tellg();

			in >> X;
			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			in >> Y;
			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			in >> Z;
			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			return true;
		}

		SmdVector::operator glm::vec3(void)
		{
			return glm::vec3(X, Y, Z);
		}
	}
}
