#!/bin/bash
echo "Починаємо збірку проєкту..."
mkdir -p build
cd build
cmake ..
make
echo "Збірка успішна! Виконуваний файл знаходиться у папці build."