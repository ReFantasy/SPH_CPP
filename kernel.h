//
// Created by ReFantasy on 2022/3/24.
//

#ifndef SPH_KERNEL_H
#define SPH_KERNEL_H
#include "glm/glm.hpp"

float cubic_kernel(glm::vec2 r, float support_radius);
glm::vec2 cubic_kernel_derivative(glm::vec2 r, float support_radius);
#endif //SPH_KERNEL_H
