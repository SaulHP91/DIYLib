#ifndef MQOSCENE_HPP
#define MQOSCENE_HPP

#include <diy/mqo/mqoVector.hpp>

#include <fstream>
#include <vector>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mqo
	{
		class MqoLight;
		class MqoScene
		{
		public:
			DIYLIB_API MqoScene(void);
			DIYLIB_API ~MqoScene(void);

			MqoVector Position;
			MqoVector LookAt;
			float Head;
			float Pich;
			bool Ortho;
			float Zoom2;
			MqoVector Ambient;
			std::vector<MqoLight> Lights;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};
	}
}

#endif
 