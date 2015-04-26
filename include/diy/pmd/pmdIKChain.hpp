#ifndef PMDINVERSEKINEMATICS_HPP
#define PMDINVERSEKINEMATICS_HPP

#include <vector>
#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace pmd
	{
		class PmdIKChain
		{
		public:
			DIYLIB_API PmdIKChain(void);
			DIYLIB_API ~PmdIKChain(void);

			short Target;
			short EndEffector;
			short MaxIterations;
			float MaxAngle;
			std::vector<short> Links;

			DIYLIB_API void LoadFromFStream(std::ifstream& in);
		};
	}
}

#endif
 