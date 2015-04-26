#ifndef SMDBONE_HPP
#define SMDBONE_HPP

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
		class SmdBone
		{
		public:
			DIYLIB_API SmdBone(void);
			DIYLIB_API ~SmdBone(void);

			int Index;
			std::string Name;
			int ParentIndex;

			DIYLIB_API bool ParseFromSStream(std::istringstream& in);
		};
	}
}

#endif
 