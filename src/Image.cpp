#include "Image.hpp"

Image::Image (const cv::String & file) {
    image = imread (file, cv::IMREAD_UNCHANGED);
    isGrayscale = image.channels() == 1;
    image = imread (file, cv::IMREAD_COLOR);
}

Image::Image () {
    isGrayscale = false;
}

cv::Vec3b Image::at (int x, int y) {
    return image.at<cv::Vec3b> (y, x);
}

void Image::Show (const cv::String & winname) {
    imshow (winname, image);
}

bool Image::IsGrayscale () {
    return isGrayscale;
}

bool Image::IsValid () {
    return image.data;
}

void Image::Clicked (int x, int y) {
    
}
