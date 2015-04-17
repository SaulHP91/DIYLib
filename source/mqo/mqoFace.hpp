#ifndef MQOFACE_HPP
#define MQOFACE_HPP

#include "mqoTexcoord.hpp"

#include <vector>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace diy
{

	class MqoFace
	{
	public:
		MqoFace(void);
		~MqoFace(void);

		int VertexCount;
		glm::ivec4 VertexIndices;
		int MaterialIndex;
		MqoTexCoord VertexTexCoords[4];

		bool ParseFromFStream(std::ifstream& in);
	};

}

#endif

