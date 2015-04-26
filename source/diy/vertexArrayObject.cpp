#include <gl/glew.h>

#include <diy/vertexArrayObject.hpp>

namespace diy
{

	VertexArrayObject::VertexArrayObject(void) :
		mHandle(0)
	{
		;
	}

	VertexArrayObject::~VertexArrayObject(void)
	{
		if (0 == mHandle)
		{
			return;
		}

		glDeleteVertexArrays(1, &mHandle);

		mHandle = 0;
	}

	unsigned int VertexArrayObject::GetHandle(void)
	{
		return mHandle;
	}

	void VertexArrayObject::Begin(void)
	{
		if (0 == mHandle)
		{
			glGenVertexArrays(1, &mHandle);
		}

		if (0 != mHandle)
		{
			glBindVertexArray(mHandle);
		}
	}

	void VertexArrayObject::End(void)
	{
		glBindVertexArray(0);
	}

}
