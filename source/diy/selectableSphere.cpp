#include <diy/selectableSphere.hpp>
#include <diy/intersection.hpp>

namespace diy
{

	SelectableSphere::SelectableSphere(void) :
		mRadius(0.5f),
		mCenter(0.0f)
	{
		;
	}

	SelectableSphere::~SelectableSphere(void)
	{
		;
	}

	void SelectableSphere::SetRadius(float radius)
	{
		if (radius <= 0.0f)
		{
			return;
		}

		mRadius = radius;
	}

	float SelectableSphere::GetRadius(void)
	{
		return mRadius;
	}

	void SelectableSphere::SetCenter(glm::vec3 center)
	{
		mCenter = center;
	}

	glm::vec3 SelectableSphere::GetCenter(void)
	{
		return mCenter;
	}

	bool SelectableSphere::Intersect(glm::vec3 rayOrigin, glm::vec3 rayDirection)
	{
		float out;
		if (SphereIntersection(mCenter, mRadius, rayOrigin, rayDirection, &out))
		{
			mIntersection = rayOrigin + out * rayDirection;

			return true;
		}

		return false;
	}

}
