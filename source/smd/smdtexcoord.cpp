#include "smdTexCoord.hpp"

namespace diy
{

	SmdTexCoord::SmdTexCoord(void) :
		U(0.0f),
		V(0.0f)
	{
		;
	}

	SmdTexCoord::~SmdTexCoord(void)
	{
		;
	}

	bool SmdTexCoord::ParseFromSStream(std::istringstream& in)
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

	SmdTexCoord::operator glm::vec2(void)
	{
		return glm::vec2(U, V);
	}

}

 