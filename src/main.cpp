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
    if ( argc != 2 )
    {
        printf("usage: cv_test <Image_Path>\n");
        return -1;
    }

    Mat image = imread( argv[1], IMREAD_GRAYSCALE );

    if ( !image.data )
    {
        printf( "No image data \n" );
        return -1;
    }

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
    threshold( sobel, thresSobel, BIN_THRES, 255, 1 );
    threshold( canny, thresCanny, BIN_THRES, 255, 1 );
    threshold( laplace, thresLaplace, BIN_THRES, 255, 1 );


    imshow( "Original Image", image );

    imshow( "Sobel", sobel );
    imshow( "Thresholded Sobel", thresSobel );

    imshow( "Canny", canny );
    imshow( "Thresholded Canny", thresCanny );

    imshow( "Laplace", laplace );
    imshow( "Thresholded Laplace", thresLaplace );

    waitKey(0);

    return 0;
}