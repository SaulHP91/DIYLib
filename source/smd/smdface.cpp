#include "smdFace.hpp"

namespace diy
{

	SmdFace::SmdFace(void)
	{
		;
	}

	SmdFace::~SmdFace(void)
	{
		;
	}

	bool SmdFace::ParseFromFStream(std::ifstream& in)
	{
		static char line[256];

		std::streampos pos = in.tellg();

		in.getline(line, 255);
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}
		Texture = line;

		while (isspace(in.peek()))
		{
			in.ignore();
		}

		in.getline(line, 255);
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		if (!Vertices[0].ParseFromSStream(std::istringstream(line)))
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		while (isspace(in.peek()))
		{
			in.ignore();
		}

		in.getline(line, 255);
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		if (!Vertices[1].ParseFromSStream(std::istringstream(line)))
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		while (isspace(in.peek()))
		{
			in.ignore();
		}

		in.getline(line, 255);
		if (!in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		if (!Vertices[2].ParseFromSStream(std::istringstream(line)))
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		return true;
	}

}
