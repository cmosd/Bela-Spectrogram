//
// Created by mariussolomes on 16/06/19.
//

#ifndef BELA_SPECTROGRAM_FFT_H
#define BELA_SPECTROGRAM_FFT_H

#include <complex>
#include <iostream>
#include <valarray>

const double PI = 3.141592653589793238460;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

// Cooley–Tukey FFT (in-place, divide-and-conquer)
// Higher memory requirements and redundancy although more intuitive
void fft(CArray& x);

#endif  // BELA_SPECTROGRAM_FFT_H
