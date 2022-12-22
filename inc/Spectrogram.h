//
// Created by mariussolomes on 16/06/19.
//

#ifndef BELA_SPECTROGRAM_SPECTROGRAM_H
#define BELA_SPECTROGRAM_SPECTROGRAM_H

#include "FFT.h"
#include "Matrix.h"
#include <complex>

#define ALPHA .97
#define FRAME_SIZE .025
#define FRAME_STRIDE .01
#define NFFT 1024
#define NFILT 80

using cplx = std::complex<double>;

class Spectrogram
{
  public:
    explicit Spectrogram(Matrix* inputMatrix, int sampleRate);
    void SaveSpectrogram(char* path);

    Matrix inputMatrix;
    Matrix spectrogram;

  private:
    int SAMPLE_RATE_;
    void PreEmphasis();
    auto Hamming(double n, double frameLen) -> double;
    auto Compute() -> Matrix;
};

#endif  // BELA_SPECTROGRAM_SPECTROGRAM_H
