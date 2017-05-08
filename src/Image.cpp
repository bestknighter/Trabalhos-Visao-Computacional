#include "Image.hpp"

Image::Image (const std::string & file) {
    image = imread (file, cv::IMREAD_UNCHANGED);
    isGrayscale = image.channels() == 1;
    image = imread (file, cv::IMREAD_COLOR);
}

Image::Image () {
    isGrayscale = false;
}

cv::Vec3b Image::at (int x, int y) const {
    return image.at<cv::Vec3b> (y, x);
}

void Image::PaintPixels () {
    paintedImage = image.clone ();
    if (paintedImage.isContinuous ()) { // Se o frame for pequeno o suficiente para caber continuamente na memoria
                                        // Use o metodo mais rapido e eficiente para pintar
        
    } else { // Se nao, use o metodo seguro mas bem mais lento
        
    }
}

void Image::Show (const std::string & winname, bool showPainted) const {
    imshow (winname, showPainted ? paintedImage : image);
}

bool Image::IsGrayscale () const {
    return isGrayscale;
}

bool Image::IsValid () const {
    return image.data;
}

void Image::Clicked (int x, int y) {
    printf ("Click! (%d,%d): ", x, y);

    selected = true;
    if (IsGrayscale ()) {
        printf ("Light (%d)\n", color[0]);
    } else {
        printf ("RGB (%d, %d, %d)\n", color[2], color[1], color[0]);
    }
}

int Image::GetCols() const {
    return image.cols;
}

int Image::GetRows() const {
    return image.rows;
}

cv::Mat Image::GetRoi(cv::Rect region) const {
    return image(region);
}