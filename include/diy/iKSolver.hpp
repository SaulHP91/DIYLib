#ifndef IKSOLVER_H
#define IKSOLVER_H

#include <glm/glm.hpp>

void IKSolverCCD(glm::mat4* transformations, int transformations_count, glm::vec3 global_target, int max_iterations = 100, float epsilon = 0.001);

void IKSolverFast(glm::mat4* transformations, int transformations_count, glm::vec3 global_target);

#endif
 