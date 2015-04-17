#ifndef MQOFILE_HPP
#define MQOFILE_HPP

#include "mqoScene.hpp"
#include "mqoMaterial.hpp"
#include "mqoObject.hpp"

#include <fstream>
#include <vector>

namespace diy
{

	class MqoFile
	{
	public:
		MqoFile(void);
		~MqoFile(void);

		std::string Name;
		MqoScene Scene;
		std::vector<MqoMaterial> Materials;
		std::vector<MqoObject> Objects;

		void Clear(void);
		bool ParseFromFile(std::string path);
	};

}

#endif
