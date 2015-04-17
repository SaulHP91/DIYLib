#include "mqoString.hpp"

namespace diy
{

	MqoString::MqoString(void)
	{
		;
	}

	MqoString::~MqoString(void)
	{
		;
	}

	bool MqoString::ParseFromFStream(std::ifstream& in)
	{
		static char buffer[_MAX_FNAME];

		std::streampos pos = in.tellg();

		while (isspace(in.peek()))
		{
			in.ignore();
		}

		char token;
		in >> token;
		if (token != '\"' || !in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		for (char* p_buffer = buffer; true; ++p_buffer)
		{
			*p_buffer = in.get();
			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}
			if (*p_buffer == '\"')
			{
				*p_buffer = 0;
				break;
			}
		}

		Text = buffer;

		return true;
	}

	bool MqoString::ParseFromSStream(std::istringstream& in)
	{
		static char buffer[_MAX_FNAME];

		std::streampos pos = in.tellg();

		while (isspace(in.peek()))
		{
			in.ignore();
		}

		char token;
		in >> token;
		if (token != '\"' || !in.good())
		{
			in.clear();
			in.seekg(pos);
			return false;
		}

		for (char* p_buffer = buffer; true; ++p_buffer)
		{
			*p_buffer = in.get();
			if (!in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}
			if (*p_buffer == '\"')
			{
				*p_buffer = 0;
				break;
			}
		}

		Text = buffer;

		return true;
	}

	MqoString::operator std::string(void)
	{
		return Text;
	}

}
