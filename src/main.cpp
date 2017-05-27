#include <cstdio>

#include "opencv2/opencv.hpp"
#include "CMakeVars.hpp"

typedef unsigned char uchar;

using namespace cv;

#define NIVEIS_DE_CINZA 256

int main(int argc, char** argv ) {

    // Lê a imagem
    if ( argc != 2 ) {
        printf("Caminho para imagem esperado mas nao recebido...\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], IMREAD_GRAYSCALE );

    if ( !image.data ) {
        printf( "No image data\n" );
        return -1;
    }
    
    // Computa GLCM-0 (GLCM0) e GLCM-45(GLCM1)
    Mat GLCM0 = Mat::zeros( Size(NIVEIS_DE_CINZA, NIVEIS_DE_CINZA), CV_32FC1 );
    Mat GLCM1 = Mat::zeros( Size(NIVEIS_DE_CINZA, NIVEIS_DE_CINZA), CV_32FC1 );

    for ( int j = 0; j < image.rows; ++j ) {
        for ( int i = 0; i < image.cols-1; ++i ) {
            uchar p = NIVEIS_DE_CINZA*image.at<uchar>( j, i )/256;

            uchar p0 = NIVEIS_DE_CINZA*image.at<uchar>( j, i+1 )/256;
            ++GLCM0.at<float>( p, p0 );

            if ( 0 < j ) {
                uchar p1 = NIVEIS_DE_CINZA*image.at<uchar>( j-1, i+1 )/256;
                ++GLCM1.at<float>( p, p1 );
            }
        }
    }

    // Normaliza as GLCMs
    float numPixels0 = image.rows*(image.cols-1);
    float numPixels1 = (image.rows-1)*(image.cols-1);

    GLCM0.forEach<float>(
            [numPixels0]
            ( float& p, const int position[] __attribute__((unused)) )
            { p = p/numPixels0; }
        );
    GLCM1.forEach<float>(
            [numPixels1]
            ( float& p, const int position[] __attribute__((unused)) )
            { p = p/numPixels1; }
        );

    // Salva as GLCMs num arquivo .xml
    FileStorage glcm( String(argv[1])+".xml", FileStorage::WRITE );

    glcm << "GLCM-0" << GLCM0;
    glcm << "GLCM-45" << GLCM1;

    glcm.release();

    return 0;
}