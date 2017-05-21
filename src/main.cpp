#include <stdio.h>

#include "opencv2/opencv.hpp"

#include "CMakeVars.hpp"
#include "Image.hpp"

using namespace cv;

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: cv_test <Image_Path>\n");
        return -1;
    }

    Image image( argv[1] );

    if ( !image.IsValid() )
    {
        printf( "No image data \n" );
        return -1;
    }
    namedWindow( "Display Image", WINDOW_AUTOSIZE );
    image.Show( "Display Image" );

    waitKey(0);

    return 0;
}