#include <diy/mqo/mqo.hpp>

namespace diy
{
	namespace mqo
	{
		MqoVector::MqoVector(void) :
			X(0.0f),
			Y(0.0f),
			Z(0.0f)
		{
			;
		}

		MqoVector::MqoVector(float s) :
			X(s),
			Y(s),
			Z(s)
		{
			;
		}

		MqoVector::MqoVector(float x, float y, float z) :
			X(x),
			Y(y),
			Z(z)
		{
			;
		}

		MqoVector::~MqoVector(void)
		{
			;
		}

		bool MqoVector::ParseFromFStream(std::ifstream& in)
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

		bool MqoVector::ParseFromSStream(std::istringstream& in)
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

		MqoVector::operator glm::vec3(void)
		{
			return glm::vec3(X, Y, Z);
		}
	}
}
