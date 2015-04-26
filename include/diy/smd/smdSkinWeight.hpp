#ifndef SMDSKINWEIGHT_HPP
#define SMDSKINWEIGHT_HPP

#include <sstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace smd
	{
		class SmdSkinWeight
		{
		public:
			DIYLIB_API SmdSkinWeight(void);
			DIYLIB_API ~SmdSkinWeight(void);

			int Index;
			float Weight;

			DIYLIB_API bool ParseFromSStream(std::istringstream& in);
		};
	}
}

#endif
