#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XSkinMeshHeader::XSkinMeshHeader(void)
		{
			;
		}

		XSkinMeshHeader::~XSkinMeshHeader(void)
		{
			;
		}

		bool XSkinMeshHeader::ParseFromFStream(std::ifstream& in)
		{
			std::streampos pos;
			char separator;

			if (!in.good())
			{
				return false;
			}

			in.clear();
			pos = in.tellg();

			if (!NMaxSkinWeightsPerVertex.ParseFromFStream(in))
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

			if (!NMaxSkinWeightsPerFace.ParseFromFStream(in))
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

			if (!NBones.ParseFromFStream(in))
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
