
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem():ParticleSystem(1)
{
	// position
	m_vVecState.push_back(Vector3f(1.0f, 1.0f, 1.0f));
	// velocity
	m_vVecState.push_back(Vector3f(0.0f, 0.0f, 0.0f));
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(const vector<Vector3f>& state) const
{
	vector<Vector3f> f;

	// YOUR CODE HERE
	f.push_back(Vector3f(-state[0].y(), state[0].x(), 0));
	f.push_back(Vector3f(0.0f, 0.0f, 0.0f));
	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
	Vector3f pos ;//YOUR PARTICLE POSITION
	glPushMatrix();
	glTranslatef(m_vVecState[0].x(), m_vVecState[0].y(), m_vVecState[0].z());
	glutSolidSphere(0.075f,10.0f,10.0f);
	glPopMatrix();
}
