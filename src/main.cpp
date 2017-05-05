#include <cstdio>
#include <cfloat>
#include <string>

#include "opencv2/opencv.hpp"

#include "CMakeVars.hpp"
#include "Image.hpp"

using namespace cv;

const float df = 25.0; // mm
const float baseline = 120.0; // mm

Vec3i SAD (Mat const& A, Mat const& B, int channels = 1);

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

    printf ("Digite o tamanho da janela desejado (inteiro impar): ");
    int size = 0;
    scanf ("%d", &size);
    int W = (size-1)/2;

    Mat matchingPixels = Mat (imL.GetCols (), imL.GetRows (), CV_64FC3);
    for (int j = imL.GetRows()-1-W; j >= W; --j) {
        for (int i = imL.GetCols()-1; i >= W; --i) {
            Mat ROIL = imL.GetRoi(Rect(i, j, size, size));
            float smallestSADv = FLT_MAX;
            float smallestSADx = i;
            for (int k = imR.GetCols()-1; k >= 0; --k) {
                Mat ROIR = imR.GetRoi(Rect(k, j, size, size));
                float SADv = norm(SAD(ROIL,ROIR));
                if (SADv < smallestSADv) {
                    smallestSADv = SADv;
                    smallestSADx = k;
                }
            }
            matchingPixels.at<Vec3f>(i,j) = {smallestSADx, (float)j, smallestSADv};
        }
    }

    waitKey(0);

    return 0;
}

Vec3i SAD (Mat const& A, Mat const& B, int channels) {
    Vec3i sum (0,0,0);
    for (int j = A.rows-1; j >= 0; --j) {
        for (int i = A.cols-1; i >= 0; --i) {
            Vec3b aPx = A.at<Vec3b> (j, i);
            Vec3b bPx = B.at<Vec3b> (j, i);
            for (int k = 0; k < channels; ++k) {
                int diff = aPx[k] - bPx[k];
                sum[k] += diff >= 0? diff: -diff;
            }
        }
    }
    return sum;
}
