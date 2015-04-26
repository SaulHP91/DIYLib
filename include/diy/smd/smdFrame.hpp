#ifndef SMDFRAME_HPP
#define SMDFRAME_HPP

#include <diy/smd/smdVector.hpp>

#include <sstream>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace smd
	{
		class SmdFrame
		{
		public:
			DIYLIB_API SmdFrame(void);
			DIYLIB_API ~SmdFrame(void);

			int Time, NodeIndex;

			SmdVector Translation;
			SmdVector Rotation;

			DIYLIB_API bool ParseFromSStream(std::istringstream& in);

			DIYLIB_API glm::mat4 GetMatrix(void);
		};
	}
}

#endif
