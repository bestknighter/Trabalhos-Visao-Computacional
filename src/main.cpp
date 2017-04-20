#include <cstdio>
#include <string>

#include "opencv2/opencv.hpp"

#include "CMakeVars.hpp"
#include "Video.hpp"

#define NOME_JANELA "Display Image"

using namespace cv;

void VideoClickHandler (int event, int x, int y, int flags, void* userdata);

int main (int argc, char** argv) {
    if (argc != 2) {
        printf("usage: Projeto_2 <webcam_index>\n");
        return -1;
    }

    // Video vid {std::stoi (argv[1])};
    Video vid {argv[1]};

    if (!vid.IsValid ()) {
        printf("Invalid stream\n");
        return -1;
    }

    namedWindow (NOME_JANELA, WINDOW_AUTOSIZE);
    cv::setMouseCallback (NOME_JANELA, VideoClickHandler, &vid);
    
    while (vid.NextFrame () && 255 == waitKey (1)) {
        vid.Show (NOME_JANELA);
    }

    return 0;
}

void VideoClickHandler (int event, int x, int y, int flags, void* userdata) {
    Video* vid = (Video*)userdata;
    if (cv::EVENT_LBUTTONDOWN == event) {
        vid->Clicked (x, y);
    }
    vid->mouse = {x,y};
}
