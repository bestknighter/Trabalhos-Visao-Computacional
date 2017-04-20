#include <cstdio>
#include <string>
#include <vector>

#include "opencv2/opencv.hpp"

#include "CMakeVars.hpp"
#include "Video.hpp"

#define JANELA_RAW "Imagem Raw"
#define JANELA_CORRIGIDA "Imagem Corrigida"

#define ESC_KEY 27
#define SPACE_KEY 32

#define BOARD_W 8
#define BOARD_H 6

using namespace cv;

void Instrucoes ();
void VideoClickHandler (int event, int x, int y, int flags, void* userdata);
void Calibrar (Video& vid);

int tecla = -1;

int main (int argc, char** argv) {
    if (argc != 2) {
        printf("usage: Projeto_2 <webcam_index>\n");
        return -1;
    }

    Instrucoes ();

    Video vid {std::stoi (argv[1])};
    // Video vid {argv[1]};

    if (!vid.IsValid ()) {
        printf("Invalid stream\n");
        return -1;
    }

    namedWindow (JANELA_RAW, WINDOW_AUTOSIZE);
    setMouseCallback (JANELA_RAW, VideoClickHandler, &vid);
    Calibrar (vid);

    // Exibe imagem, linha e distancia
    {
        while (vid.NextFrame () && ESC_KEY != tecla) {
            vid.DrawOnImage ();
            vid.Show (JANELA_RAW);

            tecla = waitKey(1);
            printf("%d ", tecla);
        }
    }

    return 0;
}

void Instrucoes () {
    printf (" - Pressione ESC para sair da aplicacao\n");
    printf (" - Pressione ESPACO para capturar uma snapshot\n");
    system ("PAUSE");
}

void VideoClickHandler (int event, int x, int y, int flags, void* userdata) {
    Video* vid = (Video*)userdata;
    if (cv::EVENT_LBUTTONDOWN == event) {
        vid->Clicked (x, y);
    }
    vid->mouse = {x,y};
}

void Calibrar (Video& vid) {
    int n_snaps;
    int snaps_capturados = 0;
    std::vector<std::vector<Point2f>> cantosTotal;
    std::vector<std::vector<Vec3f>> idxCantosTotal;
    Mat<float> instrinsic {3, 3, 1};
    Mat<float> distortions {4, 1, 1};

    printf ("Quantos snaps capturar? ");
    scanf ("%d", &n_snaps);

    tecla = -1;
    while (vid.NextFrame () && snaps_capturados < n_snaps) {
        if (ESC_KEY == tecla) return;

        if (SPACE_KEY == tecla) {
            std::vector<Point2f> cantosPorImagem;
            std::vector<Vec3f> idxCantosDetectados;
            Mat temp = vid.GetImage ().clone ();
            bool achou = findChessboardCorners (temp, {BOARD_W,BOARD_H}, cantosPorImagem, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
            if (achou) {
                cornerSubPix (temp, cantosPorImagem, {11,11}, {-1,-1}, {CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1});
                if (cantosPorImagem.size () == BOARD_W*BOARD_H) {
                    printf ("%d de %d snapshots capturados com sucesso\n", ++snaps_capturados, n_snaps);
                    cantosTotal.push_back (cantosPorImagem);
                    for (float i = 0; i < BOARD_W*BOARD_H; ++i) {
                        idxCantosDetectados.emplace_back (i / BOARD_W, i % BOARD_W, 0.0);
                    }
                    idxCantosTotal.push_back (idxCantosDetectados);
                } else {
                    printf ("Snapshot parcialmente oculto, tente novamente...\n");
                }
            }
            drawChessboardCorners (temp, {8,6}, {cantosPorImagem}, achou);
            namedWindow ("Previz");
            imshow ("Previz", temp);
            waitKey (500);
            destroyWindow ("Previz");
        }

        vid.Show (JANELA_RAW);

        tecla = waitKey(1);
    }

    if (snaps_capturados == n_snaps) {

    }
}