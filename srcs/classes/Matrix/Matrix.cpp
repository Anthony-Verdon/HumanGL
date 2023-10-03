#include "Matrix.hpp"

Matrix::Matrix()
{
    rows = 1;
    columns = 1;
    data = new float[rows * columns];
}

Matrix::Matrix(unsigned int rows, unsigned int columns)
{
    if (rows == 0 || columns == 0)
        throw(Utils::Exception("MATRIX::OPERATOR *::INVALID_SIZE\n"
        "MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns)));
    this->data = new float[rows * columns];
    this->rows = rows;
    this->columns = columns;
}

Matrix::Matrix(const Matrix &copy)
{
    data = NULL;
    *this = copy;
}

Matrix & Matrix::operator=(const Matrix &copy)
{
    if (&copy != this)
    {
        rows = copy.getRows();
        columns = copy.getColumns();
        delete []data;
        data = new float[rows * columns];
        setData(copy.getData(), rows * columns);
    }
    return (*this);
}

Matrix::~Matrix()
{
    delete []data;
}

bool Matrix::operator!=(const Matrix &instance)
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

bool Matrix::operator==(const Matrix &instance)
{
    if (*this != instance)
        return (false);
    return (true);
}

Matrix Matrix::operator+(const Matrix &instance) const
{
    Matrix result(rows, columns);
    float value;

    if (rows != instance.getRows() || columns != instance.getColumns())
        throw(Utils::Exception("MATRIX::OPERATOR +::MATRIX_INCOMPATIBLE\n"
        "LEFT MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns) + "\n"
        "RIGHT MATRIX SIZE => " + std::to_string(instance.getRows()) + " * " + std::to_string(instance.getColumns())));

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
    float value;

    if (rows != instance.getRows() || columns != instance.getColumns())
        throw(Utils::Exception("MATRIX::OPERATOR -::MATRIX_INCOMPATIBLE\n"
        "LEFT MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns) + "\n"
        "RIGHT MATRIX SIZE => " + std::to_string(instance.getRows()) + " * " + std::to_string(instance.getColumns())));
    
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
    float value;

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

Matrix Matrix::operator*(float number) const
{
    Matrix result(rows, columns);
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

Matrix operator*(float number, const Matrix &instance)
{
    Matrix result(instance.getRows(), instance.getColumns());
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

float * Matrix::getData() const
{
    return (data);
}


float Matrix::getData(unsigned int rowIndex, unsigned int columnIndex) const
{
    if (rowIndex >= rows || columnIndex >= columns)
        throw(Utils::Exception("MATRIX::GET_DATA::INVALID_INDEX\n"
        "MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns) + "\n"
        "ROW INDEX => " + std::to_string(rowIndex) + "\n"
        "COLUMN INDEX => " + std::to_string(columnIndex)));

    return (data[rowIndex * columns + columnIndex]);
}

//only for vector
float Matrix::getX() const
{
    if (rows < 1 || rows > 4 || columns != 1)
     throw(Utils::Exception("MATRIX::GET_X::INVALID_SIZE\n"
        "MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns)));
    
    return (getData(0, 0));
}

//only for vector
float Matrix::getY() const
{
    if (rows < 2 || rows > 4 || columns != 1)
     throw(Utils::Exception("MATRIX::GET_X::INVALID_SIZE\n"
        "MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns)));
    
    return (getData(1, 0));
}

//only for vector
float Matrix::getZ() const
{
    if (rows < 3 || rows > 4 || columns != 1)
     throw(Utils::Exception("MATRIX::GET_X::INVALID_SIZE\n"
        "MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns)));
    
    return (getData(2, 0));
}

//only for vector
float Matrix::getW() const
{
    if (rows != 4 || columns != 1)
     throw(Utils::Exception("MATRIX::GET_X::INVALID_SIZE\n"
        "MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns)));
    
    return (getData(3, 0));
}

unsigned int Matrix::getRows() const
{
    return (rows);
}

unsigned int Matrix::getColumns() const
{
    return (columns);
}

void Matrix::setData(unsigned int rowIndex, unsigned int columnIndex, float value)
{
    if (rowIndex >= rows || columnIndex >= columns)
        throw(Utils::Exception("MATRIX::SET_DATA::INVALID_INDEX\n"
        "MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns) + "\n"
        "ROW INDEX => " + std::to_string(rowIndex) + "\n"
        "COLUMN INDEX => " + std::to_string(columnIndex)));

    data[rowIndex * columns + columnIndex] = value;
}

void Matrix::setData(float *values, unsigned int size)
{
    if (size != rows * columns)
        throw(Utils::Exception("MATRIX::SET_DATA::INVALID_SIZE\n"
        "MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns) + "\n"
        "SIZE => " + std::to_string(size)));

    for (size_t i = 0; i < size; i++)
        data[i] = values[i];
}

void Matrix::uniform(float value)
{
    for (size_t i = 0; i < rows * columns; i++)
        data[i] = value;
}

void Matrix::identity()
{
    if (rows != columns)
        throw(Utils::Exception("MATRIX::IDENTITY::INVALID_SIZE\n"
        "MATRIX SIZE => " + std::to_string(rows) + " * " + std::to_string(columns)));
    
    uniform(0);
    for (size_t i = 0; i < rows; i++)
        setData(i, i, 1);
}

Matrix Matrix::rotate(const Matrix &instance, float angle, const Matrix &vector)
{
    Matrix result(instance);
    Matrix vectorNormalized(Matrix::normalize(vector));

    if (instance.getRows() != 4 || instance.getColumns() != 4)
        throw(Utils::Exception("MATRIX::ROTATE::MATRIX_INVALID_SIZE\n"
        "MATRIX SIZE => " + std::to_string(instance.getRows()) + " * " + std::to_string(instance.getColumns())));

    if (vector.getRows() != 3 || vector.getColumns() != 1)
        throw(Utils::Exception("MATRIX::ROTATE::VECTOR_INVALID_SIZE\n"
        "VECTOR SIZE => " + std::to_string(vector.getRows()) + " * " + std::to_string(vector.getColumns())));
    

    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);
    float rotationMatrixValues[] = {
        cosAngle + powf(vectorNormalized.getX(), 2) * (1 - cosAngle),
        vectorNormalized.getX() * vectorNormalized.getY() * (1 - cosAngle) - vectorNormalized.getZ() * sinAngle,
        vectorNormalized.getX() * vectorNormalized.getZ() * (1 - cosAngle) + vectorNormalized.getY() * sinAngle,
        0,
        //new line
        vectorNormalized.getY() * vectorNormalized.getX() * (1 - cosAngle) + vectorNormalized.getZ() * sinAngle,
        cosAngle + powf(vectorNormalized.getY(), 2) * (1 - cosAngle),
        vectorNormalized.getY() * vectorNormalized.getZ() * (1 - cosAngle) - vectorNormalized.getX() * sinAngle,
        0,
        //new line
        vectorNormalized.getZ() * vectorNormalized.getX() * (1 - cosAngle) - vectorNormalized.getY() * sinAngle,
        vectorNormalized.getZ() * vectorNormalized.getY() * (1 - cosAngle) + vectorNormalized.getX() * sinAngle,
        cosAngle + powf(vectorNormalized.getZ(), 2) * (1 - cosAngle),
        0,
        //new line
        0,
        0,
        0,
        1

    };
    result.setData(rotationMatrixValues, 16);
    return (result);    
}

Matrix Matrix::perspective(float fov, float aspect, float near, float far)
{
    Matrix result(4, 4);

    float tangent = tanf(Utils::DegToRad(fov / 2));
    float halfHeight = near * tangent;
    float halfWidth = halfHeight * aspect;

    float left = halfWidth;
    float right = -halfWidth;
    float top = halfHeight;
    float bottom = -halfHeight;

    /*
    modification : 
    add a minus for (0, 0)
    swap (2, 3) and (3, 2)
    */
    float perspectiveMatrixValues[] = {
        -((2 * near) / (right - left)),
        0,
        (right + left) / (right - left),
        0,
        //new line
        0,
        (2 * near) / (top - bottom),
        (top + bottom) / (top - bottom),
        0,
        //new line
        0,
        0,
        (-(far + near)) / (far - near),
        -1, 
        //new line
        0,
        0,
        (-2 * far * near) / (far - near),
        0
    };
    result.setData(perspectiveMatrixValues, 16);
    return (result);    
}

Matrix Matrix::normalize(const Matrix &vector)
{
    Matrix result(vector);
    float length;

    if (vector.getRows() != 3 || vector.getColumns() != 1)
        throw(Utils::Exception("MATRIX::ROTATE::VECTOR_INVALID_SIZE\n"
        "VECTOR SIZE => " + std::to_string(vector.getRows()) + " * " + std::to_string(vector.getColumns())));

    length = sqrt(powf(vector.getX(), 2) + powf(vector.getY(), 2) + powf(vector.getZ(), 2));
    if (length != 0)
    {
        result.setData(0, 0, vector.getX() / length);
        result.setData(1, 0, vector.getY() / length);
        result.setData(2, 0, vector.getZ() / length);
    }
    return (result);
}

Matrix Matrix::crossProduct(const Matrix &vectorA, const Matrix &vectorB)
{
    Matrix result(3, 1);
    

    if (vectorA.getRows() != 3 || vectorA.getColumns() != 1)
        throw(Utils::Exception("MATRIX::CROSS_PRODUCT::VECTOR_A_INVALID_SIZE\n"
        "VECTOR SIZE => " + std::to_string(vectorA.getRows()) + " * " + std::to_string(vectorA.getColumns())));
    if (vectorB.getRows() != 3 || vectorB.getColumns() != 1)
        throw(Utils::Exception("MATRIX::CROSS_PRODUCT::VECTOR_B_INVALID_SIZE\n"
        "VECTOR SIZE => " + std::to_string(vectorB.getRows()) + " * " + std::to_string(vectorB.getColumns())));
    
    float pointI[] = {
        vectorA.getX(),
        vectorA.getY(),
        vectorA.getZ()
    };

    float pointJ[] = {
        vectorB.getX(),
        vectorB.getY(),
        vectorB.getZ()
    };

    float vectorIJ[] = {
        pointJ[0] - pointI[0],
        pointJ[1] - pointI[1],
        pointJ[2] - pointI[2]
    };

    float crossValues[] = {
        vectorA.getY() * vectorIJ[2] - vectorA.getZ() * vectorIJ[1],
        vectorA.getZ() * vectorIJ[0] - vectorA.getX() * vectorIJ[2],
        vectorA.getX() * vectorIJ[1] - vectorA.getY() * vectorIJ[0]
    };
    result.setData(crossValues, 3);
    return (result);
}

float Matrix::dotProduct(const Matrix &vectorA, const Matrix &vectorB)
{
    float dotProduct;
    
    if (vectorA.getRows() != 3 || vectorA.getColumns() != 1)
        throw(Utils::Exception("MATRIX::DOT_PRODUCT::VECTOR_A_INVALID_SIZE\n"
        "VECTOR SIZE => " + std::to_string(vectorA.getRows()) + " * " + std::to_string(vectorA.getColumns())));
    if (vectorB.getRows() != 3 || vectorB.getColumns() != 1)
        throw(Utils::Exception("MATRIX::DOT_PRODUCT::VECTOR_B_INVALID_SIZE\n"
        "VECTOR SIZE => " + std::to_string(vectorB.getRows()) + " * " + std::to_string(vectorB.getColumns())));
    
    dotProduct = vectorA.getX() * vectorB.getX() + vectorA.getY() * vectorB.getY() + vectorA.getZ() * vectorB.getZ();
    return (dotProduct);
}

Matrix Matrix::lookAt(const Matrix &position, const Matrix &target, const Matrix &initialUpVector)
{
    Matrix front(Matrix::normalize(position - target));
    Matrix right(Matrix::normalize(Matrix::crossProduct(initialUpVector, front)));
    Matrix up(Matrix::normalize(Matrix::crossProduct(front, right)));
    Matrix translation(3, 1);
    Matrix result(4, 4);

    translation.setData(0, 0, Matrix::dotProduct(position, right));
    translation.setData(1, 0, Matrix::dotProduct(position, up));
    translation.setData(2, 0, Matrix::dotProduct(position, front));

    float values[] = {
        right.getX(), up.getX(), front.getX(), 0,
        right.getY(), up.getY(), front.getY(), 0,
        right.getZ(), up.getZ(), front.getZ(), 0,
        -translation.getX(), -translation.getY(), -translation.getZ(), 1
    };
    result.setData(values, 16);
    return (result);
}

Matrix Matrix::Zero(const Matrix &instance)
{
    Matrix result(instance);

    result.uniform(0);
    return (result);
}

std::ostream& operator << (std::ostream &os, const Matrix &instance)
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