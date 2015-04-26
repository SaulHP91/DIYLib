#include <diy/selectablePointCloud.hpp>
#include <diy/selector.hpp>
#include <diy/camera.hpp>
#include <diy/intersection.hpp>

#include <glm/glm.hpp>

namespace diy
{

	SelectablePointCloud::SelectablePointCloud(void) :
		mCamera(0),
		mPointCount(0),
		mPointsPointer(0),
		mPointSize(1.0f),
		mSelectedPointIndex(-1),
		mMatrix(1.0f),
		mInverseMatrix(1.0f)
	{
		;
	}

	SelectablePointCloud::~SelectablePointCloud(void)
	{
		;
	}


	void SelectablePointCloud::SetCamera(Camera* camera)
	{
		mCamera = camera;
	}

	Camera* SelectablePointCloud::GetCamera(void)
	{
		return mCamera;
	}

	void SelectablePointCloud::SetPointCount(int pointCount)
	{
		if (pointCount < 0)
		{
			return;
		}

		mPointCount = pointCount;
	}

	int SelectablePointCloud::GetPointCount(void)
	{
		return mPointCount;
	}

	void SelectablePointCloud::SetPointsPointer(const float* pointsPointer)
	{
		mPointsPointer = pointsPointer;
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

	bool SelectablePointCloud::Intersect(glm::vec3 rayOrigin, glm::vec3 rayDirection)
	{
		mSelectedPointIndex = -1;

		Camera* camera = mCamera;
		if (!camera && mSelector)
		{
			camera = mSelector->GetCamera();
		}

		if (!mPointCount || !mPointsPointer || !camera)
		{
			return false;
		}

		rayOrigin = glm::vec3(mInverseMatrix * glm::vec4(rayOrigin, 1.0f));
		rayDirection = glm::normalize(glm::vec3(mInverseMatrix * glm::vec4(rayDirection, 0.0f)));

		float nearest;
		for (int i = 0; i < mPointCount; ++i)
		{
			glm::vec3 c = glm::vec3(mPointsPointer[3 * i], mPointsPointer[3 * i + 1], mPointsPointer[3 * i + 2]);
			float r = camera->GetScaleFactor(c, mPointSize / 2.0f);
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
