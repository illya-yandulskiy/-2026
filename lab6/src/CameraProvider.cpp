#include "../include/CameraProvider.hpp"
#include <iostream>

CameraProvider::CameraProvider() {
    cap.open(0); 
    if (!cap.isOpened()) {
        std::cerr << "Помилка: Не вдалося відкрити камеру!" << std::endl;
    }
}

CameraProvider::~CameraProvider() {
    cap.release(); 
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    cap >> frame; 
    return frame;
}