#ifndef MQOSTRING_HPP
#define MQOSTRING_HPP

#include <string>
#include <fstream>
#include <sstream>

namespace diy
{

	class MqoString
	{
	public:
		MqoString(void);
		~MqoString(void);

		std::string Text;

		bool ParseFromFStream(std::ifstream& in);
		bool ParseFromSStream(std::istringstream& in);

		operator std::string(void);
	};

}

#endif
