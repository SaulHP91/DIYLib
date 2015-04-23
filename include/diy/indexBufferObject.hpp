#ifndef INDEXBUFFEROBJECT_HPP
#define INDEXBUFFEROBJECT_HPP

#include <diy/attributePointer.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	class IndexBufferObject
	{
	public:
		DIYLIB_API IndexBufferObject(void);
		DIYLIB_API ~IndexBufferObject(void);

		DIYLIB_API unsigned int GetHandle(void);

		DIYLIB_API void Begin(void);
		DIYLIB_API void End(void);

		DIYLIB_API void Data(int size, const void* data, int usage);

	private:
		unsigned int mHandle;
	};

}

#endif
