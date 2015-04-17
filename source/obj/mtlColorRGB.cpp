#include "obj/mtlColorRGB.hpp"

namespace diy
{

	MtlColorRGB::MtlColorRGB(void) :
		R(0.0f),
		G(0.0f),
		B(0.0f)
	{
		;
	}

	MtlColorRGB::MtlColorRGB(float gs) :
		R(gs),
		G(gs),
		B(gs)
	{
		;
	}

	MtlColorRGB::MtlColorRGB(float r, float g, float b) :
		R(r),
		G(g),
		B(b)
	{
		;
	}

	MtlColorRGB::~MtlColorRGB(void)
	{
		;
	}

	bool MtlColorRGB::ParseFromSStream(std::istringstream& in)
	{
		std::streampos pos = in.tellg();

		in >> R;
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		in >> G;
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		in >> B;
		if (in.fail())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		return true;
	}

	MtlColorRGB::operator glm::vec3(void)
	{
		return glm::vec3(R, G, B);
	}

}
