#include <gl/glew.h>

#include <diy/vertexBufferObject.hpp>

namespace diy
{

	VertexBufferObject::VertexBufferObject(void) :
		mHandle(0)
	{
		;
	}

	VertexBufferObject::~VertexBufferObject(void)
	{
		if (0 == mHandle)
		{
			return;
		}

		glDeleteBuffers(1, &mHandle);

		mHandle = 0;
	}

	unsigned int VertexBufferObject::GetHandle(void)
	{
		return mHandle;
	}

	void VertexBufferObject::Begin(void)
	{
		if (0 == mHandle)
		{
			glGenBuffers(1, &mHandle);
		}

		if (0 != mHandle)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mHandle);
		}
	}

	void VertexBufferObject::End(void)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBufferObject::Data(int size, const void* data, int usage)
	{
		Begin();
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		End();		
	}

	const AttributePointer VertexBufferObject::AsAttributePointer(int size, int type, bool normalized, int stride, int offset)
	{
		return AttributePointer(mHandle, size, type, normalized, stride, reinterpret_cast<void*>(offset));
	}

}
