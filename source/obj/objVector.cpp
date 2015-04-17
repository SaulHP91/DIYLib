#include "objVector.hpp"

namespace diy
{

	ObjVector::ObjVector(void) :
		X(0.0f),
		Y(0.0f),
		Z(0.0f)
	{
		;
	}

	ObjVector::~ObjVector(void)
	{
		;
	}

	bool ObjVector::ParseFromSStream(std::istringstream& in)
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
		if (in.fail())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		return true;
	}

	ObjVector::operator glm::vec3(void)
	{
		return glm::vec3(X, Y, Z);
	}

}
