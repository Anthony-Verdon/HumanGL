#include "Matrix.hpp"
#include "../Utils/Utils.hpp"
#include <cmath>

namespace AlgOps
{

    template<size_t R, size_t C>
    Matrix<R, C>::Matrix()
    {
        rows = R;
        columns = C;
        if (rows == 0 || columns == 0)
        throw(Exception("OPERATOR *", "INVALID_SIZE", rows, columns));
    data = std::make_unique<float[]>(rows * columns);
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
            rows = copy.getRows();
            columns = copy.getColumns();
            data = std::make_unique<float[]>(rows * columns);
            setData(copy.getData(), rows * columns);
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
        if (rows != instance.getRows() || columns != instance.getColumns())
        return (true);
    for (size_t i = 0; i < rows * columns; i++)
    {
        if (data[i] != instance.getData()[i])
        return (true);
    }
    return (false);
    }

    template <size_t R, size_t C>
    bool Matrix<R, C>::operator==(const Matrix<R, C> &instance)
    {
        if (*this != instance)
        return (false);
    return (true);
    }

    template <size_t R, size_t C>
    Matrix<R, C> Matrix<R, C>::operator+(const Matrix<R, C> &instance) const
    {
        Matrix<R, C> result;
        float value;
        
        if (rows != instance.getRows() || columns != instance.getColumns())
        throw(Exception("OPERATOR +", "MATRIX_INCOMPATIBLE", *this, instance));

    for (size_t x = 0; x < rows; x++)
    {
        for (size_t y = 0; y < columns; y++)
        {
            value = data[x * columns + y] + instance.getData()[x * columns + y];
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
        
        if (rows != instance.getRows() || columns != instance.getColumns())
        throw(Exception("OPERATOR -", "MATRIX_INCOMPATIBLE", *this, instance));

    for (size_t x = 0; x < rows; x++)
    {
        for (size_t y = 0; y < columns; y++)
        {
            value = data[x * columns + y] - instance.getData()[x * columns + y];
            result.setData(x, y, value);
        }
    }

    return (result);
    }

    template <size_t R, size_t C>
    Matrix<R, C> Matrix<R, C>::operator*(const Matrix<R, C> &instance) const
    {
        Matrix<R, C> result;
        float value;
        
        if (columns != instance.getRows())
        throw(Exception("OPERATOR *", "MATRIX_INCOMPATIBLE", *this, instance));

    for (size_t y = 0; y < result.getRows(); y++)
    {
        for (size_t x = 0; x < result.getColumns(); x++)
        {
            value = 0;
            for (size_t i = 0; i < instance.getRows(); i++)
            value += this->getData(y, i) * instance.getData(i, x);
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
        
        for (size_t y = 0; y < result.getRows(); y++)
        {
            for (size_t x = 0; x < result.getColumns(); x++)
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
        Matrix<R, C> result;
        float value;
        
        for (size_t y = 0; y < result.getRows(); y++)
        {
            for (size_t x = 0; x < result.getColumns(); x++)
            {
                value = instance.getData(y, x) * number;
                result.setData(y, x, value);
            }
        }
        return (result);
    }

    template <size_t R, size_t C>
    float *Matrix<R, C>::getData() const
    {
        return (&data[0]);
    }

    template <size_t R, size_t C>
    float Matrix<R, C>::getData(unsigned int rowIndex, unsigned int columnIndex) const
    {
        if (rowIndex >= rows || columnIndex >= columns)
        throw(Exception("GET_DATA", "INVALID_INDEX", *this, rowIndex, columnIndex));

    return (data[rowIndex * columns + columnIndex]);
    }

    // only for vector
    template <size_t R, size_t C>
    float Matrix<R, C>::getX() const
    {
        if (rows < 1 || rows > 4 || columns != 1)
        throw(Exception("GET_X", "INVALID_SIZE", *this));

    return (getData(0, 0));
    }

    // only for vector
    template <size_t R, size_t C>
    float Matrix<R, C>::getY() const
    {
        if (rows < 2 || rows > 4 || columns != 1)
        throw(Exception("GET_Y", "INVALID_SIZE", *this));

    return (getData(1, 0));
    }

    // only for vector
    template <size_t R, size_t C>
    float Matrix<R, C>::getZ() const
    {
        if (rows < 3 || rows > 4 || columns != 1)
        throw(Exception("GET_Z", "INVALID_SIZE", *this));

    return (getData(2, 0));
    }

    // only for vector
    template <size_t R, size_t C>
    float Matrix<R, C>::getW() const
    {
        if (rows != 4 || columns != 1)
        throw(Exception("GET_W", "INVALID_SIZE", *this));

    return (getData(3, 0));
    }

    template <size_t R, size_t C>
    unsigned int Matrix<R, C>::getRows() const
    {
        return (rows);
    }

    template <size_t R, size_t C>
    unsigned int Matrix<R, C>::getColumns() const
    {
        return (columns);
    }

    template <size_t R, size_t C>
    void Matrix<R, C>::setData(unsigned int rowIndex, unsigned int columnIndex, float value)
    {
        if (rowIndex >= rows || columnIndex >= columns)
        throw(Exception("SET_DATA", "INVALID_INDEX", *this, rowIndex, columnIndex));

    data[rowIndex * columns + columnIndex] = value;
    }

    template <size_t R, size_t C>
    void Matrix<R, C>::setData(float *values, unsigned int size)
    {
        if (size != rows * columns)
        throw(Exception("SET_DATA", "INVALID_SIZE", *this, size));

    for (size_t i = 0; i < size; i++)
    data[i] = values[i];
    }

    template <size_t R, size_t C>
    void Matrix<R, C>::uniform(float value)
    {
        for (size_t i = 0; i < rows * columns; i++)
        data[i] = value;
    }

    template <size_t R, size_t C>
    void Matrix<R, C>::identity()
    {
        if (rows != columns)
        throw(Exception("IDENTITY", "INVALID_SIZE", *this));

        uniform(0);
        for (size_t i = 0; i < rows; i++)
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
        this->errorMessage = "MATRIX::" + functionName + "::" + errorMessage;
        this->errorMessage += "\n|\n| ";
        this->errorMessage += "matrix: " + std::to_string(matrix.getRows()) + " * " + std::to_string(matrix.getColumns());
        this->errorMessage += "\n|";
    }


    template <size_t R, size_t C>
    Matrix<R, C>::Exception::Exception(const std::string &functionName, const std::string &errorMessage, const Matrix<R, C> &matrix, unsigned int size)
    {
        this->errorMessage = "MATRIX::" + functionName + "::" + errorMessage;
        this->errorMessage += "\n|\n| ";
        this->errorMessage += "matrix: " + std::to_string(matrix.getRows()) + " * " + std::to_string(matrix.getColumns());
        this->errorMessage += "\n|";
        this->errorMessage += "size: " + std::to_string(size);
        this->errorMessage += "\n|";
    }

    template <size_t R, size_t C>
    Matrix<R, C>::Exception::Exception(const std::string &functionName, const std::string &errorMessage, const Matrix<R, C> &matrix, unsigned int rows, unsigned int columns)
    {
        this->errorMessage = "MATRIX::" + functionName + "::" + errorMessage;
        this->errorMessage += "\n|\n| ";
        this->errorMessage += "matrix: " + std::to_string(matrix.getRows()) + " * " + std::to_string(matrix.getColumns());
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
        Matrix result(instance);
        
        result.uniform(0);
        return (result);
    }

    template <size_t R, size_t C>
    std::ostream &operator<<(std::ostream &os, const Matrix<R, C> &instance)
    {
        os << std::endl;
        for (size_t y = 0; y < instance.getRows(); y++)
        {
            for (size_t x = 0; x < instance.getColumns(); x++)
            os << instance.getData(y, x) << " ";
        os << std::endl;
        }

        return (os);
    }
}