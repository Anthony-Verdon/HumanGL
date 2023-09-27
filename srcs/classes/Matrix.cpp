#include "Matrix.hpp"

Matrix::Matrix()
{

}

Matrix::Matrix(const unsigned int &width, const unsigned int &height)
{
    data = new int[width * height];
    this->width = width;
    this->height = height;
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
        width = copy.getWidth();
        height = copy.getHeight();
    }
    return (*this);
}

Matrix::~Matrix()
{
    delete []data;
}

Matrix Matrix::operator+(const Matrix &instance) const
{
    Matrix result(width, height);
    int value;

    if (width != instance.getWidth() || height != instance.getHeight())
        std::cerr << "matrice not same dimension" << std::endl;

    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            value = data[x * width + y] + instance.getData()[x * width + y];
            result.setData(x, y, value);
        }
    }
    return (result);
}

Matrix Matrix::operator-(const Matrix &instance) const
{
    Matrix result(width, height);
    int value;

    if (width != instance.getWidth() || height != instance.getHeight())
        std::cerr << "matrice not same dimension" << std::endl;
    
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            value = data[x * width + y] - instance.getData()[x * width + y];
            result.setData(x, y, value);
        }
    }

    return (result);
}

Matrix Matrix::operator*(const Matrix &instance) const
{
    Matrix result(height, instance.getWidth());
    int value;

    if (width != instance.getHeight())
        std::cerr << "matrice not good dimension" << std::endl;

    for (size_t x = 0; x < result.getWidth(); x++)
    {
        for (size_t y = 0; y < result.getHeight(); y++)
        {
            value = 0;
            for (size_t i = 0; i < width; i++)
                value += getData(x, i) * instance.getData(i, y);   
            result.setData(x, y, value);
        }
    }

    return (result);
}

Matrix Matrix::operator/(const Matrix &instance) const
{
   Matrix result(height, instance.getWidth());
    int value;

    if (width != instance.getHeight())
        std::cerr << "matrice not good dimension" << std::endl;

    for (size_t x = 0; x < result.getWidth(); x++)
    {
        for (size_t y = 0; y < result.getHeight(); y++)
        {
            value = 0;
            for (size_t i = 0; i < width; i++)
            {
                if (instance.getData(i, y) == 0)
                    std::cerr << "division by 0" << std::endl;
                value += getData(x, i) / instance.getData(i, y);
            }
            result.setData(x, y, value);
        }
    }

    return (result);
}

int * Matrix::getData() const
{
    return (data);
}


int Matrix::getData(const unsigned int &x, const unsigned int &y) const
{
    if (x >= width || y >= height)
        std::cerr << "invalid index" << std::endl;
    return (data[x * width + y]);
}
unsigned int Matrix::getWidth() const
{
    return (width);
}

unsigned int Matrix::getHeight() const
{
    return (height);
}

void Matrix::setData(const unsigned int &x, const unsigned int &y, const int &value)
{
    if (x >= width || y >= height)
        std::cerr << "invalid index" << std::endl;
    data[x * width + y] = value;
}

void Matrix::setData(int *values, const unsigned int size)
{
    if (size != width * height)
        std::cerr << "err" << std::endl;
    for (size_t i = 0; i < size; i++)
        data[i] = values[i];
}

std::ostream& operator << (std::ostream& os, const Matrix& instance)
{
    os << std::endl;
    for (size_t x = 0; x < instance.getWidth(); x++)
    {
        for (size_t y = 0; y < instance.getHeight(); y++)
            os << instance.getData()[x * instance.getWidth() + y] << ' ';
        os << std::endl;
    }

    return (os);
}