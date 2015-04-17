#ifndef SMDTEXCOORD_HPP
#define SMDTEXCOORD_HPP

#include <sstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace diy
{

	class SmdTexCoord
	{
	public:
		SmdTexCoord(void);
		~SmdTexCoord(void);

		float U, V;

		bool ParseFromSStream(std::istringstream& in);

		operator glm::vec2(void);
	};

}

#endif

 