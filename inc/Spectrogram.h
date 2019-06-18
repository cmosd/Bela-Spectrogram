//
// Created by mariussolomes on 16/06/19.
//

#ifndef BELA_SPECTROGRAM_SPECTROGRAM_H
#define BELA_SPECTROGRAM_SPECTROGRAM_H

#include <complex.h>
#include "Matrix.h"
#include "FFT.h"


#define ALPHA			.97
#define FRAME_SIZE		.025
#define FRAME_STRIDE	.01
#define NFFT			1024
#define NFILT			40

typedef std::complex<double> cplx;

class Spectrogram{
public:
    explicit Spectrogram(Matrix* inputMatrix, int sampleRate);
    void saveSpectrogram(char* path);

    Matrix inputMatrix;
    Matrix spectrogram;

private:
    int SAMPLE_RATE;
    void preEmphasis();
    double hamming(double n, double frameLen);
    Matrix compute();
};


#endif //BELA_SPECTROGRAM_SPECTROGRAM_H
