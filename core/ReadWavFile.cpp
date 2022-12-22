//
// Created by mariussolomes on 17/06/19.
//
#include "../inc/ReadWavFile.h"

ReadWavFile::ReadWavFile(char* fileName)
  : dataMatrix(1, 1)
  , fileName(fileName)
{
    FILE* wavFile = fopen(fileName, "r");
    // Read the header
    int headerSize = sizeof(wav_hdr), filelength = 0;
    size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);

    Matrix m((int)wavHeader.Subchunk2Size / 2, 1);

    for (int i = 0; i < (int)wavHeader.Subchunk2Size / 2; i++)
    {
        auto* data = new short[1];
        bytesRead = fread(data, sizeof(short), 1, wavFile);
        m.data[i] = *data;
    }

    dataMatrix = m;
}