#include "Matrix.hpp"
#include "../Utils/Utils.hpp"
#include <cmath>

namespace AlgOps
{

    template<size_t R, size_t C>
    Matrix<R, C>::Matrix()
    {
        if (R == 0 || R == 0)
            throw(Exception("OPERATOR *", "INVALID_SIZE", R, C));
        size = R * C;
        data = std::make_unique<float[]>(R * C);
    }

    template <size_t R, size_t C>
    Matrix<R, C>::Matrix(const Matrix<R, C> &copy)
    {
        *this = copy;
    }

    template <size_t R, size_t C>
    Matrix<R, C> &Matrix<R, C>::operator=(const Matrix<R, C> &copy)
    {
        if (&copy != this)
        {
            size = R * C;
            data = std::make_unique<float[]>(R * C);
            setData(copy.getData(), R * C);
        }
        return (*this);
    }

    template <size_t R, size_t C>
    Matrix<R, C>::~Matrix()
    {
    }

    template <size_t R, size_t C>
    bool Matrix<R, C>::operator!=(const Matrix<R, C> &instance)
    {
        for (size_t i = 0; i < R * C; i++)
        {
            if (data[i] != instance.getData()[i])
            return (true);
        }
        return (false);
    }

    template <size_t R, size_t C>
    bool Matrix<R, C>::operator==(const Matrix<R, C> &instance)
    {
        return (!(*this != instance));
    }

    template <size_t R, size_t C>
    Matrix<R, C> Matrix<R, C>::operator+(const Matrix<R, C> &instance) const
    {
        Matrix<R, C> result;
        float value;
        
        for (size_t x = 0; x < R; x++)
        {
            for (size_t y = 0; y < C; y++)
            {
                value = data[x * C + y] + instance.getData()[x * C + y];
                result.setData(x, y, value);
            }
        }
        return (result);
    }

    template <size_t R, size_t C>
    Matrix<R, C> Matrix<R, C>::operator-(const Matrix<R, C> &instance) const
    {
        Matrix<R, C> result;
        float value;
        
        for (size_t x = 0; x < R; x++)
        {
            for (size_t y = 0; y < C; y++)
            {
                value = data[x * C + y] - instance.getData()[x * C + y];
                result.setData(x, y, value);
            }
        }

        return (result);
    }

    template <size_t R1, size_t C1, size_t R2, size_t C2>
    Matrix<R1, C2> operator*(const Matrix<R1, C1> &matA, const Matrix<R2, C2> &matB)
    {
        Matrix<R1, C2> result;
        float value;
        
        if (C1 != R2)
        {
            throw(std::runtime_error("error")); //tmp
            //throw(Exception("OPERATOR *", "MATRIX_INCOMPATIBLE", matA, matB));
        }

        for (size_t y = 0; y < R1; y++)
        {
            for (size_t x = 0; x < C2; x++)
            {
                value = 0;
                for (size_t i = 0; i < R2; i++)
                    value += matA.getData(y, i) * matB.getData(i, x);
                result.setData(y, x, value);
            }
        }
        return (result);
    }

    template <size_t R, size_t C>
    Matrix<R, C> Matrix<R, C>::operator*(float number) const
    {
        Matrix<R, C> result;
        float value;
        
        for (size_t y = 0; y < R; y++)
        {
            for (size_t x = 0; x < C; x++)
            {
                value = getData(y, x) * number;
                result.setData(y, x, value);
            }
        }
        return (result);
    }

    template <size_t R, size_t C>
    Matrix<R, C> operator*(float number, const Matrix<R, C> &instance)
    {
        return (instance * number);
    }

    template <size_t R, size_t C>
    float *Matrix<R, C>::getData() const
    {
        return (&data[0]);
    }

    template <size_t R, size_t C>
    float Matrix<R, C>::getData(unsigned int rowIndex, unsigned int columnIndex) const
    {
        if (rowIndex >= R || columnIndex >= C)
            throw(Exception("GET_DATA", "INVALID_INDEX", *this, rowIndex, columnIndex));

        return (data[rowIndex * C + columnIndex]);
    }

    // only for vector
    template <size_t R, size_t C>
    float Matrix<R, C>::getX() const
    {
        if (R < 1 || R > 4 || C != 1)
            throw(Exception("GET_X", "INVALID_SIZE", *this));

        return (getData(0, 0));
    }

    // only for vector
    template <size_t R, size_t C>
    float Matrix<R, C>::getY() const
    {
        if (R < 2 || R > 4 || C != 1)
            throw(Exception("GET_Y", "INVALID_SIZE", *this));

        return (getData(1, 0));
    }

    // only for vector
    template <size_t R, size_t C>
    float Matrix<R, C>::getZ() const
    {
        if (R < 3 || R > 4 || C != 1)
            throw(Exception("GET_Z", "INVALID_SIZE", *this));

        return (getData(2, 0));
    }

    // only for vector
    template <size_t R, size_t C>
    float Matrix<R, C>::getW() const
    {
        if (R != 4 || C != 1)
            throw(Exception("GET_W", "INVALID_SIZE", *this));

        return (getData(3, 0));
    }

    template <size_t R, size_t C>
    void Matrix<R, C>::setData(unsigned int rowIndex, unsigned int columnIndex, float value)
    {
        if (rowIndex >= R || columnIndex >= C)
            throw(Exception("SET_DATA", "INVALID_INDEX", *this, rowIndex, columnIndex));

        data[rowIndex * C + columnIndex] = value;
    }

    template <size_t R, size_t C>
    void Matrix<R, C>::setData(float *values, unsigned int size)
    {
        if (size != this->size)
            throw(Exception("SET_DATA", "INVALID_SIZE", *this, size));

        for (size_t i = 0; i < size; i++)
            data[i] = values[i];
    }

    template <size_t R, size_t C>
    void Matrix<R, C>::uniform(float value)
    {
        for (size_t i = 0; i < size; i++)
            data[i] = value;
    }

    template <size_t R, size_t C>
    void Matrix<R, C>::identity()
    {
        if (R != C)
            throw(Exception("IDENTITY", "INVALID_SIZE", *this));

        uniform(0);
        for (size_t i = 0; i < R; i++)
            setData(i, i, 1);
    }
        
    template <size_t R, size_t C>
    Matrix<R, C>::Exception::Exception(const std::string &functionName, const std::string &errorMessage, unsigned int rows, unsigned int columns)
    {
        this->errorMessage = "MATRIX::" + functionName + "::" + errorMessage;
        this->errorMessage += "\n|\n| ";
        this->errorMessage += "rows and columns: " + std::to_string(rows) + " * " + std::to_string(columns);
        this->errorMessage += "\n|";
    }


    template <size_t R, size_t C>
    Matrix<R, C>::Exception::Exception(const std::string &functionName, const std::string &errorMessage, const Matrix<R, C> &matrix)
    {
        (void)matrix;
        this->errorMessage = "MATRIX::" + functionName + "::" + errorMessage;
        this->errorMessage += "\n|\n| ";
        this->errorMessage += "matrix: " + std::to_string(R) + " * " + std::to_string(C);
        this->errorMessage += "\n|";
    }


    template <size_t R, size_t C>
    Matrix<R, C>::Exception::Exception(const std::string &functionName, const std::string &errorMessage, const Matrix<R, C> &matrix, unsigned int size)
    {
        (void)matrix;
        this->errorMessage = "MATRIX::" + functionName + "::" + errorMessage;
        this->errorMessage += "\n|\n| ";
        this->errorMessage += "matrix: " + std::to_string(R) + " * " + std::to_string(C);
        this->errorMessage += "\n|";
        this->errorMessage += "size: " + std::to_string(size);
        this->errorMessage += "\n|";
    }

    template <size_t R, size_t C>
    Matrix<R, C>::Exception::Exception(const std::string &functionName, const std::string &errorMessage, const Matrix<R, C> &matrix, unsigned int rows, unsigned int columns)
    {
        (void)matrix;
        this->errorMessage = "MATRIX::" + functionName + "::" + errorMessage;
        this->errorMessage += "\n|\n| ";
        this->errorMessage += "matrix: " + std::to_string(R) + " * " + std::to_string(C);
        this->errorMessage += "\n|";
        this->errorMessage += "rows and columns: " + std::to_string(rows) + " * " + std::to_string(columns);
        this->errorMessage += "\n|";
    }

    template <size_t R, size_t C>
    Matrix<R, C>::Exception::Exception(const std::string &functionName, const std::string &errorMessage, const Matrix<R, C> &leftMatrix, const Matrix<R, C> &rightMatrix)
    {
        this->errorMessage = "MATRIX::" + functionName + "::" + errorMessage;
        this->errorMessage += "\n|\n| ";
        this->errorMessage +=
        "left matrix: " + std::to_string(leftMatrix.getRows()) + " * " + std::to_string(leftMatrix.getColumns());
        this->errorMessage += "\n| ";
        this->errorMessage +=
        "right matrix: " + std::to_string(rightMatrix.getRows()) + " * " + std::to_string(rightMatrix.getColumns());
        this->errorMessage += "\n|";
    }

    template <size_t R, size_t C>
    const char *Matrix<R, C>::Exception::what(void) const throw()
    {
        return (errorMessage.c_str());
    }

    template <size_t R, size_t C>
    Matrix<R, C> Zero(const Matrix<R, C> &instance)
    {
        Matrix<R, C> result = instance;
        
        result.uniform(0);
        return (result);
    }

    template <size_t R, size_t C>
    std::ostream &operator<<(std::ostream &os, const Matrix<R, C> &instance)
    {
        os << std::endl;
        for (size_t y = 0; y < R; y++)
        {
            for (size_t x = 0; x < C; x++)
                os << instance.getData(y, x) << " ";
            os << std::endl;
        }

        return (os);
    }
}