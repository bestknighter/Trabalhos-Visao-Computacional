#include "Video.hpp"

Video::Video (const cv::String& file) : Image () {
    cap = {file};
    selected = false;
}

Video::Video (int camIndex) : Image () {
    cap = {camIndex};
    selected = false;
}

void Video::Show (const cv::String& winname, bool showPainted) {
    imshow (winname, showPainted ? paintedImage : image);
}

bool Video::IsValid () {
    return cap.isOpened ();
}

bool Video::NextFrame () {
    cap >> image;
    return !image.empty ();
}