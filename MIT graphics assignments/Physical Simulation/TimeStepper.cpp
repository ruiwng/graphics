#include "TimeStepper.hpp"

using namespace std;
///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	auto state = particleSystem->getState();
	auto f = particleSystem->evalF(state);
	auto count = state.size();
	vector<Vector3f> new_state;
	for (auto i = 0; i < count; i += 2) {
		auto &position = state[i];
		auto &velocity = f[i];
		auto &acceleration = f[i + 1];
		auto new_position = position + velocity * stepSize;
		auto new_velocity = velocity + acceleration * stepSize;
		new_state.push_back(new_position);
		new_state.push_back(new_velocity);
	}
	particleSystem->setState(new_state);
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	auto state = particleSystem->getState();
	auto f = particleSystem->evalF(state);
	auto count = state.size();
	vector<Vector3f> new_state;
	for (auto i = 0; i < count; i += 2) {
		auto &position = state[i];
		auto &velocity = f[i];
		auto &acceleration = f[i + 1];
		auto new_position = position + velocity * stepSize;
		auto new_velocity = velocity + acceleration * stepSize;
		new_state.push_back(new_position);
		new_state.push_back(new_velocity);
	}
	auto f2 = particleSystem->evalF(new_state);
	vector<Vector3f> final_state;
	for (auto i = 0; i < count; i += 2) {
		auto &position = state[i];
		auto &velocity1 = f[i];
		auto &acceleration1 = f[i + 1];
		auto &velocity2 = f2[i];
		auto &acceleration2 = f2[i + 1];
		auto new_position = position + stepSize * (velocity1 + velocity2) / 2;
		auto new_velocity = velocity1 + stepSize * (acceleration1 + acceleration2) / 2;
		final_state.push_back(new_position);
		final_state.push_back(new_velocity);
	}
	particleSystem->setState(final_state);
}

void RK4::takeStep(ParticleSystem* particleSystem, float stepSize) {

}
