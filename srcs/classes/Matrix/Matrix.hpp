#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <cmath>

class Matrix;

#include "../Utils/Utils.hpp"

class Matrix
{
    private:
        float *data;
        unsigned int rows;
        unsigned int columns;

    public:
        Matrix();
        Matrix(unsigned int rows, unsigned int columns);
        Matrix(const Matrix &copy);
        Matrix &operator=(const Matrix &copy);
        ~Matrix();

        bool operator==(const Matrix &copy);
        bool operator!=(const Matrix &copy);
        Matrix operator+(const Matrix &instance) const;
        Matrix operator-(const Matrix &instance) const;
        Matrix operator*(const Matrix &instance) const;
        Matrix operator*(float number) const;

        float *getData() const;
        float getData(unsigned int rowIndex, unsigned int columnIndex) const;
        float getX() const;
        float getY() const;
        float getZ() const;
        float getW() const;
        unsigned int getRows() const;
        unsigned int getColumns() const;

        void setData(unsigned int rowIndex, unsigned int columnIndex, float value);
        void setData(float *values, unsigned int size);
        void uniform(float value);
        void identity();

        static Matrix rotate(const Matrix &instance, float angle, const Matrix &vector);
        static Matrix perspective(float fov, float aspect, float near, float far);
        static Matrix normalize(const Matrix &vector);
        static Matrix crossProduct(const Matrix &vectorA, const Matrix &vectorB);
        static float dotProduct(const Matrix &vectorA, const Matrix &vectorB);
        static Matrix lookAt(const Matrix &position, const Matrix &target, const Matrix &initialUpVector);
        static Matrix Zero(const Matrix &instance);
};

std::ostream &operator << (std::ostream &os, const Matrix &instance);
Matrix operator * (float number, const Matrix &instance);

#endif