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
    Matrix(const unsigned int &rows, const unsigned int &columns);
    Matrix(const Matrix &copy);
    Matrix &operator=(const Matrix &copy);
    ~Matrix();

    Matrix operator+(const Matrix &instance) const;
    Matrix operator-(const Matrix &instance) const;
    Matrix operator*(const Matrix &instance) const;

    int *getData() const;
    int getData(const unsigned int &rowIndex, const unsigned int &columnIndex) const;
    unsigned int getRows() const;
    unsigned int getColumns() const;

    void uniform();
    void setData(const unsigned int &rowIndex, const unsigned int &columnIndex, const int &value);
    void setData(int *values, const unsigned int size);
};

std::ostream& operator << (std::ostream& os, const Matrix& instance);

#endif