#include "smdFile.hpp"

namespace diy
{

	SmdFile::SmdFile(void)
	{
		;
	}

	SmdFile::~SmdFile(void)
	{
		Clear();
	}

	void SmdFile::Clear(void)
	{
		Name.clear();

		Bones.clear();
		Bones.swap(std::vector<SmdBone>(Bones));

		Frames.clear();
		Frames.swap(std::vector<SmdFrame>(Frames));

		Faces.clear();
		Faces.swap(std::vector<SmdFace>(Faces));
	}

	bool SmdFile::ParseFromFile(std::string path)
	{
		static char buffer[256];

		Clear();

		bool result = false;
		std::ifstream in(path.c_str());
		if (in.is_open())
		{
			_splitpath(path.c_str(), 0, 0, buffer, 0);
			Name = buffer;

			in.getline(buffer, 255);
			if (!_stricmp(buffer, "version 1"))
			{
				if (ParseBonesFromFStream(in))
				{
					if (ParseFramesFromFStream(in))
					{
						if (ParseFacesFromFStream(in))
						{
							result = true;
						}
					}
				}
			}
			in.close();
		}
		return result;
	}

	bool SmdFile::ParseBonesFromFStream(std::ifstream& in)
	{
		static char buffer[256];

		while (isspace(in.peek()))
		{
			in.ignore();
		}
		in.getline(buffer, 255);
		if (in.fail())
		{
			return false;
		}
		if (!_stricmp(buffer, "nodes"))
		{
			while (true)
			{
				while (isspace(in.peek()))
				{
					in.ignore();
				}
				in.getline(buffer, 255);
				if (!in.good())
				{
					return false;
				}
				if (!_stricmp(buffer, "end"))
				{
					break;
				}
				else
				{
					Bones.push_back(SmdBone());
					if (!Bones.back().ParseFromSStream(std::istringstream(buffer)))
					{
						return false;
					}
				}
			}
		}
		return true;
	}

	bool SmdFile::ParseFramesFromFStream(std::ifstream& in)
	{
		static char buffer[256];
		static SmdFrame frame;

		while (isspace(in.peek()))
		{
			in.ignore();
		}
		in.getline(buffer, 255);
		if (in.fail())
		{
			return false;
		}
		if (!_stricmp(buffer, "skeleton"))
		{
			while (true)
			{
				while (isspace(in.peek()))
				{
					in.ignore();
				}
				std::streampos p = in.tellg();
				in >> buffer;
				if (!_stricmp(buffer, "end"))
				{
					break;
				}
				else if (!_stricmp(buffer, "time"))
				{
					in >> frame.Time;
					if (!in.good())
					{
						return false;
					}
				}
				else
				{
					in.clear();
					in.seekg(p);
					in.getline(buffer, 255);
					strcat(buffer, "\r");
					if (!in.good())
					{
						return false;
					}
					if (frame.ParseFromSStream(std::istringstream(buffer)))
					{
						Frames.push_back(frame);
					}
					else
					{
						return false;
					}
				}
			}
		}
		return true;
	}

	bool SmdFile::ParseFacesFromFStream(std::ifstream& in)
	{
		static char buffer[256];

		while (isspace(in.peek()))
		{
			in.ignore();
		}
		in.getline(buffer, 255);
		if (!in.good())
		{
			return false;
		}
		if (!_stricmp(buffer, "triangles"))
		{
			while (true)
			{
				while (isspace(in.peek()))
				{
					in.ignore();
				}
				std::streampos p = in.tellg();
				in.getline(buffer, 255);
				if (!in.good())
				{
					return false;
				}
				if (!_stricmp(buffer, "end"))
				{
					break;
				}
				else
				{
					in.clear();
					in.seekg(static_cast<std::streampos>(static_cast<std::streamoff>(p) - 1));
					SmdFace smdFace;
					if (smdFace.ParseFromFStream(in))
					{
						Faces.push_back(smdFace);
					}
					else
					{
						in.seekg(p);
						if (smdFace.ParseFromFStream(in))
						{
							Faces.push_back(smdFace);
						}
						else
						{
							return false;
						}
					}
				}
			}
		}
		return true;
	}

}
