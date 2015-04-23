#ifndef NAMEDSHADERARGUMENT_HPP
#define NAMEDSHADERARGUMENT_HPP

#include <diy/attributePointer.hpp>

#include <glm/glm.hpp>

#include <string>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	class NamedShaderArgument
	{
	public:
		DIYLIB_API ~NamedShaderArgument(void);

		DIYLIB_API void operator=(bool value);
		DIYLIB_API void operator=(int value);
		DIYLIB_API void operator=(float value);
		DIYLIB_API void operator=(glm::vec2 value);
		DIYLIB_API void operator=(glm::vec3 value);
		DIYLIB_API void operator=(glm::vec4 value);
		DIYLIB_API void operator=(glm::mat3 value);
		DIYLIB_API void operator=(glm::mat4 value);
		DIYLIB_API void operator=(AttributePointer attributePointer);

	private:
		friend class Shader;

		NamedShaderArgument(unsigned int program, const char* name);
		unsigned int mProgram;
		const char* mName;
	};

}

#endif