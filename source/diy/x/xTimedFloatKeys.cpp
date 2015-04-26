#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XTimedFloatKeys::XTimedFloatKeys(void)
		{
			;
		}

		XTimedFloatKeys::~XTimedFloatKeys(void)
		{
			;
		}

		bool XTimedFloatKeys::ParseFromFStream(std::ifstream& in)
		{
			std::streampos pos;
			char separator;

			if (!in.good())
			{
				return false;
			}

			in.clear();
			pos = in.tellg();

			if (!Time.ParseFromFStream(in))
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

			if (!TFKeys.ParseFromFStream(in))
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
