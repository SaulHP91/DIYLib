#include <gl/glew.h>

#include <diy/camera.hpp>
#include <diy/shader.hpp>

#include <glm/ext.hpp>

namespace diy
{

	Shader Camera::mPivotShader;
	unsigned int Camera::mPivotPositionBuffer = 0;
	unsigned int Camera::mPivotFaceBuffer = 0;

	Shader Camera::mCameraShader;
	unsigned int Camera::mCameraPositionBuffer = 0;
	unsigned int Camera::mCameraNormalBuffer = 0;
	unsigned int Camera::mCameraFaceBuffer = 0;

	bool Camera::mInitialized = false;

	Camera::Camera(void) :
		mEnabled(true),
		mVisible(true),
		mViewport(0, 0, 640, 480),
		mFovy(30.0f),
		mAspect(4.0f / 3.0f),
		mOrthoNear(-10.0f),
		mPerspectiveNear(0.1f),
		mFar(10.0f),
		mPivot(0.0f),
		mOrientation(0.0f),
		mZoom(1.0f),
		mDistance(1.866025403784f),
		mUnitaryDistance(1.866025403784f),
		mProjectionModel(Perspective),
		mProjectionMatrix(1.0f),
		mParentMatrix(1.0f),
		mInverseParentMatrix(1.0f),
		mMatrix(1.0f),
		mInverseMatrix(1.0f),
		mFinalMatrix(1.0f),
		mInverseFinalMatrix(1.0f),
		mBillboardMatrix(1.0f),
		mInverseBillboardMatrix(1.0f),
		mNormalParentMatrix(1.0f),
		mInverseNormalParentMatrix(1.0f),
		mNormalMatrix(1.0f),
		mInverseNormalMatrix(1.0f),
		mNormalFinalMatrix(1.0f),
		mInverseNormalFinalMatrix(1.0f),
		mNormalBillboardMatrix(1.0f),
		mInverseNormalBillboardMatrix(1.0f),
		mPosition(0.0f),
		mPivotConstrained(false),
		mOrientationConstrained(false),
		mZoomConstrained(false),
		mDistanceConstrained(false),
		mPivotLocked(false),
		mOrientationLocked(false),
		mZoomLocked(false),
		mDistanceLocked(false),
		mMinPivot(-FLT_MAX, -FLT_MAX, -FLT_MAX),
		mMaxPivot(FLT_MAX, FLT_MAX, FLT_MAX),
		mMinOrientation(-FLT_MAX, -FLT_MAX, -FLT_MAX),
		mMaxOrientation(FLT_MAX, FLT_MAX, FLT_MAX),
		mMinZoom(-FLT_MAX),
		mMaxZoom(FLT_MAX),
		mMinDistance(-FLT_MAX),
		mMaxDistance(FLT_MAX),
		mStyle(CustomGirl)
	{
		Update();
	}

	Camera::~Camera(void)
	{
		;
	}

	void Camera::SetEnabled(bool enabled)
	{
		mEnabled = true;
	}

	bool Camera::GetEnabled(void)
	{
		return mEnabled;
	}

	void Camera::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	bool Camera::GetVisible(void)
	{
		return mVisible;
	}

	void Camera::SetViewport(glm::vec4 viewport)
	{
		if (viewport.w > 0)
		{
			mViewport = viewport;
			mAspect = mViewport[2] / mViewport[3];

			Update();
		}
	}

	glm::vec4 Camera::GetViewport(void)
	{
		return mViewport;
	}

	void Camera::SetFovy(float fovy)
	{
		if (mEnabled && mProjectionModel == Perspective)
		{
			if (fovy > 0.0f && fovy < 180.0f)
			{
				mFovy = fovy;
			}

			Update();
		}
	}

	float Camera::GetFovy(void)
	{
		return mFovy;
	}

	float Camera::GetAspect(void)
	{
		return mAspect;
	}

	void Camera::SetNear(float _near)
	{
		if (!mEnabled)
		{
			return;
		}

		switch (mProjectionModel)
		{
		case Ortho:
		{
			mOrthoNear = _near;
		}
		break;
		case Perspective: default:
		{
			if (_near > 0.0f)
			{
				mPerspectiveNear = _near;
			}
		}
		}

		Update();
	}

	float Camera::GetNear(void)
	{
		switch (mProjectionModel)
		{
		case Ortho:
		{
			return mOrthoNear;
		}
		break;
		case Perspective: default:
		{
			return mPerspectiveNear;
		}
		}
	}

	void Camera::SetFar(float _far)
	{
		mFar = _far;

		Update();
	}

	float Camera::GetFar(void)
	{
		return mFar;
	}

	void Camera::ApplyViewport(void)
	{
		glViewport(mViewport.x, mViewport.y, mViewport.z, mViewport.w);
	}

	void Camera::ApplyProjectionMatrix(void)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(mProjectionMatrix));
	}

	void Camera::ApplyFinalMatrix(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(mFinalMatrix));
	}

#include <target.c>

	void Camera::RenderPivot(Camera* camera)
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

		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat4 ftransform;

		if (camera && camera != this)
		{
			float f = 100.0f * mZoom / 2.0f;
			switch (camera->mProjectionModel)
			{
			case Ortho:
			{
				f *= 2.0f * (1.0f / camera->mZoom) * (camera->mDistance / camera->mFocalDistance);
			}
			break;
			case Perspective: default:
			{
				f *= camera->GetDistanceTo(glm::vec3(mParentMatrix * glm::vec4(mPivot, 1.0f))) / camera->mFocalDistance;
			}
			}

			ftransform = camera->mProjectionMatrix * camera->mFinalMatrix * mInverseFinalMatrix * glm::translate(glm::vec3(0.0f, 0.0f, -mDistance)) * glm::scale(glm::vec3(f));
		}
		else
		{
			float f = 100.0f * (mDistance / mFocalDistance);

			ftransform = mProjectionMatrix * glm::translate(glm::vec3(0.0f, 0.0f, -mDistance)) * glm::scale(glm::vec3(f));
		}

		mPivotShader.Begin();
		mPivotShader.EnableVertexAttribArray("aPosition");

		mPivotShader.Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(ftransform));

		glBindBuffer(GL_ARRAY_BUFFER, mPivotPositionBuffer);
		mPivotShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPivotFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * target_faces, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		mPivotShader.DisableVertexAttribArray("aPosition");

		mPivotShader.End();

		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	}

	void Camera::RenderFrustum(Camera* camera)
	{
		if (!mVisible || !camera || camera == this)
		{
			return;
		}

		float n, v_n, h_n, v_f, h_f;
		switch (mProjectionModel)
		{
		case Ortho:
		{
			n = mOrthoNear;
			v_n = mDistance / (2.0f * mUnitaryDistance);
			h_n = mAspect * v_n;
			v_f = v_n;
			h_f = h_n;
		}
		break;
		case Perspective: default:
		{
			n = mPerspectiveNear;
			v_n = n / mUnitaryDistance;
			h_n = mAspect * v_n;
			v_f = mFar / mUnitaryDistance;
			h_f = mAspect * v_f;
		}
		}

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadMatrixf(glm::value_ptr(camera->mProjectionMatrix));

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadMatrixf(glm::value_ptr(camera->mFinalMatrix));
		glMultMatrixf(glm::value_ptr(mInverseFinalMatrix));

		glColor3f(0, 1.0f, 1.0f);

		glBegin(GL_LINE_LOOP);
		glVertex3f(h_n, v_n, -n);
		glVertex3f(-h_n, v_n, -n);
		glVertex3f(-h_n, -v_n, -n);
		glVertex3f(h_n, -v_n, -n);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex3f(h_f, v_f, -mFar);
		glVertex3f(-h_f, v_f, -mFar);
		glVertex3f(-h_f, -v_f, -mFar);
		glVertex3f(h_f, -v_f, -mFar);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(h_n, v_n, -n);
		glVertex3f(h_f, v_f, -mFar);
		glVertex3f(-h_n, v_n, -n);
		glVertex3f(-h_f, v_f, -mFar);
		glVertex3f(-h_n, -v_n, -n);
		glVertex3f(-h_f, -v_f, -mFar);
		glVertex3f(h_n, -v_n, -n);
		glVertex3f(h_f, -v_f, -mFar);
		glEnd();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	void Camera::RenderAxes(Camera* camera)
	{
		if (!mVisible || !camera || camera == this)
		{
			return;
		}

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadMatrixf(glm::value_ptr(camera->mProjectionMatrix));

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadMatrixf(glm::value_ptr(camera->mFinalMatrix));
		glMultMatrixf(glm::value_ptr(mInverseFinalMatrix));
		float s = 150.0f * mZoom / 2.0f;
		if (camera->GetProjectionModel() == Camera::Ortho)
		{
			s *= 2.0f * (1.0f / camera->mZoom) * (camera->mDistance / camera->mFocalDistance);
		}
		else
		{
			s *= camera->GetDistanceTo(mPosition) / camera->mFocalDistance;
		}
		glScalef(s, s, s);

		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
		glEnd();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

#include "camera.c"

	void Camera::Render(Camera* camera)
	{
		if (!mVisible || !camera || camera == this)
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

		float f = 50.0f * mZoom / 2.0f;
		if (camera->GetProjectionModel() == Camera::Ortho)
		{
			f *= 2.0f * (1.0f / camera->mZoom) * (camera->mDistance / camera->mFocalDistance);
		}
		else
		{
			f *= camera->GetDistanceTo(mPosition) / camera->mFocalDistance;
		}

		glm::mat4 projectionMatrix = camera->mProjectionMatrix;
		glm::mat4 modelViewMatrix = camera->mFinalMatrix * mInverseFinalMatrix * glm::scale(glm::vec3(f));
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));

		mCameraShader.Begin();
		mCameraShader.EnableVertexAttribArray("aPosition");
		mCameraShader.EnableVertexAttribArray("aNormal");

		mCameraShader.Uniform(GL_FLOAT_MAT4, "uProjectionMatrix", 1, glm::value_ptr(projectionMatrix));
		mCameraShader.Uniform(GL_FLOAT_MAT4, "uModelViewMatrix", 1, glm::value_ptr(modelViewMatrix));
		mCameraShader.Uniform(GL_FLOAT_MAT3, "uNormalMatrix", 1, glm::value_ptr(normalMatrix));

		glBindBuffer(GL_ARRAY_BUFFER, mCameraPositionBuffer);
		mCameraShader.VertexAttribPointer(GL_FLOAT_VEC3, "aPosition", 0);

		glBindBuffer(GL_ARRAY_BUFFER, mCameraNormalBuffer);
		mCameraShader.VertexAttribPointer(GL_FLOAT_VEC3, "aNormal", 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCameraFaceBuffer);
		glDrawElements(GL_TRIANGLES, 3 * camera_faces, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		mCameraShader.DisableVertexAttribArray("aPosition");
		mCameraShader.DisableVertexAttribArray("aNormal");
		mCameraShader.End();
	}

	void Camera::SetPivot(glm::vec3 pivot)
	{
		if (!mEnabled || mPivotLocked)
		{
			return;
		}

		mPivot = pivot;

		Update();
	}

	glm::vec3 Camera::GetPivot(void)
	{
		return mPivot;
	}

	void Camera::SetOrientation(glm::vec3 orientation)
	{
		if (!mEnabled || mOrientationLocked)
		{
			return;
		}

		mOrientation = orientation;

		Update();
	}

	glm::vec3 Camera::GetOrientation(void)
	{
		return mOrientation;
	}

	void Camera::SetZoom(float zoom)
	{
		if (!mEnabled || mZoomLocked)
		{
			return;
		}

		if (zoom > 0.0f)
		{
			mZoom = zoom;
		}

		Update();
	}

	float Camera::GetZoom(void)
	{
		return mZoom;
	}

	void Camera::LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
	{
		mPivot = center;
		glm::vec3 view = eye - center;
		mDistance = glm::length(view);
		glm::vec3 left = glm::cross(up, view);
		up = glm::cross(view, left);
		mOrientation = glm::eulerAngles(glm::quat_cast(glm::mat3(glm::normalize(left), glm::normalize(up), glm::normalize(view))));

		Update();
	}

	void Camera::SetProjectionModel(ProjectionModel projectionModel)
	{
		if (!mEnabled)
		{
			return;
		}

		mProjectionModel = projectionModel;

		Update();
	}

	Camera::ProjectionModel Camera::GetProjectionModel(void)
	{
		return mProjectionModel;
	}

	glm::mat4 Camera::GetProjectionMatrix(void)
	{
		return mProjectionMatrix;
	}

	void Camera::SetParentMatrix(glm::mat4 parentMatrix)
	{
		if (!mEnabled)
		{
			return;
		}

		mParentMatrix = parentMatrix;
		mInverseParentMatrix = glm::inverse(mParentMatrix);
		mNormalParentMatrix = glm::transpose(glm::mat3(glm::inverse(mParentMatrix)));
		mInverseNormalParentMatrix = glm::transpose(glm::mat3(glm::inverse(mInverseParentMatrix)));

		Update();
	}

	glm::mat4 Camera::GetParentMatrix(void)
	{
		return mParentMatrix;
	}

	glm::mat4 Camera::GetInverseParentMatrix(void)
	{
		return mInverseParentMatrix;
	}

	glm::mat4 Camera::GetMatrix(void)
	{
		return mMatrix;
	}

	glm::mat4 Camera::GetInverseMatrix(void)
	{
		return mInverseMatrix;
	}

	glm::mat4 Camera::GetFinalMatrix(void)
	{
		return mFinalMatrix;
	}

	glm::mat4 Camera::GetInverseFinalMatrix(void)
	{
		return mInverseFinalMatrix;
	}

	glm::mat4 Camera::GetBillboardMatrix(void)
	{
		return mBillboardMatrix;
	}

	glm::mat4 Camera::GetInverseBillboardMatrix(void)
	{
		return mInverseBillboardMatrix;
	}

	glm::mat3 Camera::GetNormalMatrix(void)
	{
		return mNormalMatrix;
	}

	glm::mat3 Camera::GetInverseNormalMatrix(void)
	{
		return mInverseNormalMatrix;
	}

	glm::mat3 Camera::GetNormalFinalMatrix(void)
	{
		return mNormalFinalMatrix;
	}

	glm::mat3 Camera::GetInverseNormalFinalMatrix(void)
	{
		return mInverseNormalFinalMatrix;
	}

	glm::mat3 Camera::GetNormalBillboardMatrix(void)
	{
		return mNormalBillboardMatrix;
	}

	glm::mat3 Camera::GetInverseNormalBillboardMatrix(void)
	{
		return mInverseNormalBillboardMatrix;
	}

	float Camera::GetUnitaryDistance(void)
	{
		return mUnitaryDistance;
	}

	float Camera::GetFocalDistance(void)
	{
		return mFocalDistance;
	}

	void Camera::SetDistance(float distance)
	{
		if (!mEnabled || mDistanceLocked)
		{
			return;
		}

		if (distance != 0.0f)
		{
			mDistance = distance;

			Update();
		}
	}

	float Camera::GetDistance(void)
	{
		return mDistance;
	}

	float Camera::GetDistanceTo(glm::vec3 target)
	{
		return glm::length(target - mPosition);
	}

	float Camera::GetScaleFactor(glm::vec3 centerOfInterest, float verticalSizeInPixels, float actualVerticalSize)
	{
		switch (mProjectionModel)
		{
		case Ortho:
		{
			return 2.0f * (1.0f / mZoom) * (verticalSizeInPixels / actualVerticalSize) * (mDistance / mFocalDistance);
		}
		break;
		case Perspective: default:
		{
			return (verticalSizeInPixels / actualVerticalSize) * (glm::length(centerOfInterest - mPosition) / mFocalDistance);
		}
		}
	}

	glm::vec3 Camera::GetPosition(void)
	{
		return mPosition;
	}

	void Camera::SetPivotConstrained(bool pivotConstrained)
	{
		mPivotConstrained = pivotConstrained;
	}

	bool Camera::GetPivotConstrained(void)
	{
		return mPivotConstrained;
	}

	void Camera::SetOrientationConstrained(bool orientationConstrained)
	{
		mOrientationConstrained = orientationConstrained;
	}

	bool Camera::GetOrientationConstrained(void)
	{
		return mOrientationConstrained;
	}

	void Camera::SetZoomConstrained(bool zoomConstrained)
	{
		mZoomConstrained = zoomConstrained;
	}

	bool Camera::GetZoomConstrained(void)
	{
		return mZoomConstrained;
	}

	void Camera::SetDistanceConstrained(bool distanceConstrained)
	{
		mDistanceConstrained = distanceConstrained;
	}

	bool Camera::GetDistanceConstrained(void)
	{
		return mDistanceConstrained;
	}

	void Camera::SetConstrained(bool constrained)
	{
		mPivotConstrained = constrained;
		mOrientationConstrained = constrained;
		mZoomConstrained = constrained;
		mDistanceConstrained = constrained;
	}

	bool Camera::GetConstrained(void)
	{
		return (mPivotConstrained || mOrientationConstrained || mZoomConstrained || mDistanceConstrained);
	}

	void Camera::SetMinPivot(glm::vec3 minPivot)
	{
		if (!mEnabled)
		{
			return;
		}

		mMinPivot = minPivot;

		if (mMaxPivot.x < mMinPivot.x)
		{
			mMaxPivot.x = mMinPivot.x;
		}
		if (mMaxPivot.y < mMinPivot.y)
		{
			mMaxPivot.y = mMinPivot.y;
		}
		if (mMaxPivot.z < mMinPivot.z)
		{
			mMaxPivot.z = mMinPivot.z;
		}

		Update();
	}

	glm::vec3 Camera::GetMinPivot(void)
	{
		return mMinPivot;
	}

	void Camera::SetMaxPivot(glm::vec3 maxPivot)
	{
		if (!mEnabled)
		{
			return;
		}

		mMaxPivot = maxPivot;

		if (mMinPivot.x > mMaxPivot.x)
		{
			mMinPivot.x = mMaxPivot.x;
		}
		if (mMinPivot.y > mMaxPivot.y)
		{
			mMinPivot.y = mMaxPivot.y;
		}
		if (mMinPivot.z > mMaxPivot.z)
		{
			mMinPivot.z = mMaxPivot.z;
		}

		Update();
	}

	glm::vec3 Camera::GetMaxPivot(void)
	{
		return mMaxPivot;
	}

	void Camera::SetMinOrientation(glm::vec3 minOrientation)
	{
		if (!mEnabled)
		{
			return;
		}

		mMinOrientation = minOrientation;

		if (mMaxOrientation.x < mMinOrientation.x)
		{
			mMaxOrientation.x = mMinOrientation.x;
		}
		if (mMaxOrientation.y < mMinOrientation.y)
		{
			mMaxOrientation.y = mMinOrientation.y;
		}
		if (mMaxOrientation.z < mMinOrientation.z)
		{
			mMaxOrientation.z = mMinOrientation.z;
		}

		Update();
	}

	glm::vec3 Camera::GetMinOrientation(void)
	{
		return mMinOrientation;
	}

	void Camera::SetMaxOrientation(glm::vec3 maxOrientation)
	{
		if (!mEnabled)
		{
			return;
		}

		mMaxOrientation = maxOrientation;

		if (mMinOrientation.x > mMaxOrientation.x)
		{
			mMinOrientation.x = mMaxOrientation.x;
		}
		if (mMinOrientation.y > mMaxOrientation.y)
		{
			mMinOrientation.y = mMaxOrientation.y;
		}
		if (mMinOrientation.z > mMaxOrientation.z)
		{
			mMinOrientation.z = mMaxOrientation.z;
		}

		Update();
	}

	glm::vec3 Camera::GetMaxOrientation(void)
	{
		return mMaxOrientation;
	}

	void Camera::SetMinZoom(float minZoom)
	{
		if (!mEnabled)
		{
			return;
		}

		mMinZoom = minZoom;

		if (mMaxZoom < mMinZoom)
		{
			mMaxZoom = mMinZoom;
		}

		Update();
	}

	float Camera::GetMinZoom(void)
	{
		return mMinZoom;
	}

	void Camera::SetMaxZoom(float maxZoom)
	{
		if (!mEnabled)
		{
			return;
		}

		mMaxZoom = maxZoom;

		if (mMinZoom > mMaxZoom)
		{
			mMinZoom = mMaxZoom;
		}

		Update();
	}

	float Camera::GetMaxZoom(void)
	{
		return mMaxZoom;
	}

	void Camera::SetMinDistance(float minDistance)
	{
		if (!mEnabled)
		{
			return;
		}

		mMinDistance = minDistance;

		if (mMaxDistance < mMinDistance)
		{
			mMaxDistance = mMinDistance;
		}

		Update();
	}

	float Camera::GetMinDistance(void)
	{
		return mMinDistance;
	}

	void Camera::SetMaxDistance(float maxDistance)
	{
		if (!mEnabled)
		{
			return;
		}

		mMaxDistance = maxDistance;

		if (mMinDistance > mMaxDistance)
		{
			mMinDistance = mMaxDistance;
		}

		Update();
	}

	float Camera::GetMaxDistance(void)
	{
		return mMaxDistance;
	}

	void Camera::SetPivotLocked(bool pivotLocked)
	{
		mPivotLocked = pivotLocked;
	}

	bool Camera::GetPivotLocked(void)
	{
		return mPivotLocked;
	}

	void Camera::SetOrientationLocked(bool orientationLocked)
	{
		mOrientationLocked = orientationLocked;
	}

	bool Camera::GetOrientationLocked(void)
	{
		return mOrientationLocked;
	}

	void Camera::SetZoomLocked(bool zoomLocked)
	{
		mZoomLocked = zoomLocked;
	}

	bool Camera::GetZoomLocked(void)
	{
		return mZoomLocked;
	}

	void Camera::SetDistanceLocked(bool distanceLocked)
	{
		mDistanceLocked = distanceLocked;
	}

	bool Camera::GetDistanceLocked(void)
	{
		return mDistanceLocked;
	}

	void Camera::SetLocked(bool locked)
	{
		mPivotLocked = locked;
		mOrientationLocked = locked;
		mZoomLocked = locked;
		mDistanceLocked = locked;
	}

	bool Camera::GetLocked(void)
	{
		return (mPivotLocked || mOrientationLocked || mZoomLocked || mDistanceLocked);
	}

	void Camera::Update(void)
	{
		mUnitaryDistance = 1.0f / (2.0f * glm::tan(glm::radians(mFovy) / 2.0f));
		mFocalDistance = mViewport[3] * mUnitaryDistance;

		if (mPivotConstrained)
		{
			if (mPivot.x < mMinPivot.x)
			{
				mPivot.x = mMinPivot.x;
			}
			else if (mPivot.x > mMaxPivot.x)
			{
				mPivot.x = mMaxPivot.x;
			}
			if (mPivot.y < mMinPivot.y)
			{
				mPivot.y = mMinPivot.y;
			}
			else if (mPivot.y > mMaxPivot.y)
			{
				mPivot.y = mMaxPivot.y;
			}
			if (mPivot.z < mMinPivot.z)
			{
				mPivot.z = mMinPivot.z;
			}
			else if (mPivot.z > mMaxPivot.z)
			{
				mPivot.z = mMaxPivot.z;
			}
		}

		if (mOrientationConstrained)
		{
			if (mOrientation.x < mMinOrientation.x)
			{
				mOrientation.x = mMinOrientation.x;
			}
			else if (mOrientation.x > mMaxOrientation.x)
			{
				mOrientation.x = mMaxOrientation.x;
			}
			if (mOrientation.y < mMinOrientation.y)
			{
				mOrientation.y = mMinOrientation.y;
			}
			else if (mOrientation.y > mMaxOrientation.y)
			{
				mOrientation.y = mMaxOrientation.y;
			}
			if (mOrientation.z < mMinOrientation.z)
			{
				mOrientation.z = mMinOrientation.z;
			}
			else if (mOrientation.z > mMaxOrientation.z)
			{
				mOrientation.z = mMaxOrientation.z;
			}
		}

		if (mZoomConstrained)
		{
			if (mZoom < mMinZoom)
			{
				mZoom = mMinZoom;
			}
			else if (mZoom > mMaxZoom)
			{
				mZoom = mMaxZoom;
			}
		}

		if (mDistanceConstrained)
		{
			if (mDistance < mMinDistance)
			{
				mDistance = mMinDistance;
			}
			else if (mDistance > mMaxDistance)
			{
				mDistance = mMaxDistance;
			}
		}

		switch (mProjectionModel)
		{
		case Ortho:
		{
			float f = mDistance / (2.0f * mUnitaryDistance);
			mProjectionMatrix = glm::ortho(-mAspect * f, mAspect * f, -f, f, mOrthoNear, mFar);
		}
		break;
		case Perspective: default:
		{
			mProjectionMatrix = glm::perspective(glm::radians(mFovy), mAspect, mPerspectiveNear, mFar);
		}
		}

		mInverseMatrix = glm::translate(mPivot) * glm::scale(glm::vec3(2.0f / mZoom, 2.0f / mZoom, 2.0f / mZoom)) * glm::toMat4(glm::quat(glm::radians(mOrientation))) * glm::translate(glm::vec3(0.0f, 0.0f, mDistance));
		mInverseFinalMatrix = mParentMatrix * mInverseMatrix;
		mMatrix = glm::inverse(mInverseMatrix);
		mFinalMatrix = glm::inverse(mInverseFinalMatrix);
		mBillboardMatrix = glm::mat4(glm::normalize(mInverseFinalMatrix[0]), glm::normalize(mInverseFinalMatrix[1]), glm::normalize(mInverseFinalMatrix[2]), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		mInverseBillboardMatrix = glm::inverse(mBillboardMatrix);
		mNormalMatrix = glm::mat3(glm::transpose(glm::inverse(mMatrix)));
		mNormalFinalMatrix = glm::mat3(glm::transpose(glm::inverse(mFinalMatrix)));
		mInverseNormalMatrix = glm::inverse(mNormalMatrix);
		mInverseNormalFinalMatrix = glm::inverse(mNormalFinalMatrix);
		mNormalBillboardMatrix = glm::mat3(mBillboardMatrix);
		mInverseNormalBillboardMatrix = glm::mat3(mInverseBillboardMatrix);

		mPosition = glm::vec3(mInverseFinalMatrix[3]);
	}

	void Camera::LoadPreset(Preset preset)
	{
		switch (preset)
		{
		case Home:
		{
			mOrientation = glm::vec3(-30.0f, 45.0f, 0.0f);
		}
		break;
		case Front:
		{
			mOrientation = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		break;
		case Back:
		{
			mOrientation = glm::vec3(0.0f, 180.0f, 0.0f);
		}
		break;
		case SideLeft:
		{
			mOrientation = glm::vec3(0.0f, 90.0f, 0.0f);
		}
		break;
		case SideRight:
		{
			mOrientation = glm::vec3(0.0f, -90.0f, 0.0f);
		}
		break;
		case Top:
		{
			mOrientation = glm::vec3(-90.0f, 0.0f, 0.0f);
		}
		break;
		case Bottom:
		{
			mOrientation = glm::vec3(90.0f, 0.0f, 0.0f);
		}
		break;
		default:
		{
			;
		}
		}

		Update();
	}

	glm::vec3 Camera::Project(glm::vec3 obj)
	{
		return glm::project(obj, mMatrix, mProjectionMatrix, mViewport);
	}

	glm::vec3 Camera::UnProject(glm::vec3 win)
	{
		return glm::unProject(win, mMatrix, mProjectionMatrix, mViewport);
	}

	void Camera::GetRay(glm::vec2 mousePos, glm::vec3& origin, glm::vec3& direction)
	{
		mousePos -= glm::vec2(mViewport);

		switch (mProjectionModel)
		{
		case Ortho:
		{
			glm::vec3 cursor(mousePos.x - mViewport[2] / 2.0f, mousePos.y - mViewport[3] / 2.0f, mFar);
			cursor.x *= mAspect * mDistance / (2.0f * mUnitaryDistance);
			cursor.y *= mDistance / (2.0f * mUnitaryDistance);
			cursor.x /= mViewport[2] / 2.0f;
			cursor.y /= mViewport[3] / 2.0f;
			origin = glm::vec3(mInverseFinalMatrix * glm::vec4(cursor.x, cursor.y, -mOrthoNear, 1.0f));
			direction = mNormalBillboardMatrix * glm::vec3(0.0f, 0.0f, -1.0f);
		}
		break;
		case Perspective: default:
		{
			glm::vec3 cursor(mousePos.x - mViewport[2] / 2.0f, mousePos.y - mViewport[3] / 2.0f, -mFocalDistance);
			origin = glm::vec3(mInverseFinalMatrix * glm::vec4(cursor.x * mPerspectiveNear / mFocalDistance, cursor.y * mPerspectiveNear / mFocalDistance, -mPerspectiveNear, 1.0f));
			direction = glm::normalize(mNormalBillboardMatrix * cursor);
		}
		}
	}

	Camera::State::State(void)
	{
		Clear();
	}

	Camera::State::~State(void)
	{
		;
	}

	void Camera::State::Clear(void)
	{
		for (int i = 0; i < 3; ++i)
		{
			mKeyboardKeys[i] = false;
			mMouseButtons[i] = false;
		}
	}

	Camera::State& Camera::State::operator <<(KeyboardKey key)
	{
		mKeyboardKeys[key] = true;

		return *this;
	}

	Camera::State& Camera::State::operator >>(KeyboardKey key)
	{
		mKeyboardKeys[key] = false;

		return *this;
	}

	void Camera::State::SetPressed(KeyboardKey key, bool pressed)
	{
		mKeyboardKeys[key] = pressed;
	}

	bool Camera::State::GetPressed(KeyboardKey key)
	{
		return mKeyboardKeys[key];
	}

	Camera::State& Camera::State::operator <<(MouseButton button)
	{
		mMouseButtons[button] = true;

		return *this;
	}

	Camera::State& Camera::State::operator >>(MouseButton button)
	{
		mMouseButtons[button] = false;

		return *this;
	}

	void Camera::State::SetPressed(MouseButton button, bool pressed)
	{
		mMouseButtons[button] = pressed;
	}

	bool Camera::State::GetPressed(MouseButton button)
	{
		return mMouseButtons[button];
	}

	void Camera::MouseMove(State state, glm::vec2 mousePos)
	{
		mOldMousePos = mMousePos;
		mMousePos = mousePos;

		if (!mEnabled)
		{
			return;
		}

		glm::vec2 mouseDelta = mMousePos - mOldMousePos;

		if (mousePos.x >= mViewport[0] && mousePos.x <= mViewport[0] + mViewport[2] && mousePos.y >= mViewport[1] && mousePos.y <= mViewport[1] + mViewport[3])
		{
			switch (mStyle)
			{
			case Illusion: MouseMoveIllusion(state, -mouseDelta); break;
			case CustomGirl: MouseMoveCustomGirl(state, -mouseDelta); break;
			case Maya: MouseMoveMaya(state, -mouseDelta); break;
			case StudioMax: MouseMoveStudioMax(state, -mouseDelta); break;
			case Metasequoia: MouseMoveMetasequoia(state, -mouseDelta); break;
			case Blender: MouseMoveBlender(state, -mouseDelta); break;
			case MilkShape: MouseMoveMilkShape(state, -mouseDelta); break;
			default:;
			}
		}
	}

	void Camera::SetStyle(Style style)
	{
		mStyle = style;
	}

	Camera::Style Camera::GetStyle(void)
	{
		return mStyle;
	}

	void Camera::MouseMoveIllusion(State state, glm::vec2 mouseDelta)
	{
		float d = 2.0f * (1.0f / mZoom) * (mDistance / mFocalDistance);
		float f = 180.0f / mViewport[3];

		if (state.GetPressed(State::Left) && state.GetPressed(State::Right))
		{
			if (!mPivotLocked)
			{
				mPivot += mInverseNormalParentMatrix * (d * -mouseDelta.x) * mNormalBillboardMatrix * glm::vec3(1.0f, 0.0f, 0.0f);
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.y) * mNormalBillboardMatrix * glm::vec3(0.0f, 0.0f, 1.0f);
			}
		}
		else if (state.GetPressed(State::Right))
		{
			if (!mPivotLocked)
			{
				mPivot += mInverseNormalParentMatrix * glm::vec3(0.0f, (d * -mouseDelta.y), 0.0f);
			}

			if (!mDistanceLocked)
			{
				mDistance += (d * -mouseDelta.x) * mZoom;
			}
		}
		else if (state.GetPressed(State::Left))
		{
			if (!mOrientationLocked)
			{
				mOrientation += glm::vec3(f * -mouseDelta.y, f * mouseDelta.x, 0.0f);
			}
		}
		else if (state.GetPressed(State::Middle))
		{
			if (!mPivotLocked)
			{
				mPivot += mInverseNormalParentMatrix * (d * -mouseDelta.x) * mNormalBillboardMatrix * glm::vec3(1.0f, 0.0f, 0.0f);
				mPivot += mInverseNormalParentMatrix * (d * -mouseDelta.y) * mNormalBillboardMatrix * glm::vec3(0.0f, 1.0f, 0.0f);
			}
		}

		Update();
	}

	void Camera::MouseMoveCustomGirl(State state, glm::vec2 mouseDelta)
	{
		float d = 2.0f * (1.0f / mZoom) * (mDistance / mFocalDistance);
		float f = 180.0f / mViewport[3];

		if (state.GetPressed(State::Left) && state.GetPressed(State::Right))
		{
			if (!mPivotLocked)
			{
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.x) * mNormalBillboardMatrix * glm::vec3(1.0f, 0.0f, 0.0f);
				mPivot += mInverseNormalParentMatrix * (d * -mouseDelta.y) * mNormalBillboardMatrix * glm::vec3(0.0f, 0.0f, 1.0f);
			}
		}
		else if (state.GetPressed(State::Right))
		{
			if (!mDistanceLocked)
			{
				mDistance += (d * -mouseDelta.y) * mZoom;
			}
		}
		else if (state.GetPressed(State::Left))
		{
			if (!mOrientationLocked)
			{
				mOrientation += glm::vec3(f * -mouseDelta.y, f * mouseDelta.x, 0.0f);
			}
		}
		else if (state.GetPressed(State::Middle))
		{
			if (!mPivotLocked)
			{
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.x) * mNormalBillboardMatrix * glm::vec3(1.0f, 0.0f, 0.0f);
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.y) * mNormalBillboardMatrix * glm::vec3(0.0f, 1.0f, 0.0f);
			}
		}

		Update();
	}

	void Camera::MouseMoveMaya(State state, glm::vec2 mouseDelta)
	{
		float d = 2.0f * (1.0f / mZoom) * (mDistance / mFocalDistance);
		float f = 180.0f / mViewport[3];

		if (state.GetPressed(State::Alt) && state.GetPressed(State::Right))
		{
			if (!mDistanceLocked)
			{
				mDistance += (d * mouseDelta.x) * mZoom;
				mDistance += (d * -mouseDelta.y) * mZoom;
			}
		}
		else if (state.GetPressed(State::Alt) && state.GetPressed(State::Left))
		{
			if (!mOrientationLocked)
			{
				mOrientation += glm::vec3(f * -mouseDelta.y, f * mouseDelta.x, 0.0f);
			}
		}
		else if (state.GetPressed(State::Alt) && state.GetPressed(State::Middle))
		{
			if (!mPivotLocked)
			{
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.x) * mNormalBillboardMatrix * glm::vec3(1.0f, 0.0f, 0.0f);
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.y) * mNormalBillboardMatrix * glm::vec3(0.0f, 1.0f, 0.0f);
			}
		}

		Update();
	}

	void Camera::MouseMoveStudioMax(State state, glm::vec2 mouseDelta)
	{
		float d = 2.0f * (1.0f / mZoom) * (mDistance / mFocalDistance);
		float f = 180.0f / mViewport[3];

		if (state.GetPressed(State::Alt) && state.GetPressed(State::Middle))
		{
			if (!mOrientationLocked)
			{
				mOrientation += glm::vec3(f * -mouseDelta.y, f * mouseDelta.x, 0.0f);
			}
		}
		else if (state.GetPressed(State::Middle))
		{
			if (!mPivotLocked)
			{
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.x) * mNormalBillboardMatrix * glm::vec3(1.0f, 0.0f, 0.0f);
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.y) * mNormalBillboardMatrix * glm::vec3(0.0f, 1.0f, 0.0f);
			}
		}

		Update();
	}

	void Camera::MouseMoveMetasequoia(State state, glm::vec2 mouseDelta)
	{
		float d = 2.0f * (1.0f / mZoom) * (mDistance / mFocalDistance);
		float f = 180.0f / mViewport[3];

		if (state.GetPressed(State::Left) && state.GetPressed(State::Right))
		{
			if (!mDistanceLocked)
			{
				mDistance += (d * mouseDelta.y) * mZoom;
			}
		}
		else if (state.GetPressed(State::Right))
		{
			if (!mOrientationLocked)
			{
				mOrientation += glm::vec3(f * -mouseDelta.y, f * mouseDelta.x, 0.0f);
			}
		}
		else if (state.GetPressed(State::Middle))
		{
			if (!mPivotLocked)
			{
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.x) * mNormalBillboardMatrix * glm::vec3(1.0f, 0.0f, 0.0f);
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.y) * mNormalBillboardMatrix * glm::vec3(0.0f, 1.0f, 0.0f);
			}
		}

		Update();
	}

	void Camera::MouseMoveBlender(State state, glm::vec2 mouseDelta)
	{
		float d = 2.0f * (1.0f / mZoom) * (mDistance / mFocalDistance);
		float f = 180.0f / mViewport[3];

		if (state.GetPressed(State::Control) && state.GetPressed(State::Middle))
		{
			if (!mDistanceLocked)
			{
				mDistance += (d * mouseDelta.y) * mZoom;
			}
		}
		else if (state.GetPressed(State::Shift) && state.GetPressed(State::Middle))
		{
			if (!mPivotLocked)
			{
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.x) * mNormalBillboardMatrix * glm::vec3(1.0f, 0.0f, 0.0f);
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.y) * mNormalBillboardMatrix * glm::vec3(0.0f, 1.0f, 0.0f);
			}
		}
		else if (state.GetPressed(State::Middle))
		{
			if (!mOrientationLocked)
			{
				mOrientation += glm::vec3(f * -mouseDelta.y, f * mouseDelta.x, 0.0f);
			}
		}

		Update();
	}

	void Camera::MouseMoveMilkShape(State state, glm::vec2 mouseDelta)
	{
		float d = 2.0f * (1.0f / mZoom) * (mDistance / mFocalDistance);
		float f = 180.0f / mViewport[3];

		if (state.GetPressed(State::Left))
		{
			if (!mOrientationLocked)
			{
				mOrientation += glm::vec3(f * -mouseDelta.y, f * mouseDelta.x, 0.0f);
			}
		}
		else if (state.GetPressed(State::Middle))
		{
			if (!mPivotLocked)
			{
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.x) * mNormalBillboardMatrix * glm::vec3(1.0f, 0.0f, 0.0f);
				mPivot += mInverseNormalParentMatrix * (d * mouseDelta.y) * mNormalBillboardMatrix * glm::vec3(0.0f, 1.0f, 0.0f);
			}
		}

		Update();
	}

	void Camera::MouseWheel(State state, int wheelDelta, glm::vec2 mousePos)
	{
		if (!mEnabled || !wheelDelta)
		{
			return;
		}

		if (mousePos.x >= mViewport[0] && mousePos.x <= mViewport[0] + mViewport[2] && mousePos.y >= mViewport[1] && mousePos.y <= mViewport[1] + mViewport[3])
		{
			if (!mZoomLocked)
			{
				switch (mStyle)
				{
				case MilkShape:
				{
					mZoom *= wheelDelta > 0 ? 1.0f / 1.05f : 1.05f;
				}
				break;
				default:
				{
					mZoom *= wheelDelta > 0 ? 1.05f : 1.0f / 1.05f;
				}
				}
			}
		}

		Update();
	}

	void Camera::FitBoundingSphere(glm::vec3 center, float radius)
	{
		mPivot = center;
		mZoom = (mDistance / mFocalDistance) * (mViewport[3] / radius);

		Update();
	}

	void Camera::FitBoundingBox(glm::vec3 center, glm::vec3 halfExtends, glm::mat4 matrix)
	{
		glm::vec3 corner0 = glm::vec3(mInverseBillboardMatrix * matrix * glm::vec4(center + glm::vec3(1.0f, 1.0f, 1.0f) * halfExtends, 1.0f));
		glm::vec3 corner1 = glm::vec3(mInverseBillboardMatrix * matrix * glm::vec4(center + glm::vec3(1.0f, 1.0f, -1.0f) * halfExtends, 1.0f));
		glm::vec3 corner2 = glm::vec3(mInverseBillboardMatrix * matrix * glm::vec4(center + glm::vec3(-1.0f, 1.0f, -1.0f) * halfExtends, 1.0f));
		glm::vec3 corner3 = glm::vec3(mInverseBillboardMatrix * matrix * glm::vec4(center + glm::vec3(-1.0f, 1.0f, 1.0f) * halfExtends, 1.0f));
		glm::vec3 corner4 = glm::vec3(mInverseBillboardMatrix * matrix * glm::vec4(center + glm::vec3(1.0f, -1.0f, 1.0f) * halfExtends, 1.0f));
		glm::vec3 corner5 = glm::vec3(mInverseBillboardMatrix * matrix * glm::vec4(center + glm::vec3(1.0f, -1.0f, -1.0f) * halfExtends, 1.0f));
		glm::vec3 corner6 = glm::vec3(mInverseBillboardMatrix * matrix * glm::vec4(center + glm::vec3(-1.0f, -1.0f, -1.0f) * halfExtends, 1.0f));
		glm::vec3 corner7 = glm::vec3(mInverseBillboardMatrix * matrix * glm::vec4(center + glm::vec3(-1.0f, -1.0f, 1.0f) * halfExtends, 1.0f));
		float minY = corner0.y;
		float maxY = minY;
		if (corner1.y < minY)
		{
			minY = corner1.y;
		}
		else if (corner1.y > maxY)
		{
			maxY = corner1.y;
		}
		if (corner2.y < minY)
		{
			minY = corner2.y;
		}
		else if (corner2.y > maxY)
		{
			maxY = corner2.y;
		}
		if (corner3.y < minY)
		{
			minY = corner3.y;
		}
		else if (corner3.y > maxY)
		{
			maxY = corner3.y;
		}
		if (corner4.y < minY)
		{
			minY = corner4.y;
		}
		else if (corner4.y > maxY)
		{
			maxY = corner4.y;
		}
		if (corner5.y < minY)
		{
			minY = corner5.y;
		}
		else if (corner5.y > maxY)
		{
			maxY = corner5.y;
		}
		if (corner6.y < minY)
		{
			minY = corner6.y;
		}
		else if (corner6.y > maxY)
		{
			maxY = corner6.y;
		}
		if (corner7.y < minY)
		{
			minY = corner7.y;
		}
		else if (corner7.y > maxY)
		{
			maxY = corner7.y;
		}

		mPivot = center;
		mZoom = (mDistance / mFocalDistance) * (2.0f * mViewport[3] / (maxY - minY));

		Update();
	}

	void Camera::Initialize(void)
	{
		glGenBuffers(1, &mPivotPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mPivotPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, target_vertices * 3 * sizeof(float), target_v, GL_STATIC_DRAW);

		glGenBuffers(1, &mPivotFaceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPivotFaceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, target_faces * 3 * sizeof(unsigned int), target_f, GL_STATIC_DRAW);

		mPivotShader.CompileSource("uniform mat4 uFTransform;attribute vec3 aPosition;void main(){gl_Position = uFTransform * vec4(aPosition, 1.0);}", GL_VERTEX_SHADER);

		mPivotShader.CompileSource("void main(){gl_FragColor = vec4(1.0, 1.0, 1.0, 0.5);}", GL_FRAGMENT_SHADER);

		mPivotShader.Link();

		glGenBuffers(1, &mCameraPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mCameraPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, camera_vertices * 3 * sizeof(float), camera_v, GL_STATIC_DRAW);

		glGenBuffers(1, &mCameraNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mCameraNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, camera_vertices * 3 * sizeof(float), camera_vn, GL_STATIC_DRAW);

		glGenBuffers(1, &mCameraFaceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCameraFaceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, camera_faces * 3 * sizeof(unsigned int), camera_f, GL_STATIC_DRAW);

		mCameraShader.CompileSource("uniform mat4 uProjectionMatrix;uniform mat4 uModelViewMatrix;uniform mat3 uNormalMatrix;attribute vec3 aPosition;attribute vec3 aNormal;varying vec3 vPosition;varying vec3 vNormal;void main(){vec4 position = uModelViewMatrix * vec4(aPosition, 1.0);vPosition = vec3(position);vNormal = uNormalMatrix * aNormal;gl_Position = uProjectionMatrix * position;}", GL_VERTEX_SHADER);

		mCameraShader.CompileSource("varying vec3 vPosition;varying vec3 vNormal;void main(){vec3 eye = normalize(-vPosition);vec3 normal = normalize(vNormal);vec3 light = vec3(0.0, 0.0, 1.0);vec3 halfv = normalize(eye + light);float diffuse = max(0.0, dot(normal, light));float specular = pow(max(0.0, dot(normal, halfv)), 40.0);gl_FragColor = vec4(diffuse * vec3(0.1) + specular * vec3(0.1), 1.0);}", GL_FRAGMENT_SHADER);

		mCameraShader.Link();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mInitialized = true;
	}

}
