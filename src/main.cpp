#include <stdio.h>

#include "opencv2/opencv.hpp"

#include "CMakeVars.hpp"

// Sobel e Laplace
#define SIZE Size(3,3)
#define DEPTH CV_16S
#define SCALE 1
#define DELTA 0
#define KSIZE 3

// Canny
#define THRES 50
#define RATIO 3

// Threshold
#define BIN_THRES 60

using namespace cv;

int main(int argc, char** argv )
{
    if ( argc != 2 ) {
        printf("Please give a path to a jpg image (without extension) in the ./resources/srcs/ folder\n");
        return -1;
    }

    String filename = argv[1];
    Mat image = imread( "./resources/srcs/"+filename+".jpg", IMREAD_GRAYSCALE );

    if ( !image.data ) {
        printf( "No image data \n" );
        return -1;
    }

    /***********************************************************************
    *  Ground Truth
    ***********************************************************************/
    Mat gt = imread( "./resources/gt/"+filename+"gt.jpg", IMREAD_GRAYSCALE );
    if ( !gt.data ) {
        printf( "Couldn't load ground truth image \n" );
        return -1;
    }

    /***********************************************************************
    *  Detecting Borders
    ***********************************************************************/
    Mat blurred;
    GaussianBlur( image, blurred, SIZE, 0, 0, BORDER_DEFAULT );

    /***********************************************************************
    *  Sobel
    ***********************************************************************/
    Mat gradientX, gradientY;
    Sobel( blurred, gradientX, DEPTH, 1, 0, KSIZE, SCALE, DELTA, BORDER_DEFAULT );
    Sobel( blurred, gradientY, DEPTH, 0, 1, KSIZE, SCALE, DELTA, BORDER_DEFAULT );
    convertScaleAbs( gradientX, gradientX );
    convertScaleAbs( gradientY, gradientY );

    Mat sobel;
    addWeighted( gradientX, 0.5, gradientY, 0.5, 0, sobel );

    /***********************************************************************
    *  Canny
    ***********************************************************************/
    Mat canny;
    Canny( blurred, canny, THRES, THRES*RATIO, KSIZE );

    /***********************************************************************
    *  Laplace
    ***********************************************************************/
    Mat laplace;
    Laplacian( blurred, laplace, DEPTH, KSIZE, SCALE, DELTA, BORDER_DEFAULT );
    convertScaleAbs( laplace, laplace );

    /***********************************************************************
    *  Thresholding
    ***********************************************************************/
    Mat thresSobel, thresCanny, thresLaplace;
    threshold( sobel, thresSobel, BIN_THRES, 255, 0 );
    threshold( canny, thresCanny, BIN_THRES, 255, 0 );
    threshold( laplace, thresLaplace, BIN_THRES, 255, 0 );
    
    threshold( gt, gt, BIN_THRES, 255, 1 );

    imshow( "Original Image", image );
    imshow( "Ground Truth", gt );

    imshow( "Sobel", sobel );
    imshow( "Thresholded Sobel", thresSobel );

    imshow( "Canny", canny );
    imshow( "Thresholded Canny", thresCanny );

    imshow( "Laplace", laplace );
    imshow( "Thresholded Laplace", thresLaplace );

    imwrite( "./resources/det/" + filename + "-sobel.jpg", thresSobel );
    imwrite( "./resources/det/" + filename + "-canny.jpg", thresCanny );
    imwrite( "./resources/det/" + filename + "-laplace.jpg", thresLaplace );

    waitKey(0);

    return 0;
}