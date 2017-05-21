#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

#include "opencv2/opencv.hpp"

class Image {
  public:
    Image (const cv::String& file);
    cv::Vec3b at (int x, int y);
    virtual void Show (const cv::String & winname);
    bool IsGrayscale ();
    virtual bool IsValid ();
    void Clicked (int x, int y);
    cv::Mat GetMat();
  protected:
    Image ();
    cv::Mat image;
    bool isGrayscale;
};

#endif //_IMAGE_HPP_