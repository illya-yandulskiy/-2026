#pragma once

enum class ProcessMode {
    NORMAL,
    INVERT_COLORS,
    GAUSSIAN_BLUR,
    CANNY_EDGES,
    SOBEL_FILTER,
    FACE_DETECT 
};

class KeyProcessor {
private:
    ProcessMode currentMode;
public:
    KeyProcessor();
    void processKey(int key);
    ProcessMode getCurrentMode() const;
};