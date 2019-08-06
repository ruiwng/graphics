#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <algorithm>

#include "ray.h"
#include "camera.h"
#include "group.h"
#include "hit.h"
#include "material.h"
#include "scene_parser.h"
#include "image.h"
#include "light.h"
#include "glCanvas.h"

extern int thetaSteps;
extern int phiSteps;
extern bool enableGouraud;

namespace {
	char *inputFile = nullptr;
	int width = 100;
	int height = 100;
	char *outputFile = nullptr;
	float depthMin = 0;
	float depthMax = 1;
	char *depthFile = nullptr;
	char *normalFile = nullptr;
	bool isShadeBack = false;
	bool isGui = false;

	SceneParser *pSceneParser = nullptr;

	void parseCmdLine(int argc, char *argv[]) {
		for (int i = 1; i < argc; ++i) {
			if (!strcmp(argv[i], "-input")) {
				inputFile = argv[++i];
			}
			else if (!strcmp(argv[i], "-size")) {
				width = atoi(argv[++i]);
				height = atoi(argv[++i]);
			}
			else if (!strcmp(argv[i], "-output")) {
				outputFile = argv[++i];
			}
			else if (!strcmp(argv[i], "-depth")) {
				depthMin = atof(argv[++i]);
				depthMax = atof(argv[++i]);
				depthFile = argv[++i];
			}
			else if (!strcmp(argv[i], "-normals")) {
				normalFile = argv[++i];
			}
			else if (!strcmp(argv[i], "-shade_back")) {
				isShadeBack = true;
			}
			else if (!strcmp(argv[i], "-gui")) {
				isGui = true;
			}
			else if (!strcmp(argv[i], "-tessellation")) {
				thetaSteps = atoi(argv[++i]);
				phiSteps = atoi(argv[++i]);
			}
			else if (!strcmp(argv[i], "-gouraud")) {
				enableGouraud = true;
			}
			else {
				printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
				assert(0);
			}
		}
		assert(inputFile != nullptr);
		assert(outputFile != nullptr);
	}

	void renderFunction() {
		Image *pImage = new Image(width, height);
		Image *pNormalImage = nullptr;
		if (normalFile != nullptr)
			pNormalImage = new Image(width, height);
		Image *pDepthImage = nullptr;
		if (depthFile != nullptr)
			pDepthImage = new Image(width, height);

		Group *pGroup = pSceneParser->getGroup();
		Vec3f backgroudColor = pSceneParser->getBackgroundColor();
		Vec3f ambientColor = pSceneParser->getAmbientLight();
		Camera *pCamera = pSceneParser->getCamera();
		int numLights = pSceneParser->getNumLights();

		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				Ray ray = pCamera->generateRay(Vec2f(float(i) / width, float(j) / height));
				//Ray ray(Vec3f(-1.17500007, -0.400000095, 10.0000000), Vec3f(0.0f, 0.0f, -1.0f));
				//Ray ray(Vec3f(0.0f, 0.0f, 10.0f), Vec3f(0.0f, 0.0f, -1.0f));
				Hit hit(std::numeric_limits<float>::max(), nullptr, Vec3f(0.0f, 0.0f, 0.0f));
				bool isHit = pGroup->intersect(ray, hit, pCamera->getTMin());
				if (!isHit)
					pImage->SetPixel(i, j, backgroudColor);
				else
				{
					Vec3f normal = hit.getNormal();
					bool isBack = normal.Dot3(ray.getDirection()) > 0.0f;
					if (isShadeBack && isBack)
						normal.Negate();
					if (isBack && !isShadeBack) {
						pImage->SetPixel(i, j, Vec3f(0.0f, 0.0f, 0.0f));
					}
					else
					{
						Vec3f inter = hit.getIntersectionPoint();
						Material* pMaterial = hit.getMaterial();
						Vec3f materialColor = hit.getMaterial()->getDiffuseColor();
						Vec3f color = ambientColor * materialColor;
						for (int k = 0; k < numLights; ++k) {
							Light *light = pSceneParser->getLight(k);
							Vec3f dirToLight, diffuseColor;
							float distanceToLight;
							light->getIllumination(inter, dirToLight, diffuseColor, distanceToLight);
							float dif = normal.Dot3(dirToLight);
							color += pMaterial->Shade(ray, hit, dirToLight, diffuseColor);
						}
						pImage->SetPixel(i, j, color);
					}
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

				if (pNormalImage != nullptr) {
					Vec3f normal = hit.getNormal();
					normal = Vec3f(abs(normal.x()), abs(normal.y()), abs(normal.z()));
					pNormalImage->SetPixel(i, j, normal);
				}
			}
		}

		pImage->SaveTGA(outputFile);
		if (pDepthImage != nullptr)
			pDepthImage->SaveTGA(depthFile);
		if (pNormalImage != nullptr)
			pNormalImage->SaveTGA(normalFile);

		delete pImage;
		if (pDepthImage != nullptr) {
			delete pDepthImage;
			pDepthImage = nullptr;
		}
		if (pNormalImage != nullptr) {
			delete pNormalImage;
			pNormalImage = nullptr;
		}
	}
}


int main(int argc, char *argv[]) {
	parseCmdLine(argc, argv);

	pSceneParser = new SceneParser(inputFile);

	if (isGui)
	{
		GLCanvas glCanvas;
		glCanvas.initialize(argc, argv, pSceneParser, renderFunction);
	}
	else
		renderFunction();
	
	
	delete pSceneParser;

	return 0;
}