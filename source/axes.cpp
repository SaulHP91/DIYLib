#include <gl/glew.h>

#include <diy/axes.hpp>
#include <diy/camera.hpp>
#include <diy/shader.hpp>

#include <glm/ext.hpp>

namespace diy
{

	unsigned int Axes::mAxisPositionBuffer = 0;
	unsigned int Axes::mAxisNormalBuffer = 0;
	unsigned int Axes::mAxisFaceBuffer = 0;
	Shader Axes::mXAxisShader;
	Shader Axes::mYAxisShader;
	Shader Axes::mZAxisShader;

	unsigned int Axes::mXPositionBuffer = 0;
	unsigned int Axes::mXFaceBuffer = 0;
	Shader Axes::mXShader;

	unsigned int Axes::mYPositionBuffer = 0;
	unsigned int Axes::mYFaceBuffer = 0;
	Shader Axes::mYShader;

	unsigned int Axes::mZPositionBuffer = 0;
	unsigned int Axes::mZFaceBuffer = 0;
	Shader Axes::mZShader;

	bool Axes::mInitialized = false;

	Axes::Axes(void) :
		mVisible(true),
		mSize(50.0f)
	{
		;
	}

	Axes::~Axes(void)
	{
		;
	}

	void Axes::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	bool Axes::GetVisible(void)
	{
		return mVisible;
	}

	void Axes::SetSize(float size)
	{
		mSize = size;
	}

	float Axes::GetSize(void)
	{
		return mSize;
	}

#include <axis.c>
#include <x_upper.c>
#include <y_upper.c>
#include <z_upper.c>

	void Axes::Render(Camera* camera)
	{
		if (!mVisible)
		{
			return;
		}

		if (!mInitialized)
		{
			Initialize();
		}
		if (!mInitialized)
		{
			return;
		}

		float f = mSize / float(camera->GetViewport()[3]);

		glm::mat4 R = camera->GetInverseBillboardMatrix();

		glm::mat4 modelViewMatrix = glm::translate(glm::vec3(-(camera->GetAspect() / 2.0f - 1.5f * f), -(0.5f - 1.5f * f), 0.0f)) * glm::scale(glm::vec3(f));
		glm::mat4 ftransform = glm::ortho(-camera->GetAspect() / 2.0f, camera->GetAspect() / 2.0f, -0.5f, 0.5f) * modelViewMatrix;

		if (R[0].z < 0.99f)
		{
			mXAxisShader.Begin();
			mXAxisShader.EnableVertexAttribArray("aPosition");
			mXAxisShader.EnableVertexAttribArray("aNormal");

			mXAxisShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(ftransform * R));
			mXAxisShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(modelViewMatrix * R)))));

			glBindBuffer(GL_ARRAY_BUFFER, mAxisPositionBuffer);
			mXAxisShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

			glBindBuffer(GL_ARRAY_BUFFER, mAxisNormalBuffer);
			mXAxisShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mAxisFaceBuffer);
			glDrawElements(GL_TRIANGLES, 3 * axis_faces, GL_UNSIGNED_INT, 0);

			mXAxisShader.DisableVertexAttribArray("aPosition");
			mXAxisShader.DisableVertexAttribArray("aNormal");
			mXAxisShader.End();

			mXShader.Begin();
			mXShader.EnableVertexAttribArray("aPosition");

			mXShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(ftransform * glm::translate(1.25f * glm::vec3(R[0])) * glm::scale(glm::vec3(0.25f))));

			glBindBuffer(GL_ARRAY_BUFFER, mXPositionBuffer);
			mXShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mXFaceBuffer);
			glDrawElements(GL_TRIANGLES, 3 * x_upper_faces, GL_UNSIGNED_INT, 0);

			mXShader.DisableVertexAttribArray("aPosition");
			mXShader.End();
		}
		if (R[1].z < 0.99f)
		{
			mYAxisShader.Begin();
			mYAxisShader.EnableVertexAttribArray("aPosition");
			mYAxisShader.EnableVertexAttribArray("aNormal");

			mYAxisShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(ftransform * R * glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f))));
			mYAxisShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(modelViewMatrix * R * glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)))))));

			glBindBuffer(GL_ARRAY_BUFFER, mAxisPositionBuffer);
			mYAxisShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

			glBindBuffer(GL_ARRAY_BUFFER, mAxisNormalBuffer);
			mYAxisShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mAxisFaceBuffer);
			glDrawElements(GL_TRIANGLES, 3 * axis_faces, GL_UNSIGNED_INT, 0);

			mYAxisShader.DisableVertexAttribArray("aPosition");
			mYAxisShader.DisableVertexAttribArray("aNormal");
			mYAxisShader.End();

			mYShader.Begin();
			mYShader.EnableVertexAttribArray("aPosition");

			mYShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(ftransform * glm::translate(1.25f * glm::vec3(R[1])) * glm::scale(glm::vec3(0.25f))));

			glBindBuffer(GL_ARRAY_BUFFER, mYPositionBuffer);
			mYShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mYFaceBuffer);
			glDrawElements(GL_TRIANGLES, 3 * y_upper_faces, GL_UNSIGNED_INT, 0);

			mYShader.DisableVertexAttribArray("aPosition");
			mYShader.End();
		}
		if (R[2].z < 0.99f)
		{
			mZAxisShader.Begin();
			mZAxisShader.EnableVertexAttribArray("aPosition");
			mZAxisShader.EnableVertexAttribArray("aNormal");

			mZAxisShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(ftransform * R * glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f))));
			mZAxisShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(modelViewMatrix * R * glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)))))));

			glBindBuffer(GL_ARRAY_BUFFER, mAxisPositionBuffer);
			mZAxisShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

			glBindBuffer(GL_ARRAY_BUFFER, mAxisNormalBuffer);
			mZAxisShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mAxisFaceBuffer);
			glDrawElements(GL_TRIANGLES, 3 * axis_faces, GL_UNSIGNED_INT, 0);

			mZAxisShader.DisableVertexAttribArray("aPosition");
			mZAxisShader.DisableVertexAttribArray("aNormal");
			mZAxisShader.End();

			mZShader.Begin();
			mZShader.EnableVertexAttribArray("aPosition");

			mZShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(ftransform * glm::translate(1.25f * glm::vec3(R[2])) * glm::scale(glm::vec3(0.25f))));

			glBindBuffer(GL_ARRAY_BUFFER, mZPositionBuffer);
			mZShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mZFaceBuffer);
			glDrawElements(GL_TRIANGLES, 3 * z_upper_faces, GL_UNSIGNED_INT, 0);

			mZShader.DisableVertexAttribArray("aPosition");
			mZShader.End();
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Axes::Initialize(void)
	{
		glGenBuffers(1, &mAxisPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mAxisPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, axis_vertices * 3 * sizeof(float), axis_v, GL_STATIC_DRAW);

		glGenBuffers(1, &mAxisNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mAxisNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, axis_vertices * 3 * sizeof(float), axis_vn, GL_STATIC_DRAW);

		glGenBuffers(1, &mAxisFaceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mAxisFaceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, axis_faces * 3 * sizeof(unsigned int), axis_f, GL_STATIC_DRAW);

		mXAxisShader.CompileSource("uniform mat4 uFTransform;uniform mat3 uNormalMatrix;attribute vec3 aPosition;attribute vec3 aNormal;varying vec3 vNormal;void main(){vNormal = uNormalMatrix * aNormal;gl_Position = uFTransform * vec4(aPosition, 1.0);}", GL_VERTEX_SHADER);

		mXAxisShader.CompileSource("varying vec3 vNormal;void main(){vec3 normal = normalize(vNormal);vec3 light = vec3(0.0, 0.0, 1.0);float diffuse = max(0.0, dot(normal, light));gl_FragColor = vec4(diffuse * vec3(1.0, 0.0, 0.0), 1.0);}", GL_FRAGMENT_SHADER);

		mXAxisShader.Link();

		mYAxisShader.CompileSource("uniform mat4 uFTransform;uniform mat3 uNormalMatrix;attribute vec3 aPosition;attribute vec3 aNormal;varying vec3 vNormal;void main(){vNormal = uNormalMatrix * aNormal;gl_Position = uFTransform * vec4(aPosition, 1.0);}", GL_VERTEX_SHADER);

		mYAxisShader.CompileSource("varying vec3 vNormal;void main(){vec3 normal = normalize(vNormal);vec3 light = vec3(0.0, 0.0, 1.0);float diffuse = max(0.0, dot(normal, light));gl_FragColor = vec4(diffuse * vec3(0.0, 1.0, 0.0), 1.0);}", GL_FRAGMENT_SHADER);

		mYAxisShader.Link();

		mZAxisShader.CompileSource("uniform mat4 uFTransform;uniform mat3 uNormalMatrix;attribute vec3 aPosition;attribute vec3 aNormal;varying vec3 vNormal;void main(){vNormal = uNormalMatrix * aNormal;gl_Position = uFTransform * vec4(aPosition, 1.0);}", GL_VERTEX_SHADER);

		mZAxisShader.CompileSource("varying vec3 vNormal;void main(){vec3 normal = normalize(vNormal);vec3 light = vec3(0.0, 0.0, 1.0);float diffuse = max(0.0, dot(normal, light));gl_FragColor = vec4(diffuse * vec3(0.0, 0.0, 1.0), 1.0);}", GL_FRAGMENT_SHADER);

		mZAxisShader.Link();

		glGenBuffers(1, &mXPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mXPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, x_upper_vertices * 3 * sizeof(float), x_upper_v, GL_STATIC_DRAW);

		glGenBuffers(1, &mXFaceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mXFaceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, x_upper_faces * 3 * sizeof(unsigned int), x_upper_f, GL_STATIC_DRAW);

		mXShader.CompileSource("uniform mat4 uFTransform;attribute vec3 aPosition;void main(){gl_Position = uFTransform * vec4(aPosition, 1.0);}", GL_VERTEX_SHADER);

		mXShader.CompileSource("void main(){gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);}", GL_FRAGMENT_SHADER);

		mXShader.Link();

		glGenBuffers(1, &mYPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mYPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, y_upper_vertices * 3 * sizeof(float), y_upper_v, GL_STATIC_DRAW);

		glGenBuffers(1, &mYFaceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mYFaceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, y_upper_faces * 3 * sizeof(unsigned int), y_upper_f, GL_STATIC_DRAW);

		mYShader.CompileSource("uniform mat4 uFTransform;attribute vec3 aPosition;void main(){gl_Position = uFTransform * vec4(aPosition, 1.0);}", GL_VERTEX_SHADER);

		mYShader.CompileSource("void main(){gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);}", GL_FRAGMENT_SHADER);

		mYShader.Link();

		glGenBuffers(1, &mZPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mZPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, z_upper_vertices * 3 * sizeof(float), z_upper_v, GL_STATIC_DRAW);

		glGenBuffers(1, &mZFaceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mZFaceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, z_upper_faces * 3 * sizeof(unsigned int), z_upper_f, GL_STATIC_DRAW);

		mZShader.CompileSource("uniform mat4 uFTransform;attribute vec3 aPosition;void main(){gl_Position = uFTransform * vec4(aPosition, 1.0);}", GL_VERTEX_SHADER);

		mZShader.CompileSource("void main(){gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);}", GL_FRAGMENT_SHADER);

		mZShader.Link();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mInitialized = true;
	}

}
