#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XFloat::XFloat(float value) :
			Value(value)
		{
			;
		}

		XFloat::~XFloat(void)
		{
			Value = 0.0f;
		}

		bool XFloat::ParseFromFStream(std::ifstream& in)
		{
			std::streampos pos;

			if (!in.good())
			{
				return false;
			}

			in.clear();
			pos = in.tellg();

			in >> Value;
			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			return true;
		}

		XFloat::operator float(void)
		{
			return Value;
		}
	}
}
