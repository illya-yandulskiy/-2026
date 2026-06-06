#include "../include/KeyProcessor.hpp"

KeyProcessor::KeyProcessor() : currentMode(ProcessMode::NORMAL) {}

void KeyProcessor::processKey(int key) {
    switch (key) {
        case '1': 
            currentMode = ProcessMode::NORMAL; 
            break;
        case '2': 
            currentMode = ProcessMode::INVERT_COLORS; 
            break;
        case '3': 
            currentMode = ProcessMode::GAUSSIAN_BLUR; 
            break;
        case '4': 
            currentMode = ProcessMode::CANNY_EDGES; 
            break;
        case '5': 
            currentMode = ProcessMode::SOBEL_FILTER; 
            break;
        
        case 'f':
        case 'F': 
            currentMode = ProcessMode::FACE_DETECT; 
            break;
            
        default: 
            break;
    }
}

ProcessMode KeyProcessor::getCurrentMode() const {
    return currentMode;
}