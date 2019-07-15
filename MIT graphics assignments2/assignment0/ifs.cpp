#include <stdlib.h>

#include "ifs.h"
#include "vectors.h"

Vec2f generateRandomVec2() {
	float x = float(rand()) / RAND_MAX;
	float y = float(rand()) / RAND_MAX;
	return Vec2f(x, y);
}

IFS::IFS(int transformCount) : transformCount(transformCount) {
	transformArray = new Matrix[transformCount];
	probabilityArray = new float[transformCount];
}

IFS::~IFS() {
	if (transformArray != nullptr) {
		delete[] transformArray;
		transformArray = nullptr;
	}
	if (probabilityArray != nullptr) {
		delete[] probabilityArray;
		probabilityArray = nullptr;
	}
}

void IFS::setMatrixAndProbabilityByIndex(int index, const Matrix& m, float probability) {
	assert(index >= 0 && index < transformCount);
	transformArray[index] = m;
	probabilityArray[index] = probability;
}

void IFS::render(Image& image, int numPoints, int numIters) {
	int width = image.Width();
	int height = image.Height();
	for (int i = 0; i < numPoints; ++i) {
		Vec2f v = generateRandomVec2();
		for (int j = 0; j < numIters; ++j) {
			int index = selectMatrixIndex();
			transformArray[index].Transform(v);
		}
		int w = width * v.x();
		int h = height * v.y();
		w = w % width;
		h = h % height;
		if (w < 0)
			w += width;
		if (h < 0)
			h += height;
		image.SetPixel(w, h, Vec3f(1.0f, 0.0f, 0.0f));
	}
}

int IFS::selectMatrixIndex() const{
	float r = float(rand()) / RAND_MAX;
	for (int i = 0; i < transformCount; ++i) {
		if (probabilityArray[i] > r)
			return i;
		r -= probabilityArray[i];
	}
	return transformCount - 1;
}

IFS* IFS::read(const char* fileName) {
	FILE *file = fopen(fileName, "r");
	assert(file != NULL);
	int numTransforms;
	fscanf(file, "%d", &numTransforms);
	IFS *pIfs = new IFS(numTransforms);
	for (int i = 0; i < numTransforms; ++i) {
		float probability;
		fscanf(file, "%f", &probability);
		Matrix m;
		m.Read3x3(file);
		pIfs->setMatrixAndProbabilityByIndex(i, m, probability);
	}
	fclose(file);
	return pIfs;
}
