#include "diy\selectablePointCloud.hpp"
#include "diy\camera.hpp"
#include "diy\intersection.hpp"

#include <glm/glm.hpp>

namespace diy
{

	SelectablePointCloud::SelectablePointCloud(void) :
		mPointCount(0),
		mPointsPointer(0),
		mPointSize(1.0f),
		mSelectedPointIndex(-1),
		mCamera(0),
		mInitializedBoundingSphere(false),
		mBoundingSphereCenter(0.0f),
		mBoundingSphereRadius(0.0f),
		mMatrix(1.0f),
		mInverseMatrix(1.0f)
	{
		;
	}

	SelectablePointCloud::~SelectablePointCloud(void)
	{
		;
	}

	void SelectablePointCloud::SetPointCount(int pointCount)
	{
		if (pointCount < 0)
		{
			return;
		}

		mPointCount = pointCount;

		mInitializedBoundingSphere = false;
	}

	int SelectablePointCloud::GetPointCount(void)
	{
		return mPointCount;
	}

	void SelectablePointCloud::SetPointsPointer(const float* pointsPointer)
	{
		mPointsPointer = pointsPointer;

		mInitializedBoundingSphere = false;
	}

	const float* SelectablePointCloud::GetPointsPointer(void)
	{
		return mPointsPointer;
	}

	void SelectablePointCloud::SetPointSize(float pointSize)
	{
		if (pointSize < 1.0f)
		{
			return;
		}

		mPointSize = pointSize;
	}

	float SelectablePointCloud::GetPointSize(void)
	{
		return mPointSize;
	}

	bool SelectablePointCloud::Pick(glm::vec3 rayOrigin, glm::vec3 rayDirection)
	{
		if (!mInitializedBoundingSphere)
		{
			InitializeBoundingSphere();
		}

		mSelectedPointIndex = -1;

		if (!mPointCount || !mPointsPointer || !mCamera)
		{
			return false;
		}

		rayOrigin = glm::vec3(mInverseMatrix * glm::vec4(rayOrigin, 1.0f));
		rayDirection = glm::normalize(glm::vec3(mInverseMatrix * glm::vec4(rayDirection, 0.0f)));

		if (!SphereIntersection(mBoundingSphereCenter, mBoundingSphereRadius, rayOrigin, rayDirection))
		{
			return false;
		}

		float nearest;
		for (int i = 0; i < mPointCount; ++i)
		{
			glm::vec3 c = glm::vec3(mPointsPointer[3 * i], mPointsPointer[3 * i + 1], mPointsPointer[3 * i + 2]);
			float r = mCamera->GetScaleFactor(c, mPointSize / 2.0f);
			float out;
			if (!SphereIntersection(c, r, rayOrigin, rayDirection, &out))
			{
				continue;
			}

			if (mSelectedPointIndex < 0)
			{
				mSelectedPointIndex = i;
				nearest = out;
			}
			else
			{
				if (out < nearest)
				{
					mSelectedPointIndex = i;
					nearest = out;
				}
			}
		}

		if (mSelectedPointIndex < 0)
		{
			return false;
		}

		mIntersection = glm::vec3(mMatrix * glm::vec4(rayOrigin + nearest * rayDirection, 1.0f));

		return true;
	}

	int SelectablePointCloud::GetSelectedPointIndex(void)
	{
		return mSelectedPointIndex;
	}

	void SelectablePointCloud::SetCamera(Camera* camera)
	{
		mCamera = camera;
	}

	Camera* SelectablePointCloud::GetCamera(void)
	{
		return mCamera;
	}

	void SelectablePointCloud::InitializeBoundingSphere(void)
	{
		if (mInitializedBoundingSphere)
		{
			return;
		}

		if (!mPointCount || !mPointsPointer)
		{
			return;
		}

		glm::vec3 min(mPointsPointer[0], mPointsPointer[1], mPointsPointer[2]);
		glm::vec3 max(min);

		for (int i = 1; i < mPointCount; ++i)
		{
			if (mPointsPointer[3 * i] < min.x)
			{
				min.x = mPointsPointer[3 * i];
			}
			else if (mPointsPointer[3 * i] > max.x)
			{
				max.x = mPointsPointer[3 * i];
			}

			if (mPointsPointer[3 * i + 1] < min.y)
			{
				min.y = mPointsPointer[3 * i + 1];
			}
			else if (mPointsPointer[3 * i + 1] > max.y)
			{
				max.y = mPointsPointer[3 * i + 1];
			}

			if (mPointsPointer[3 * i + 2] < min.z)
			{
				min.z = mPointsPointer[3 * i + 2];
			}
			else if (mPointsPointer[3 * i + 2] > max.z)
			{
				max.z = mPointsPointer[3 * i + 2];
			}
		}

		mBoundingSphereCenter = (min + max) / 2.0f;
		mBoundingSphereRadius = glm::length(max - mBoundingSphereCenter);

		mInitializedBoundingSphere = true;
	}

	void SelectablePointCloud::SetMatrix(glm::mat4 matrix)
	{
		mMatrix = matrix;

		mInverseMatrix = glm::inverse(mMatrix);
	}

	glm::mat4 SelectablePointCloud::GetMatrix(void)
	{
		return mMatrix;
	}

	glm::mat4 SelectablePointCloud::GetInverseMatrix(void)
	{
		return mInverseMatrix;
	}

}
