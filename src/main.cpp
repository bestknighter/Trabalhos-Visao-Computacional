#include <cstdio>

#include "opencv2/opencv.hpp"

#include "CMakeVars.hpp"

#define PREC_PIX_TYPE unsigned char

// Sobel
#define S_SIZE Size(3,3)
#define S_DEPTH CV_16S
#define S_SCALE 1
#define S_DELTA 0
#define S_KSIZE 3

// Canny
#define C_THRES 30
#define C_RATIO 3
#define C_KSIZE 3

// Laplace
#define L_SIZE Size(3,3)
#define L_DEPTH CV_16S
#define L_SCALE 1
#define L_DELTA 0
#define L_KSIZE 3

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
    GaussianBlur( image, blurred, S_SIZE, 0, 0, BORDER_DEFAULT );

    /***********************************************************************
    *  Sobel
    ***********************************************************************/
    Mat gradientX, gradientY;
    Sobel( blurred, gradientX, S_DEPTH, 1, 0, S_KSIZE, S_SCALE, S_DELTA, BORDER_DEFAULT );
    Sobel( blurred, gradientY, S_DEPTH, 0, 1, S_KSIZE, S_SCALE, S_DELTA, BORDER_DEFAULT );
    convertScaleAbs( gradientX, gradientX );
    convertScaleAbs( gradientY, gradientY );

    Mat sobel;
    addWeighted( gradientX, 0.5, gradientY, 0.5, 0, sobel );

    /***********************************************************************
    *  Canny
    ***********************************************************************/
    Mat canny;
    Canny( blurred, canny, C_THRES, C_THRES*C_RATIO, C_KSIZE );

    /***********************************************************************
    *  Laplace
    ***********************************************************************/
    Mat laplace;
    Laplacian( blurred, laplace, L_DEPTH, L_KSIZE, L_SCALE, L_DELTA, BORDER_DEFAULT );
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

    int sobelHit = 0;
    int cannyHit = 0;
    int laplaceHit = 0;
    for ( int j = 0; j < image.rows; ++j ) {
        for ( int i = 0; i < image.cols; ++i ) {
            PREC_PIX_TYPE gtPix = gt.at<PREC_PIX_TYPE>( j, i );
            sobelHit += ( thresSobel.at<PREC_PIX_TYPE>( j, i ) == gtPix ) ? 1 : 0;
            cannyHit += ( thresCanny.at<PREC_PIX_TYPE>( j, i ) == gtPix ) ? 1 : 0;
            laplaceHit += ( thresLaplace.at<PREC_PIX_TYPE>( j, i ) == gtPix ) ? 1 : 0;
        }
    }

    float numPix = image.rows * image.cols;
    float precSobel = sobelHit/numPix;
    float precCanny = cannyHit/numPix;
    float precLaplace = laplaceHit/numPix;

    printf ( "-- Precisao\nSobel: %f\nCanny: %f\nLaplace: %f\n", precSobel, precCanny, precLaplace );

    waitKey(0);

    return 0;
}