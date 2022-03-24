//
// Created by ReFantasy on 2022/3/24.
//

#ifndef SPH_SPH_H
#define SPH_SPH_H
#include "basic_model.h"
#include "glsl_shader.h"
#include <random>

template <typename T, typename Distribution = std::uniform_real_distribution<T>>
T RandomNumber(const T &lo = 0, const T &hi = 1)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	Distribution dist(lo, hi);
	return dist(rng);
}

void display();

class SPH
{
public:
	SPH()
	{
		// 随机初始化一些粒子
		int n = 100;
		for(int i =0; i<n;i++)
		{
			Vertex v;
			v.Position= { RandomNumber<float>(-1,1),RandomNumber<float>(-1,1),0};
			mesh._vertices.push_back(v);
		}
//		Vertex v;
//		v.Position= {-0.2,0.5,0};
//		mesh._vertices.push_back(v);



		mesh.GenGLBuffers();

		shader.BuildInShader();

	}

	Particles mesh;
	GLSLShader shader;
};
#endif //SPH_SPH_H
