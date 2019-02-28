#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vecmath.h>


class ParticleSystem
{
public:

	ParticleSystem(int numParticles=0);

	int m_numParticles;
	
	// for a given state, evaluate derivative f(X,t)
	virtual std::vector<Vector3f> evalF(const std::vector<Vector3f>& state) const = 0;
	
	// getter method for the system's state
	std::vector<Vector3f> getState(){ return m_vVecState; };
	
	// setter method for the system's state
	void setState(const std::vector<Vector3f>  & newState) { m_vVecState = newState; };
	
	virtual void draw() = 0;
	
	Vector3f getPosition(int index) const { return m_vVecState[index << 1]; }
protected:

	std::vector<Vector3f> m_vVecState;
	
};

#endif
