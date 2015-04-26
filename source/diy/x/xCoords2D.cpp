#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XCoords2D::XCoords2D(void)
		{
			;
		}

		XCoords2D::~XCoords2D(void)
		{
			;
		}

		bool XCoords2D::ParseFromFStream(std::ifstream& in)
		{
			std::streampos pos;
			char separator;

			if (!in.good())
			{
				return false;
			}

			in.clear();
			pos = in.tellg();

			if (!U.ParseFromFStream(in))
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

			if (!V.ParseFromFStream(in))
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

		XCoords2D::operator glm::vec2(void)
		{
			return glm::vec2(U.Value, -V.Value);
		}
	}
}
