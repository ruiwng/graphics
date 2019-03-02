#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>

using namespace std;

float clampedDepth(float depthInput, float depthMin, float depthMax) {
	if (depthInput < depthMin)
		return depthMin;
	else if (depthInput > depthMax)
		return depthMax;
	else
		return depthInput;
}

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
  // Fill in your implementation here.

  // This loop loops over each of the input arguments.
  // argNum is initialized to 1 because the first
  // "argument" provided to the program is actually the
  // name of the executable (in our case, "a4").
	SceneParser *sceneParser = NULL;
	int screenWidth = 0, screenHeight = 0;
	char outputFile[1024] = { '\0' };

	char depthFile[1024] = { '\0' };
	bool saveDepthFile = false;
	int minDepth, maxDepth;

	char normalFile[1024] = { '\0' };
	bool saveNormalFile = false;

	for (int argNum = 1; argNum < argc; ++argNum) {
		std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
	}
	for( int argNum = 1; argNum < argc; ++argNum )
	{
		if (strcmp(argv[argNum], "-input") == 0) {
			sceneParser = new SceneParser(argv[++argNum]);
		}
		else if (strcmp(argv[argNum], "-size") == 0) {
			sscanf(argv[++argNum], "%d", &screenWidth);
			sscanf(argv[++argNum], "%d", &screenHeight);
		}
		else if (strcmp(argv[argNum], "-output") == 0) {
			strcpy(outputFile, argv[++argNum]);
		}
		else if (strcmp(argv[argNum], "-depth") == 0) {
			saveDepthFile = true;
			sscanf(argv[++argNum], "%d", &minDepth);
			sscanf(argv[++argNum], "%d", &maxDepth);
			strcpy(depthFile, argv[++argNum]);
		}
		else if (strcmp(argv[argNum], "-normals") == 0) {
			saveNormalFile = true;
			strcpy(normalFile, argv[++argNum]);
		}
	}
	if (sceneParser == NULL) {
		cout << "input file cannot be NULL" << endl;
		exit(0);
	}
    
	if (screenWidth == 0 || screenHeight == 0) {
		cout << "screen width and height cannot be zero" << endl;
		exit(0);
	}

	if (strcmp(outputFile, "") == 0) {
		cout << "output file cannot be NULL" << endl;
		exit(0);
	}

	// First, parse the scene using SceneParser.
	// Then loop over each pixel in the image, shooting a ray
	// through that pixel and finding its intersection with
	// the scene.  Write the color at the intersection to that
	// pixel in your output image.
  


 
	///TODO: below demonstrates how to use the provided Image class
	 ///Should be removed when you start
	//Vector3f pixelColor (1.0f,0,0);
	//width and height
	//Image image(screenWidth, screenHeight);
	//image.SetPixel( 5,5, pixelColor );
	//image.SaveImage("demo.bmp");
	Camera *camera = sceneParser->getCamera();
	Group *group = sceneParser->getGroup();

	Image image(screenWidth, screenHeight);
	
	Image *depthImage = NULL;
	if (saveDepthFile)
		depthImage = new Image(screenWidth, screenHeight);

	Image *normalImage = NULL;
	if (saveNormalFile)
		normalImage = new Image(screenWidth, screenHeight);

	int numLight = sceneParser->getNumLights();
	Vector3f backgroundColor = sceneParser->getBackgroundColor();
	Vector3f ambient = sceneParser->getAmbientLight();

	for (int i = 0; i < screenHeight; ++i) {
		for (int j = 0; j < screenWidth; ++j) {
			Ray ray = camera->generateRay(Vector2f(float(j) / screenWidth, float(i) / screenHeight));
			Hit hit;
			bool isIntersect = group->intersect(ray, hit, camera->getTMin());

			if (isIntersect) {
				Vector3f hitPoint = ray.getOrigin() + ray.getDirection() * hit.getT();
				Vector3f color(ambient);
				for (int k = 0; k < numLight; ++k) {
					Light *light = sceneParser->getLight(k);
					Vector3f direction, col;
					float distance;
					light->getIllumination(hitPoint, direction, col, distance);
					color += hit.getMaterial()->Shade(ray, hit, direction, col);
				}
				image.SetPixel(j, i, color);
			}
			else
				image.SetPixel(j, i, backgroundColor);

			// depth file
			if (saveDepthFile) {
				float depth;
				if (!isIntersect)
					depth = FLT_MAX;
				else {
					depth = (ray.getDirection() * hit.getT()).z();
					depth = abs(depth);
				}
				depth = clampedDepth(depth, minDepth, maxDepth);
				depthImage->SetPixel(j, i, (depth - minDepth) / (maxDepth - minDepth) * Vector3f(0.8f, 0.8f, 0.8f));
			}

			// normal file
			if (saveNormalFile) {
				Vector3f color(0.0f, 0.0f, 0.0f);
				if (isIntersect) {
					Vector3f normal = hit.getNormal();
					color = Vector3f(abs(normal.x()), abs(normal.y()), abs(normal.z()));
				}
				normalImage->SetPixel(j, i, color);
			}
		}
	}
	image.SaveImage(outputFile);

	if (saveDepthFile)
		depthImage->SaveImage(depthFile);

	if (saveNormalFile)
		normalImage->SaveImage(normalFile);

	delete sceneParser;
	sceneParser = NULL;
	return 0;
}

