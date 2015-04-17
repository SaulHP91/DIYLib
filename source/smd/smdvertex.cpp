#include "smdVertex.hpp"

namespace diy
{

	SmdVertex::SmdVertex(void) :
		BoneIndex(-1)
	{
		;
	}

	SmdVertex::~SmdVertex(void)
	{
		SkinWeights.clear();
		SkinWeights.swap(std::vector<SmdSkinWeight>(SkinWeights));
	}

	bool SmdVertex::ParseFromSStream(std::istringstream& in)
	{
		std::streampos pos = in.tellg();

		in >> BoneIndex;
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		if (!Position.ParseFromSStream(in))
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		if (!Normal.ParseFromSStream(in))
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		if (!TexCoord.ParseFromSStream(in))
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		int skinWeightCount;
		in >> skinWeightCount;
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		SkinWeights.resize(skinWeightCount);

		for (std::vector<SmdSkinWeight>::iterator smd_skin_weight = SkinWeights.begin(); smd_skin_weight != SkinWeights.end(); ++smd_skin_weight)
		{
			if (!smd_skin_weight->ParseFromSStream(in))
			{
				in.clear();
				in.seekg(pos);
				return false;
			}
		}

		return true;
	}

}
