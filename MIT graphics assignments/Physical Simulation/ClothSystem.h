#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>
#include <set>
#include <tuple>
#include "particleSystem.h"
#include "springManager.h"



class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem();
	std::vector<Vector3f> evalF(const std::vector<Vector3f>& state) const;
	
	void draw();

private:
	inline static Vector3f computeNormal(const Vector3f& a, const Vector3f& b, const Vector3f& c) {
		return Vector3f::cross(b - a, c - b).normalized();
	}
	SpringManager springManager;
	std::set<int> fixedParticles;
	std::vector<std::tuple<int, int, int> > faces;
	int mRowCount;
	int mColumnCount;
};


#endif
