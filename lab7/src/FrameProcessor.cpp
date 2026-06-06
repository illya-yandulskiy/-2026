#include "../include/FrameProcessor.hpp"
#include <opencv2/imgproc.hpp>

cv::Mat FrameProcessor::process(const cv::Mat& input, ProcessMode mode) {
    cv::Mat output;
    
    switch (mode) {
        case ProcessMode::INVERT_COLORS:
            cv::bitwise_not(input, output);
            break;
        case ProcessMode::GAUSSIAN_BLUR:
            cv::GaussianBlur(input, output, cv::Size(15, 15), 0);
            break;
        case ProcessMode::CANNY_EDGES:
            cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
            cv::Canny(output, output, 100, 200);
            cv::cvtColor(output, output, cv::COLOR_GRAY2BGR); 
            break;
        case ProcessMode::SOBEL_FILTER:
            cv::Sobel(input, output, CV_8U, 1, 1);
            break;
            
        case ProcessMode::NORMAL:
        case ProcessMode::FACE_DETECT: 
        default:
            output = input.clone();
            break;
    }
    
    return output;
}