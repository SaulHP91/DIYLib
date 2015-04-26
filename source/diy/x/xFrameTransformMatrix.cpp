#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XFrameTransformMatrix::XFrameTransformMatrix(void)
		{
			;
		}

		XFrameTransformMatrix::~XFrameTransformMatrix(void)
		{
			;
		}

		bool XFrameTransformMatrix::ParseFromFStream(std::ifstream& in)
		{
			std::streampos pos;
			char separator;

			if (!in.good())
			{
				return false;
			}

			in.clear();
			pos = in.tellg();

			if (!FrameMatrix.ParseFromFStream(in))
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
