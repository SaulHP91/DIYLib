#include <diy/manipulator.hpp>
#include <diy/camera.hpp>
#include <diy/selector.hpp>
#include <diy/selectableMesh.hpp>
#include <diy/matrixStack.hpp>
#include <diy/shader.hpp>

#include <glm/own.hpp>

#include <gl/glew.h>

namespace diy
{

#include <arrow.c>
#include <gimbal.c>
#include <scaler.c>
#include <plane.c>
#include <pivot.c>

	unsigned int Manipulator::mArrowPositionBuffer = 0;
	unsigned int Manipulator::mArrowNormalBuffer = 0;
	unsigned int Manipulator::mArrowFaceBuffer = 0;

	unsigned int Manipulator::mGimbalPositionBuffer = 0;
	unsigned int Manipulator::mGimbalNormalBuffer = 0;
	unsigned int Manipulator::mGimbalFaceBuffer = 0;

	unsigned int Manipulator::mScalerPositionBuffer = 0;
	unsigned int Manipulator::mScalerNormalBuffer = 0;
	unsigned int Manipulator::mScalerFaceBuffer = 0;

	unsigned int Manipulator::mPlanePositionBuffer = 0;
	unsigned int Manipulator::mPlaneNormalBuffer = 0;
	unsigned int Manipulator::mPlaneFaceBuffer = 0;

	unsigned int Manipulator::mPivotPositionBuffer = 0;
	unsigned int Manipulator::mPivotNormalBuffer = 0;
	unsigned int Manipulator::mPivotFaceBuffer = 0;

	bool Manipulator::mInitializedBuffersAndShaders = false;

	Shader Manipulator::mShader;

	Manipulator::Manipulator(void) :
		mCamera(0),
		mParentTranslation(0.0f),
		mParentRotation(1.0f, 0.0f, 0.0f, 0.0f),
		mParentScaling(1.0f),
		mParentMatrix(1.0f),
		mTranslation(0.0f),
		mRotation(1.0f, 0.0f, 0.0f, 0.0f),
		mScaling(1.0f),
		mMatrix(1.0f),
		mFinalMatrix(1.0f),
		mFinalTranslation(0.0f),
		mFinalRotation(1.0f, 0.0f, 0.0f, 0.0f),
		mFinalScaling(1.0f),
		mVisible(true),
		mSize(100.0f),
		mFocused(None),
		mSelected(None),
		mActing(false),
		mSpace(Object),
		mStyle(Translation),
		mSnapTranslation(false),
		mTranslationStep(1.0f),
		mSnapRotation(false),
		mRotationStep(1.0f),
		mData(0),
		mOnChange(0),
		mDT(0.0f),
		mDAngle(0.0f),
		mXArrowName(XArrow),
		mYArrowName(YArrow),
		mZArrowName(ZArrow),
		mXGimbalName(XGimbal),
		mYGimbalName(YGimbal),
		mZGimbalName(ZGimbal),
		mXScalerName(XScaler),
		mYScalerName(YScaler),
		mZScalerName(ZScaler),
		mXYPlaneName(XYPlane),
		mYZPlaneName(YZPlane),
		mZXPlaneName(ZXPlane),
		mOriginPivotName(OriginPivot)
	{
		mSelector = new Selector();

		mSelectableXArrow = mSelector->NewSelectable<SelectableMesh>();
		mSelectableXArrow->SetVertexCount(arrow_vertices);
		mSelectableXArrow->SetVerticesPointer(arrow_v);
		mSelectableXArrow->SetFaceCount(arrow_faces);
		mSelectableXArrow->SetFacesPointer(arrow_f);
		mSelectableXArrow->SetData(&mXArrowName);

		mSelectableYArrow = mSelector->NewSelectable<SelectableMesh>();
		mSelectableYArrow->SetVertexCount(arrow_vertices);
		mSelectableYArrow->SetVerticesPointer(arrow_v);
		mSelectableYArrow->SetFaceCount(arrow_faces);
		mSelectableYArrow->SetFacesPointer(arrow_f);
		mSelectableYArrow->SetData(&mYArrowName);

		mSelectableZArrow = mSelector->NewSelectable<SelectableMesh>();
		mSelectableZArrow->SetVertexCount(arrow_vertices);
		mSelectableZArrow->SetVerticesPointer(arrow_v);
		mSelectableZArrow->SetFaceCount(arrow_faces);
		mSelectableZArrow->SetFacesPointer(arrow_f);
		mSelectableZArrow->SetData(&mZArrowName);

		mSelectableXGimbal = mSelector->NewSelectable<SelectableMesh>();
		mSelectableXGimbal->SetVertexCount(gimbal_vertices);
		mSelectableXGimbal->SetVerticesPointer(gimbal_v);
		mSelectableXGimbal->SetFaceCount(gimbal_faces);
		mSelectableXGimbal->SetFacesPointer(gimbal_f);
		mSelectableXGimbal->SetData(&mXGimbalName);

		mSelectableYGimbal = mSelector->NewSelectable<SelectableMesh>();
		mSelectableYGimbal->SetVertexCount(gimbal_vertices);
		mSelectableYGimbal->SetVerticesPointer(gimbal_v);
		mSelectableYGimbal->SetFaceCount(gimbal_faces);
		mSelectableYGimbal->SetFacesPointer(gimbal_f);
		mSelectableYGimbal->SetData(&mYGimbalName);

		mSelectableZGimbal = mSelector->NewSelectable<SelectableMesh>();
		mSelectableZGimbal->SetVertexCount(gimbal_vertices);
		mSelectableZGimbal->SetVerticesPointer(gimbal_v);
		mSelectableZGimbal->SetFaceCount(gimbal_faces);
		mSelectableZGimbal->SetFacesPointer(gimbal_f);
		mSelectableZGimbal->SetData(&mZGimbalName);

		mSelectableXScaler = mSelector->NewSelectable<SelectableMesh>();
		mSelectableXScaler->SetVertexCount(scaler_vertices);
		mSelectableXScaler->SetVerticesPointer(scaler_v);
		mSelectableXScaler->SetFaceCount(scaler_faces);
		mSelectableXScaler->SetFacesPointer(scaler_f);
		mSelectableXScaler->SetData(&mXScalerName);

		mSelectableYScaler = mSelector->NewSelectable<SelectableMesh>();
		mSelectableYScaler->SetVertexCount(scaler_vertices);
		mSelectableYScaler->SetVerticesPointer(scaler_v);
		mSelectableYScaler->SetFaceCount(scaler_faces);
		mSelectableYScaler->SetFacesPointer(scaler_f);
		mSelectableYScaler->SetData(&mYScalerName);

		mSelectableZScaler = mSelector->NewSelectable<SelectableMesh>();
		mSelectableZScaler->SetVertexCount(scaler_vertices);
		mSelectableZScaler->SetVerticesPointer(scaler_v);
		mSelectableZScaler->SetFaceCount(scaler_faces);
		mSelectableZScaler->SetFacesPointer(scaler_f);
		mSelectableZScaler->SetData(&mZScalerName);

		mSelectableXYPlane = mSelector->NewSelectable<SelectableMesh>();
		mSelectableXYPlane->SetVertexCount(plane_vertices);
		mSelectableXYPlane->SetVerticesPointer(plane_v);
		mSelectableXYPlane->SetFaceCount(plane_faces);
		mSelectableXYPlane->SetFacesPointer(plane_f);
		mSelectableXYPlane->SetData(&mXYPlaneName);

		mSelectableYZPlane = mSelector->NewSelectable<SelectableMesh>();
		mSelectableYZPlane->SetVertexCount(plane_vertices);
		mSelectableYZPlane->SetVerticesPointer(plane_v);
		mSelectableYZPlane->SetFaceCount(plane_faces);
		mSelectableYZPlane->SetFacesPointer(plane_f);
		mSelectableYZPlane->SetData(&mYZPlaneName);

		mSelectableZXPlane = mSelector->NewSelectable<SelectableMesh>();
		mSelectableZXPlane->SetVertexCount(plane_vertices);
		mSelectableZXPlane->SetVerticesPointer(plane_v);
		mSelectableZXPlane->SetFaceCount(plane_faces);
		mSelectableZXPlane->SetFacesPointer(plane_f);
		mSelectableZXPlane->SetData(&mZXPlaneName);

		mSelectableOriginPivot = mSelector->NewSelectable<SelectableMesh>();
		mSelectableOriginPivot->SetVertexCount(pivot_vertices);
		mSelectableOriginPivot->SetVerticesPointer(pivot_v);
		mSelectableOriginPivot->SetFaceCount(pivot_faces);
		mSelectableOriginPivot->SetFacesPointer(pivot_f);
		mSelectableOriginPivot->SetData(&mOriginPivotName);

		mMatrixStack = new MatrixStack();
	}

	Manipulator::~Manipulator(void)
	{
		mVisible = false;

		delete mSelector;

		delete mMatrixStack;
	}

	void Manipulator::SetParentMatrix(glm::mat4 parentMatrix)
	{
		mParentMatrix = parentMatrix;

		mParentScaling = glm::vec3(glm::length(glm::vec3(mParentMatrix[0])), glm::length(glm::vec3(mParentMatrix[1])), glm::length(glm::vec3(mParentMatrix[2])));
		mParentRotation = glm::toQuat(glm::mat3(glm::normalize(glm::vec3(mParentMatrix[0])), glm::normalize(glm::vec3(mParentMatrix[1])), glm::normalize(glm::vec3(mParentMatrix[2]))));
		mParentTranslation = glm::vec3(mParentMatrix[3]);

		Update();
	}

	glm::mat4 Manipulator::GetParentMatrix(void)
	{
		return mParentMatrix;
	}

	void Manipulator::ApplyParentMatrix(void)
	{
		glMultMatrixf(glm::value_ptr(mParentMatrix));
	}

	void Manipulator::SetMatrix(glm::mat4 matrix)
	{
		mScaling = glm::vec3(glm::length(glm::vec3(matrix[0])), glm::length(glm::vec3(matrix[1])), glm::length(glm::vec3(matrix[2])));
		mRotation = glm::toQuat(glm::mat3(glm::normalize(glm::vec3(matrix[0])), glm::normalize(glm::vec3(matrix[1])), glm::normalize(glm::vec3(matrix[2]))));
		mTranslation = glm::vec3(matrix[3]);

		Update();
	}

	glm::mat4 Manipulator::GetMatrix(void)
	{
		return mMatrix;
	}

	void Manipulator::ApplyMatrix(void)
	{
		glMultMatrixf(glm::value_ptr(mMatrix));
	}

	glm::mat4 Manipulator::GetFinalMatrix(void)
	{
		return mFinalMatrix;
	}

	void Manipulator::ApplyFinalMatrix(void)
	{
		glMultMatrixf(glm::value_ptr(mFinalMatrix));
	}

	void Manipulator::RenderXArrow(void)
	{
		if (mSpace != Eye)
		{
			glm::vec3 a;
			if (mSpace == Object)
			{
				a = glm::vec3(mCamera->GetFinalMatrix() * mFinalMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
			}
			else
			{
				a = glm::vec3(mCamera->GetFinalMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
			}
			if (abs(glm::normalize(a).z) > 0.99f)
			{
				return;
			}
		}

		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == XArrow)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mArrowPositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mArrowNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mArrowFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * arrow_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();
	}

	void Manipulator::RenderYArrow(void)
	{
		if (mSpace != Eye)
		{
			glm::vec3 a;
			if (mSpace == Object)
			{
				a = glm::vec3(mCamera->GetFinalMatrix() * mFinalMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
			}
			else
			{
				a = glm::vec3(mCamera->GetFinalMatrix() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
			}
			if (abs(glm::normalize(a).z) > 0.99f)
			{
				return;
			}
		}

		mMatrixStack->Push();
		mMatrixStack->Top() *= glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0, 1.0f));

		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == YArrow)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mArrowPositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mArrowNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mArrowFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * arrow_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();

		mMatrixStack->Pop();
	}

	void Manipulator::RenderZArrow(void)
	{
		if (mSpace != Eye)
		{
			glm::vec3 a;
			if (mSpace == Object)
			{
				a = glm::vec3(mCamera->GetFinalMatrix() * mFinalMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
			}
			else
			{
				a = glm::vec3(mCamera->GetFinalMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
			}
			if (abs(glm::normalize(a).z) > 0.99f)
			{
				return;
			}
		}

		mMatrixStack->Push();
		mMatrixStack->Top() *= glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 1.0, 0.0f));

		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == ZArrow)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mArrowPositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mArrowNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mArrowFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * arrow_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();

		mMatrixStack->Pop();
	}

	void Manipulator::RenderXGimbal(void)
	{
		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == XGimbal)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mGimbalPositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mGimbalNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGimbalFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * gimbal_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();
	}

	void Manipulator::RenderYGimbal(void)
	{
		mMatrixStack->Push();
		mMatrixStack->Top() *= glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0, 1.0f));

		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == YGimbal)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mGimbalPositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mGimbalNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGimbalFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * gimbal_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();

		mMatrixStack->Pop();
	}

	void Manipulator::RenderZGimbal(void)
	{
		mMatrixStack->Push();
		mMatrixStack->Top() *= glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 1.0, 0.0f));

		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == ZGimbal)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mGimbalPositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mGimbalNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGimbalFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * gimbal_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();

		mMatrixStack->Pop();
	}

	void Manipulator::RenderXScaler(void)
	{
		glm::vec3 a = glm::vec3(mCamera->GetFinalMatrix() * glm::toMat4(mRotation) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		if (abs(glm::normalize(a).z) > 0.99f)
		{
			return;
		}

		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == XScaler)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mScalerPositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mScalerNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mScalerFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * scaler_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();
	}

	void Manipulator::RenderYScaler(void)
	{
		glm::vec3 a = glm::vec3(mCamera->GetFinalMatrix() * glm::toMat4(mRotation) * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		if (abs(glm::normalize(a).z) > 0.99f)
		{
			return;
		}

		mMatrixStack->Push();
		mMatrixStack->Top() *= glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0, 1.0f));

		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == YScaler)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mScalerPositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mScalerNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mScalerFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * scaler_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();

		mMatrixStack->Pop();
	}

	void Manipulator::RenderZScaler(void)
	{
		glm::vec3 a = glm::vec3(mCamera->GetFinalMatrix() * glm::toMat4(mRotation) * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
		if (abs(glm::normalize(a).z) > 0.99f)
		{
			return;
		}

		mMatrixStack->Push();
		mMatrixStack->Top() *= glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 1.0, 0.0f));

		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == ZScaler)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mScalerPositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mScalerNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mScalerFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * scaler_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();

		mMatrixStack->Pop();
	}

	void Manipulator::RenderXYPlane(void)
	{
		if (mSpace != Eye)
		{
			glm::vec3 a;
			if (mSpace == Object)
			{
				a = glm::vec3(mCamera->GetFinalMatrix() * mFinalMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
			}
			else
			{
				a = glm::vec3(mCamera->GetFinalMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
			}
			if (abs(glm::normalize(a).z) < 0.1f)
			{
				return;
			}
		}

		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == XYPlane)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mPlanePositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mPlaneNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPlaneFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * plane_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();
	}

	void Manipulator::RenderYZPlane(void)
	{
		if (mSpace != Eye)
		{
			glm::vec3 a;
			if (mSpace == Object)
			{
				a = glm::vec3(mCamera->GetFinalMatrix() * mFinalMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
			}
			else
			{
				a = glm::vec3(mCamera->GetFinalMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
			}
			if (abs(glm::normalize(a).z) < 0.1f)
			{
				return;
			}
		}

		mMatrixStack->Push();
		mMatrixStack->Top() *= glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 1.0, 0.0f));

		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == YZPlane)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mPlanePositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mPlaneNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPlaneFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * plane_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();

		mMatrixStack->Pop();
	}

	void Manipulator::RenderZXPlane(void)
	{
		if (mSpace != Eye)
		{
			glm::vec3 a;
			if (mSpace == Object)
			{
				a = glm::vec3(mCamera->GetFinalMatrix() * mFinalMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
			}
			else
			{
				a = glm::vec3(mCamera->GetFinalMatrix() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
			}
			if (abs(glm::normalize(a).z) < 0.1f)
			{
				return;
			}
		}

		mMatrixStack->Push();
		mMatrixStack->Top() *= glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0, 0.0f));

		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == ZXPlane)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mPlanePositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mPlaneNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPlaneFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * plane_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();

		mMatrixStack->Pop();
	}

	void Manipulator::RenderOriginPivot(void)
	{
		mShader.Begin();

		mShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mCamera->GetProjectionMatrix() * mMatrixStack->Top()));
		mShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(mMatrixStack->Top())))));
		if (mFocused == OriginPivot)
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		else
		{
			mShader.Uniform(GL_FLOAT_VEC3, "uColor", 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));
		}

		mShader.EnableVertexAttribArray("aPosition");
		mShader.EnableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, mPivotPositionBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mPivotNormalBuffer);
		mShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPivotFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * pivot_faces, GL_UNSIGNED_INT, 0);

		mShader.DisableVertexAttribArray("aPosition");
		mShader.DisableVertexAttribArray("aNormal");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mShader.End();
	}

	Manipulator::SelectionName Manipulator::Pick(int x, int y)
	{
		mMatrixStack->Top() = glm::translate(mFinalTranslation);

		glm::vec3 _x;
		glm::vec3 _y;
		glm::vec3 _z;

		if (mSpace == Object)
		{
			_x = glm::normalize(glm::vec3(mCamera->GetFinalMatrix() * mFinalMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
			_y = glm::normalize(glm::vec3(mCamera->GetFinalMatrix() * mFinalMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
			_z = glm::normalize(glm::vec3(mCamera->GetFinalMatrix() * mFinalMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
		}
		else
		{
			_x = glm::normalize(glm::vec3(mCamera->GetFinalMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
			_y = glm::normalize(glm::vec3(mCamera->GetFinalMatrix() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
			_z = glm::normalize(glm::vec3(mCamera->GetFinalMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
		}

		if (mStyle != Scaling)
		{
			if (mSpace == Object)
			{
				mMatrixStack->Top() *= glm::toMat4(mFinalRotation);
			}
			else if (mSpace == Eye)
			{
				mMatrixStack->Top() *= mCamera->GetBillboardMatrix();
			}
		}
		else
		{
			mMatrixStack->Top() *= glm::toMat4(mFinalRotation);
		}

		float f = mCamera->GetScaleFactor(mFinalTranslation, mSize);
		mMatrixStack->Top() *= glm::scale(glm::vec3(f));

		mSelectableXArrow->SetEnabled(false);
		mSelectableYArrow->SetEnabled(false);
		mSelectableZArrow->SetEnabled(false);
		mSelectableXGimbal->SetEnabled(false);
		mSelectableYGimbal->SetEnabled(false);
		mSelectableZGimbal->SetEnabled(false);
		mSelectableXScaler->SetEnabled(false);
		mSelectableYScaler->SetEnabled(false);
		mSelectableZScaler->SetEnabled(false);
		mSelectableXYPlane->SetEnabled(false);
		mSelectableYZPlane->SetEnabled(false);
		mSelectableZXPlane->SetEnabled(false);
		mSelectableOriginPivot->SetEnabled(false);

		switch (mStyle)
		{
		case Translation:
		{
			if (_x.z < 0.99)
			{
				mSelectableXArrow->SetEnabled(true);
				mSelectableXArrow->SetMatrix(mMatrixStack->Top());
			}

			if (_y.z < 0.99)
			{
				mMatrixStack->Push();
				mMatrixStack->Top() *= glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				mSelectableYArrow->SetEnabled(true);
				mSelectableYArrow->SetMatrix(mMatrixStack->Top());
				mMatrixStack->Pop();
			}

			if (mSpace != Eye)
			{
				if (_z.z < 0.99)
				{
					mMatrixStack->Push();
					mMatrixStack->Top() *= glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					mSelectableZArrow->SetEnabled(true);
					mSelectableZArrow->SetMatrix(mMatrixStack->Top());
					mMatrixStack->Pop();
				}

				if (abs(_z.z) > 0.1)
				{
					mSelectableXYPlane->SetEnabled(true);
					mSelectableXYPlane->SetMatrix(mMatrixStack->Top());
				}

				if (abs(_x.z) > 0.1)
				{
					mMatrixStack->Push();
					mMatrixStack->Top() *= glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					mSelectableYZPlane->SetEnabled(true);
					mSelectableYZPlane->SetMatrix(mMatrixStack->Top());
					mMatrixStack->Pop();
				}

				if (abs(_y.z) > 0.1)
				{
					mMatrixStack->Push();
					mMatrixStack->Top() *= glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					mSelectableZXPlane->SetEnabled(true);
					mSelectableZXPlane->SetMatrix(mMatrixStack->Top());
					mMatrixStack->Pop();
				}
			}

			mSelectableOriginPivot->SetEnabled(true);
			mSelectableOriginPivot->SetMatrix(mMatrixStack->Top());
		}
		break;
		case Rotation:
		{
			mSelectableXGimbal->SetEnabled(true);
			mSelectableXGimbal->SetMatrix(mMatrixStack->Top());

			mMatrixStack->Push();
			mMatrixStack->Top() *= glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			mSelectableYGimbal->SetEnabled(true);
			mSelectableYGimbal->SetMatrix(mMatrixStack->Top());
			mMatrixStack->Pop();

			mMatrixStack->Push();
			mMatrixStack->Top() *= glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			mSelectableZGimbal->SetEnabled(true);
			mSelectableZGimbal->SetMatrix(mMatrixStack->Top());
			mMatrixStack->Pop();

			if (mSpace != Eye)
			{
				mSelectableOriginPivot->SetEnabled(true);
				mSelectableOriginPivot->SetMatrix(mMatrixStack->Top());
			}
		}
		break;
		case Scaling:
		{
			if (_x.z < 0.99)
			{
				mSelectableXScaler->SetEnabled(true);
				mSelectableXScaler->SetMatrix(mMatrixStack->Top());
			}

			if (_y.z < 0.99)
			{
				mMatrixStack->Push();
				mMatrixStack->Top() *= glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				mSelectableYScaler->SetEnabled(true);
				mSelectableYScaler->SetMatrix(mMatrixStack->Top());
				mMatrixStack->Pop();
			}

			if (_z.z < 0.99)
			{
				mMatrixStack->Push();
				mMatrixStack->Top() *= glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				mSelectableZScaler->SetEnabled(true);
				mSelectableZScaler->SetMatrix(mMatrixStack->Top());
				mMatrixStack->Pop();
			}

			mSelectableOriginPivot->SetEnabled(true);
			mSelectableOriginPivot->SetMatrix(mMatrixStack->Top());
		}
		break;
		default:
		{
			;
		}
		}

		mSelector->MouseMove(glm::vec2(x, y));
		if (!mSelector->GetSelected())
		{
			return None;
		}

		mMouseContactPoint = mSelector->GetSelected()->GetIntersection();

		mDepth = mCamera->Project(mMouseContactPoint).z;

		mWindowPosition = glm::vec3(x, y, mDepth);

		mRadius = mMouseContactPoint - mFinalTranslation;
		mRadius = glm::normalize(mRadius);

		return *reinterpret_cast<SelectionName*>(mSelector->GetSelected()->GetData());
	}

	void Manipulator::MouseMove(int x, int y)
	{
		if (mVisible)
		{
			if (mActing)
			{
				glm::mat4 prev = mMatrix;

				mWindowPosition = glm::vec3(mOldX, mOldY, mDepth);

				mOldMousePosition = mCamera->UnProject(mWindowPosition);

				mWindowPosition = glm::vec3(x, y, mDepth);

				mMousePosition = mCamera->UnProject(mWindowPosition);

				switch (mSelected)
				{
				case XArrow: case XGimbal: case XScaler:
				{
					mAxis = glm::vec3(1.0f, 0.0f, 0.0f);
				}
				break;
				case YArrow: case YGimbal: case YScaler:
				{
					mAxis = glm::vec3(0.0f, 1.0f, 0.0f);
				}
				break;
				case ZArrow: case ZGimbal: case ZScaler:
				{
					mAxis = glm::vec3(0.0f, 0.0f, 1.0f);
				}
				break;
				case XYPlane:
				{
					mAxis1 = glm::vec3(1.0f, 0.0f, 0.0f);
					mAxis2 = glm::vec3(0.0f, 1.0f, 0.0f);
				}
				break;
				case YZPlane:
				{
					mAxis1 = glm::vec3(0.0f, 1.0f, 0.0f);
					mAxis2 = glm::vec3(0.0f, 0.0f, 1.0f);
				}
				break;
				case ZXPlane:
				{
					mAxis1 = glm::vec3(1.0f, 0.0f, 0.0f);
					mAxis2 = glm::vec3(0.0f, 0.0f, 1.0f);
				}
				break;
				default:
				{
					;
				}
				}
				if (mSelected == XGimbal || mSelected == YGimbal || mSelected == ZGimbal)
				{
					mMouse = mMousePosition - mFinalTranslation;

					if (mSpace == Object)
					{
						mAxis_ = glm::vec3(glm::toMat4(mFinalRotation) * glm::vec4(mAxis, 0.0f));
						mBinormal = glm::cross(mAxis_, mRadius);

						mNewRadius = glm::dot(mMouse, mBinormal) * mBinormal + glm::dot(mMouse, mRadius) * mRadius;
						mNewRadius = glm::normalize(mNewRadius);

						if (glm::fabs(glm::dot(mRadius, mNewRadius)) < 1.0f)
						{
							float angle = glm::orientedAngle(mRadius, mNewRadius, mAxis_);

							if (mSnapRotation)
							{
								mDAngle += angle;
								if (glm::fabs(mDAngle) >= mRotationStep)
								{
									angle = glm::round(mDAngle / mRotationStep) * mRotationStep;
									mDAngle -= angle;
								}
								else
								{
									angle = 0.0f;
								}
							}

							glm::mat4 finalRotation = glm::toMat4(mFinalRotation) * glm::rotate(glm::radians(angle), mAxis);
							mFinalRotation = glm::toQuat(glm::mat3(glm::normalize(glm::vec3(finalRotation[0])), glm::normalize(glm::vec3(finalRotation[1])), glm::normalize(glm::vec3(finalRotation[2]))));

							mRadius = mNewRadius;
						}
					}
					else if (mSpace == Eye)
					{
						mAxis_ = mCamera->GetNormalBillboardMatrix() * mAxis;
						mBinormal = glm::cross(mAxis_, mRadius);

						mNewRadius = glm::dot(mMouse, mBinormal) * mBinormal + glm::dot(mMouse, mRadius) * mRadius;
						mNewRadius = glm::normalize(mNewRadius);

						if (glm::fabs(glm::dot(mRadius, mNewRadius)) < 1.0f)
						{
							float angle = glm::orientedAngle(mRadius, mNewRadius, mAxis_);

							if (mSnapRotation)
							{
								mDAngle += angle;
								if (glm::fabs(mDAngle) >= mRotationStep)
								{
									angle = glm::round(mDAngle / mRotationStep) * mRotationStep;
									mDAngle -= angle;
								}
								else
								{
									angle = 0.0f;
								}
							}

							glm::mat4 finalRotation = glm::rotate(glm::radians(angle), mAxis_) * glm::toMat4(mFinalRotation);
							mFinalRotation = glm::toQuat(glm::mat3(glm::normalize(glm::vec3(finalRotation[0])), glm::normalize(glm::vec3(finalRotation[1])), glm::normalize(glm::vec3(finalRotation[2]))));

							mRadius = mNewRadius;
						}
					}
					else
					{
						mBinormal = glm::cross(mAxis, mRadius);

						mNewRadius = glm::dot(mMouse, mBinormal) * mBinormal + glm::dot(mMouse, mRadius) * mRadius;
						mNewRadius = glm::normalize(mNewRadius);

						if (glm::fabs(glm::dot(mRadius, mNewRadius)) < 1.0f)
						{
							float angle = glm::orientedAngle(mRadius, mNewRadius, mAxis);

							if (mSnapRotation)
							{
								mDAngle += angle;
								if (glm::fabs(mDAngle) >= mRotationStep)
								{
									angle = glm::round(mDAngle / mRotationStep) * mRotationStep;
									mDAngle -= angle;
								}
								else
								{
									angle = 0.0f;
								}
							}

							glm::mat4 finalRotation = glm::rotate(glm::radians(angle), mAxis) * glm::toMat4(mFinalRotation);
							mFinalRotation = glm::toQuat(glm::mat3(glm::normalize(glm::vec3(finalRotation[0])), glm::normalize(glm::vec3(finalRotation[1])), glm::normalize(glm::vec3(finalRotation[2]))));

							mRadius = mNewRadius;
						}
					}
				}
				else if (mSelected == XArrow || mSelected == YArrow || mSelected == ZArrow)
				{
					mMouse = mMousePosition - mOldMousePosition;

					if (mSpace == Object)
					{
						mAxis_ = glm::vec3(glm::toMat4(mFinalRotation) * glm::vec4(mAxis, 0.0f));
						glm::vec3 t = glm::dot(mMouse, mAxis_) * mAxis_;

						if (mSnapTranslation)
						{
							mDT += t;
							if (glm::abs(mDT.x) >= mTranslationStep || glm::abs(mDT.y) >= mTranslationStep || glm::abs(mDT.z) >= mTranslationStep)
							{
								t = glm::round(mDT / mTranslationStep) * mTranslationStep;
								mDT -= t;
							}
							else
							{
								t = glm::vec3(0.0f);
							}
						}

						mFinalTranslation += t;
					}
					else if (mSpace == Eye)
					{
						mAxis_ = mCamera->GetNormalBillboardMatrix() * mAxis;
						glm::vec3 t = glm::dot(mMouse, mAxis_) * mAxis_;

						if (mSnapTranslation)
						{
							mDT += t;
							if (glm::abs(mDT.x) >= mTranslationStep || glm::abs(mDT.y) >= mTranslationStep || glm::abs(mDT.z) >= mTranslationStep)
							{
								t = glm::round(mDT / mTranslationStep) * mTranslationStep;
								mDT -= t;
							}
							else
							{
								t = glm::vec3(0.0f);
							}
						}

						mFinalTranslation += t;
					}
					else
					{
						glm::vec3 t = glm::dot(mMouse, mAxis) * mAxis;

						if (mSnapTranslation)
						{
							mDT += t;
							if (glm::abs(mDT.x) >= mTranslationStep || glm::abs(mDT.y) >= mTranslationStep || glm::abs(mDT.z) >= mTranslationStep)
							{
								t = glm::round(mDT / mTranslationStep) * mTranslationStep;
								mDT -= t;
							}
							else
							{
								t = glm::vec3(0.0f);
							}
						}

						mFinalTranslation += t;
					}
				}
				else if (mSelected == XYPlane || mSelected == YZPlane || mSelected == ZXPlane)
				{
					mMouse = mMousePosition - mOldMousePosition;

					if (mSpace == Object)
					{
						mAxis1_ = glm::vec3(glm::toMat4(mFinalRotation) * glm::vec4(mAxis1, 0.0f));
						mAxis2_ = glm::vec3(glm::toMat4(mFinalRotation) * glm::vec4(mAxis2, 0.0f));
						glm::vec3 t = glm::dot(mMouse, mAxis1_) * mAxis1_ + glm::dot(mMouse, mAxis2_) * mAxis2_;

						if (mSnapTranslation)
						{
							mDT += t;
							if (glm::abs(mDT.x) >= mTranslationStep || glm::abs(mDT.y) >= mTranslationStep || glm::abs(mDT.z) >= mTranslationStep)
							{
								t = glm::round(mDT / mTranslationStep) * mTranslationStep;
								mDT -= t;
							}
							else
							{
								t = glm::vec3(0.0f);
							}
						}

						mFinalTranslation += t;
					}
					else if (mSpace == Eye)
					{
						mAxis1_ = mCamera->GetNormalBillboardMatrix() * mAxis1;
						mAxis2_ = mCamera->GetNormalBillboardMatrix() * mAxis2;
						glm::vec3 t = glm::dot(mMouse, mAxis1_) * mAxis1_ + glm::dot(mMouse, mAxis2_) * mAxis2_;

						if (mSnapTranslation)
						{
							mDT += t;
							if (glm::abs(mDT.x) >= mTranslationStep || glm::abs(mDT.y) >= mTranslationStep || glm::abs(mDT.z) >= mTranslationStep)
							{
								t = glm::round(mDT / mTranslationStep) * mTranslationStep;
								mDT -= t;
							}
							else
							{
								t = glm::vec3(0.0f);
							}
						}

						mFinalTranslation += t;
					}
					else
					{
						glm::vec3 t = glm::dot(mMouse, mAxis1) * mAxis1 + glm::dot(mMouse, mAxis2) * mAxis2;

						if (mSnapTranslation)
						{
							mDT += t;
							if (glm::abs(mDT.x) >= mTranslationStep || glm::abs(mDT.y) >= mTranslationStep || glm::abs(mDT.z) >= mTranslationStep)
							{
								t = glm::round(mDT / mTranslationStep) * mTranslationStep;
								mDT -= t;
							}
							else
							{
								t = glm::vec3(0.0f);
							}
						}

						mFinalTranslation += t;
					}
				}
				else if (mSelected == XScaler || mSelected == YScaler || mSelected == ZScaler)
				{
					mMouse = mMousePosition - mOldMousePosition;

					mAxis_ = glm::vec3(glm::toMat4(mFinalRotation) * glm::vec4(mAxis, 0.0f));
					glm::vec3 s = glm::dot(mMouse, mAxis_) * mAxis + glm::vec3(1.0f);

					mFinalScaling *= s;
				}
				else if (mSelected == OriginPivot)
				{
					mMouse = mMousePosition - mOldMousePosition;

					switch (mStyle)
					{
					case Translation:
					{
						glm::vec3 t = mMouse;

						if (mSnapTranslation)
						{
							mDT += t;
							if (glm::abs(mDT.x) >= mTranslationStep || glm::abs(mDT.y) >= mTranslationStep || glm::abs(mDT.z) >= mTranslationStep)
							{
								t = glm::round(mDT / mTranslationStep) * mTranslationStep;
								mDT -= t;
							}
							else
							{
								t = glm::vec3(0.0f);
							}
						}

						mFinalTranslation += t;
					}
					break;
					case Rotation:
					{
						glm::mat4 camera = mCamera->GetInverseFinalMatrix();
						camera[0] = glm::normalize(camera[0]);
						camera[1] = glm::normalize(camera[1]);
						camera[2] = glm::normalize(camera[2]);
						camera[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

						mAxis = mCamera->GetNormalBillboardMatrix() * glm::vec3(1.0f, 0.0f, 0.0f);
						mAxis_ = mCamera->GetNormalBillboardMatrix() * glm::vec3(0.0f, 1.0f, 0.0f);
						glm::mat4 R(1.0f);

						R = glm::rotate(glm::radians(float(mOldY - y) * 180.0f / mSize), mAxis);
						R = glm::rotate(R, glm::radians(float(x - mOldX) * 180.0f / mSize), mAxis_);

						glm::mat4 finalRotation = R * glm::toMat4(mFinalRotation);
						mFinalRotation = glm::toQuat(glm::mat3(glm::normalize(glm::vec3(finalRotation[0])), glm::normalize(glm::vec3(finalRotation[1])), glm::normalize(glm::vec3(finalRotation[2]))));
					}
					break;
					case Scaling:
					{
						glm::mat4 camera = mCamera->GetInverseFinalMatrix();
						camera[0] = glm::normalize(camera[0]);
						camera[1] = glm::normalize(camera[1]);
						camera[2] = glm::normalize(camera[2]);
						camera[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

						mAxis = glm::vec3(1.0f, 1.0f, 0.0f);
						mAxis_ = mCamera->GetNormalBillboardMatrix() * mAxis;

						glm::vec3 s = glm::dot(mMouse, mAxis_) * glm::vec3(1.0f, 1.0f, 1.0f) + glm::vec3(1.0f);

						mFinalScaling *= s;
					}
					default:
					{
						;
					}
					}
				}

				mFinalMatrix = glm::translate(mFinalTranslation) * glm::toMat4(mFinalRotation) * glm::scale(mFinalScaling);
				mMatrix = glm::inverse(mParentMatrix) * mFinalMatrix;
				mScaling = glm::vec3(glm::length(glm::vec3(mMatrix[0])), glm::length(glm::vec3(mMatrix[1])), glm::length(glm::vec3(mMatrix[2])));
				mRotation = glm::toQuat(glm::mat3(glm::normalize(glm::vec3(mMatrix[0])), glm::normalize(glm::vec3(mMatrix[1])), glm::normalize(glm::vec3(mMatrix[2]))));
				mTranslation = glm::vec3(mMatrix[3]);

				if (mOnChange)
				{
					mOnChange(this, mData);
				}
			}
			else
			{
				mFocused = Pick(x, y);
			}
		}

		mOldX = x;
		mOldY = y;
	}

	void Manipulator::MouseDown(int x, int y)
	{
		if (mVisible)
		{
			if (mFocused != None)
			{
				mSelected = mFocused;
				mActing = true;
			}
		}
		mOldX = x;
		mOldY = y;
	}

	void Manipulator::MouseUp(void)
	{
		if (!mVisible)
		{
			return;
		}

		mDT = glm::vec3(0.0f);
		mDAngle = 0.0f;

		mActing = false;
	}

	void Manipulator::Render()
	{
		if (!mInitializedBuffersAndShaders)
		{
			InitializeBuffersAndShaders();
		}
		if (!mInitializedBuffersAndShaders)
		{
			return;
		}

		if (!mVisible)
		{
			return;
		}

		mMatrixStack->Top() = mCamera->GetFinalMatrix() * glm::translate(mFinalTranslation);

		if (mStyle != Scaling)
		{
			if (mSpace == Object)
			{
				mMatrixStack->Top() *= glm::toMat4(mFinalRotation);
			}
			else if (mSpace == Eye)
			{
				mMatrixStack->Top() *= mCamera->GetBillboardMatrix();
			}
		}
		else
		{
			mMatrixStack->Top() *= glm::toMat4(mFinalRotation);
		}

		float f = mCamera->GetScaleFactor(mFinalTranslation, mSize);
		mMatrixStack->Top() *= glm::scale(glm::vec3(f));

		switch (mStyle)
		{
		case Translation:
		{
			RenderXArrow();
			RenderYArrow();
			if (mSpace != Eye)
			{
				RenderZArrow();
				RenderXYPlane();
				RenderYZPlane();
				RenderZXPlane();
			}
			RenderOriginPivot();
		}
		break;
		case Rotation:
		{
			RenderXGimbal();
			RenderYGimbal();
			RenderZGimbal();
			if (mSpace != Eye)
			{
				RenderOriginPivot();
			}
		}
		break;
		case Scaling:
		{
			RenderXScaler();
			RenderYScaler();
			RenderZScaler();
			RenderOriginPivot();
		}
		break;
		default:
		{
			;
		}
		}
	}

	void Manipulator::Show(void)
	{
		mVisible = true;
	}

	void Manipulator::Hide(void)
	{
		mVisible = false;
	}

	bool Manipulator::IsVisible(void)
	{
		return mVisible;
	}

	bool Manipulator::IsActing(void)
	{
		return mActing;
	}

	void Manipulator::SetSize(float size)
	{
		if (size > 0.0)
		{
			mSize = size;
		}
	}

	float Manipulator::GetSize(void)
	{
		return mSize;
	}

	void Manipulator::SetSpace(ManipulatorSpace space)
	{
		mSpace = space;
	}

	void Manipulator::SetStyle(ManipulatorStyle style)
	{
		mStyle = style;
	}

	void Manipulator::SetCamera(class Camera* camera)
	{
		mCamera = camera;

		mSelector->SetCamera(camera);
	}

	Camera* Manipulator::GetCamera(void)
	{
		return mCamera;
	}

	void Manipulator::SetTranslation(glm::vec3 translation)
	{
		mTranslation = translation;

		Update();
	}

	glm::vec3 Manipulator::GetTranslation(void)
	{
		return mTranslation;
	}

	void Manipulator::SetRotation(glm::quat rotation)
	{
		mRotation = rotation;

		Update();
	}

	glm::quat Manipulator::GetRotation(void)
	{
		return mRotation;
	}

	void Manipulator::SetEulerAngles(glm::vec3 eulerAngles)
	{
		mRotation = glm::quat(glm::radians(eulerAngles));

		Update();
	}

	glm::vec3 Manipulator::GetEulerAngles(void)
	{
		static glm::vec3 rot;
		static bool first = true;
		if (first)
		{
			rot = glm::matrixToRotation(mMatrix);
			first = false;
		}
		else
		{
			rot = glm::matrixToRotation(mMatrix, rot);
		}
		return rot;
	}

	void Manipulator::SetAngleAxis(float angle, glm::vec3 axis)
	{
		mRotation = glm::angleAxis(angle, axis);

		Update();
	}

	float Manipulator::GetAngle(void)
	{
		return glm::angle(mRotation);
	}

	glm::vec3 Manipulator::GetAxis(void)
	{
		return glm::axis(mRotation);
	}

	void Manipulator::SetScaling(glm::vec3 scaling)
	{
		mScaling = scaling;

		Update();
	}

	glm::vec3 Manipulator::GetScaling(void)
	{
		return mScaling;
	}

	void Manipulator::Update(void)
	{
		mMatrix = glm::translate(mTranslation) * glm::toMat4(mRotation) * glm::scale(mScaling);

		mFinalMatrix = mParentMatrix * mMatrix;

		mFinalScaling = glm::vec3(glm::length(glm::vec3(mFinalMatrix[0])), glm::length(glm::vec3(mFinalMatrix[1])), glm::length(glm::vec3(mFinalMatrix[2])));
		mFinalRotation = glm::toQuat(glm::mat3(glm::normalize(glm::vec3(mFinalMatrix[0])), glm::normalize(glm::vec3(mFinalMatrix[1])), glm::normalize(glm::vec3(mFinalMatrix[2]))));
		mFinalTranslation = glm::vec3(mFinalMatrix[3]);
	}

	bool Manipulator::SnapTranslation(void)
	{
		return mSnapTranslation;
	}

	void Manipulator::SnapTranslationOn(void)
	{
		mSnapTranslation = true;
	}

	void Manipulator::SnapTranslationOff(void)
	{
		mSnapTranslation = false;
	}

	void Manipulator::SetTranslationStep(float translationStep)
	{
		mTranslationStep = translationStep;
	}

	float Manipulator::GetTranslationStep(void)
	{
		return mTranslationStep;
	}

	bool Manipulator::SnapRotation(void)
	{
		return mSnapRotation;
	}

	void Manipulator::SnapRotationOn(void)
	{
		mSnapRotation = true;
	}

	void Manipulator::SnapRotationOff(void)
	{
		mSnapRotation = false;
	}

	void Manipulator::SetRotationStep(float rotationStep)
	{
		mRotationStep = rotationStep;
	}

	float Manipulator::GetRotationStep(void)
	{
		return mRotationStep;
	}

	void Manipulator::SetData(void* data)
	{
		mData = data;
	}

	void* Manipulator::GetData(void)
	{
		return mData;
	}

	void Manipulator::SetOnChange(MANIPULATOR_ONCHANGE_CALLBACK on_change)
	{
		mOnChange = on_change;
	}

	void Manipulator::InitializeBuffersAndShaders(void)
	{
		if (mInitializedBuffersAndShaders)
		{
			return;
		}

		glGenBuffers(1, &mArrowPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mArrowPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, arrow_vertices * 3 * sizeof(float), arrow_v, GL_STATIC_DRAW);

		glGenBuffers(1, &mArrowNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mArrowNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, arrow_vertices * 3 * sizeof(float), arrow_vn, GL_STATIC_DRAW);

		glGenBuffers(1, &mArrowFaceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mArrowFaceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrow_faces * 3 * sizeof(unsigned int), arrow_f, GL_STATIC_DRAW);

		glGenBuffers(1, &mGimbalPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mGimbalPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, gimbal_vertices * 3 * sizeof(float), gimbal_v, GL_STATIC_DRAW);

		glGenBuffers(1, &mGimbalNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mGimbalNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, gimbal_vertices * 3 * sizeof(float), gimbal_vn, GL_STATIC_DRAW);

		glGenBuffers(1, &mGimbalFaceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGimbalFaceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, gimbal_faces * 3 * sizeof(unsigned int), gimbal_f, GL_STATIC_DRAW);

		glGenBuffers(1, &mScalerPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mScalerPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, scaler_vertices * 3 * sizeof(float), scaler_v, GL_STATIC_DRAW);

		glGenBuffers(1, &mScalerNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mScalerNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, scaler_vertices * 3 * sizeof(float), scaler_vn, GL_STATIC_DRAW);

		glGenBuffers(1, &mScalerFaceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mScalerFaceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, scaler_faces * 3 * sizeof(unsigned int), scaler_f, GL_STATIC_DRAW);

		glGenBuffers(1, &mPlanePositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mPlanePositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, plane_vertices * 3 * sizeof(float), plane_v, GL_STATIC_DRAW);

		glGenBuffers(1, &mPlaneNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mPlaneNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, plane_vertices * 3 * sizeof(float), plane_vn, GL_STATIC_DRAW);

		glGenBuffers(1, &mPlaneFaceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPlaneFaceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane_faces * 3 * sizeof(unsigned int), plane_f, GL_STATIC_DRAW);

		glGenBuffers(1, &mPivotPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mPivotPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, pivot_vertices * 3 * sizeof(float), pivot_v, GL_STATIC_DRAW);

		glGenBuffers(1, &mPivotNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mPivotNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, pivot_vertices * 3 * sizeof(float), pivot_vn, GL_STATIC_DRAW);

		glGenBuffers(1, &mPivotFaceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPivotFaceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pivot_faces * 3 * sizeof(unsigned int), pivot_f, GL_STATIC_DRAW);

		mShader.CompileSource("uniform mat4 uFTransform;uniform mat3 uNormalMatrix;attribute vec3 aPosition;attribute vec3 aNormal;varying vec3 vNormal;void main(){vNormal = uNormalMatrix * aNormal;gl_Position = uFTransform * vec4(aPosition, 1.0);}", GL_VERTEX_SHADER);

		mShader.CompileSource("varying vec3 vNormal;uniform vec3 uColor;void main(){vec3 normal = normalize(vNormal);vec3 light = vec3(0.0, 0.0, 1.0);float diffuse = max(0.0, dot(normal, light));gl_FragColor = vec4(diffuse * uColor, 1.0);}", GL_FRAGMENT_SHADER);

		mShader.Link();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mInitializedBuffersAndShaders = true;
	}

}
