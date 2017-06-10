#include <stdio.h>

#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"

#include "HarrisDetector.hpp"

#include "CMakeVars.hpp"

using namespace cv;

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: Projeto.exe <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], IMREAD_COLOR );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);


    Mat grayscaleImageHarris;
    cvtColor(image, grayscaleImageHarris, CV_BGR2GRAY);
    Mat grayscaleImageSURF = grayscaleImageHarris.clone();

    HarrisDetector harris; // Create Harris detector instance
    harris.detect(grayscaleImageHarris); // Compute Harris values
    std::vector<cv::Point> pts; // Detect Harris corners
    harris.getCorners(pts, 0.01);
    harris.drawOnImage(grayscaleImageHarris, pts); // Draw Harris corners

    waitKey(0);
    namedWindow("Harris", WINDOW_AUTOSIZE );
    imshow("Harris", grayscaleImageHarris);



    std::vector<KeyPoint> keypoints; // vector of keypoints
    Ptr<xfeatures2d::SURF> surf = xfeatures2d::SURF::create(2500.); // Construct the SURF feature detector object
    surf->detect(grayscaleImageSURF, keypoints); // Detect the SURF features

    // Draw the keypoints with scale and orientation information
    drawKeypoints(grayscaleImageSURF, keypoints, grayscaleImageSURF, Scalar(255,255,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    waitKey(0);
    namedWindow("Surf", WINDOW_AUTOSIZE );
    imshow("Surf", grayscaleImageSURF);

    
    // TODO daqui em diante (abaixo foi so crtl+c crtl+v)
    SurfDescriptorExtractor surfDesc; // Construction of the SURF descriptor extractor
    Mat descriptors1; // Extraction of the SURF descriptors
    surfDesc.compute(image1,keypoints1,descriptors1);
    BruteForceMatcher<cv::L2<float>> matcher; // Construction of the matcher
    std::vector<DMatch> matches; // Match the two image descriptors
    matcher.match(descriptors1, descriptors2, matches);

    waitKey(0);

    return 0;
}