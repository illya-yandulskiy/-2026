#include <iostream>
#include "../include/CameraProvider.hpp"
#include "../include/KeyProcessor.hpp"
#include "../include/FrameProcessor.hpp"
#include "../include/Display.hpp"

int main() {
    CameraProvider camera;
    KeyProcessor keyProc;
    FrameProcessor frameProc;
    Display display("Lab 6 - Video Processing");

    std::cout << "Запуск камери...\n";
    std::cout << "Натисніть 1-5 для зміни режимів.\n";
    std::cout << "Натисніть 'q' або 'ESC' для виходу.\n";

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) {
            std::cerr << "Не вдалося отримати кадр!" << std::endl;
            break;
        }

        cv::Mat processed = frameProc.process(frame, keyProc.getCurrentMode());
        
        display.show(processed);

        int key = cv::waitKey(30);
        if (key == 'q' || key == 27) { 
            break;
        } else if (key != -1) {
            keyProc.processKey(key);
        }
    }

    return 0;
}