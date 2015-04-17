#ifndef SMDVECTOR_HPP
#define SMDVECTOR_HPP

#include <sstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace diy
{

	class SmdVector
	{
	public:
		SmdVector(void);
		~SmdVector(void);

		float X, Y, Z;

		bool ParseFromSStream(std::istringstream& in);

		operator glm::vec3(void);
	};

}

#endif
 