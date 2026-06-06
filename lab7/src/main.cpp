#include <iostream>
#include "../include/CameraProvider.hpp"
#include "../include/KeyProcessor.hpp"
#include "../include/FrameProcessor.hpp"
#include "../include/Display.hpp"
#include "../include/FaceDetector.hpp" 

int main() {
    CameraProvider camera;
    KeyProcessor keyProc;
    FrameProcessor frameProc;
    Display display("Lab 7 - DNN Face Detection");

    FaceDetector faceDetector("deploy.prototxt", "res10_300x300_ssd_iter_140000.caffemodel");
    faceDetector.start();

    std::cout << "Натисніть 'F' для детекції облич, '1-5' для фільтрів, 'Q' для виходу.\n";

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) break;

        cv::Mat processed = frameProc.process(frame, keyProc.getCurrentMode());

        if (keyProc.getCurrentMode() == ProcessMode::FACE_DETECT) {
            faceDetector.updateFrame(frame);
            
            auto faces = faceDetector.getFaces();
            
            for (const auto& face : faces) {
                cv::rectangle(processed, face, cv::Scalar(0, 255, 0), 3);
            }
        }

        display.show(processed);

        int key = cv::waitKey(30);
        if (key == 'q' || key == 'Q' || key == 27) break;
        else if (key != -1) keyProc.processKey(key);
    }

    faceDetector.stop(); 
    return 0;
}