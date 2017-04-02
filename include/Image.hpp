#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

#include "opencv2/opencv.hpp"

class Image {
  public:
    Image (const cv::String& file);
    bool selected;
    float margin = 0.05;
    uchar colorToPaint[3] = {0,0,255};
    cv::Vec3b at (int x, int y);
    void PaintPixels ();
    virtual void Show (const cv::String & winname, bool showPainted = false);
    bool IsGrayscale ();
    virtual bool IsValid ();
    void Clicked (int x, int y);
  protected:
    Image ();
    cv::Mat image;
    cv::Mat paintedImage;
    int marginColor[2][3] = {0};
    bool isGrayscale;
};

#endif //_IMAGE_HPP_