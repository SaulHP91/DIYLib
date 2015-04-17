#include "objTexCoord.hpp"

namespace diy
{

	ObjTexCoord::ObjTexCoord(void) :
		U(0.0f),
		V(0.0f)
	{
		;
	}

	ObjTexCoord::~ObjTexCoord(void)
	{
		;
	}

	bool ObjTexCoord::ParseFromSStream(std::istringstream& in)
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
		if (in.fail())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		static int W;

		pos = in.tellg();

		in >> W;
		if (in.fail())
		{
			in.clear();
			in.seekg(pos);
		}

		return true;
	}

	ObjTexCoord::operator glm::vec2(void)
	{
		return glm::vec2(U, V);
	}

}
