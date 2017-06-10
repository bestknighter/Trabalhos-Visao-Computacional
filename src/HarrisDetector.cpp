#include "HarrisDetector.hpp"

HarrisDetector::HarrisDetector()
         : k(0.01), maxStrength(0.0), threshold(0.01)
         , neighbourhood(3), aperture(3), nonMaxSize(3) {
    setLocalMaxWindowSize(nonMaxSize);
}

void HarrisDetector::setLocalMaxWindowSize(int size) {
    nonMaxSize = size;
    kernel.create(nonMaxSize, nonMaxSize, CV_8U);
}

// Harris computation
void HarrisDetector::detect(const Mat& image) {
    cornerHarris(image, cornerStrength, neighbourhood, aperture, k);
    
    double minStrength; // not used
    minMaxLoc(cornerStrength, &minStrength, &maxStrength);
    
    Mat dilated; // temporary image
    dilate(cornerStrength, dilated, Mat());
    compare(cornerStrength, dilated, localMax, CMP_EQ);
}

Mat HarrisDetector::getCornerMap(double qualityLevel) {
    Mat cornerMap;
    
    threshold = qualityLevel*maxStrength;
    cv::threshold(cornerStrength, cornerTh, threshold, 255, THRESH_BINARY);

    cornerTh.convertTo(cornerMap, CV_8U);
    
    bitwise_and(cornerMap, localMax, cornerMap);

    return cornerMap;
}

void HarrisDetector::getCorners(std::vector<Point> &points, double qualityLevel) {
    Mat cornerMap = getCornerMap(qualityLevel); // Get the corner map
    getCorners(points, cornerMap); // Get the corners
}

void HarrisDetector::getCorners(std::vector<Point> &points, const Mat& cornerMap) {
    for( int y = 0; y < cornerMap.rows; y++ ) {
        // Iterate over the pixels to obtain all features
        const uchar* rowPtr = cornerMap.ptr<uchar>(y);
        for( int x = 0; x < cornerMap.cols; x++ ) {
            if (rowPtr[x]) { // if it is a feature point
                points.push_back(Point(x,y));
            }
        }
    }
}

void HarrisDetector::drawOnImage(Mat &image, const std::vector<Point> &points
                                , Scalar color, int radius, int thickness) {
    std::vector<Point>::const_iterator it = points.begin();
    while (it!=points.end()) { // for all corners
        // draw a circle at each corner location
        circle(image, *it, radius, color, thickness);
        ++it;
    }
}
