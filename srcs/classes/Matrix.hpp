#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>

class Matrix
{
private:
    Matrix();

    int *data;
    unsigned int rows;
    unsigned int columns;

public:
    Matrix(unsigned int rows, unsigned int columns);
    Matrix(const Matrix &copy);
    Matrix &operator=(const Matrix &copy);
    ~Matrix();

    Matrix operator+(const Matrix &instance) const;
    Matrix operator-(const Matrix &instance) const;
    Matrix operator*(const Matrix &instance) const;
    Matrix operator*(int number) const;

    int *getData() const;
    int getData(unsigned int rowIndex, unsigned int columnIndex) const;
    unsigned int getRows() const;
    unsigned int getColumns() const;

    void uniform(int value);
    void identity();
    void setData(unsigned int rowIndex, unsigned int columnIndex, int value);
    void setData(int *values, unsigned int size);
};

std::ostream &operator << (std::ostream &os, const Matrix &instance);
Matrix operator * (int number, const Matrix &instance);

#endif