#ifndef _VIDEO_HPP_
#define _VIDEO_HPP_

#include <cmath>
#include <string>

#include "opencv2/opencv.hpp"

class Video;

#include "Image.hpp"

class Video : public Image {
  public:
    Video (const cv::String& file);
    Video (int camIndex);
    bool IsValid ();
    bool NextFrame ();
    void DrawOnImage ();
    void Clicked (int x, int y);
    cv::Point mouse;
  private:
    cv::VideoCapture cap;
    cv::Point cliqueTemp;
    float distancia;
    cv::Point points[2];
    cv::Scalar cor {0,0,255};
    int primeiroClique = false;
    bool desenhar = false;
};

#endif // _VIDEO_HPP_
