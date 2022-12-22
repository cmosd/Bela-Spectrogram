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
double WavDataNormalisation::getSignalSize()
{
    return dataMatrix.rows / (double)waveHeader.SamplesPerSec;
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
    double signalSize = getSignalSize();
    if (signalSize > size)
    {
        trim(size);
    }

    else if (signalSize < size)
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
    int maxIndex = (int)(waveHeader.SamplesPerSec * size);
    Matrix newMatrix(maxIndex, dataMatrix.cols);
    for (int i = 0; i < newMatrix.rows * newMatrix.cols; i++)
    {
        newMatrix.data[i] = dataMatrix.data[i];
    }
    // overriding matrix.
    dataMatrix = newMatrix;
}

/**
 * implementing the looping
 * @param size duration in secs of audio
 */
void WavDataNormalisation::loop(int size)
{
    int matrixSize = (int)size * waveHeader.SamplesPerSec;
    Matrix newMatrix(matrixSize, 1);
    int counter = 0;
    for (int i = 0; i < newMatrix.rows * newMatrix.cols; i++)
    {
        if (i < dataMatrix.rows)
        {
            newMatrix.data[i] = dataMatrix.data[i];
        }
        else
        {
            newMatrix.data[i] = dataMatrix.data[counter];
            counter++;

            if (counter > dataMatrix.rows)
            {
                counter = 0;
            }
        }
    }
    // overriding matrix.
    dataMatrix = newMatrix;
}
