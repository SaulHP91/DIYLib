#ifndef SMDFACE_HPP
#define SMDFACE_HPP

#include "smdVertex.hpp"

#include <string>
#include <fstream>

namespace diy
{

	class SmdFace
	{
	public:
		SmdFace(void);
		~SmdFace(void);

		std::string Texture;
		SmdVertex Vertices[3];

		bool ParseFromFStream(std::ifstream& in);
	};

}

#endif
 