//
// Created by mariussolomes on 16/06/19.
//

#include "../inc/Matrix.h"

/* constructor */
Matrix::Matrix(int r, int c)
  : rows(r)
  , cols(c)
{
    initMatrix();
}

/* initilising matrix with zeros */
void Matrix::initMatrix()
{
    std::vector<double> d = {0};

    for (int i = 0; i < rows * cols; i++)
    {
        d.push_back(0);
    }

    data = d;
}

/* prints the matrix */
void Matrix::print()
{
    int counter = 0;
    for (auto itr = data.begin(); itr != data.end(); itr++)
    {
        /* when ``counter`` reaches the number of rows, it will print new line */
        if (counter == rows)
        {
            std::cout << "\n";
            counter = 0;
        }
        std::cout << *itr << " ";
        counter++;
    }
    std::cout << "\n";
}

/* Matrix Fucntions */

Matrix matrixAddition(Matrix* m1, Matrix* m2)
{
    // matrix addition does not require a change of shape
    Matrix newMatrix(m1->rows, m1->cols);

    for (int i = 0; i < m1->rows * m1->cols; i++)
    {
        newMatrix.data[i] = m1->data[i] + m2->data[i];
    }

    return newMatrix;
}

Matrix matrixTranspose(Matrix* m)
{
    Matrix newMatrix(m->cols, m->rows);

    for (int i = 0; i < newMatrix.rows; i++)
    {
        for (int j = 0; j < newMatrix.cols; j++)
        {
            newMatrix.data[i * (newMatrix.cols) + j] = m->data[j * (m->cols) + i];
        }
    }
    return newMatrix;
}

Matrix matrixDotProduct(Matrix* m1, Matrix* m2)
{
    Matrix newMatrix(m1->rows, m2->cols);

    if (m1->cols != m2->rows)
    {
        std::cerr << "Matrices have invalid dimensions" << std::endl;
        return newMatrix;
    }

    for (int i = 0; i < m1->rows; i++)
    {
        for (int j = 0; j < m2->cols; j++)
        {
            double sum = 0;
            for (int k = 0; k < m1->cols; k++)
            {
                sum = sum + m1->data[i * (m1->cols) + k] * m2->data[k * (m2->cols) + j];
            }
            newMatrix.data[i * (newMatrix.cols) + j] = sum;
        }
    }
    return newMatrix;
}

Matrix duplicateMatrix(Matrix* m)
{
    Matrix newMatrix(m->rows, m->cols);
    newMatrix.data = m->data;
    return newMatrix;
}

Matrix append1DMatrices(Matrix* m1, Matrix* m2)
{
    Matrix newMatrix(m1->rows + m2->rows, 1);

    for (int i = 0; i < m1->rows * m1->cols; i++)
    {
        newMatrix.data[i] = m1->data[i];
    }

    int counter = 0;
    for (int i = m1->rows; i < (m1->rows + m2->rows) * m2->cols; i++)
    {
        newMatrix.data[i] = m2->data[counter];
        counter++;
    }
    return newMatrix;
}

Matrix arrange(int n_rows, int n_cols, double start, double end, double increment)
{
    Matrix m(n_rows, n_cols);
    double value = start;
    for (int i = 0; i < m.rows * m.cols; i++)
    {
        m.data[i] = value + increment;
        value = m.data[i];
        if (value == end)
        {
            value = start;
        }
    }
    return m;
}

Matrix lookupByPosition(Matrix* m, Matrix* position)
{
    Matrix newMatrix(position->rows, position->cols);

    for (int i = 0; i < newMatrix.rows * newMatrix.cols; i++)
    {
        int index = (int)position->data[i];
        newMatrix.data[i] = m->data[index];
    }
    return newMatrix;
}

Matrix matrixReplaceZeros(Matrix* m)
{
    for (int i = 0; i < m->cols * m->rows; i++)
    {
        if (m->data[i] == 0)
        {
            m->data[i] = EPS;
        }
    }
    return *m;
}

/**
 * FEATURE-02/FIXING NORMALISER
 */

/**
 * returns the elements of the matrix up to ``position``
 * @param m Matrix
 * @param position
 * @return
 */
Matrix matrixElementsUpto(Matrix* m, int position)
{
    Matrix newMatrix(position, m->cols);

    for (int i = 0; i < m->rows * m->cols; i++)
    {
        newMatrix.data[i] = m->data[i];
        if (i == position)
        {
            break;
        }
        else
            continue;
    }
    int val = newMatrix.data[4201];
    return newMatrix;
}