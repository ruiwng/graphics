#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "ray.h"
#include "camera.h"
#include "group.h"
#include "hit.h"
#include "material.h"
#include "scene_parser.h"
#include "image.h"

char *inputFile = nullptr;
int width = 100;
int height = 100;
char *outputFile = nullptr;
float depthMin = 0;
float depthMax = 1;
char *depthFile = nullptr;


void parseCmdLine(int argc, char *argv[]) {
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-input") == 0) {
			inputFile = argv[++i];
		}
		else if (strcmp(argv[i], "-size") == 0) {
			width = atoi(argv[++i]);
			height = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-output") == 0) {
			outputFile = argv[++i];
		}
		else if (strcmp(argv[i], "-depth") == 0) {
			depthMin = atof(argv[++i]);
			depthMax = atof(argv[++i]);
			depthFile = argv[++i];
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}
	assert(inputFile != nullptr);
	assert(outputFile != nullptr);
}


int main(int argc, char *argv[]) {
	parseCmdLine(argc, argv);

	SceneParser *pSceneParser = new SceneParser(inputFile);
	Image *pImage = new Image(width, height);
	Image *pDepthImage = nullptr;
	if (depthFile != nullptr)
		pDepthImage = new Image(width, height);

	Group *pGroup = pSceneParser->getGroup();
	Vec3f backgroudColor = pSceneParser->getBackgroundColor();
	Camera *pCamera = pSceneParser->getCamera();

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			Ray ray = pCamera->generateRay(Vec2f(float(i) / width, float(j) / height));
			Hit hit(std::numeric_limits<float>::max(), nullptr);
			bool isHit = pGroup->intersect(ray, hit, pCamera->getTMin());
			if (!isHit)
				pImage->SetPixel(i, j, backgroudColor);
			else
			{
				Vec3f color = hit.getMaterial()->getDiffuseColor();
				pImage->SetPixel(i, j, color);
			}

			if (pDepthImage != nullptr) {
				float depth;
				if (!isHit)
					depth = depthMax;
				else
					depth = hit.getT();

				if (depth < depthMin)
					depth = depthMin;
				else if (depth > depthMax)
					depth = depthMax;
				float value = (depthMax - depth) / (depthMax - depthMin);
				pDepthImage->SetPixel(i, j, Vec3f(value, value, value));
			}
		}
	}

	pImage->SaveTGA(outputFile);
	if(pDepthImage != nullptr)
		pDepthImage->SaveTGA(depthFile);

	delete pSceneParser;
	delete pImage;
	
	if (pDepthImage != nullptr) {
		delete pDepthImage;
		pDepthImage = nullptr;
	}
	return 0;
}