#ifndef _VIDEO_HPP_
#define _VIDEO_HPP_

#include <cstdio>

#include "opencv2/opencv.hpp"

class Video;

#include "Image.hpp"

class Video : public Image {
  public:
    Video (const cv::String& file);
    Video (int camIndex);
    bool IsValid ();
    bool NextFrame ();
  private:
    cv::VideoCapture cap;
};

#endif // _VIDEO_HPP_
