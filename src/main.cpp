//
// Created by mariussolomes on 16/06/19.
//

#include "../inc/ReadWavFile.h"
#include "../inc/Spectrogram.h"
#include "../inc/WavDataNormalisation.h"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "You must specify an input and output file." << std::endl;
        return -1;
    }
    char* fname(argv[1]);
    char* saveName(argv[2]);

    // reading wav file
    ReadWavFile wav(fname);
    int sampleRate = wav.wavHeader.SamplesPerSec;
    Matrix c = wav.dataMatrix;

    // normalising sound to have a specific size
    WavDataNormalisation wdn(c, wav.wavHeader);
    wdn.normalise(10);

    // computing spectrogam
    Spectrogram spec(&wdn.dataMatrix, sample_rate);
    //spec.SaveSpectrogram(save_name);
    spec.ToPNG(save_name);

    return 0;
}
