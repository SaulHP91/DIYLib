#ifndef SHADER_HPP
#define SHADER_HPP

#include <cstdarg>
#include <string>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	class Shader
	{
	public:
		DIYLIB_API Shader(void);
		DIYLIB_API ~Shader(void);

		DIYLIB_API int GetShader(int type);
		DIYLIB_API int GetProgram(void);

		DIYLIB_API void Clear(void);

		DIYLIB_API void CompileFile(const char* file, int type, const char* header = 0);
		DIYLIB_API void CompileSource(const char* source, int type);
		DIYLIB_API void Link(void);

		DIYLIB_API void Begin(void);

		DIYLIB_API int GetUniformLocation(const char* name);

		DIYLIB_API void Uniform(int type, const char* name, ...);
		DIYLIB_API void Uniform(int type, int uniformLocation, ...);
		DIYLIB_API void Uniform(int type, int uniformLocation, va_list argv);

		DIYLIB_API void Uniform(int type, const char* name, int count, const void* value);
		DIYLIB_API void Uniform(int type, int uniformLocation, int count, const void* value);

		DIYLIB_API int GetAttribLocation(const char* name);

		DIYLIB_API void VertexAttrib(int type, const char* name, ...);
		DIYLIB_API void VertexAttrib(int type, int attribLocation, ...);
		DIYLIB_API void VertexAttrib(int type, int attribLocation, va_list argv);

		DIYLIB_API void VertexAttrib(int type, const char* name, const float* value);
		DIYLIB_API void VertexAttrib(int type, int attribLocation, const float* value);

		DIYLIB_API void EnableVertexAttribArray(const char* name);
		DIYLIB_API void EnableVertexAttribArray(int attribLocation);

		DIYLIB_API void VertexAttribPointer(int type, const char* name, const void* pointer);
		DIYLIB_API void VertexAttribPointer(int type, int attribLocation, const void* pointer);

		DIYLIB_API void DisableVertexAttribArray(const char* name);
		DIYLIB_API void DisableVertexAttribArray(int attribLocation);

		DIYLIB_API void End(void);

		std::string GetShaderInfoLog(int type);
		std::string GetProgramInfoLog(void);

		DIYLIB_API unsigned int GetSubroutineIndex(int type, const char* name);
		DIYLIB_API int GetSubroutineUniformLocation(int type, const char* name);

		DIYLIB_API void UniformSubroutine(int type, unsigned int value);
		DIYLIB_API void UniformSubroutines(int type, int count, const unsigned int* value);

	private:
		int mVertexShader;
		int mTessControlShader;
		int mTessEvaluationShader;
		int mGeometryShader;
		int mFragmentShader;
		int mProgram;
	};

}

#endif
