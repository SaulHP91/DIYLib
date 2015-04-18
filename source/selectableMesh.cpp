#include "diy/selectableMesh.hpp"
#include "diy/intersection.hpp"

#include <glm/ext.hpp>

namespace diy
{

	SelectableMesh::SelectableMesh(void) :
		mVertexCount(0),
		mVerticesPointer(0),
		mFaceCount(0),
		mFacesPointer(0),
		mSelectedFaceIndex(-1),
		mInitializedBoundingSphere(false),
		mBoundingSphereCenter(0.0f),
		mBoundingSphereRadius(0.0f),
		mMatrix(1.0f),
		mInverseMatrix(1.0f)
	{
		;
	}

	SelectableMesh::~SelectableMesh(void)
	{
		;
	}

	void SelectableMesh::SetVertexCount(int vertexCount)
	{
		if (vertexCount < 0)
		{
			return;
		}

		mVertexCount = vertexCount;

		mInitializedBoundingSphere = false;
	}

	int SelectableMesh::GetVertexCount(void)
	{
		return mVertexCount;
	}

	void SelectableMesh::SetVerticesPointer(const float* verticesPointer)
	{
		mVerticesPointer = verticesPointer;

		mInitializedBoundingSphere = false;
	}

	const float* SelectableMesh::GetVerticesPointer(void)
	{
		return mVerticesPointer;
	}

	void SelectableMesh::SetFaceCount(int faceCount)
	{
		if (faceCount < 0)
		{
			return;
		}

		mFaceCount = faceCount;
	}

	int SelectableMesh::GetFaceCount(void)
	{
		return mFaceCount;
	}

	void SelectableMesh::SetFacesPointer(const int* facesPointer)
	{
		mFacesPointer = facesPointer;
	}

	const int* SelectableMesh::GetFacesPointer(void)
	{
		return mFacesPointer;
	}

	bool SelectableMesh::Pick(glm::vec3 rayOrigin, glm::vec3 rayDirection)
	{
		if (!mInitializedBoundingSphere)
		{
			InitializeBoundingSphere();
		}

		mSelectedFaceIndex = -1;

		if (!mVertexCount || !mVerticesPointer || !mFaceCount || !mFacesPointer || !mEnabled)
		{
			return false;
		}

		rayOrigin = glm::vec3(mInverseMatrix * glm::vec4(rayOrigin, 1.0f));
		rayDirection = glm::normalize(glm::vec3(mInverseMatrix * glm::vec4(rayDirection, 0.0f)));

		if (!SphereIntersection(mBoundingSphereCenter, mBoundingSphereRadius, rayOrigin, rayDirection))
		{
			return false;
		}

		glm::vec4 nearest;
		for (int i = 0; i < mFaceCount; ++i)
		{
			glm::vec3 v1 = glm::vec3(mVerticesPointer[3 * mFacesPointer[3 * i]], mVerticesPointer[3 * mFacesPointer[3 * i] + 1], mVerticesPointer[3 * mFacesPointer[3 * i] + 2]);
			glm::vec3 v2 = glm::vec3(mVerticesPointer[3 * mFacesPointer[3 * i + 1]], mVerticesPointer[3 * mFacesPointer[3 * i + 1] + 1], mVerticesPointer[3 * mFacesPointer[3 * i + 1] + 2]);
			glm::vec3 v3 = glm::vec3(mVerticesPointer[3 * mFacesPointer[3 * i + 2]], mVerticesPointer[3 * mFacesPointer[3 * i + 2] + 1], mVerticesPointer[3 * mFacesPointer[3 * i + 2] + 2]);
			glm::vec4 out;
			if
				(
				TriangleIntersection
				(
				v1,
				v2,
				v3,
				rayOrigin,
				rayDirection,
				&out
				)
				)
			{
				if (mSelectedFaceIndex < 0)
				{
					mSelectedFaceIndex = i;
					nearest = out;
				}
				else
				{
					if (out[3] < nearest[3])
					{
						mSelectedFaceIndex = i;
						nearest = out;
					}
				}
			}
		}

		if (mSelectedFaceIndex < 0)
		{
			return false;
		}

		glm::vec3 v1 = glm::vec3(mVerticesPointer[3 * mFacesPointer[3 * mSelectedFaceIndex]], mVerticesPointer[3 * mFacesPointer[3 * mSelectedFaceIndex] + 1], mVerticesPointer[3 * mFacesPointer[3 * mSelectedFaceIndex] + 2]);
		glm::vec3 v2 = glm::vec3(mVerticesPointer[3 * mFacesPointer[3 * mSelectedFaceIndex + 1]], mVerticesPointer[3 * mFacesPointer[3 * mSelectedFaceIndex + 1] + 1], mVerticesPointer[3 * mFacesPointer[3 * mSelectedFaceIndex + 1] + 2]);
		glm::vec3 v3 = glm::vec3(mVerticesPointer[3 * mFacesPointer[3 * mSelectedFaceIndex + 2]], mVerticesPointer[3 * mFacesPointer[3 * mSelectedFaceIndex + 2] + 1], mVerticesPointer[3 * mFacesPointer[3 * mSelectedFaceIndex + 2] + 2]);
		mIntersection = glm::vec3(mMatrix * glm::vec4(nearest[0] * v1 + nearest[1] * v2 + nearest[2] * v3, 1.0f));

		return true;
	}

	int SelectableMesh::GetSelectedFaceIndex(void)
	{
		return mSelectedFaceIndex;
	}

	void SelectableMesh::InitializeBoundingSphere(void)
	{
		if (mInitializedBoundingSphere)
		{
			return;
		}

		if (!mVertexCount || !mVerticesPointer)
		{
			return;
		}

		glm::vec3 min(mVerticesPointer[0], mVerticesPointer[1], mVerticesPointer[2]);
		glm::vec3 max(min);

		for (int i = 1; i < mVertexCount; ++i)
		{
			if (mVerticesPointer[3 * i] < min.x)
			{
				min.x = mVerticesPointer[3 * i];
			}
			else if (mVerticesPointer[3 * i] > max.x)
			{
				max.x = mVerticesPointer[3 * i];
			}

			if (mVerticesPointer[3 * i + 1] < min.y)
			{
				min.y = mVerticesPointer[3 * i + 1];
			}
			else if (mVerticesPointer[3 * i + 1] > max.y)
			{
				max.y = mVerticesPointer[3 * i + 1];
			}

			if (mVerticesPointer[3 * i + 2] < min.z)
			{
				min.z = mVerticesPointer[3 * i + 2];
			}
			else if (mVerticesPointer[3 * i + 2] > max.z)
			{
				max.z = mVerticesPointer[3 * i + 2];
			}
		}

		mBoundingSphereCenter = (min + max) / 2.0f;
		mBoundingSphereRadius = glm::length(max - mBoundingSphereCenter);

		mInitializedBoundingSphere = true;
	}

	void SelectableMesh::SetMatrix(glm::mat4 matrix)
	{
		mMatrix = matrix;

		mInverseMatrix = glm::inverse(mMatrix);
	}

	glm::mat4 SelectableMesh::GetMatrix(void)
	{
		return mMatrix;
	}

	glm::mat4 SelectableMesh::GetInverseMatrix(void)
	{
		return mInverseMatrix;
	}

}
