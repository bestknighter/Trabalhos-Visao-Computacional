#include <cstdio>
#include <string>

#include "opencv2/opencv.hpp"

#include "CMakeVars.hpp"
#include "Image.hpp"

using namespace cv;

const float df = 25.0; // mm
const float baseline = 120.0; // mm

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: cv_test <Image_Path>\n");
        return -1;
    }

    Image imL = Image (std::string(argv[1]) + "L.png");
    Image imR = Image (std::string(argv[1]) + "R.png");

    if ( !imL.IsValid() || !imR.IsValid() )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Left", WINDOW_AUTOSIZE );
    namedWindow("Display Right", WINDOW_AUTOSIZE );
    imL.Show ("Display Left");
    imR.Show ("Display Right");

    waitKey(0);

    return 0;
}