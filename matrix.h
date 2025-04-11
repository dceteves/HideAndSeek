# pragma once

#include <iostream>
#include <string>

template<typename T>
class Matrix
{
    int row, col;
    T** matrix;

public:
    Matrix(int x = 0, int y = 0) : row(x), col(y), matrix(nullptr) {} // def & param cstor
    Matrix(int r, int c, Matrix& m) : row(r), col(c), matrix(m) {} // copy cstor

    void InitMatrix();
    void SetDimensions(int x, int y);
    T GetValue(int n1, int n2) { return matrix[n1][n2]; }
    T* GetMatrixRow(int r) { return matrix[r]; }
    void ChangeMatrixValue(int x, int y, T value) { matrix[x][y] = value; }   
    void DisplayMatrix();

    T* operator[](int i);
};

template<typename T>
void Matrix<T>::InitMatrix() 
{
    matrix = new T * [row];
    for (int i = 0; i < row; i++)
        matrix[i] = new T[col];
};

template<typename T>
void Matrix<T>::SetDimensions(int x, int y)
{
    row = x;
    col = y;
}

template<typename T>
void Matrix<T>::DisplayMatrix()
{
    for (int i = 0; i < row; i++) 
    {
        for (int j = 0; j < col; j++)
            std::cout << matrix[i][j];
        std::cout << std::endl;
    }
}

template<typename T>
T* Matrix<T>::operator[](int i)
{
    if (i < 0 || i > row) {
        system("cls");
        std::cout << "Matrix Class Out of bounds index access" << std::endl;
        exit(EXIT_FAILURE);
    }
    return matrix[i];
}




