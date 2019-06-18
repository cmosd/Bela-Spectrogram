cmake_minimum_required(VERSION 2.8)
project(Bela-Spectrogram)

set(CMAKE_CXX_STANDARD 14)


add_executable(Bela-Spectrogram src/main.cpp inc/Matrix.h core/Matrix.cpp inc/ReadTextFile.h core/ReadTextFile.cpp inc/Spectrogram.h core/Spectrogram.cpp inc/FFT.h core/FFT.cpp inc/ReadWavFile.h core/ReadWavFile.cpp)