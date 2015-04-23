#include <gl/glew.h>

#include <glm/ext.hpp>

#include <diy/namedShaderArgument.hpp>

namespace diy
{

	NamedShaderArgument::NamedShaderArgument(unsigned int program, const char* name) :
		mProgram(program),
		mName(name)
	{

	}

	NamedShaderArgument::~NamedShaderArgument(void)
	{
		;
	}

	void NamedShaderArgument::operator=(bool value)
	{
		glProgramUniform1i(mProgram, glGetUniformLocation(mProgram, mName), value);
	}

	void NamedShaderArgument::operator=(int value)
	{
		glProgramUniform1i(mProgram, glGetUniformLocation(mProgram, mName), value);
	}

	void NamedShaderArgument::operator=(float value)
	{
		glProgramUniform1f(mProgram, glGetUniformLocation(mProgram, mName), value);
	}

	void NamedShaderArgument::operator=(glm::vec2 value)
	{
		glProgramUniform2f(mProgram, glGetUniformLocation(mProgram, mName), value.x, value.y);
	}

	void NamedShaderArgument::operator=(glm::vec3 value)
	{
		glProgramUniform3f(mProgram, glGetUniformLocation(mProgram, mName), value.x, value.y, value.z);
	}

	void NamedShaderArgument::operator=(glm::vec4 value)
	{
		glProgramUniform4f(mProgram, glGetUniformLocation(mProgram, mName), value.x, value.y, value.z, value.w);
	}

	void NamedShaderArgument::operator=(glm::mat3 value)
	{
		glProgramUniformMatrix3fv(mProgram, glGetUniformLocation(mProgram, mName), 1, 0, glm::value_ptr(value));
	}

	void NamedShaderArgument::operator=(glm::mat4 value)
	{
		glProgramUniformMatrix4fv(mProgram, glGetUniformLocation(mProgram, mName), 1, 0, glm::value_ptr(value));
	}

	void NamedShaderArgument::operator=(AttributePointer attributePointer)
	{
		int location = glGetAttribLocation(mProgram, mName);

		glEnableVertexAttribArray(location);
		glBindBuffer(GL_ARRAY_BUFFER, attributePointer.mHandle);
		glVertexAttribPointer(location, attributePointer.mSize, attributePointer.mType, attributePointer.mNormalized, attributePointer.mStride, attributePointer.mPointer);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
