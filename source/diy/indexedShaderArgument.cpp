#include <gl/glew.h>

#include <glm/ext.hpp>

#include <diy/indexedShaderArgument.hpp>

namespace diy
{

	IndexedShaderArgument::IndexedShaderArgument(unsigned int program, int location) :
		mProgram(program),
		mLocation(location)
	{

	}

	IndexedShaderArgument::~IndexedShaderArgument(void)
	{
		;
	}

	void IndexedShaderArgument::operator=(bool value)
	{
		glProgramUniform1i(mProgram, mLocation, value);
	}

	void IndexedShaderArgument::operator=(int value)
	{
		glProgramUniform1i(mProgram, mLocation, value);
	}

	void IndexedShaderArgument::operator=(float value)
	{
		glProgramUniform1f(mProgram, mLocation, value);
	}

	void IndexedShaderArgument::operator=(glm::vec2 value)
	{
		glProgramUniform2f(mProgram, mLocation, value.x, value.y);
	}

	void IndexedShaderArgument::operator=(glm::vec3 value)
	{
		glProgramUniform3f(mProgram, mLocation, value.x, value.y, value.z);
	}

	void IndexedShaderArgument::operator=(glm::vec4 value)
	{
		glProgramUniform4f(mProgram, mLocation, value.x, value.y, value.z, value.w);
	}

	void IndexedShaderArgument::operator=(glm::mat3 value)
	{
		glProgramUniformMatrix3fv(mProgram, mLocation, 1, 0, glm::value_ptr(value));
	}

	void IndexedShaderArgument::operator=(glm::mat4 value)
	{
		glProgramUniformMatrix4fv(mProgram, mLocation, 1, 0, glm::value_ptr(value));
	}

	void IndexedShaderArgument::operator=(AttributePointer attributePointer)
	{
		glEnableVertexAttribArray(mLocation);
		glBindBuffer(GL_ARRAY_BUFFER, attributePointer.mHandle);
		glVertexAttribPointer(mLocation, attributePointer.mSize, attributePointer.mType, attributePointer.mNormalized, attributePointer.mStride, attributePointer.mPointer);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
