#ifndef XSKINWEIGHTS_HPP
#define XSKINWEIGHTS_HPP

#include <diy/x/xString.hpp>
#include <diy/x/xDWord.hpp>
#include <diy/x/xArray.hpp>
#include <diy/x/xMatrix4x4.hpp>

#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		class XFloat;
		class XSkinWeights
		{
		public:
			DIYLIB_API XSkinWeights(void);
			DIYLIB_API ~XSkinWeights(void);

			XString TransformNodeName;
			XDWord NWeights;
			XArray<XDWord> VertexIndices;
			XArray<XFloat> Weights;
			XMatrix4X4 MatrixOffset;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
