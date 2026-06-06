#!/bin/bash
echo "Оновлення списку пакетів та встановлення OpenCV, CMake і компілятора..."
sudo apt update
sudo apt install -y libopencv-dev cmake g++ make
echo "Встановлення завершено!"