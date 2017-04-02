#include <cstdio>
#include <cmath>
#include <string>
#include "opencv2/opencv.hpp"

#include "Image.hpp"
#include "Video.hpp"
#include "CMakeVars.hpp"

#define NOME_JANELA "Display Image"

void ImageClickHandler (int event, int x, int y, int flags, void* userdata);
void VideoClickHandler (int event, int x, int y, int flags, void* userdata);

int main(int argc, char** argv )
{
    /* Para Imagens /
    {
        if (argc != 2) {
            printf("usage: Projeto1 <Image_Path>\n");
            return -1;
        }

        Image im {argv[1]};
        if (!im.IsValid ()) {
            printf("No image data\n");
            return -1;
        }

        cv::namedWindow (NOME_JANELA, cv::WINDOW_AUTOSIZE);
        im.Show (NOME_JANELA);

        cv::setMouseCallback (NOME_JANELA, ImageClickHandler, &im);

        cv::waitKey (0);
    }
    //*/

    /* Para Videos e Cameras */
    {
        if (argc != 2) {
            printf("usage: Projeto1 <Video_Path>\n");
            return -1;
        }

        Video vid {std::stoi ({argv[1]})}; // Camera
        // Video vid {argv[1]}; // Video
        if (!vid.IsValid ()) {
            printf("Video invalid. Probably the video/camera doesn't exist or is being used\n");
            return -1;
        }

        cv::namedWindow (NOME_JANELA, cv::WINDOW_AUTOSIZE);
        cv::setMouseCallback (NOME_JANELA, VideoClickHandler, &vid);

        char key;
        for (bool naoAcabou = vid.NextFrame (); (key = (char)cv::waitKey (1)) < 0 && naoAcabou; naoAcabou = vid.NextFrame ()) {
            if (vid.selected) {
                vid.PaintPixels ();
            }
            vid.Show (NOME_JANELA, vid.selected);
        }

    }
    //*/

    return 0;
}

void ImageClickHandler (int event, int x, int y, int flags, void* userdata) {
    if (cv::EVENT_LBUTTONDOWN == event) {
        Image* image = (Image*)userdata;
        image->Clicked (x, y);
        image->PaintPixels ();
        image->Show (NOME_JANELA, true);
    }
}

void VideoClickHandler (int event, int x, int y, int flags, void* userdata) {
    if (cv::EVENT_LBUTTONDOWN == event) {
        Video* vid = (Video*)userdata;
        vid->Clicked (x, y);
    }
}