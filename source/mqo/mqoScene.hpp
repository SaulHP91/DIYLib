#ifndef MQOSCENE_HPP
#define MQOSCENE_HPP

#include "mqoVector.hpp"
#include "mqoLight.hpp"

#include <fstream>
#include <vector>

namespace diy
{

	class MqoScene
	{
	public:
		MqoScene(void);
		~MqoScene(void);

		MqoVector Position;
		MqoVector LookAt;
		float Head;
		float Pich;
		bool Ortho;
		float Zoom2;
		MqoVector Ambient;
		std::vector<MqoLight> Lights;

		bool ParseFromFStream(std::ifstream& in);
	};

}

#endif
 