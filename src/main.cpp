#include <stdio.h>

#include "opencv2/opencv.hpp"

#include "CMakeVars.hpp"

// Sobel
#define SIZE Size(3,3)
#define DEPTH CV_16S
#define SCALE 1
#define DELTA 0

// Canny
#define THRES 50
#define RATIO 3
#define KSIZE 3

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
    Sobel( blurred, gradientX, DEPTH, 1, 0, 3, SCALE, DELTA, BORDER_DEFAULT );
    Sobel( blurred, gradientY, DEPTH, 0, 1, 3, SCALE, DELTA, BORDER_DEFAULT );
    convertScaleAbs( gradientX, gradientX );
    convertScaleAbs( gradientY, gradientY );

    Mat sobel;
    addWeighted( gradientX, 0.5, gradientY, 0.5, 0, sobel );

    /***********************************************************************
    *  Canny
    ***********************************************************************/
    Mat canny;
    Canny( blurred, canny, THRES, THRES*RATIO, KSIZE );


    // Mat laplace;


    namedWindow( "Original Image", WINDOW_AUTOSIZE );
    imshow( "Original Image", image );

    namedWindow( "Sobel", WINDOW_AUTOSIZE );
    imshow( "Sobel", sobel );

    namedWindow( "Canny", WINDOW_AUTOSIZE );
    imshow( "Canny", canny );

    waitKey(0);

    return 0;
}