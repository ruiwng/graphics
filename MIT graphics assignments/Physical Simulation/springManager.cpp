#include  "springManager.h"
#include  "particleSystem.h"
#include  <assert.h>
using namespace std;


int Spring::queryOtherPosition(int p) const {
	assert(p == p1 || p == p2);
	if (p == p1)
		return p2;
	else
		return p1;
}

void SpringManager::addSpring(float rest_length, float constant, int p1, int p2) {
	m_vSpringArray.push_back(Spring(rest_length, constant, p1, p2));
	int index = m_vSpringArray.size() - 1;

	if (m_ParticleToSpring.find(p1) == m_ParticleToSpring.end())
		m_ParticleToSpring[p1] = vector<int>();
	m_ParticleToSpring[p1].push_back(index);

	if (m_ParticleToSpring.find(p2) == m_ParticleToSpring.end())
		m_ParticleToSpring[p2] = vector<int>();
	m_ParticleToSpring[p2].push_back(index);
}

Vector3f SpringManager::querySpringForce(const vector<Vector3f> &state, int p) const {
	auto iter = m_ParticleToSpring.find(p);
	if ( iter == m_ParticleToSpring.end())
		return Vector3f(0.0f, 0.0f, 0.0f);

	Vector3f position = state[p << 1];
	auto springArray = iter->second;
	Vector3f force(0.0f, 0.0f, 0.0f);

	for (auto spring_index : springArray) {
		const Spring& spring = m_vSpringArray[spring_index];
		int other_p = spring.queryOtherPosition(p);
		Vector3f other_position = state[other_p << 1];
		Vector3f x(other_position - position);
		float length = x.abs();
		x.normalize();
		float restLength = spring.getRestLength();
		float constant = spring.getConstant();
		force += (length - restLength) * constant * x;

	}
	return force;
}