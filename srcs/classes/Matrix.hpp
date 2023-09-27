#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>

class Matrix
{
private:
    Matrix();

    int *data;
    unsigned int width;
    unsigned int height;

public:
    Matrix(const unsigned int &width, const unsigned int &height);
    Matrix(const Matrix &copy);
    Matrix &operator=(const Matrix &copy);
    ~Matrix();

    Matrix operator+(const Matrix &instance) const;
    Matrix operator-(const Matrix &instance) const;
    Matrix operator*(const Matrix &instance) const;
    Matrix operator/(const Matrix &instance) const;

    int *getData() const;
    int getData(const unsigned int &x, const unsigned int &y) const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void uniform();
    void setData(const unsigned int &x, const unsigned int &y, const int &value);
    void setData(int *values, const unsigned int size);
};

std::ostream& operator << (std::ostream& os, const Matrix& instance);

#endif