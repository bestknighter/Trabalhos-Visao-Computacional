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
void Calibrar (Video& vid, Mat& intrinsicMat, Mat& distortionsMat, int n_snaps);
void Medias (const std::vector<Mat>& intrinsics, const std::vector<Mat>& distortionsVec, Mat& mediaIntr, Mat& mediaDist);

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

    int n_snaps, n_calibs;
    printf ("Quantas calibracaoes fazer? ");
    scanf ("%d", &n_calibs);
    printf ("Quantos snapshots capturar? ");
    scanf ("%d", &n_snaps);

    std::vector<Mat> intrinsicos (n_calibs);
    std::vector<Mat> distortions (n_calibs);


    namedWindow (JANELA_RAW, WINDOW_AUTOSIZE);
    setMouseCallback (JANELA_RAW, VideoClickHandler, &vid);
    
    // Calibra
    for (int i = 0; i < n_calibs; i++) {
        std::string nomeIntr = "intrinsic";
        std::string nomeDist = "distortions";
        std::string ext = ".xml";
        
        Calibrar (vid, intrinsicos[i], distortions[i], n_snaps);
        
        nomeIntr += std::to_string (i+1) + ext;
        nomeDist += std::to_string (i+1) + ext;
        FileStorage fsIntrin {nomeIntr, FileStorage::WRITE};
        FileStorage fsDistor {nomeDist, FileStorage::WRITE};

        fsIntrin << "intrinsic_data" << intrinsicos[i];
        fsDistor << "distortion_data" << distortions[i];
    }

    // Calcula e salva a media
    {
        Mat mediaIntrin, mediaDistr;
        Medias (intrinsicos, distortions, mediaIntrin, mediaDistr);

        FileStorage fsIntMed {"intrinsicsMedia.xml", FileStorage::WRITE};
        FileStorage fsDisMed {"distortionsMedia.xml", FileStorage::WRITE};

        fsIntMed << "intrinsic_data" << mediaIntrin;
        fsDisMed << "distortion_data" << mediaDistr;
    }

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

void Calibrar (Video& vid, Mat& intrinsicMat, Mat& distortionsMat, int n_snaps) {
    int snaps_capturados = 0;
    std::vector<std::vector<Point2f>> cantosTotal;
    std::vector<std::vector<Vec3f>> idxCantosTotal;
    Mat intrinsic = Mat::eye(3, 3, CV_64F);
    Mat distortions = Mat::zeros(8, 1, CV_64F);

    tecla = -1;
    while (vid.NextFrame () && snaps_capturados < n_snaps) {
        if (ESC_KEY == tecla) return;

        if (SPACE_KEY == tecla) {
            std::vector<Point2f> cantosPorImagem;
            std::vector<Vec3f> idxCantosDetectados;
            Mat temp;
            cvtColor (vid.GetImage (), temp, CV_BGR2GRAY, 1);
            bool achou = findChessboardCorners (temp, {BOARD_W,BOARD_H}, cantosPorImagem, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
            if (achou) {
                cornerSubPix (temp, cantosPorImagem, {11,11}, {-1,-1}, {CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1});
                if (cantosPorImagem.size () == BOARD_W*BOARD_H) {
                    printf ("%d de %d snapshots capturados com sucesso\n", ++snaps_capturados, n_snaps);
                    cantosTotal.push_back (cantosPorImagem);
                    for (int i = 0; i < BOARD_W*BOARD_H; ++i) {
                        idxCantosDetectados.emplace_back (i / BOARD_W, i % BOARD_W, 0.0);
                    }
                    idxCantosTotal.push_back (idxCantosDetectados);
                } else {
                    printf ("Snapshot parcialmente oculto, tente novamente...\n");
                }
            }
            temp = vid.GetImage ().clone ();
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

        std::vector<Mat> rot, tran;
        Mat devPadInt, devPadExt, erroPorImagem;

        calibrateCamera (idxCantosTotal, cantosTotal, vid.GetSize (), intrinsic, distortions, rot, tran, devPadInt, devPadExt, erroPorImagem);

        intrinsicMat = intrinsic.clone ();
        distortionsMat = distortions.clone ();
    }
}

void Medias (const std::vector<Mat>& intrinsics, const std::vector<Mat>& distortionsVec, Mat& mediaIntr, Mat& mediaDist) {
    mediaIntr = Mat::zeros (intrinsics[0].rows, intrinsics[0].cols, intrinsics[0].type ());
    mediaDist = Mat::zeros (distortionsVec[0].rows, distortionsVec[0].cols, distortionsVec[0].type ());

    int n_elemsVec = intrinsics.size ();
    for (int i = 0; i < n_elemsVec; ++i) {

        int n_elemsMat = intrinsics[i].rows*intrinsics[i].cols;
        for (int j = 0; j < n_elemsMat; j++) {
            mediaIntr.begin <double> () [j] += intrinsics[i].begin <double> () [j] / n_elemsVec;
        }
    }

    n_elemsVec = distortionsVec.size ();
    for (int i = 0; i < n_elemsVec; ++i) {

        int n_elemsMat = distortionsVec[i].rows*distortionsVec[i].cols;
        for (int j = 0; j < n_elemsMat; j++) {
            mediaDist.begin <double> () [j] += distortionsVec[i].begin <double> () [j] / n_elemsVec;
        }
    }
}
