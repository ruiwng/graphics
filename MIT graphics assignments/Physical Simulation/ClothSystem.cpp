#include "ClothSystem.h"
#include <GL/freeglut.h>

using namespace std;

namespace {
	void drawTriangle(const Vector3f& a, const Vector3f& b, const Vector3f& c) {
		Vector3f normal = Vector3f::cross(b - a, c - b);
		normal.normalize();
		glNormal3fv(normal);
		glVertex3fv(a);
		glVertex3fv(b);
		glVertex3fv(c);
	}
}

//TODO: Initialize here
ClothSystem::ClothSystem():mRowCount(30), mColumnCount(30)
{
	float step = 0.2f;
	float step2 = sqrt(2 * step * step);
	float fConstant = 10.0f;
	Vector3f start(-step * mColumnCount / 2, 2.0f, -2.0f);
	for (int i = 0; i < mRowCount; ++i) {
		for (int j = 0; j < mColumnCount; ++j) {
			m_vVecState.push_back(start + Vector3f(step * j, 0.0f, step * i));
			m_vVecState.push_back(Vector3f(0.0f, 0.0f, 0.0f));
		}
	}
	m_numParticles = mRowCount * mColumnCount;
	for (int i = 0; i < mRowCount; ++i) {
		for (int j = 0; j < mColumnCount; ++j) {
			int index = i * mColumnCount + j;
			// add structrual springs
			if (i + 1 < mRowCount)
				springManager.addSpring(step, fConstant, index, index + mColumnCount);
			if (j + 1 < mColumnCount)
				springManager.addSpring(step, fConstant, index, index + 1);
			
			// add shear springs
			if (i - 1 >= 0 && j + 1 < mColumnCount)
				springManager.addSpring(step2, fConstant, index, index - mColumnCount + 1);
			if(i + 1 <= mRowCount - 1 && j + 1 < mColumnCount)
				springManager.addSpring(step2, fConstant, index, index + mColumnCount + 1);

			// add flex springs
			if (i + 2 < mRowCount)
				springManager.addSpring(2 * step, fConstant, index, index + (mColumnCount << 1));
			if (j + 2 < mColumnCount)
				springManager.addSpring(2 * step, fConstant, index, index + 2);
			
			// generate faces
			if (i + 1 < mRowCount && j + 1 < mColumnCount) {
				faces.push_back(tuple<int, int, int>(index, index + mColumnCount, index + 1));
				faces.push_back(tuple<int, int, int>(index + mColumnCount, index + mColumnCount + 1, index + 1));
			}
		}
	}
	fixedParticles.insert(0);
	fixedParticles.insert(mColumnCount - 1);
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(const vector<Vector3f>& state) const
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
			Vector3f gravity = Vector3f(0.0f, -0.05f, 0.0f);
			Vector3f drag = -state[i + 1] * 0.1f;
			Vector3f total = (springForce + gravity + drag);
			f.push_back(total);
		}
	}
	// YOUR CODE HERE
	return f;
}

///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
{
	//for (int i = 0; i < m_numParticles; i++) {
	//	Vector3f pos = m_vVecState[i << 1];//  position of particle i. YOUR CODE HERE
	//	glPushMatrix();
	//	glTranslatef(pos[0], pos[1], pos[2]);
	//	glutSolidSphere(0.05f, 10.0f, 10.0f);
	//	glPopMatrix();
	//}
	
	// compute normals
	int index = 0;
	vector<Vector3f> normalArray;
	for (int i = 0; i < mRowCount; ++i) {
		for (int j = 0; j < mColumnCount; ++j) {
			int count = 0;
			Vector3f normal(0.0f, 0.0f, 0.0f);
			// upper-left triangle
			if (i > 0 && j > 0) {
				count += 1;
				normal += computeNormal(getPosition(index), getPosition(index - mColumnCount), getPosition(index - 1));
			}
			// lower-left triangle
			if (i < mRowCount - 1 && j > 0) {
				count += 1;
				normal += computeNormal(getPosition(index), getPosition(index - 1), getPosition(index + mColumnCount));
			}
			// lower-right triangle
			if (i < mRowCount - 1 && j < mColumnCount - 1) {
				count += 1;
				normal += computeNormal(getPosition(index), getPosition(index + mColumnCount), getPosition(index + 1));
			}
			// upper-right triangle
			if (i > 0 && j < mColumnCount - 1) {
				count += 1;
				normal += computeNormal(getPosition(index), getPosition(index + 1), getPosition(index - mColumnCount));
			}
			normal = normal / float(count);
			normal.normalize();
			normalArray.push_back(normal);
			index += 1;
		}
	}

	glBegin(GL_TRIANGLES);
	for (unsigned i = 0; i < faces.size(); ++i) {
		int index = get<0>(faces[i]);
		glNormal3fv(normalArray[index]);
		glVertex3fv(getPosition(index));

		index = get<1>(faces[i]);
		glNormal3fv(normalArray[index]);
		glVertex3fv(getPosition(index));

		index = get<2>(faces[i]);
		glNormal3fv(normalArray[index]);
		glVertex3fv(getPosition(index));
	}
	glEnd();

}

