#include <diy/smd/smd.hpp>

namespace diy
{
	namespace smd
	{
		SmdSkinWeight::SmdSkinWeight(void) :
			Index(-1),
			Weight(0.0f)
		{
			;
		}

		SmdSkinWeight::~SmdSkinWeight(void)
		{
			;
		}

		bool SmdSkinWeight::ParseFromSStream(std::istringstream& in)
		{
			std::streampos pos = in.tellg();

			in >> Index;
			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			in >> Weight;
			if (in.fail())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			return true;
		}
	}
}
