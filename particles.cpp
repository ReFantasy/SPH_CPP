#include "particles.h"
#include <iostream>


bool Particles::GenGLBuffers()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0],GL_DYNAMIC_DRAW); // GL_STATIC_DRAW GL_DYNAMIC_DRAW


	// set the vertex attribute pointers

	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

	// vertex normals
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// vertex Color
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

	glBindVertexArray(0);

	return true;
}

void Particles::Draw( const GLSLShader &shader)
{
	glUseProgram(shader.ID);
	shader.setInt(std::string("triangle_num_per_circle"),triangle_num_of_circle);
	shader.setFloat(std::string("circle_radius"),particle_radius);

	// 绑定顶点数组对象
    glBindVertexArray(VAO);

	// 更新粒子位置数据到GPU
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0],GL_DYNAMIC_DRAW);

	// 绘制
	glDrawArrays(GL_POINTS,0,_vertices.size());

	// 解除绑定
    glBindVertexArray(0);
	glUseProgram(0);

}

