#include "smdBone.hpp"

namespace diy
{

	SmdBone::SmdBone(void) :
		Index(-1),
		ParentIndex(-1)
	{
		;
	}

	SmdBone::~SmdBone(void)
	{
		;
	}

	bool SmdBone::ParseFromSStream(std::istringstream& in)
	{
		static char buffer[255];
		static char* pBuffer = 0;

		std::streampos pos = in.tellg();

		in >> Index;
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		while (in.get() != '\"' && in.good())
		{
			;
		}
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}
		pBuffer = buffer;
		while (true)
		{
			*pBuffer = in.get();
			if (*pBuffer == '\"' || !in.good())
			{
				break;
			}
			++pBuffer;
		}
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}
		*pBuffer = '\0';
		Name = buffer;

		in >> ParentIndex;
		if (in.fail())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		return true;
	}

}
