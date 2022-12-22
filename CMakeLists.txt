# cmake_minimum_required(VERSION 2.8)
project(Bela-Spectrogram)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 14)


add_executable(Bela-Spectrogram src/main.cpp inc/Matrix.h core/Matrix.cpp inc/Spectrogram.h core/Spectrogram.cpp inc/FFT.h core/FFT.cpp inc/ReadWavFile.h core/ReadWavFile.cpp inc/WavDataNormalisation.h core/WavDataNormalisation.cpp)
