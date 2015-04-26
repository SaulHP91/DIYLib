#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XAnimTicksPerSecond::XAnimTicksPerSecond(void)
		{
			;
		}

		XAnimTicksPerSecond::~XAnimTicksPerSecond(void)
		{
			;
		}

		bool XAnimTicksPerSecond::ParseFromFStream(std::ifstream& in)
		{
			std::streampos pos;
			char separator;

			if (!in.good())
			{
				return false;
			}

			in.clear();
			pos = in.tellg();

			if (!AnimTicksPerSecond.ParseFromFStream(in))
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			in >> separator;
			if (!in.good() || separator != ';')
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			return true;
		}
	}
}
