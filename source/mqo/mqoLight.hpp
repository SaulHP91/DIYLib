#ifndef MQOLIGHT_HPP
#define MQOLIGHT_HPP

#include "mqoVector.hpp"

#include <fstream>

namespace diy
{

	class MqoLight
	{
	public:
		MqoLight(void);
		~MqoLight(void);

		MqoVector Direction;
		MqoVector Color;

		bool ParseFromFStream(std::ifstream& in);
	};

}

#endif
 