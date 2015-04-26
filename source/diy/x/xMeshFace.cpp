#include <diy/x/x.hpp>

namespace diy
{
	namespace x
	{
		XMeshFace::XMeshFace(void)
		{
			;
		}

		XMeshFace::~XMeshFace(void)
		{
			NFaceVertexIndices.~XDWord();
			FaceVertexIndices.~XArray<XDWord>();
		}

		bool XMeshFace::ParseFromFStream(std::ifstream& in)
		{
			std::streampos pos;
			char separator;

			if (!in.good())
			{
				return false;
			}

			in.clear();
			pos = in.tellg();

			if (!NFaceVertexIndices.ParseFromFStream(in))
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

			if (!FaceVertexIndices.ParseFromFStream(in, NFaceVertexIndices))
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
