#ifndef  SPRING_MANAGER_H
#define  SPRING_MANAGER_H

#include <vector>
#include <map>
#include <Vector3f.h>

class ParticleSystem;

class Spring
{
public:
	Spring(float fRestLength, float fConstant, int p1, int p2) :fRestLength(fRestLength), fConstant(fConstant), p1(p1), p2(p2) {}
	
	int queryOtherPosition(int p) const;
	float getRestLength() const { return fRestLength; }
	float getConstant() const { return fConstant; }

private:
	int p1, p2;
	float fRestLength;
	float fConstant;
};


class SpringManager {
public:
	SpringManager() {}
	void addSpring(float rest_length, float constant, int p1, int p2);
	Vector3f querySpringForce(const std::vector<Vector3f>& state, int p) const;
private:
	std::vector<Spring> m_vSpringArray;
	std::map<int, std::vector<int> > m_ParticleToSpring;
};

#endif