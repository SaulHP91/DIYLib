#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XColorRGBA::XColorRGBA(void) :
			Alpha(1.0f)
		{
			;
		}

		XColorRGBA::~XColorRGBA(void)
		{
			;
		}

		bool XColorRGBA::ParseFromFStream(std::ifstream& in)
		{
			std::streampos pos;
			char separator;

			if (!in.good())
			{
				return false;
			}

			in.clear();
			pos = in.tellg();

			if (!Red.ParseFromFStream(in))
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

			if (!Green.ParseFromFStream(in))
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

			if (!Blue.ParseFromFStream(in))
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

			if (!Alpha.ParseFromFStream(in))
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

		XColorRGBA::operator glm::vec4(void)
		{
			return glm::vec4(Red.Value, Green.Value, Blue.Value, Alpha.Value);
		}
	}
}
