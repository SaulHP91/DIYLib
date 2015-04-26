#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XTextureFileName::XTextureFileName(void)
		{
			;
		}

		XTextureFileName::~XTextureFileName(void)
		{
			;
		}

		bool XTextureFileName::ParseFromFStream(std::ifstream& in)
		{
			std::streampos pos;
			char separator;

			if (!in.good())
			{
				return false;
			}

			in.clear();
			pos = in.tellg();

			if (!FileName.ParseFromFStream(in))
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
