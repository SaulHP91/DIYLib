#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XMatrix4X4::XMatrix4X4(void)
		{
			Matrix.Values.resize(16);
			Matrix.Values[0] = 1.0f;
			Matrix.Values[5] = 1.0f;
			Matrix.Values[10] = 1.0f;
			Matrix.Values[15] = 1.0f;
		}

		XMatrix4X4::~XMatrix4X4(void)
		{
			;
		}

		bool XMatrix4X4::ParseFromFStream(std::ifstream& in)
		{
			std::streampos pos;
			char separator;

			if (!in.good())
			{
				return false;
			}

			in.clear();
			pos = in.tellg();

			if (!Matrix.ParseFromFStream(in, 16))
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

		XMatrix4X4::operator glm::mat4(void)
		{
			return glm::mat4
				(
				glm::vec4(Matrix.Values[0], Matrix.Values[1], -Matrix.Values[2], Matrix.Values[3]),
				glm::vec4(Matrix.Values[4], Matrix.Values[5], -Matrix.Values[6], Matrix.Values[7]),
				glm::vec4(-Matrix.Values[8], -Matrix.Values[9], Matrix.Values[10], -Matrix.Values[11]),
				glm::vec4(Matrix.Values[12], Matrix.Values[13], -Matrix.Values[14], Matrix.Values[15])
				);
		}
	}
}
