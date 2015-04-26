#ifndef MQOLIGHT_HPP
#define MQOLIGHT_HPP

#include <diy/mqo/mqoVector.hpp>

#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mqo
	{
		class MqoLight
		{
		public:
			DIYLIB_API MqoLight(void);
			DIYLIB_API ~MqoLight(void);

			MqoVector Direction;
			MqoVector Color;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
 