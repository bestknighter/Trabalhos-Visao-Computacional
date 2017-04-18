#include "Video.hpp"

Video::Video (const cv::String& file) : Image () {
    cap = {file};
}

Video::Video (int camIndex) : Image () {
    cap = {camIndex};
}

bool Video::IsValid () {
    return cap.isOpened ();
}

bool Video::NextFrame () {
    cap >> image;
    return !image.empty ();
}
