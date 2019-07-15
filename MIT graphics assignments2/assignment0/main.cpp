#include "ifs.h"
#include "image.h"


char* inputFile = nullptr;
int numPoints = 1000;
int numIters = 10;
int imageSize = 100;
char* outputFile = nullptr;


void parseCmdline(int argc, char* argv[]) {
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-input") == 0) {
			inputFile = argv[++i];
		}
		else if (strcmp(argv[i], "-points") == 0) {
			numPoints = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-size") == 0) {
			imageSize = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-output") == 0) {
			outputFile = argv[++i];
		}
		else if (strcmp(argv[i], "-iters") == 0) {
			numIters = atoi(argv[++i]);
		}
		else {
			printf("whoops error with command line argument %d: '%s' \n", i, argv[i]);
		}
	}
	assert(inputFile != nullptr && outputFile != nullptr);
}

int main(int argc, char* argv[]) {
	parseCmdline(argc, argv);
	Image image(imageSize, imageSize);
	
	IFS *pIfs = IFS::read(inputFile);

	pIfs->render(image, numPoints, numIters);

	image.SaveTGA(outputFile);

	return 0;
}