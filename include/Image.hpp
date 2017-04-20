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
    cv::Mat& GetImage ();
    virtual bool IsValid ();
    virtual void Clicked (int x, int y);
    cv::Size GetSize ();
  protected:
    Image ();
    cv::Mat image;
    bool isGrayscale;
};

#endif // _IMAGE_HPP_
