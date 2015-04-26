#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XDWord::XDWord(int value) :
			Value(value)
		{
			;
		}

		XDWord::~XDWord(void)
		{
			Value = 0;
		}

		bool XDWord::ParseFromFStream(std::ifstream& in)
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

		XDWord::operator int(void)
		{
			return Value;
		}
	}
}
