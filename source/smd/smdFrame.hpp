#ifndef SMDFRAME_HPP
#define SMDFRAME_HPP

#include "smdVector.hpp"

#include <sstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>


namespace diy
{

	class SmdFrame
	{
	public:
		SmdFrame(void);
		~SmdFrame(void);

		int Time, NodeIndex;

		SmdVector Translation;
		SmdVector Rotation;

		bool ParseFromSStream(std::istringstream& in);

		glm::mat4 GetMatrix(void);
	};

}

#endif
