#include "Matrix.hpp"
#include "Utils.hpp"

Matrix::Matrix()
{

}

Matrix::Matrix(const unsigned int &rows, const unsigned int &columns)
{
    data = new int[rows * columns];
    this->rows = rows;
    this->columns = columns;
}

Matrix::Matrix(const Matrix &copy)
{
    *this = copy;
}

Matrix & Matrix::operator=(const Matrix &copy)
{
    if (&copy != this)
    {
        data = copy.getData();
        rows = copy.getRows();
        columns = copy.getColumns();
    }
    return (*this);
}

Matrix::~Matrix()
{
    delete []data;
}

Matrix Matrix::operator+(const Matrix &instance) const
{
    Matrix result(rows, columns);
    int value;

    if (rows != instance.getRows() || columns != instance.getColumns())
        throw(Utils::Exception("MATRIX::OPERATOR +::MATRIX INCOMPATIBLE"));

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

Matrix Matrix::operator-(const Matrix &instance) const
{
    Matrix result(rows, columns);
    int value;

    if (rows != instance.getRows() || columns != instance.getColumns())
        throw(Utils::Exception("MATRIX::OPERATOR -::MATRIX INCOMPATIBLE"));
    
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

Matrix Matrix::operator*(const Matrix &instance) const
{
    Matrix result(rows, instance.getColumns());
    int value;

    if (columns != instance.getRows())
        throw(Utils::Exception("MATRIX::OPERATOR *::MATRIX_INCOMPATIBLE\n"
        "LEFT MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns) + "\n"
        "RIGHT MATRIX SIZE => " + std::to_string(instance.getRows()) + " * " + std::to_string(instance.getColumns())));

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

int * Matrix::getData() const
{
    return (data);
}


int Matrix::getData(const unsigned int &rowIndex, const unsigned int &columnIndex) const
{
    if (rowIndex >= rows || columnIndex >= columns)
        throw(Utils::Exception("MATRIX::GET_DATA::INVALID_INDEX\n"
        "MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns) + "\n"
        "ROW INDEX => " + std::to_string(rowIndex) + "\n"
        "COLUMN INDEX => " + std::to_string(columnIndex)));

    return (data[rowIndex * columns + columnIndex]);
}
unsigned int Matrix::getRows() const
{
    return (rows);
}

unsigned int Matrix::getColumns() const
{
    return (columns);
}

void Matrix::setData(const unsigned int &rowIndex, const unsigned int &columnIndex, const int &value)
{
    if (rowIndex >= rows || columnIndex >= columns)
        throw(Utils::Exception("MATRIX::SET_DATA::INVALID_INDEX\n"
        "MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns) + "\n"
        "ROW INDEX => " + std::to_string(rowIndex) + "\n"
        "COLUMN INDEX => " + std::to_string(columnIndex)));

    data[rowIndex * columns + columnIndex] = value;
}

void Matrix::setData(int *values, const unsigned int size)
{
    if (size != rows * columns)
        throw(Utils::Exception("MATRIX::SET_DATA::INVALID_SIZE\n"
        "MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns) + "\n"
        "SIZE => " + std::to_string(size)));

    for (size_t i = 0; i < size; i++)
        data[i] = values[i];
}

std::ostream& operator << (std::ostream& os, const Matrix& instance)
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