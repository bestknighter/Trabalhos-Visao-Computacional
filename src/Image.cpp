#include "Image.hpp"

Image::Image (const cv::String & file) {
    image = imread (file, cv::IMREAD_UNCHANGED);
    isGrayscale = image.channels() == 1;
    image = imread (file, cv::IMREAD_COLOR);
}

Image::Image () {
    isGrayscale = false;
}

cv::Vec3b Image::at (int x, int y) {
    return image.at<cv::Vec3b> (y, x);
}

void Image::PaintPixels () {
    paintedImage = image.clone ();
    if (paintedImage.isContinuous ()) { // Se o frame for pequeno o suficiente para caber continuamente na memoria
                                        // Use o metodo mais rapido e eficiente para pintar
        uchar* pixel = paintedImage.data;
        for (int i = 0; i < paintedImage.rows*paintedImage.cols*3; i+=3, pixel+=3) {
            if (
                marginColor[0][0] <= pixel[0] && pixel[0] <= marginColor[1][0]
             && marginColor[0][1] <= pixel[1] && pixel[1] <= marginColor[1][1]
             && marginColor[0][2] <= pixel[2] && pixel[2] <= marginColor[1][2]
            ) {
               pixel[0] = colorToPaint[0];
               pixel[1] = colorToPaint[1];
               pixel[2] = colorToPaint[2];
            }
        }
    } else { // Se nao, use o metodo seguro mas bem mais lento
        for (int i = 0; i < paintedImage.rows; ++i) {
            for (int j = 0; j < paintedImage.cols; ++j) {
                cv::Vec3b pixel = paintedImage.at<cv::Vec3b> (i, j);
                if (
                    marginColor[0][0] <= pixel[0] && pixel[0] <= marginColor[1][0]
                 && marginColor[0][1] <= pixel[1] && pixel[1] <= marginColor[1][1]
                 && marginColor[0][2] <= pixel[2] && pixel[2] <= marginColor[1][2]
                ) {
                   paintedImage.at<cv::Vec3b> (i, j) = colorToPaint;
                }
            }
        }
}

void Image::Show (const cv::String & winname, bool showPainted) {
    imshow (winname, showPainted ? paintedImage : image);
}

bool Image::IsGrayscale () {
    return isGrayscale;
}

bool Image::IsValid () {
    return image.data;
}

void Image::Clicked (int x, int y) {
    printf ("Click! (%d,%d): ", x, y);

    selected = true;
    cv::Vec3b color = image.at<cv::Vec3b> (y, x);
    for (int i = 0; i<3; ++i) {
        marginColor[0][i] = color[i]*(1-margin);
        marginColor[1][i] = color[i]*(1+margin);
    }
    if (IsGrayscale ()) {
        printf ("Light (%d)\n", color[0]);
    } else {
        printf ("RGB (%d, %d, %d)\n", color[2], color[1], color[0]);
    }
}