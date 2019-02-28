#include "pendulumSystem.h"

using namespace std;

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		
		// for this system, we care about the position and the velocity
		m_vVecState.push_back(Vector3f(1.0f * i, 0.0f, 0.0f));
		m_vVecState.push_back(Vector3f(0.0f, 0.0f, 0.0f));
		if (i != 0)
			springManager.addSpring(1.0f, 5.0f, i - 1, i);
	}
	fixedParticles.insert(0);
	//fixedParticles.insert(m_numParticles - 1);
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(const vector<Vector3f>& state) const
{
	vector<Vector3f> f;
	int num = state.size();
	for (int i = 0; i < num; i += 2) {
		f.push_back(state[i + 1]);
		int index = i >> 1;
		if (fixedParticles.find(index) != fixedParticles.end())
			f.push_back(Vector3f(0.0f, 0.0f, 0.0f));
		else
		{
			Vector3f springForce = springManager.querySpringForce(state, index);
			Vector3f gravity = Vector3f(0.0f, -2.0f, 0.0f);
			Vector3f drag = -state[i + 1] * 0.01f;
			f.push_back(springForce + gravity + drag);
		}
	}
	// YOUR CODE HERE
	
	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = m_vVecState[i << 1];//  position of particle i. YOUR CODE HERE
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glutSolidSphere(0.2f,10.0f,10.0f);
		glPopMatrix();
	}
}
