#include "../include/FrameProcessor.hpp"

cv::Mat FrameProcessor::process(const cv::Mat& inputFrame, ProcessMode mode) {
    if (inputFrame.empty()) return inputFrame;

    cv::Mat result;
    switch (mode) {
        case ProcessMode::NORMAL:
            result = inputFrame.clone();
            break;
        case ProcessMode::INVERT_COLORS:
            cv::bitwise_not(inputFrame, result); 
            break;
        case ProcessMode::GAUSSIAN_BLUR:
            cv::GaussianBlur(inputFrame, result, cv::Size(15, 15), 0); 
            break;
        case ProcessMode::CANNY_EDGES: { // 
            cv::Mat gray;
            cv::cvtColor(inputFrame, gray, cv::COLOR_BGR2GRAY);
            cv::Canny(gray, result, 50, 150); 
            break;
        } 
        case ProcessMode::SOBEL_FILTER: { 
            cv::Mat graySobel, grad_x, grad_y, abs_grad_x, abs_grad_y;
            cv::cvtColor(inputFrame, graySobel, cv::COLOR_BGR2GRAY);
            cv::Sobel(graySobel, grad_x, CV_16S, 1, 0, 3);
            cv::Sobel(graySobel, grad_y, CV_16S, 0, 1, 3);
            cv::convertScaleAbs(grad_x, abs_grad_x);
            cv::convertScaleAbs(grad_y, abs_grad_y);
            cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, result); // Фільтр Собеля
            break;
        } 
    }
    return result;
}