#ifndef MATRIXSTACK_HPP
#define MATRIXSTACK_HPP

#include <glm/glm.hpp>

#include <stack>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	class MatrixStack
	{
	public:
		DIYLIB_API MatrixStack(void);
		DIYLIB_API ~MatrixStack(void);

		DIYLIB_API void Push(void);
		DIYLIB_API void Pop(void);
		DIYLIB_API glm::mat4& Top(void);

	private:
		std::stack<glm::mat4> mMatrixStack;
	};

}

#endif