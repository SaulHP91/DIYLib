#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XString::XString(void)
		{
			;
		}

		XString::~XString(void)
		{
			;
		}

		bool XString::ParseFromFStream(std::ifstream& in)
		{
			static char buffer[256];

			if (!in.good())
			{
				return false;
			}

			if (!XParseStringFromFStream(buffer, in))
			{
				return false;
			}

			Text = buffer;

			return true;
		}

		XString::operator std::string(void)
		{
			return Text;
		}
	}
}
