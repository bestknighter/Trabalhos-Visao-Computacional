#ifndef HARRISDETECTOR_H
#define HARRISDETECTOR_H

#include "opencv2/opencv.hpp"

#include <memory>

using namespace cv;

class HarrisDetector {
  private:
    Mat cornerStrength; // 32-bit float image of corner strength
    Mat cornerTh; // 32-bit float image of thresholded corners
    Mat localMax; // image of local maxima (internal)
    Mat kernel; // kernel for non-max suppression
    double k; // Harris parameter
    double maxStrength; // maximum strength for threshold computation
    double threshold; // calculated threshold (internal)
    int neighbourhood; // size of neighborhood for derivatives smoothing
    int aperture; // aperture for gradient computation
    int nonMaxSize; // size of neighborhood for non-max suppression
  public:
    HarrisDetector();
    void setLocalMaxWindowSize(int size);
    Mat getCornerMap(double qualityLevel);
    void detect(const Mat& image);
    void getCorners(std::vector<Point> &points, double qualityLevel);
    void getCorners(std::vector<Point> &points, const Mat& cornerMap);
    void drawOnImage(Mat &image, const std::vector<Point> &points
                    , Scalar color=Scalar(255,255,255)
                    , int radius=3, int thickness=1);
};

#endif // HARRISDETECTOR_H