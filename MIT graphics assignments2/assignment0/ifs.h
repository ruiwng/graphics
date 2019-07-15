#ifndef  _IFS_H_
#define  _IFS_H_

#include "matrix.h"
#include "image.h"


class IFS {
public:
	IFS(int transformCount);
	~IFS();

	void setMatrixAndProbabilityByIndex(int index, const Matrix& m, float probability);
	static IFS* read(const char* fileName);
	void render(Image& image, int numPoints, int numIters);

private:
	int selectMatrixIndex() const;
	int transformCount;
	Matrix* transformArray;
	float* probabilityArray;
};

#endif