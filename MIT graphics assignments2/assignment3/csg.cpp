#include "csg.h"
#include "ray.h"
#include "hit.h"

CSG::CSG(Object3D *leftChild, Object3D *rightChild, CSG_OP operation) : leftChild(leftChild), rightChild(rightChild), operation(operation) {}

bool CSG::intersect(const Ray &r, Hit &h, float tmin) {
	std::vector<HitPair> hitArray;
	bool isHit = intersectAll(r, hitArray);
	if (!isHit)
		return false;
	for (auto &hitInfo : hitArray) {
		const Hit &first = hitInfo.first;
		if (first.getT() < h.getT() && first.getT() >= tmin) {
			h = first;
			return true;
		}
		const Hit &second = hitInfo.second;
		if (second.getT() < h.getT() && second.getT() >= tmin) {
			h = second;
			return true;
		}
		
	}
	return false;
}

bool CSG::intersectAll(const Ray &r, std::vector<HitPair> &hitArray) {
	std::vector<HitPair> leftHit;
	std::vector<HitPair> rightHit;
	leftChild->intersectAll(r, leftHit);
	rightChild->intersectAll(r, rightHit);

	if (operation == CSG_INTERSECTION)
		hitArray = computeIntersection(leftHit, rightHit);
	else if (operation == CSG_UNION)
		hitArray = computeUnion(leftHit, rightHit);
	else
		hitArray = computeDifference(leftHit, rightHit);

	return !hitArray.empty();
}

std::vector<HitPair> CSG::computeIntersection(const std::vector<HitPair> &leftArray, const std::vector<HitPair> &rightArray) {
	auto leftSize = leftArray.size();
	auto rightSize = rightArray.size();
	auto i = 0, j = 0;

	std::vector<HitPair> result;
	while (i < leftSize && j < rightSize) {
		float l1 = leftArray[i].first.getT(), l2 = leftArray[i].second.getT();
		float r1 = rightArray[j].first.getT(), r2 = rightArray[j].second.getT();
		if (!(l1 > r2 || r1 > l2)) {
			//have intersection
			Hit h1 = l1 >= r1 ? leftArray[i].first : rightArray[j].first;
			Hit h2 = l2 <= r2 ? leftArray[i].second : rightArray[j].second;
			result.push_back(make_pair(h1, h2));
		}
		if (l2 <= r2)
			++i;
		else
			++j;
	}
	return result;
}

std::vector<HitPair> CSG::computeUnion(const std::vector<HitPair> &leftArray, const std::vector<HitPair> &rightArray) {
	auto leftSize = leftArray.size();
	auto rightSize = rightArray.size();
	auto i = 0, j = 0;

	bool isStartHit = false;
	Hit hit1, hit2;
	std::vector<HitPair> result;
	while (i < leftSize && j < rightSize) {
		float l1 = leftArray[i].first.getT(), l2 = leftArray[i].second.getT();
		float r1 = rightArray[j].first.getT(), r2 = rightArray[j].second.getT();

		if (!isStartHit) {
			if (l1 > r2) {
				result.push_back(rightArray[j++]);
				continue;
			}
			else if (r1 > l2) {
				result.push_back(leftArray[i++]);
				continue;
			}
			hit1 = l1 <= r1 ? leftArray[i].first : rightArray[j].first;
			hit2 = l2 >= r2 ? leftArray[i].second : rightArray[j].second;
			isStartHit = true;
		}
		else 
		{
			float t = hit2.getT();
			if (t < l1 && t < r1) {
				result.push_back(make_pair(hit1, hit2));
				isStartHit = false;
				continue;
			}
			if (t >= l1 && t < l2) {
				hit2 = leftArray[i].second;
				t = hit2.getT();
			}

			if (t >= r1 && t < r2) {
				hit2 = rightArray[j].second;
				t = hit2.getT();
			}
			
			if (t >= l2)
				++i;

			if (t >= r2)
				++j;
		}
	}

	auto k = i < leftSize ? i : j;
	auto size = i < leftSize ? leftSize : rightSize;
	const std::vector<HitPair>& unfinish = i < leftSize ? leftArray : rightArray;

	while (k < size) {
		if (!isStartHit) {
			result.push_back(unfinish[k++]);
			continue;
		}
		else {
			float t = hit2.getT();
			float l1 = unfinish[k].first.getT(), l2 = unfinish[k].second.getT();
			if (t < l1) {
				result.push_back(make_pair(hit1, hit2));
				isStartHit = false;
				continue;
			}
			if (t < l2) {
				hit2 = unfinish[k].second;
			}
			++k;
		}
	}
	if (isStartHit)
		result.push_back(make_pair(hit1, hit2));
	return result;
}

std::vector<HitPair> CSG::computeDifference(const std::vector<HitPair> &leftArray, const std::vector<HitPair> &rightArray) {
	auto leftSize = leftArray.size();
	auto rightSize = rightArray.size();
	auto i = 0, j = 0;

	bool initLeft = false;
	bool initRight = false;
	Hit hit1, hit2;
	std::vector<HitPair> result;

	float l1, l2, r1, r2;
	while (i < leftSize && j < rightSize) {
		if (!initLeft) {
			hit1 = leftArray[i].first;
			hit2 = leftArray[i].second;
			l1 = hit1.getT(), l2 = hit2.getT();
			
			initLeft = true;
		}
		if (!initRight) {
			r1 = rightArray[j].first.getT(), r2 = rightArray[j].second.getT();
			initRight = true;
		}

		if (r2 <= l1) {
			//          l1|_________|l2
			// r1|_______|r2
			++j;
			initRight = false;
		}
		else if (l2 <= r1) {
			// l1|_________|l2
			//               r1|_______|r2
			++i;
			initLeft = false;
			result.push_back(make_pair(hit1, hit2));
		}
		else if (l1 >= r1 && l2 <= r2) {
			//   l1|_________|l2
			// r1|______________|r2
			++i;
			initLeft = false;
		}
		else if (l1 >= r1 && l1 < r2) {
			//     l1|_________|l2
			// r1|_______|r2
			hit1 = rightArray[j].second;
			hit1.reverseNormal();
			l1 = r2;
			++j;
			initRight = false;
		}
		else if (l1 <= r1 && l2 >= r2) {
			// l1|_________________|l2
			//      r1|_______|r2
			Hit newHit2 = rightArray[j].first;
			newHit2.reverseNormal();
			result.push_back(make_pair(hit1, newHit2));
			hit1 = rightArray[j].second;
			hit1.reverseNormal();
			++j;
		}
		else if (l1 <= r1 && l2 >= r1) {
			//     l1|_________|l2
			//           r1|_______|r2
			Hit newHit2 = rightArray[j].first;
			newHit2.reverseNormal();
			result.push_back(make_pair(hit1, newHit2));
			++i;
			initLeft = false;
		}
		else
			assert(false);
	}

	while (i < leftSize) {
		if (initLeft)
			result.push_back(make_pair(hit1, hit2));
		else
			result.push_back(leftArray[i]);
		++i;
	}
	return result;
}
