#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/bgsegm.hpp>
#include <string>

#include "CMakeVars.hpp"

#define FLOW_GRID_SIZE 1

using namespace cv;
using namespace cv::bgsegm;

int main(int argc, char** argv ) {
	if ( argc != 2 ) {
		printf("usage: cv_test <Image_Path>\n");
		return -1;
	}

	VideoCapture cap( argv[1] );
	Ptr<BackgroundSubtractorMOG> mog = createBackgroundSubtractorMOG();
	Ptr<BackgroundSubtractorMOG2> mog2 = createBackgroundSubtractorMOG2();
	Ptr<FarnebackOpticalFlow> fof = FarnebackOpticalFlow::create();
	int key = 255;

	Mat currentFrame, lastFrame;
	cap.read(currentFrame);

	if ( currentFrame.empty() ) {
		printf("No frame data \n");
		return -1;
	}
	imshow("Original Video", currentFrame);
	
	Mat bgFlow(currentFrame.rows, currentFrame.cols, currentFrame.type());
	do {
		lastFrame = currentFrame.clone();
		cap.read(currentFrame);

		if ( currentFrame.empty() ) {
			printf("No frame data \n");
			return -1;
		}

		Mat bgSubtracted1, bgSubtracted2, flow;
		mog->apply(currentFrame, bgSubtracted1);
		mog2->apply(currentFrame, bgSubtracted2);

		Mat i0, i1;
		cvtColor(lastFrame, i0, COLOR_BGR2GRAY);
		cvtColor(currentFrame, i1, COLOR_BGR2GRAY);
		fof->calc(i0, i1, flow);

		for(int j = 0; j < currentFrame.rows; j += FLOW_GRID_SIZE) {
			for(int i = 0; i < currentFrame.cols; i += FLOW_GRID_SIZE) {
				Point2f thisFlow = flow.at<Point2f>(j,i);
				float intensity = std::sqrt(thisFlow.x*thisFlow.x+thisFlow.y*thisFlow.y)-1;
				int B = (intensity < 0 ? -intensity*255 : 0);
				int G = (intensity < 0 ? 255*(1+intensity) : 255*(1-intensity) );
				int R = (intensity > 0 ? intensity*255 : 0);
				bgFlow.at<Vec3b>(j,i) = Vec3b(B,G,R);
			}
		}

		imshow("Original Video", currentFrame);
		imshow("BG Subtracted (MOG)", bgSubtracted1);
		imshow("BG Subtracted (MOG2)", bgSubtracted2);
		imshow("Optical Flow (Farneback)", bgFlow);
		
		key = waitKey(1);
		if(255 != key && 27 != key) {
			printf("Pressionado tecla %d (%c)\n", key, key);
		}
	} while( 27 != key );

	return 0;
}