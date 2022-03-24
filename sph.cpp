//
// Created by ReFantasy on 2022/3/24.
//

#include "sph.h"
#include "kernel.h"

int n = 10;
float dist_per_particle = 1.0 / n;
float density_0 = 100;
float k1 = 50;
float k2 = 25;
float viscosity = 0.05;
float m_v = dist_per_particle * dist_per_particle * 0.8;
float mass = density_0 * m_v;
// 积分步长
float dt = 1e-3;
// 重力加速度
glm::vec3 g = {0,-9.8,0};

void display()
{
	static SPH sph;
	sph.Substep();
	sph.Advect();
	sph.Draw();
}

SPH::SPH()
{
	grid = std::make_shared<Grid>();
	grid->grid_size = dist_per_particle*4;
	grid->Reset();

	shader.BuildInShader();
	Init();
}

void SPH::Draw()
{
	particles.Draw(shader);
}


void SPH::Init()
{

	// 初始化 n*n 的粒子
	glm::vec3 init_pos = { -0.5, 0, 0 };
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			Vertex v;
			v.Position = glm::vec3{ i * dist_per_particle, j * dist_per_particle, 0 } + init_pos;
			particles._vertices.push_back(v);
		}

	}


//    int n = 1000;
//	for(int i = 0;i<n;i++)
//	{
//		Vertex v;
//		v.Position = glm::vec3 (RandomNumber<float>(-1,1),RandomNumber<float>(-1,1),0);
//		particles._vertices.push_back(v);
//	}
//
//	Grid grid;
//	for(int i = 0;i<n;i++)
//	{
//		auto &v = particles._vertices[i];
//		grid.Add(v.Position[0],v.Position[1],i);
//	}
//
//	auto res = grid.Findneighbor(-0.5,-0.5);
//	for(auto i:res)
//	{
//		particles._vertices[i].Color = glm::vec3 (0,0,1);
//	}


	particles.GenGLBuffers();

}

void SPH::Substep()
{
//	grid->Reset();
//	for(int i=0;i<particles._vertices.size();i++)
//	{
//		grid->Add(particles._vertices[i].Position[0],particles._vertices[i].Position[1], i);
//	}

	float s = dist_per_particle*4;

	// 更新密度
	for(int i=0;i<particles._vertices.size();i++)
	{
		auto & d = particles._vertices[i].density;
		d = 0;

		for(int j=0;j<particles._vertices.size();j++)
		{
			auto r = particles._vertices[i].Position-particles._vertices[j].Position;
			d += mass* cubic_kernel(glm::vec2 {r[0],r[1]},s);
		}
		d = std::max(d,density_0);
	}

	// 更新压力
	for(int i=0;i<particles._vertices.size();i++)
	{
		auto &pressure = particles._vertices[i].pressure;
		auto &density = particles._vertices[i].density;
		pressure = std::max((float )0.0, (float)(k1 * density_0 * (std::pow(density / density_0, k2) - 1) ));
	}

	// 更新加速度
	for(int i=0;i<particles._vertices.size();i++)
	{
		particles._vertices[i].a = g;
	}

	//  non-pressureforces
	for(int i=0;i<particles._vertices.size();i++)
	{
		for(int j=0;j<particles._vertices.size();j++)
		{
			auto &vi = particles._vertices[i];
			auto &vj = particles._vertices[j];
			float v_xy = glm::dot(vi.velocity-vj.velocity, vi.Position-vj.Position);
			int dim = 2;

			float s1 = viscosity*2*(dim+2)*mass/vj.density*v_xy/
					   (glm::dot(vi.Position-vj.Position,vi.Position-vj.Position)+0.01*dt*dt);
			glm::vec3 r = vi.Position-vj.Position;
			glm::vec2 v2 = cubic_kernel_derivative(glm::vec2 (r[0],r[1]),s);
			vi.a += glm::vec3 (v2[0]*s1,v2[1]*s1,0);
		}



	}

	// pressure forces
	for(int i=0;i<particles._vertices.size();i++)
	{
		for(int j=0;j<particles._vertices.size();j++)
		{
			auto &vi = particles._vertices[i];
			auto &vj = particles._vertices[j];
			glm::vec3 r = vi.Position-vj.Position;

			float s1 = -mass*(vi.pressure/std::pow(vi.density,2)+vj.pressure/std::pow(vj.density,2));
			glm::vec2 v2 = cubic_kernel_derivative(glm::vec2(r[0],r[1]),s);
			vi.a += glm::vec3 (v2[0]*s1,v2[1]*s1,0);
		}



	}
}

void SPH::Advect()
{
	for(int i=0;i<particles._vertices.size();i++)
	{
		auto &vert = particles._vertices[i];
		vert.velocity += vert.a*dt;
		vert.Position += vert.velocity*dt;

		// 边界处理
		if(vert.Position[1]<-1)
		{
			vert.Position[1] = -1;
			vert.velocity *=-1;
		}
		if(vert.Position[1]>1)
		{
			vert.Position[1] = 1;
			vert.velocity[1] = 0;
		}
		if(vert.Position[0]<-1)
		{
			vert.Position[0] = -1;
			vert.velocity[0] = 0;
		}
		if(vert.Position[0]>1)
		{
			vert.Position[0] = 1;
			vert.velocity[0] = 0;
		}
	}
}

