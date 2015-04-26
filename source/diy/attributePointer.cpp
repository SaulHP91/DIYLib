#include <diy/attributePointer.hpp>

namespace diy
{

	AttributePointer::AttributePointer(int size, int type, bool normalized, int stride, void* pointer) :
		mHandle(0),
		mSize(size),
		mType(type),
		mNormalized(normalized),
		mStride(stride),
		mPointer(pointer)
	{
		;
	}

	AttributePointer::AttributePointer(unsigned int handle, int size, int type, bool normalized, int stride, void* pointer) :
		mHandle(handle),
		mSize(size),
		mType(type),
		mNormalized(normalized),
		mStride(stride),
		mPointer(pointer)
	{
		;
	}

	AttributePointer::~AttributePointer(void)
	{
		;
	}

}
