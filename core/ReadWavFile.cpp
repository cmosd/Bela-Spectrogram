//
// Created by mariussolomes on 17/06/19.
//
#include "../inc/ReadWavFile.h"

ReadWavFile::ReadWavFile(char* fileName)
  : dataMatrix(1, 1)
  , fileName(fileName)
{
    FILE* wav_file = fopen(fileName, "r");
    // Read the header
    int header_size = sizeof(wav_hdr);
    int filelength = 0;
    size_t bytes_read = fread(&wavHeader, 1, header_size, wav_file);

    Matrix matrix(static_cast<int>(wavHeader.Subchunk2Size) / 2, 1);

    for (int i = 0; i < static_cast<int>(wavHeader.Subchunk2Size) / 2; i++)
    {
        auto* data = new short[1];
        bytes_read = fread(data, sizeof(short), 1, wav_file);
        matrix.data[i] = *data;
    }

    dataMatrix = matrix;
}
