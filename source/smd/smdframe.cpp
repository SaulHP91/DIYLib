#include "smdFrame.hpp"

namespace diy
{

	SmdFrame::SmdFrame(void) :
		Time(0),
		NodeIndex(-1)
	{
		;
	}

	SmdFrame::~SmdFrame(void)
	{
		;
	}

	bool SmdFrame::ParseFromSStream(std::istringstream& in)
	{
		std::streampos pos = in.tellg();

		in >> NodeIndex;
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		if (!Translation.ParseFromSStream(in))
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		if (!Rotation.ParseFromSStream(in))
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		return true;
	}

	glm::mat4 SmdFrame::GetMatrix(void)
	{
		return glm::translate(glm::vec3(Translation)) * glm::yawPitchRoll(Rotation.Y, Rotation.X, Rotation.Z);
	}

}
