#ifndef MQOTEXCOORD_HPP
#define MQOTEXCOORD_HPP

#include <sstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace diy
{

	class MqoTexCoord
	{
	public:
		MqoTexCoord(void);
		~MqoTexCoord(void);

		float U, V;

		bool ParseFromSStream(std::istringstream& in);

		operator glm::vec2(void);
	};

}

#endif

 