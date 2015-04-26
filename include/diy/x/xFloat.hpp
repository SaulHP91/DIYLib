#ifndef XFLOAT_HPP
#define XFLOAT_HPP

#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		class XFloat
		{
		public:
			XFloat(float value = 0.0f);
			~XFloat(void);

			float Value;

			bool ParseFromFStream(std::ifstream& in);

			operator float(void);
		};
	}
}

#endif
