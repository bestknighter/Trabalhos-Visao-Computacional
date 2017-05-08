#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

#include <string>

#include "opencv2/opencv.hpp"

class Image {
  public:
    Image (const std::string& file);
    bool selected;
    float margin = 0.05;
    uchar color[3] = {0,0,255};
    cv::Vec3b at (int x, int y) const;
    void PaintPixels ();
    virtual void Show (const std::string & winname, bool showPainted = false) const;
    bool IsGrayscale () const;
    virtual bool IsValid () const;
    void Clicked (int x, int y);
    int GetRows() const;
    int GetCols() const;
    cv::Mat GetRoi (cv::Rect region) const;
  protected:
    Image ();
    cv::Mat image;
    cv::Mat paintedImage;
    bool isGrayscale;
};

#endif //_IMAGE_HPP_