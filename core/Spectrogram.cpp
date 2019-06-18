//
// Created by mariussolomes on 16/06/19.
//
#include "../inc/Spectrogram.h"

Spectrogram::Spectrogram(Matrix* m, int sampleRate): inputMatrix(*m), SAMPLE_RATE(sampleRate),
spectrogram(1, 1){
    preEmphasis();
    spectrogram = compute();
}

void Spectrogram::preEmphasis(){

    Matrix matrixDuplicate = duplicateMatrix(&inputMatrix);

    for (int i=0; i<inputMatrix.rows * inputMatrix.cols; i++){
        if (i!=0){
            double previousElement = matrixDuplicate.data[i-1];
            inputMatrix.data[i] -= ALPHA * previousElement;
        }
    }
}

/**
 * compute hamming
 * @param n
 * @param frameLen
 * @return
 */
double Spectrogram::hamming(double n, double frameLen){
    return 0.54 - 0.46 * cos((2 * M_PI * n) / (frameLen - 1));
}

Matrix Spectrogram::compute(){
    double frameLength_ = FRAME_SIZE * SAMPLE_RATE;
    double frameStep_ = FRAME_STRIDE * SAMPLE_RATE;
    int signalLength = inputMatrix.rows;
    int frameLength = (int)frameLength_;
    int frameStep = (int)frameStep_;

    int numFrames = (int)ceilf((float)(fabs(signalLength - frameLength) / frameStep));
    int padSignalLength = numFrames * frameStep + frameLength;
    int arraySize = fabs(padSignalLength - signalLength);

    Matrix z(arraySize, 1);
    Matrix padSignal = append1DMatrices(&inputMatrix, &z);
    Matrix tiles = arrange(numFrames, frameLength, -1, frameLength - 1, 1);
    Matrix tiles2 = arrange(frameLength, numFrames, -frameStep, (numFrames - 1) * frameStep, frameStep);
    tiles2 = matrixTranspose(&tiles2);

    Matrix indices = matrixAddition(&tiles, &tiles2);
    Matrix frames = lookupByPosition(&padSignal, &indices);

    // applying hamming
    for (int i=0; i<frames.rows; i++) {
        for (int j = 0; j < frames.cols; j++) {
            frames.data[i * frames.cols + j] *= hamming(j, frameLength);
        }
    }

    // applying FFT
    Matrix mag_frames = Matrix(frames.rows, (int)floor(NFFT / 2 + 1));
    int l=0;
    for (int i=0; i<frames.rows; i++){
        cplx buff[NFFT];
        for (int j=0; j<NFFT; j++){
            // double tmp = frames.data[i*(frames.cols) + j];
            buff[j] = frames.data[i*(frames.cols) + j];
        }

        CArray data(buff, NFFT);
        // FFT
        fft(data);
        for (int k=0; k<(int)floor(NFFT / 2 + 1); k++){
             mag_frames.data[l*(mag_frames.cols) + k] = fabs(sqrt(pow(data[k].real(), 2) + pow(data[k].imag(), 2)));
        }
        l++;
    }

    // power frames
    Matrix pow_frames = Matrix(mag_frames.rows, mag_frames.cols);
    for (int i=0; i<pow_frames.rows * pow_frames.cols; i++){
        pow_frames.data[i] = ((1.0 / NFFT) * (( pow(mag_frames.data[i], 2))));
    }

    double lowFreqMel = 0;
    double highFreqMel = (2595 * log10(1 + ((double)SAMPLE_RATE / 2) / 700));
    Matrix melPoints = arrange(42, 1, -((highFreqMel - lowFreqMel) / 41), highFreqMel, ((highFreqMel - lowFreqMel) / 41));

    Matrix hzPoints = Matrix(melPoints.rows, melPoints.cols);
    for (int i=0; i<melPoints.rows * melPoints.cols; i++){
        hzPoints.data[i] = (700 * ( pow(10, (melPoints.data[i] / 2595))- 1));
    }

    Matrix bin = Matrix(hzPoints.rows, hzPoints.cols);
    for (int i=0; i<bin.rows * bin.cols; i++){
        bin.data[i] = floor( (NFFT + 1) * hzPoints.data[i] / SAMPLE_RATE );
    }

    Matrix fBank = Matrix(NFILT, (int)floor(NFFT / 2 + 1));
    for (int m=1; m<NFILT + 1; m++){
        int f_m_minus = (int)bin.data[m-1];  // left
        int f_m = (int)bin.data[m]; // center
        int f_m_plus = (int)bin.data[m+1]; // right

        for (int j=f_m_minus; j<f_m; j++){
            fBank.data[(m - 1)*(fBank.cols) + j] = (j - bin.data[m - 1]) / (bin.data[m] - bin.data[m - 1]);
        }

        for (int j=f_m; j<f_m_plus; j++){
            fBank.data[(m - 1)*(fBank.cols) + j] = (bin.data[m + 1] - j) / (bin.data[m + 1] - bin.data[m]);
        }
    }

    fBank = matrixTranspose(&fBank);
    Matrix filterBanks = matrixDotProduct(&pow_frames, &fBank);
    filterBanks = matrixReplaceZeros(&filterBanks);  // stability
    for (int i=0; i<filterBanks.cols * filterBanks.rows; i++){
        filterBanks.data[i] = 20. * log10(filterBanks.data[i]);
    }

    return filterBanks;
}

void Spectrogram::saveSpectrogram(char *path) {
    FILE *filePtr;
    filePtr = fopen(path, "w");
    for (int i = 0; i < spectrogram.cols * spectrogram.rows; i++) {
        fprintf(filePtr, "%.10g\n", spectrogram.data[i]);
    }
}

