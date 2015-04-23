#ifndef ATTRIBUTEPOINTER_HPP
#define ATTRIBUTEPOINTER_HPP

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	class AttributePointer
	{

	public:
		DIYLIB_API AttributePointer(int size, int type, bool normalized = false, int stride = 0, void* pointer = 0);
		DIYLIB_API ~AttributePointer(void);

	private:
		friend class VertexBufferObject;
		friend class NamedShaderArgument;
		friend class IndexedShaderArgument;

		AttributePointer(unsigned int handle, int size, int type, bool normalized = false, int stride = 0, void* pointer = 0);

		unsigned int mHandle;
		int mSize;
		int mType;
		bool mNormalized;
		int mStride;
		void* mPointer;
	};

}

#endif
