#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

#include "opencv2/opencv.hpp"

class Image {
  public:
    Image (const cv::String& file);
    uchar color[3] = {0,0,255};
    cv::Vec3b at (int x, int y);
    void Show (const cv::String & winname);
    bool IsGrayscale ();
    virtual bool IsValid ();
    void Clicked (int x, int y);
  protected:
    Image ();
    cv::Mat image;
    bool isGrayscale;
};

#endif // _IMAGE_HPP_
