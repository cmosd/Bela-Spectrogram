//
// Created by mariussolomes on 18/06/19.
//
#include "../inc/WavDataNormalisation.h"
#include <utility>

WavDataNormalisation::WavDataNormalisation(Matrix data, wav_hdr waveHdr)
  : dataMatrix(std::move(data))
  , waveHeader(waveHdr)
{}

/**
 * finds the size of the signal.. this is the length of the signal / sample rate
 * @return signal size
 */
auto WavDataNormalisation::GetSignalSize() -> double
{
    return dataMatrix.rows / static_cast<double>(waveHeader.SamplesPerSec);
}

/**
 * the signal could be too large or too short. what determines this is comparing
 * ``getSignalSize()`` to the parameter ``size``
 *
 * if ``getSignalSize()`` > ``size`` then we need to trim the signal...
 *
 * otherwise: we need to loop it.
 * @param size
 */
void WavDataNormalisation::normalise(int size)
{
    double signal_size = GetSignalSize();
    if (signal_size > size)
    {
        trim(size);
    }

    else if (signal_size < size)
    {
        loop(size);
    }
}

/**
 * implementing the trimming
 * @param size duration in secs of audio
 */
void WavDataNormalisation::trim(int size)
{
    int max_index = static_cast<int>(waveHeader.SamplesPerSec * size);
    Matrix new_matrix(max_index, dataMatrix.cols);
    for (int i = 0; i < new_matrix.rows * new_matrix.cols; i++)
    {
        new_matrix.data[i] = dataMatrix.data[i];
    }
    // overriding matrix.
    dataMatrix = new_matrix;
}

/**
 * implementing the looping
 * @param size duration in secs of audio
 */
void WavDataNormalisation::loop(int size)
{
    int matrix_size = size * static_cast<int>(waveHeader.SamplesPerSec);
    Matrix new_matrix(matrix_size, 1);
    int counter = 0;
    for (int i = 0; i < new_matrix.rows * new_matrix.cols; i++)
    {
        if (i < dataMatrix.rows)
        {
            new_matrix.data[i] = dataMatrix.data[i];
        }
        else
        {
            new_matrix.data[i] = dataMatrix.data[counter];
            counter++;

            if (counter > dataMatrix.rows)
            {
                counter = 0;
            }
        }
    }
    // overriding matrix.
    dataMatrix = new_matrix;
}
