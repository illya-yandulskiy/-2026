#include "../include/FaceDetector.hpp"
#include <iostream>
#include <chrono>

FaceDetector::FaceDetector(const std::string& prototxt, const std::string& model) {
    net = cv::dnn::readNetFromCaffe(prototxt, model);
    running = false;
    hasNewFrame = false;
}

FaceDetector::~FaceDetector() { stop(); }

void FaceDetector::start() {
    running = true;
    workerThread = std::thread(&FaceDetector::detectionLoop, this);
}

void FaceDetector::stop() {
    running = false;
    cv.notify_all();
    if (workerThread.joinable()) workerThread.join();
}

void FaceDetector::updateFrame(const cv::Mat& frame) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        currentFrame = frame.clone();
        hasNewFrame = true;
    }
    cv.notify_one();
}

std::vector<cv::Rect> FaceDetector::getFaces() {
    std::lock_guard<std::mutex> lock(mtx);
    return detectedFaces;
}

void FaceDetector::detectionLoop() {
    while (running) {
        cv::Mat frameToProcess;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() { return hasNewFrame || !running; });
            if (!running) break;
            frameToProcess = currentFrame.clone();
            hasNewFrame = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        cv::Mat blob = cv::dnn::blobFromImage(frameToProcess, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0));
        net.setInput(blob);
        cv::Mat detection = net.forward();
        
        cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
        std::vector<cv::Rect> faces;
        for (int i = 0; i < detectionMat.rows; i++) {
            float conf = detectionMat.at<float>(i, 2);
            if (conf > 0.5) {
                faces.push_back(cv::Rect(
                    detectionMat.at<float>(i, 3) * frameToProcess.cols,
                    detectionMat.at<float>(i, 4) * frameToProcess.rows,
                    (detectionMat.at<float>(i, 5) - detectionMat.at<float>(i, 3)) * frameToProcess.cols,
                    (detectionMat.at<float>(i, 6) - detectionMat.at<float>(i, 4)) * frameToProcess.rows
                ));
            }
        }
        {
            std::lock_guard<std::mutex> lock(mtx);
            detectedFaces = faces;
        }
    }
}