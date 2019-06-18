//
// Created by mariussolomes on 18/06/19.
//

#ifndef BELA_SPECTROGRAM_WAVDATANORMALISATION_H
#define BELA_SPECTROGRAM_WAVDATANORMALISATION_H

#include "Matrix.h"
#include "ReadWavFile.h"

class WavDataNormalisation{
public:
    WavDataNormalisation(Matrix data, wav_hdr waveHdr);

    wav_hdr waveHeader;
    Matrix dataMatrix;

    void normalise(int size);

private:
    double getSignalSize();

    void trim(int size);
    void loop(int size);
};

#endif //BELA_SPECTROGRAM_WAVDATANORMALISATION_H
