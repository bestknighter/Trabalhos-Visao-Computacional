#include <cstdio>
#include <cfloat>
#include <string>
#include <vector>

#include "opencv2/opencv.hpp"

#include "CMakeVars.hpp"
#include "Image.hpp"

using namespace cv;

const float df = 25.0; // mm
const float baseline = 120.0; // mm

Vec3i SAD (Mat const& A, Mat const& B, int channels = 1);
std::vector<float> findSimilarPixel(Image const& A, Image const& B, int x, int y, int size, int channels = 1);

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
    printf ("Digite o tamanho da janela desejado (inteiro impar): ");
    int size = 0;
    scanf ("%d", &size);
    int W = (size-1)/2;

    int rows = imL.GetRows();
    int cols = imL.GetCols();
    std::vector<float> matchingPixels (rows*cols*3, 0.0);
    for (int j = rows-1-W; j >= W; --j) {
        for (int i = cols-1-W; i >= W; --i) {
            std::vector<float> mostSimilar = findSimilarPixel (imL, imR, i, j, size, 3);
            matchingPixels[rows*j*3+i*3] = mostSimilar[0];
            matchingPixels[rows*j*3+i*3+1] = mostSimilar[1];
            matchingPixels[rows*j*3+i*3+2] = mostSimilar[2];
        }
    }

    namedWindow("Display Left", WINDOW_AUTOSIZE );
    namedWindow("Display Right", WINDOW_AUTOSIZE );
    imL.Show ("Display Left");
    imR.Show ("Display Right");

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
                sum[k] += diff >= 0? diff : -diff;
            }
        }
    }
    return sum;
}

std::vector<float> findSimilarPixel(Image const& A, Image const& B, int x, int y, int size, int channels) {
    std::vector<float> similar (3);
    int W = (size-1)/2;
    Mat ROIA = A.GetRoi (Rect (x-W, y-W, size, size));
    int smallestX;
    float vSAD = FLT_MAX;
    for (int i = B.GetCols()-size; i >= 0; --i) {
        Mat ROIB = B.GetRoi (Rect (i, y-W, size, size));
        float sad = norm(SAD(ROIA, ROIB, channels));
        if (sad < vSAD) {
            vSAD = sad;
            smallestX = i+W;
        }
    }
    similar[0] = smallestX;
    similar[1] = y;
    similar[2] = vSAD;
    return similar;
}