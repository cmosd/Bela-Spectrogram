//
// Created by mariussolomes on 16/06/19.
//

#include "../inc/ReadWavFile.h"
#include "../inc/Spectrogram.h"
#include "../inc/WavDataNormalisation.h"

auto main(int argc, char* argv[]) -> int
{
    if (argc < 3)
    {
        std::cout << "You must specify an input and output file." << std::endl;
        return -1;
    }
    char* fname(argv[1]);
    char* save_name(argv[2]);

    // reading wav file
    ReadWavFile wav(fname);
    int sample_rate = static_cast<int>(wav.wavHeader.SamplesPerSec);
    Matrix input_matrix = wav.dataMatrix;

    // normalising sound to have a specific size
    WavDataNormalisation wdn(input_matrix, wav.wavHeader);
    wdn.normalise(10);

    // computing spectrogam
    Spectrogram spec(&wdn.dataMatrix, sample_rate);
    spec.SaveSpectrogram(save_name);

    return 0;
}
