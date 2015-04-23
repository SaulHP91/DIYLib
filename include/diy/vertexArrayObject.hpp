#ifndef VERTEXARRAYOBJECT_HPP
#define VERTEXARRAYOBJECT_HPP

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	class VertexArrayObject
	{
	public:
		DIYLIB_API VertexArrayObject(void);
		DIYLIB_API ~VertexArrayObject(void);

		DIYLIB_API unsigned int GetHandle(void);

		DIYLIB_API void Begin(void);
		DIYLIB_API void End(void);

	private:
		unsigned int mHandle;
	};

}

#endif