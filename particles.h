#ifndef __SIMPLE_MODEL_H__
#define __SIMPLE_MODEL_H__

#include "GL/glew.h"
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "glsl_shader.h"

struct Vertex
{
	// position
	glm::vec3 Position = glm::vec3{ 0, 0, 0 };

	// normal
	//glm::vec3 Normal = glm::vec3{0, 1, 0};

	// color
	glm::vec3 Color = glm::vec3{ 1, 0, 0 };

	float density = 1.0;

	float pressure = 1000.0;
};

class Particles
{
public:

	bool GenGLBuffers();

	void Draw(const GLSLShader& shader);


	std::vector<Vertex> _vertices;

protected:
	float particle_radius = 0.005;

private:
	GLuint VAO = 0, VBO = 0;
	int triangle_num_of_circle = 10;

};

#endif //__SIMPLE_MODEL_H__
