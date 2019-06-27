//
// Created by mariussolomes on 19/06/19.
//

#ifndef BELA_SPECTROGRAM_LOGISTICREGRESSION_H
#define BELA_SPECTROGRAM_LOGISTICREGRESSION_H

#include "Matrix.h"

class LogisticRegression{
public:
    LogisticRegression(Matrix theta);

    Matrix predict(Matrix X_pred);

    Matrix theta;

private:
    double sigmoid(double val);
};

#endif //BELA_SPECTROGRAM_LOGISTICREGRESSION_H
