#ifndef __SIMPLE_MODEL_H__
#define __SIMPLE_MODEL_H__
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "glsl_shader.h"

struct Vertex
{
	// position
	glm::vec3 Position = glm::vec3{ 0, 0, 0 };

	// normal
	glm::vec3 velocity = glm::vec3{0, 0, 0};

	// acceleration
	glm::vec3  a = glm::vec3 {0,0,0};

	// color
	glm::vec3 Color = glm::vec3{ 1, 0.8, 0.7 };

	float density = 1.0;

	float pressure = 1000.0;
};

class Particles
{
	friend class SPH;
public:

	bool GenGLBuffers();

	void Draw(const GLSLShader& shader);


	std::vector<Vertex> _vertices;

protected:
	float particle_radius = 0.01;

private:
	GLuint VAO = 0, VBO = 0;
	int triangle_num_of_circle = 6;

};

#endif //__SIMPLE_MODEL_H__
