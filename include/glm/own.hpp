#ifndef GLMOWN_HPP
#define GLMOWN_HPP

#include <glm/glm.hpp>

namespace glm
{
	float fabs(float value);

	mat4 matrixToTranslate(mat4 matrix);

	mat4 matrixToRotate(mat4 matrix);

	mat4 matrixToScale(mat4 matrix);

	vec3 matrixToTranslation(mat4 matrix);

	vec3 matrixToEulerAngles(mat4 matrix);

	vec3 matrixToRotation(mat4 matrix, glm::vec3 rotation = glm::vec3(0.0f));

	vec3 matrixToScaling(mat4 matrix);

	mat4 translationToTranslate(vec3 translation);

	mat4 rotationToRotate(vec3 rotation);

	mat4 scalingToScale(vec3 scaling);

	mat4 compose(vec3 translation, vec3 rotation, vec3 scaling);
};

#endif