#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vecmath.h>
#include <vector>
#include <set>
#include <GL/freeglut.h>

#include "particleSystem.h"
#include "springManager.h"

class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(int numParticles);
	
	std::vector<Vector3f> evalF(const std::vector<Vector3f>& state) const;
	
	void draw();
private:
	SpringManager springManager;
	std::set<int> fixedParticles;
};

#endif
