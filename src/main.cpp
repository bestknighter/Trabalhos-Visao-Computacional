#include <stdio.h>

#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"

#include "HarrisDetector.hpp"

#include "CMakeVars.hpp"

using namespace cv;

Mat image;
Mat grabbedFrame;
Mat seekFrame;

Mat SURFdescriptorGrabbed;
Mat SURFdescriptorSeek;

Mat grabbedHarrisDrawReference;
Mat grabbedHarrisDrawNew;
Mat grabbedSURFDrawReference;
Mat grabbedSURFDrawNew;

std::vector<KeyPoint> keypointsReference;
std::vector<KeyPoint> keypointsNew;

bool referenceFrameIsGrabbed = false;
bool seekFrameIsGrabbed = false;

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: Projeto.exe <cam index>\n");
        return -1;
    }
    
    VideoCapture cap;

    cap.open(std::atoi(argv[1]));

    if (!cap.isOpened()) {
        printf("Couldn't open camera");
    }

    int key = 0;
    do {

        cap.read(image);

        if ( !image.data )
        {
            printf("No image data \n");
            return -1;
        }

        imshow("Cam Feed", image);

        if (103 == key) { // g
            cvtColor(image, grabbedFrame, CV_BGR2GRAY);
            referenceFrameIsGrabbed = true;

            // Harris
            HarrisDetector harris; // Create Harris detector instance
            harris.detect(grabbedFrame); // Compute Harris values
            std::vector<Point> pts; // Detect Harris corners
            harris.getCorners(pts, 0.01);

            grabbedHarrisDrawReference = image.clone();
            harris.drawOnImage(grabbedHarrisDrawReference, pts); // Draw Harris corners

            namedWindow("Harris Reference", WINDOW_AUTOSIZE );
            imshow("Harris Reference", grabbedHarrisDrawReference);

            // SURF
            Ptr<xfeatures2d::SURF> surf = xfeatures2d::SURF::create(2500.); // Construct the SURF feature detector object
            surf->detect(grabbedFrame, keypointsReference); // Detect the SURF features
            surf->compute(grabbedFrame, keypointsReference, SURFdescriptorGrabbed); // Compute the SURF Descriptor

            // Draw the keypoints with scale and orientation information
            grabbedSURFDrawReference = image.clone();
            drawKeypoints(grabbedFrame, keypointsReference, grabbedSURFDrawReference, Scalar(255,255,255), DrawMatchesFlags::DRAW_OVER_OUTIMG | DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

            namedWindow("Surf Reference", WINDOW_AUTOSIZE );
            imshow("Surf Reference", grabbedSURFDrawReference);
        }

        if (referenceFrameIsGrabbed) {
            cvtColor(image, seekFrame, CV_BGR2GRAY);
            seekFrameIsGrabbed = true;
            
            // Harris
            HarrisDetector harris; // Create Harris detector instance
            harris.detect(seekFrame); // Compute Harris values
            std::vector<Point> pts; // Detect Harris corners
            harris.getCorners(pts, 0.01);

            grabbedHarrisDrawNew = image.clone();
            harris.drawOnImage(grabbedHarrisDrawNew, pts); // Draw Harris corners

            namedWindow("Harris New", WINDOW_AUTOSIZE );
            imshow("Harris New", grabbedHarrisDrawNew);
            
            // SURF
            Ptr<xfeatures2d::SURF> surf = xfeatures2d::SURF::create(2500.); // Construct the SURF feature detector object
            surf->detect(seekFrame, keypointsNew); // Detect the SURF features
            surf->compute(seekFrame, keypointsNew, SURFdescriptorSeek); // Compute the SURF Descriptor

            // Draw the keypoints with scale and orientation information
            grabbedSURFDrawNew = image.clone();
            drawKeypoints(seekFrame, keypointsNew, grabbedSURFDrawNew, Scalar(255,255,255), DrawMatchesFlags::DRAW_OVER_OUTIMG | DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

            namedWindow("Surf New", WINDOW_AUTOSIZE );
            imshow("Surf New", grabbedSURFDrawNew);

            // Matcher
            Ptr<BFMatcher> matcher = BFMatcher::create(); // Construction of the matcher
            std::vector<DMatch> matches; // Match the two image descriptors
            matcher->match(SURFdescriptorGrabbed, SURFdescriptorSeek, matches);

            std::nth_element(matches.begin(), matches.begin()+24, matches.end());
            matches.erase(matches.begin()+25, matches.end()); // remove all elements after the 25th
            Mat imageMatches;
            drawMatches(grabbedSURFDrawReference, keypointsReference, grabbedSURFDrawNew, keypointsNew, matches, imageMatches, Scalar(255,255,255));

            namedWindow("Matches", WINDOW_AUTOSIZE );
            imshow("Matches", imageMatches);
        }

        key = waitKey(1);
        if (key != 255) {
            printf("pressed: %d\n", key);
        }
    } while(27 != key); // ESC

    return 0;
}