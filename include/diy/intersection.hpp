#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	DIYLIB_API bool PlaneIntersection
		(
		glm::vec3 X0,
		glm::vec3 N,
		glm::vec3 O,
		glm::vec3 D,
		float* out = 0
		);

	DIYLIB_API bool TriangleIntersection
		(
		glm::vec3 V0,
		glm::vec3 V1,
		glm::vec3 V2,
		glm::vec3 O,
		glm::vec3 D,
		glm::vec4* out = 0
		);

	DIYLIB_API bool SphereIntersection
		(
		glm::vec3 center,
		float radius,
		glm::vec3 rayOrigin,
		glm::vec3 rayDirection,
		float* out = 0
		);

}

#endif
