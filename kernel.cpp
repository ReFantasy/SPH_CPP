//
// Created by ReFantasy on 2022/3/24.
//

#include "kernel.h"

#define pi (3.1415926)

float cubic_kernel(glm::vec2 r, float support_radius)
{
	int dim = 2;
	auto r_norm = std::sqrt(glm::dot(r, r));
	float res = 0;
	auto h = support_radius;

	float k = 40.0 / 7.0 / pi;
	k /= (h * h);
	auto q = r_norm / h;
	if (q <= 1.0)
	{
		if (q <= 0.5)
		{
			auto q2 = q * q;
			auto q3 = q2 * q;
			res = k * (6.0 * q3 - 6.0 * q2 + 1);
		}
		else
		{
			res = k * 2 * std::pow(1 - q, 3.0);
		}
	}
	return res;
}

glm::vec2 cubic_kernel_derivative(glm::vec2 r, float support_radius)
{
	int dim = 2;
	auto r_norm = std::sqrt(glm::dot(r, r));
	auto h = support_radius;
	float k = 40.0 / 7.0 / pi;
	k = 6. * k / h * h;
	auto q = r_norm / h;
	auto res = glm::vec2{ 0, 0 };
	if ((r_norm > 1e-5) && (q <= 1.0))
	{
		glm::vec2 grad_q = r / (r_norm * h);
		if (q <= 0.5)
		{
			res = glm::vec2(k * q * (3.0 * q - 2.0) * grad_q[0], k * q * (3.0 * q - 2.0) * grad_q[1]);
		}
		else
		{
			auto factor = 1.0 - q;
			auto s = k * (-factor * factor);
			res = glm::vec2(grad_q[0] * s, grad_q[1] * s);
		}
	}
	return res;
}