#ifndef _CSG_H_
#define _CSG_H_


#include "object3D.h"
#include "vectors.h"
#include "material.h"

enum CSG_OP {
	CSG_UNION,
	CSG_INTERSECTION,
	CSG_DIFFERENCE,
};


class CSG : public Object3D {
public:
	CSG(Object3D *leftChild, Object3D *rightChild, CSG_OP operation);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual bool intersectAll(const Ray &r, std::vector<HitPair> &hitArray);

	static std::vector<HitPair> computeIntersection(const std::vector<HitPair> &leftArray, const std::vector<HitPair> &rightArray);
	static std::vector<HitPair> computeUnion(const std::vector<HitPair> &leftArray, const std::vector<HitPair> &rightArray);
	static std::vector<HitPair> computeDifference(const std::vector<HitPair> &leftArray, const std::vector<HitPair> &rightArray);
private:
	Object3D *leftChild;
	Object3D *rightChild;
	CSG_OP operation;
};

#endif
