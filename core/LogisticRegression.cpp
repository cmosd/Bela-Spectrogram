//
// Created by mariussolomes on 19/06/19.
//

#include <utility>
#include "../inc/LogisticRegression.h"

LogisticRegression::LogisticRegression(Matrix theta):
theta(std::move(theta)){

}

/**
 * makes a logistic regression prediction.
 *
 * @param X_pred- could be a vector of any length...
 *                each row is multiplied by a ``theta`` using
 *                ``matrixDotProduct`` and the result is passed through
 *                the ``sigmoid``
 *
 *                if `hypothesis` > 1 - `hypothesis`, then we are predicting 1
 *                otherwise 0.
 * @return Matrix of predictions
 */
Matrix LogisticRegression::predict(Matrix X_pred) {

    Matrix outMatrix(X_pred.rows, 1);
    for (int i=0; i<X_pred.rows; i++){

        Matrix tmpMatrix(X_pred.cols, 1);
        for (int j=0; j<X_pred.cols; j++){
            tmpMatrix.data[j] = X_pred.data[i*X_pred.cols + j];
        }

        Matrix dot = matrixDotProduct(&theta, &tmpMatrix);
        double hypothesis = dot.data[0];
        hypothesis = sigmoid(hypothesis);
        if (hypothesis > 1 - hypothesis){
            outMatrix.data[i] = 1.;
        } else
            outMatrix.data[i] = 0.;
    }
    return outMatrix;
}

/**
 * calculates the sigmoid function for a given x-``val``
 * @param val x-axis value
 * @return y-axis value
 */
double LogisticRegression::sigmoid(double val) {
    return 1 / (1 + exp(- val));
}



