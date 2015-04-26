#include <diy/mqo/mqo.hpp>

namespace diy
{
	namespace mqo
	{
		MqoTexCoord::MqoTexCoord(void) :
			U(0.0f),
			V(0.0f)
		{
			;
		}

		MqoTexCoord::~MqoTexCoord(void)
		{
			;
		}

		bool MqoTexCoord::ParseFromSStream(std::istringstream& in)
		{
			std::streampos pos = in.tellg();

			in >> U;
			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			in >> V;
			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			return true;
		}

		MqoTexCoord::operator glm::vec2(void)
		{
			return glm::vec2(U, -V);
		}
	}
}
