#ifndef SMDFACE_HPP
#define SMDFACE_HPP

#include <diy/smd/smdVertex.hpp>

#include <string>
#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace smd
	{
		class SmdFace
		{
		public:
			DIYLIB_API SmdFace(void);
			DIYLIB_API ~SmdFace(void);

			std::string Texture;
			SmdVertex Vertices[3];

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
 