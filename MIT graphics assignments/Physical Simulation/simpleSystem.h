#ifndef SIMPLESYSTEM_H
#define SIMPLESYSTEM_H

#include <GL/freeglut.h>
#include <vecmath.h>
#include <vector>

#include "particleSystem.h"

using namespace std;

class SimpleSystem: public ParticleSystem
{
public:
	SimpleSystem();
	
	vector<Vector3f> evalF(const vector<Vector3f>& state) const;
	
	void draw();
	
};

#endif
