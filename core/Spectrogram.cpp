//
// Created by mariussolomes on 16/06/19.
//
#include "../inc/Spectrogram.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using cv::imwrite;
using cv::Mat;

Spectrogram::Spectrogram(Matrix* m, int sampleRate)
  : inputMatrix(*m)
  , SAMPLE_RATE_(sampleRate)
  , spectrogram(1, 1)
{
    PreEmphasis();
    spectrogram = Compute();
}

void Spectrogram::PreEmphasis()
{
    Matrix matrix_duplicate = duplicateMatrix(&inputMatrix);

    for (int i = 0; i < inputMatrix.rows * inputMatrix.cols; i++)
    {
        if (i != 0)
        {
            double previous_element = matrix_duplicate.data[i - 1];
            inputMatrix.data[i] -= ALPHA * previous_element;
        }
    }
}

/**
 * compute hamming
 * @param n
 * @param frameLen
 * @return
 */
auto Spectrogram::Hamming(double n, double frameLen) -> double
{
    return 0.54 - 0.46 * cos((2 * M_PI * n) / (frameLen - 1));
}

auto Spectrogram::Compute() -> Matrix
{
    int frame_length = FRAME_SIZE * SAMPLE_RATE_;
    int frame_step = FRAME_STRIDE * SAMPLE_RATE_;
    int signal_length = inputMatrix.rows;

    int num_frames =
        static_cast<int>(ceilf((float)(fabs(signal_length - frame_length) / frame_step) + 2));
    int pad_signal_length = num_frames * frame_step + frame_length;
    int array_size = std::abs(pad_signal_length - signal_length);

    Matrix z(array_size, 1);
    Matrix pad_signal = append1DMatrices(&inputMatrix, &z);
    Matrix tiles = arrange(num_frames, -1, frame_length, frame_length - 1, 1);
    Matrix tiles2 = arrange(frame_length, num_frames, -frame_step, (num_frames - 1) * frame_step, frame_step);
    tiles2 = matrixTranspose(&tiles2);

    Matrix indices = matrixAddition(&tiles, &tiles2);
    Matrix frames = lookupByPosition(&pad_signal, &indices);

    // applying hamming
    for (int i = 0; i < frames.rows; i++)
    {
        for (int j = 0; j < frames.cols; j++)
        {
            frames.data[i * frames.cols + j] *= Hamming(j, frame_length);
        }
    }

    // applying FFT
    Matrix mag_frames = Matrix(frames.rows, (int)floor(NFFT / 2 + 1));
    int l = 0;
    for (int i = 0; i < frames.rows; i++)
    {
        cplx buff[NFFT];
        for (int j = 0; j < NFFT; j++)
        {
            // double tmp = frames.data[i*(frames.cols) + j];
            int index_pos = i * (frames.cols) + j;
            if (index_pos > frames.rows * frames.cols)
            {
                buff[j] = 0.;
            }
            else {
                buff[j] = frames.data[index_pos];
}
        }

        CArray data(buff, NFFT);
        // FFT
        fft(data);
        for (int k = 0; k < static_cast<int>(floor(NFFT / 2 + 1)); k++)
        {
            mag_frames.data[l * (mag_frames.cols) + k] =
                fabs(sqrt(pow(data[k].real(), 2) + pow(data[k].imag(), 2)));
        }
        l++;
    }

    // power frames
    Matrix pow_frames = Matrix(mag_frames.rows, mag_frames.cols);
    for (int i = 0; i < pow_frames.rows * pow_frames.cols; i++)
    {
        pow_frames.data[i] = ((1.0 / NFFT) * ((pow(mag_frames.data[i], 2))));
    }

    double low_freq_mel = 0;
    double high_freq_mel = (2595 * log10(1 + (static_cast<double>(SAMPLE_RATE_) / 2) / 700));
    Matrix mel_points =
        arrange(NFILT + 2, 1, -((high_freq_mel - low_freq_mel) / (NFILT + 1)), high_freq_mel, ((high_freq_mel - low_freq_mel) / (NFILT + 1)));

    Matrix hz_points = Matrix(mel_points.rows, mel_points.cols);
    for (int i = 0; i < mel_points.rows * mel_points.cols; i++)
    {
        hz_points.data[i] = (700 * (pow(10, (mel_points.data[i] / 2595)) - 1));
    }

    Matrix bin = Matrix(hz_points.rows, hz_points.cols);
    for (int i = 0; i < bin.rows * bin.cols; i++)
    {
        bin.data[i] = floor((NFFT + 1) * hz_points.data[i] / SAMPLE_RATE_);
    }

    Matrix f_bank = Matrix(NFILT, static_cast<int>(floor(NFFT / 2 + 1)));
    for (int m = 1; m < NFILT + 1; m++)
    {
        int f_m_minus = static_cast<int>(bin.data[m - 1]);  // left
        int f_m = static_cast<int>(bin.data[m]);            // center
        int f_m_plus = static_cast<int>(bin.data[m + 1]);   // right

        for (int j = f_m_minus; j < f_m; j++)
        {
            f_bank.data[(m - 1) * (f_bank.cols) + j] =
                (j - bin.data[m - 1]) / (bin.data[m] - bin.data[m - 1]);
        }

        for (int j = f_m; j < f_m_plus; j++)
        {
            f_bank.data[(m - 1) * (f_bank.cols) + j] =
                (bin.data[m + 1] - j) / (bin.data[m + 1] - bin.data[m]);
        }
    }

    f_bank = matrixTranspose(&f_bank);
    Matrix filter_banks = matrixDotProduct(&pow_frames, &f_bank);
    filter_banks = matrixReplaceZeros(&filter_banks);  // stability
    for (int i = 0; i < filter_banks.cols * filter_banks.rows; i++)
    {
        filter_banks.data[i] = 20. * log10(filter_banks.data[i]);
    }

    return filter_banks;
}

void Spectrogram::SaveSpectrogram(char* path)
{
    FILE* file_ptr;

    file_ptr = fopen(path, "w");
    for (int i = 0; i < spectrogram.cols * spectrogram.rows; i++)
    {
        fprintf(file_ptr, "%.10g\n", spectrogram.data[i]);
    }
}

void Spectrogram::ToPNG(std::string path) {
    FILE* file_ptr;

    double* aaa = (spectrogram.data).data();

    Mat image = Mat(spectrogram.cols, spectrogram.cols, CV_8UC4, reinterpret_cast<unsigned*>(aaa));
    imwrite(path, image);
}
