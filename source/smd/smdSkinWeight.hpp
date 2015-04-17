#ifndef SMDSKINWEIGHT_HPP
#define SMDSKINWEIGHT_HPP

#include <sstream>

namespace diy
{

	class SmdSkinWeight
	{
	public:
		SmdSkinWeight(void);
		~SmdSkinWeight(void);

		int Index;
		float Weight;

		bool ParseFromSStream(std::istringstream& in);
	};

}

#endif
