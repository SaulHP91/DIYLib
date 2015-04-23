#include <gl/glew.h>

#include <diy/shader.hpp>

#include <fstream>
#include <vector>

namespace diy
{

	Shader::Shader(void) :
		mVertexShader(0),
		mTessControlShader(0),
		mTessEvaluationShader(0),
		mGeometryShader(0),
		mFragmentShader(0),
		mProgram(0)
	{
		;
	}

	Shader::~Shader(void)
	{
		Clear();
	}

	int Shader::GetShader(int type)
	{
		switch (type)
		{
		case GL_VERTEX_SHADER:
		{
			return mVertexShader;
			break;
		}
		case GL_TESS_CONTROL_SHADER:
		{
			return mTessControlShader;
			break;
		}
		case GL_TESS_EVALUATION_SHADER:
		{
			return mTessEvaluationShader;
			break;
		}
		case GL_GEOMETRY_SHADER:
		{
			return mGeometryShader;
			break;
		}
		case GL_FRAGMENT_SHADER:
		{
			return mFragmentShader;
			break;
		}
		default:
		{
			return 0;
		}
		}
	}

	int Shader::GetProgram(void)
	{
		return mProgram;
	}

	void Shader::Clear()
	{
		if (!glDetachShader || !glDeleteProgram || !glDeleteShader)
		{
			return;
		}

		if (mProgram)
		{
			if (mVertexShader)
			{
				glDetachShader(mProgram, mVertexShader);
			}
			if (mTessControlShader)
			{
				glDetachShader(mProgram, mTessControlShader);
			}
			if (mTessEvaluationShader)
			{
				glDetachShader(mProgram, mTessEvaluationShader);
			}
			if (mGeometryShader)
			{
				glDetachShader(mProgram, mGeometryShader);
			}
			if (mFragmentShader)
			{
				glDetachShader(mProgram, mFragmentShader);
			}
			glDeleteProgram(mProgram);
			mProgram = 0;
		}
		if (mVertexShader)
		{
			glDeleteShader(mVertexShader);
			mVertexShader = 0;
		}
		if (mTessControlShader)
		{
			glDeleteShader(mTessControlShader);
			mTessControlShader = 0;
		}
		if (mTessEvaluationShader)
		{
			glDeleteShader(mTessEvaluationShader);
			mTessEvaluationShader = 0;
		}
		if (mGeometryShader)
		{
			glDeleteShader(mGeometryShader);
			mGeometryShader = 0;
		}
		if (mFragmentShader)
		{
			glDeleteShader(mFragmentShader);
			mFragmentShader = 0;
		}
	}

	void Shader::CompileFile(const char* file, int type, const char* header)
	{
		std::ifstream in(file, std::ios::in | std::ios::binary);
		std::streampos start = in.tellg();
		in.seekg(0, std::ifstream::end);
		int size = in.tellg();
		char* source = 0;
		int headerLength = 0;
		if (header)
		{
			headerLength = strlen(header);
			source = new char[headerLength + 2 + size + 1];
			strcpy(source, header);
			source[headerLength] = '\r';
			source[headerLength + 1] = '\n';
			headerLength += 2;
		}
		else
		{
			source = new char[size + 1];
		}
		in.clear();
		in.seekg(start);
		in.read(source + headerLength, size);
		in.close();
		source[size + headerLength] = 0;

		switch (type)
		{
		case GL_VERTEX_SHADER:
		{
			mVertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(mVertexShader, 1, const_cast<const char**>(&source), 0);
			glCompileShader(mVertexShader);
			break;
		}
		case GL_TESS_CONTROL_SHADER:
		{
			mTessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(mTessControlShader, 1, const_cast<const char**>(&source), 0);
			glCompileShader(mTessControlShader);
			break;
		}
		case GL_TESS_EVALUATION_SHADER:
		{
			mTessEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(mTessEvaluationShader, 1, const_cast<const char**>(&source), 0);
			glCompileShader(mTessEvaluationShader);
			break;
		}
		case GL_GEOMETRY_SHADER:
		{
			mGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(mGeometryShader, 1, const_cast<const char**>(&source), 0);
			glCompileShader(mGeometryShader);
			break;
		}
		case GL_FRAGMENT_SHADER:
		{
			mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(mFragmentShader, 1, const_cast<const char**>(&source), 0);
			glCompileShader(mFragmentShader);
			break;
		}
		default:
		{
			;
		}
		}

		delete[] source;
	}

	void Shader::CompileSource(const char* source, int type)
	{
		switch (type)
		{
		case GL_VERTEX_SHADER:
		{
			mVertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(mVertexShader, 1, const_cast<const char**>(&source), 0);
			glCompileShader(mVertexShader);
			break;
		}
		case GL_TESS_CONTROL_SHADER:
		{
			mTessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(mTessControlShader, 1, const_cast<const char**>(&source), 0);
			glCompileShader(mTessControlShader);
			break;
		}
		case GL_TESS_EVALUATION_SHADER:
		{
			mTessEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(mTessEvaluationShader, 1, const_cast<const char**>(&source), 0);
			glCompileShader(mTessEvaluationShader);
			break;
		}
		case GL_GEOMETRY_SHADER:
		{
			mGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(mGeometryShader, 1, const_cast<const char**>(&source), 0);
			glCompileShader(mGeometryShader);
			break;
		}
		case GL_FRAGMENT_SHADER:
		{
			mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(mFragmentShader, 1, const_cast<const char**>(&source), 0);
			glCompileShader(mFragmentShader);
			break;
		}
		default:
		{
			;
		}
		}
	}

	void Shader::Link(void)
	{
		mProgram = glCreateProgram();

		if (mVertexShader)
		{
			glAttachShader(mProgram, mVertexShader);
		}
		if (mTessControlShader)
		{
			glAttachShader(mProgram, mTessControlShader);
		}
		if (mTessEvaluationShader)
		{
			glAttachShader(mProgram, mTessEvaluationShader);
		}
		if (mGeometryShader)
		{
			glAttachShader(mProgram, mGeometryShader);
		}
		if (mFragmentShader)
		{
			glAttachShader(mProgram, mFragmentShader);
		}

		glLinkProgram(mProgram);
	}

	void Shader::Begin(void)
	{
		glUseProgram(mProgram);
	}

	int Shader::GetUniformLocation(const char* name)
	{
		return glGetUniformLocation(mProgram, name);
	}

	void Shader::Uniform(int type, const char* name, ...)
	{
		va_list argv;
		va_start(argv, name);

		Uniform(type, glGetUniformLocation(mProgram, name), argv);

		va_end(argv);
	}

	void Shader::Uniform(int type, int uniformLocation, ...)
	{
		va_list argv;
		va_start(argv, uniformLocation);

		Uniform(type, uniformLocation, argv);

		va_end(argv);
	}

	void Shader::Uniform(int type, int uniformLocation, va_list argv)
	{
		switch (type)
		{
		case GL_BOOL:
		case GL_INT:
		case GL_SAMPLER_1D:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_3D:
		{
			int v0 = va_arg(argv, int);
			glUniform1i(uniformLocation, v0);
			break;
		}
		case GL_INT_VEC2:
		{
			int v0 = va_arg(argv, int);
			int v1 = va_arg(argv, int);
			glUniform2i(uniformLocation, v0, v1);
			break;
		}
		case GL_INT_VEC3:
		{
			int v0 = va_arg(argv, int);
			int v1 = va_arg(argv, int);
			int v2 = va_arg(argv, int);
			glUniform3i(uniformLocation, v0, v1, v2);
			break;
		}
		case GL_INT_VEC4:
		{
			int v0 = va_arg(argv, int);
			int v1 = va_arg(argv, int);
			int v2 = va_arg(argv, int);
			int v3 = va_arg(argv, int);
			glUniform4i(uniformLocation, v0, v1, v2, v3);
			break;
		}
		case GL_FLOAT:
		{
			float v0 = va_arg(argv, double);
			glUniform1f(uniformLocation, v0);
			break;
		}
		case GL_FLOAT_VEC2:
		{
			float v0 = va_arg(argv, double);
			float v1 = va_arg(argv, double);
			glUniform2f(uniformLocation, v0, v1);
			break;
		}
		case GL_FLOAT_VEC3:
		{
			float v0 = va_arg(argv, double);
			float v1 = va_arg(argv, double);
			float v2 = va_arg(argv, double);
			glUniform3f(uniformLocation, v0, v1, v2);
			break;
		}
		case GL_FLOAT_VEC4:
		{
			float v0 = va_arg(argv, double);
			float v1 = va_arg(argv, double);
			float v2 = va_arg(argv, double);
			float v3 = va_arg(argv, double);
			glUniform4f(uniformLocation, v0, v1, v2, v3);
			break;
		}
		default:
		{
			;
		}
		}
	}

	void Shader::Uniform(int type, const char* name, int count, const void* value)
	{
		Uniform(type, glGetUniformLocation(mProgram, name), count, value);
	}

	void Shader::Uniform(int type, int uniformLocation, int count, const void* value)
	{
		switch (type)
		{
		case GL_INT:
		case GL_SAMPLER_1D:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_3D:
		{
			glUniform1iv(uniformLocation, count, reinterpret_cast<const int*>(value));
			break;
		}
		case GL_INT_VEC2:
		{
			glUniform2iv(uniformLocation, count, reinterpret_cast<const int*>(value));
			break;
		}
		case GL_INT_VEC3:
		{
			glUniform3iv(uniformLocation, count, reinterpret_cast<const int*>(value));
			break;
		}
		case GL_INT_VEC4:
		{
			glUniform4iv(uniformLocation, count, reinterpret_cast<const int*>(value));
			break;
		}
		case GL_FLOAT:
		{
			glUniform1fv(uniformLocation, count, reinterpret_cast<const float*>(value));
			break;
		}
		case GL_FLOAT_VEC2:
		{
			glUniform2fv(uniformLocation, count, reinterpret_cast<const float*>(value));
			break;
		}
		case GL_FLOAT_VEC3:
		{
			glUniform3fv(uniformLocation, count, reinterpret_cast<const float*>(value));
			break;
		}
		case GL_FLOAT_VEC4:
		{
			glUniform4fv(uniformLocation, count, reinterpret_cast<const float*>(value));
			break;
		}
		case GL_FLOAT_MAT2:
		{
			glUniformMatrix2fv(uniformLocation, count, 0, reinterpret_cast<const float*>(value));
			break;
		}
		case GL_FLOAT_MAT3:
		{
			glUniformMatrix3fv(uniformLocation, count, 0, reinterpret_cast<const float*>(value));
			break;
		}
		case GL_FLOAT_MAT4:
		{
			glUniformMatrix4fv(uniformLocation, count, 0, reinterpret_cast<const float*>(value));
			break;
		}
		default:
		{
			;
		}
		}
	}

	int Shader::GetAttribLocation(const char* name)
	{
		return glGetAttribLocation(mProgram, name);
	}

	void Shader::VertexAttrib(int type, const char* name, ...)
	{
		va_list argv;
		va_start(argv, name);

		VertexAttrib(type, glGetAttribLocation(mProgram, name), argv);

		va_end(argv);
	}

	void Shader::VertexAttrib(int type, int attribLocation, ...)
	{
		va_list argv;
		va_start(argv, attribLocation);

		VertexAttrib(type, attribLocation, argv);

		va_end(argv);
	}

	void Shader::VertexAttrib(int type, int attribLocation, va_list argv)
	{
		switch (type)
		{
		case GL_FLOAT:
		{
			float v0 = va_arg(argv, double);
			glVertexAttrib1f(attribLocation, v0);
			break;
		}
		case GL_FLOAT_VEC2:
		{
			float v0 = va_arg(argv, double);
			float v1 = va_arg(argv, double);
			glVertexAttrib2f(attribLocation, v0, v1);
			break;
		}
		case GL_FLOAT_VEC3:
		{
			float v0 = va_arg(argv, double);
			float v1 = va_arg(argv, double);
			float v2 = va_arg(argv, double);
			glVertexAttrib3f(attribLocation, v0, v1, v2);
			break;
		}
		case GL_FLOAT_VEC4:
		{
			float v0 = va_arg(argv, double);
			float v1 = va_arg(argv, double);
			float v2 = va_arg(argv, double);
			float v3 = va_arg(argv, double);
			glVertexAttrib4f(attribLocation, v0, v1, v2, v3);
			break;
		}
		default:
		{
			;
		}
		}
	}

	void Shader::VertexAttrib(int type, const char* name, const float* value)
	{
		VertexAttrib(type, glGetAttribLocation(mProgram, name), value);
	}

	void Shader::VertexAttrib(int type, int attribLocation, const float* value)
	{
		switch (type)
		{
		case GL_FLOAT:
		{
			glVertexAttrib1fv(attribLocation, value);
			break;
		}
		case GL_FLOAT_VEC2:
		{
			glVertexAttrib2fv(attribLocation, value);
			break;
		}
		case GL_FLOAT_VEC3:
		{
			glVertexAttrib3fv(attribLocation, value);
			break;
		}
		case GL_FLOAT_VEC4:
		{
			glVertexAttrib4fv(attribLocation, value);
			break;
		}
		default:
		{
			;
		}
		}
	}

	void Shader::EnableVertexAttribArray(const char* name)
	{
		EnableVertexAttribArray(glGetAttribLocation(mProgram, name));
	}

	void Shader::EnableVertexAttribArray(int attribLocation)
	{
		glEnableVertexAttribArray(attribLocation);
	}

	void Shader::VertexAttribPointer(int type, const char* name, const void* pointer)
	{
		VertexAttribPointer(type, glGetAttribLocation(mProgram, name), pointer);
	}

	void Shader::VertexAttribPointer(int type, int attribLocation, const void* pointer)
	{
		switch (type)
		{
		case GL_FLOAT:
		{
			glVertexAttribPointer(attribLocation, 1, GL_FLOAT, 0, 0, pointer);
			break;
		}
		case GL_FLOAT_VEC2:
		{
			glVertexAttribPointer(attribLocation, 2, GL_FLOAT, 0, 0, pointer);
			break;
		}
		case GL_FLOAT_VEC3:
		{
			glVertexAttribPointer(attribLocation, 3, GL_FLOAT, 0, 0, pointer);
			break;
		}
		case GL_FLOAT_VEC4:
		{
			glVertexAttribPointer(attribLocation, 4, GL_FLOAT, 0, 0, pointer);
			break;
		}
		default:
		{
			;
		}
		}
	}

	void Shader::DisableVertexAttribArray(const char* name)
	{
		DisableVertexAttribArray(glGetAttribLocation(mProgram, name));
	}

	void Shader::DisableVertexAttribArray(int attribLocation)
	{
		glDisableVertexAttribArray(attribLocation);
	}

	void Shader::End(void)
	{
		glUseProgram(0);
	}

	const char* Shader::GetShaderInfoLog(int type)
	{
		int shader;
		switch (type)
		{
		case GL_VERTEX_SHADER:
		{
			shader = mVertexShader;
			break;
		}
		case GL_TESS_CONTROL_SHADER:
		{
			shader = mTessControlShader;
			break;
		}
		case GL_TESS_EVALUATION_SHADER:
		{
			shader = mTessEvaluationShader;
			break;
		}
		case GL_GEOMETRY_SHADER:
		{
			shader = mGeometryShader;
			break;
		}
		case GL_FRAGMENT_SHADER:
		{
			shader = mFragmentShader;
			break;
		}
		default:
		{
			return "";
		}
		}

		static char buffer[256];

		int infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			int length;
			glGetShaderInfoLog(shader, 255, &length, buffer);
			buffer[length] = 0;
			return buffer;
		}

		return "";
	}

	const char* Shader::GetProgramInfoLog(void)
	{
		static char buffer[256];

		int infoLogLength;
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			int length;
			glGetProgramInfoLog(mProgram, 255, &length, buffer);
			buffer[length] = 0;
			return buffer;
		}

		return "";
	}

	unsigned int Shader::GetSubroutineIndex(int type, const char* name)
	{
		return glGetSubroutineIndex(mProgram, type, name);
	}

	int Shader::GetSubroutineUniformLocation(int type, const char* name)
	{
		return glGetSubroutineUniformLocation(mProgram, type, name);
	}

	void Shader::UniformSubroutine(int type, unsigned int value)
	{
		glUniformSubroutinesuiv(type, 1, &value);
	}

	void Shader::UniformSubroutines(int type, int count, const unsigned int* value)
	{
		glUniformSubroutinesuiv(type, count, value);
	}

	NamedShaderArgument Shader::operator[](const char* name)
	{
		return NamedShaderArgument(mProgram, name);
	}

	IndexedShaderArgument Shader::operator[](int location)
	{
		return IndexedShaderArgument(mProgram, location);
	}

}
