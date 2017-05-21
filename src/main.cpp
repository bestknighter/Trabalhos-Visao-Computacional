#include <stdio.h>

#include "opencv2/opencv.hpp"

#include "CMakeVars.hpp"

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



    namedWindow( "Original Image", WINDOW_AUTOSIZE );
    imshow( "Original Image", image );

    waitKey(0);

    return 0;
}