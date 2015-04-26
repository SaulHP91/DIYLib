#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XMeshTextureCoords::XMeshTextureCoords(void)
		{
			;
		}

		XMeshTextureCoords::~XMeshTextureCoords(void)
		{
			;
		}

		bool XMeshTextureCoords::ParseFromFStream(std::ifstream& in)
		{
			std::streampos pos;
			char separator;

			if (!in.good())
			{
				return false;
			}

			in.clear();
			pos = in.tellg();

			if (!NTextureCoords.ParseFromFStream(in))
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

			if (!TextureCoords.ParseFromFStream(in, NTextureCoords))
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
