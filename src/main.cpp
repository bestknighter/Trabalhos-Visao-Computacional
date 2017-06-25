#include <stdio.h>

#include "opencv2/opencv.hpp"
#include <string>

#include "CMakeVars.hpp"

using namespace cv;

int main(int argc, char** argv ) {
	if ( argc != 2 ) {
		printf("usage: cv_test <Image_Path>\n");
		return -1;
	}

	VideoCapture cap( argv[1] );

	int key = 255;
	do {
		Mat frame;
		cap.read(frame);

		if ( frame.empty() ) {
			printf("No frame data \n");
			return -1;
		}

		imshow("Original Video", frame);

		key = waitKey(1);
	} while( 255 == key );

	return 0;
}