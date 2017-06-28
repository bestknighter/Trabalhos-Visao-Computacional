#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/bgsegm.hpp>
#include <string>

#include "CMakeVars.hpp"
#include "wienerfilter.h"

#define FLOW_GRID_SIZE 1
#define FRAME_OFFSET 4
#define LINE_STRETCHER 5

using namespace cv;
using namespace cv::bgsegm;

double sqrt2 = std::sqrt(2);

int main(int argc, char** argv ) {
	if ( argc != 2 ) {
		printf("usage: cv_test <Image_Path>\n");
		return -1;
	}

	VideoCapture cap( argv[1] );
	Ptr<BackgroundSubtractorMOG> mog = createBackgroundSubtractorMOG();
	Ptr<BackgroundSubtractorMOG2> mog2 = createBackgroundSubtractorMOG2();
	Ptr<FarnebackOpticalFlow> fof = FarnebackOpticalFlow::create(5, 0.5, false, 13, 10, 7, 1.5, OPTFLOW_FARNEBACK_GAUSSIAN);
	int key = 255;
	
	do {
		Mat currentFrame, lastFrame;
		cap.read(currentFrame);
		int pos = cap.get(CAP_PROP_POS_FRAMES);
		if(pos >= FRAME_OFFSET) {
			cap.set(CAP_PROP_POS_FRAMES, pos - (FRAME_OFFSET+1));
			cap.read(lastFrame);
			cap.set(CAP_PROP_POS_FRAMES, pos);
		}

		if ( currentFrame.empty() ) {
			printf("No frame data \n");
			return -1;
		}

		if ( lastFrame.empty() ) {
			continue;
		}

		Mat bgSubtracted1, bgSubtracted2, flow = Mat::zeros(currentFrame.rows, currentFrame.cols, CV_64FC2);
		// mog->apply(currentFrame, bgSubtracted1);
		// mog2->apply(currentFrame, bgSubtracted2);

		Mat i0, i1;
		cvtColor(lastFrame, i0, COLOR_BGR2GRAY);
		cvtColor(currentFrame, i1, COLOR_BGR2GRAY);
		fof->calc(i0, i1, flow);
		Mat convertedFlow = Mat::zeros(currentFrame.rows, currentFrame.cols, CV_8UC1);

		// Mat bgFlow = currentFrame.clone();
		for(int j = 0; j < currentFrame.rows; j += FLOW_GRID_SIZE) {
			for(int i = 0; i < currentFrame.cols; i += FLOW_GRID_SIZE) {
				Point2f thisFlow = flow.at<Point2f>(j,i);

				convertedFlow.at<unsigned char>(j,i) = 255*(std::sqrt( std::pow(thisFlow.x, 2) + std::pow(thisFlow.y, 2) )/sqrt2);

				// // Colorido por eixo
				// int B = 255*(thisFlow.x >= 0 ? thisFlow.x : -thisFlow.x );
				// int R = 255*(thisFlow.y >= 0 ? thisFlow.y : -thisFlow.y );
				// // Linhas
				// line( bgFlow, Point(i,j), Point(cvRound(i+thisFlow.x*LINE_STRETCHER), cvRound(j+thisFlow.y*LINE_STRETCHER)), Scalar(B,0,R) );
			}
		}

		Mat filteredFlow;
		double estimatedVariance = WienerFilter(convertedFlow, filteredFlow);
		
		Mat hist;
		int channels[] = {0};
		int histSize[] = {255};
		float intRange[] = {0,255};
		float* ranges[] = {intRange};
		calcHist(&filteredFlow, 1, channels, Mat(), hist, 1, histSize, (const float**)ranges, true, false);

		imshow("Original Video (current frame)", currentFrame);
		// imshow("BG Subtracted (MOG)", bgSubtracted1);
		// imshow("BG Subtracted (MOG2)", bgSubtracted2);
		// imshow("Optical Flow (Farneback)", bgFlow);
		imshow("Optical Flow (Unfiltered)", convertedFlow);
		imshow("Optical Flow (Filtered)", filteredFlow);
		
		key = waitKey(1);
		if(255 != key && 27 != key) {
			printf("Pressionado tecla %d (%c)\n", key, key);
		}
	} while( 27 != key );

	return 0;
}
