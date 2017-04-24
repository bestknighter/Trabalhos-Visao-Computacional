#include "Image.hpp"

Image::Image (const cv::String & file) {
    image = cv::imread (file, cv::IMREAD_UNCHANGED);
    isGrayscale = image.channels() == 1;
    image = cv::imread (file, cv::IMREAD_COLOR);

    intrinsic = cv::Mat::eye (3, 3, CV_64F);
    distortions = cv::Mat::zeros (8, 1, CV_64F);
}

Image::Image () {
    isGrayscale = false;

    intrinsic = cv::Mat::eye (3, 3, CV_64F);
    distortions = cv::Mat::zeros (8, 1, CV_64F);
}

cv::Vec3b Image::at (int x, int y) {
    return image.at<cv::Vec3b> (y, x);
}

void Image::Show (const cv::String & winname) {
    cv::imshow (winname, image);
}

void Image::SetCaracteristics (cv::Mat& intrinsics, cv::Mat& distortions) {
    this->intrinsic = intrinsics;
    this->distortions = distortions;
}

void Image::Undistort () {
    cv::Mat temp;
    cv::undistort (image, temp, intrinsic, distortions);
    image = temp;
}

bool Image::IsGrayscale () {
    return isGrayscale;
}

bool Image::IsValid () {
    return image.data;
}

void Image::Clicked (int x, int y) {
    
}

cv::Mat& Image::GetImage () {
    return image;
}

cv::Size Image::GetSize () {
    return IsValid () ? image.size () : cv::Size {0,0};
}