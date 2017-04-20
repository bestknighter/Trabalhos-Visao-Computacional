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

void Video::DrawOnImage () {
    cv::String info1 = "Xi = " + std::to_string (points[0].x) + " Yi = " + std::to_string (points[0].y);
    cv::String info2 = "Xf = " + std::to_string (points[1].x) + " Yf = " + std::to_string (points[1].y);
    cv::String info3 = "D = " + std::to_string (distancia);
    
    cv::putText (image, info1, {5,15}, cv::HersheyFonts::FONT_HERSHEY_SIMPLEX, 0.5, cor, 1, cv::LineTypes::LINE_AA);
    cv::putText (image, info2, {5,35}, cv::HersheyFonts::FONT_HERSHEY_SIMPLEX, 0.5, cor, 1, cv::LineTypes::LINE_AA);
    cv::putText (image, info3, {5,55}, cv::HersheyFonts::FONT_HERSHEY_SIMPLEX, 0.5, cor, 1, cv::LineTypes::LINE_AA);

    if (primeiroClique) {
        cv::line (image, cliqueTemp, mouse, cor, 1, cv::LineTypes::LINE_AA);
    }
    if (desenhar) {
        cv::line (image, points[0], points[1], cor, 1, cv::LineTypes::LINE_AA);
    }
}

void Video::Clicked (int x, int y) {
    if (primeiroClique) {
        points[0] = cliqueTemp;
        points[1] = {x,y};
        distancia = std::sqrt (std::pow (points[1].x - points[0].x, 2) + std::pow (points[1].y - points[0].y, 2));
        desenhar = true;
    } else {
        cliqueTemp = {x,y};
    }
    primeiroClique = !primeiroClique;
}
