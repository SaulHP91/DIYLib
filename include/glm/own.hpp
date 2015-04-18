#ifndef _GLM_OWN_
#define _GLM_OWN_

#include <glm/glm.hpp>

#include <cmath>

namespace glm
{

	__inline float fabs(float value)
	{
		return value < 0.0f ? -value : value;
	}

	__inline mat4 matrixToTranslate(mat4 matrix)
	{
		return translate(vec3(matrix[3]));
	}

	__inline mat4 matrixToRotate(mat4 matrix)
	{
		return mat4(normalize(matrix[0]), normalize(matrix[1]), normalize(matrix[2]), vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	__inline mat4 matrixToScale(mat4 matrix)
	{
		return scale(vec3(length(matrix[0]), length(matrix[1]), length(matrix[2])));
	}

	__inline vec3 matrixToTranslation(mat4 matrix)
	{
		return vec3(matrix[3]);
	}

	__inline vec3 matrixToEulerAngles(mat4 matrix)
	{
		return eulerAngles(toQuat(mat3(normalize(vec3(matrix[0])), normalize(vec3(matrix[1])), normalize(vec3(matrix[2])))));
	}

	vec3 matrixToRotation(mat4 matrix, glm::vec3 rotation = glm::vec3(0.0f))
	{
		static float x1;
		static float x2;
		static float y1;
		static float y2;
		static float z1;
		static float z2;

#ifndef GLM_FROCE_RADIANS
		rotation = glm::radians(rotation);
#endif

		if (matrix[0][2] > -0.99998 && matrix[0][2] < 0.99998)
		{
			y1 = -asin(matrix[0][2]);
			y2 = pi<float>() - y1;
			x1 = ::atan2(matrix[1][2] / cos(y1), matrix[2][2] / cos(y1));
			x2 = ::atan2(matrix[1][2] / cos(y2), matrix[2][2] / cos(y2));
			z1 = ::atan2(matrix[0][1] / cos(y1), matrix[0][0] / cos(y1));
			z2 = ::atan2(matrix[0][1] / cos(y2), matrix[0][0] / cos(y2));
			if (fabs(y1 - rotation.y) < fabs(y2 + rotation.y))
			{
				rotation.x = x1;
				rotation.y = y1;
				rotation.z = z1;
			}
			else
			{
				rotation.x = x2;
				rotation.y = y2;
				rotation.z = z2;
			}
		}
		else
		{
			if (matrix[0][2] < -0.99998)
			{
				rotation.y = half_pi<float>();
				rotation.x = rotation.z + ::atan2(matrix[1][0], matrix[2][0]);
			}
			else
			{
				rotation.y = -half_pi<float>();
				rotation.x = -rotation.z + ::atan2(-matrix[1][0], -matrix[2][0]);
			}
		}

#ifdef GLM_FORCE_RADIANS
		return rotation;
#else
		return degrees(rotation);
#endif
	}

	__inline vec3 matrixToScaling(mat4 matrix)
	{
		return vec3(length(matrix[0]), length(matrix[1]), length(matrix[2]));
	}

	__inline mat4 translationToTranslate(vec3 translation)
	{
		return translate(translation);
	}

	__inline mat4 rotationToRotate(vec3 rotation)
	{
		return glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	__inline mat4 scalingToScale(vec3 scaling)
	{
		return scale(scaling);
	}

	__inline mat4 compose(vec3 translation, vec3 rotation, vec3 scaling)
	{
		return translationToTranslate(translation) * rotationToRotate(rotation) * scalingToScale(scaling);
	}

};

#endif