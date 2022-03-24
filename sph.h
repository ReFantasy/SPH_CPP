//
// Created by ReFantasy on 2022/3/24.
//

#ifndef SPH_SPH_H
#define SPH_SPH_H
#include <memory>
#include "particles.h"
#include "glsl_shader.h"
#include <random>
#include <map>
#include <vector>

template <typename T, typename Distribution = std::uniform_real_distribution<T>>
T RandomNumber(const T &lo = 0, const T &hi = 1)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	Distribution dist(lo, hi);
	return dist(rng);
}

void display();

class Grid;

class SPH
{
public:
	SPH();
	void Draw();

	void Init();
	void Substep();
	void Advect();
public:
	std::shared_ptr<Grid> grid;

private:
	Particles particles;
	GLSLShader shader;
};

class Grid
{
public:
	Grid()
	{
		int grid_num = std::ceil(2.0/grid_size);
		for(int i = 0;i<grid_num;i++)
		{
			for(int j = 0;j<grid_num;j++)
			{
				_grid_map[std::pair<int,int>(i,j)] = std::vector<int>{};
			}
		}
	}
	void Reset()
	{
		int grid_num = std::ceil(2.0/grid_size);
		for(int i = 0;i<grid_num;i++)
		{
			for(int j = 0;j<grid_num;j++)
			{
				//_grid_map[std::pair<int,int>(i,j)].clear();
			}
		}
	}
	void Add(float x, float y, int index)
	{
		int i = std::floor((x+1)/grid_size);
		int j = std::floor((y+1)/grid_size);
		_grid_map[std::pair<int,int>(i,j)].push_back(index);
	}
	std::vector<int> Findneighbor(float x, float y)
	{
		int grid_num = std::ceil(2.0/grid_size);
		int i = std::floor((x+1)/grid_size);
		int j = std::floor((y+1)/grid_size);
		auto is_vaid = [&grid_num](int i, int j)
		{
			if(i<0||j<0)
				return false;
			if(i>=grid_num || j>=grid_num)
				return false;
			return true;
		};
		std::vector<int> res;
		for(int k=i-1;k<=i+1;k++)
		{
			for(int m = j-1;m<=j+1;m++)
			{
				if(is_vaid(k,m))
				{
					auto &v = _grid_map[std::pair<int,int>(k,m)];
					//res.insert(res.end(), v.begin(),v.end());
					for(auto e:v)
					{
						res.push_back(e);
					}
				}
			}
		}
		return res;
	}
	float grid_size = 0.1;

	std::map<std::pair<int,int>, std::vector<int>> _grid_map;
};
#endif //SPH_SPH_H
