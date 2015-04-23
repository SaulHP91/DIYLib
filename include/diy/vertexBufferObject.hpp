#ifndef VERTEXBUFFEROBJECT_HPP
#define VERTEXBUFFEROBJECT_HPP

#include <diy/attributePointer.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	class VertexBufferObject
	{
	public:
		DIYLIB_API VertexBufferObject(void);
		DIYLIB_API ~VertexBufferObject(void);

		DIYLIB_API unsigned int GetHandle(void);

		DIYLIB_API void Begin(void);
		DIYLIB_API void End(void);

		DIYLIB_API void Data(int size, const void* data, int usage);
		DIYLIB_API const AttributePointer AsAttributePointer(int size, int type, bool normalized = false, int stride = 0, int offset = 0);

	private:
		unsigned int mHandle;
	};

}

#endif
