//
// Created by ReFantasy on 2022/3/24.
//

#include "sph.h"


void display()
{
	static SPH sph;
	sph.mesh.Draw(sph.shader);
}
