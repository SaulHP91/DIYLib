#ifndef MQOSTRING_HPP
#define MQOSTRING_HPP

#include <string>
#include <fstream>
#include <sstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mqo
	{
		class MqoString
		{
		public:
			DIYLIB_API MqoString(void);
			DIYLIB_API ~MqoString(void);

			std::string Text;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
			DIYLIB_API bool ParseFromSStream(std::istringstream& in);

			DIYLIB_API operator std::string(void);
		};
	}
}

#endif
