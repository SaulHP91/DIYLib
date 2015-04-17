#ifndef SMDBONE_HPP
#define SMDBONE_HPP

#include <sstream>

namespace diy
{

	class SmdBone
	{
	public:
		SmdBone(void);
		~SmdBone(void);

		int Index;
		std::string Name;
		int ParentIndex;

		bool ParseFromSStream(std::istringstream& in);
	};

}

#endif
 