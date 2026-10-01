#include "../headers/Matrix.hpp"
#include "../headers/State.hpp"


// --------------------------------------------------------
// CONSTRUCTORS
// --------------------------------------------------------

Matrix::Matrix(const std::vector<std::vector<Complex>>& matrix_)
    : matrix(matrix_), n(matrix_.size())
{
    if (matrix.empty())
        throw std::invalid_argument("Matrix cannot be empty");

    for (const auto& row : matrix)
    {
        if (row.size() != n)
            throw std::invalid_argument(
                "Matrix must be square"
            );
    }
}


Matrix::Matrix(std::initializer_list<
        std::initializer_list<Complex>> values)
{
    n = values.size();

    if (n == 0)
        throw std::invalid_argument("Matrix cannot be empty");

    for (const auto& row : values)
    {
        if (row.size() != n)
            throw std::invalid_argument(
                "Matrix must be square"
            );

        matrix.emplace_back(row);
    }
}


// --------------------------------------------------------
// CLASS METHOD EQUIVALENT
// --------------------------------------------------------

Matrix Matrix::fromState(const State& state)
{
    Matrix result(state.n);

    for (std::size_t i = 0; i < state.n; ++i)
    {
        for (std::size_t j = 0; j < state.n; ++j)
        {
            result.matrix[i][j]
                = state[i] * std::conj(state[j]);
        }
    }

    return result;
}


// --------------------------------------------------------
// ACCESS
// --------------------------------------------------------

Complex Matrix::at(std::size_t row, std::size_t col) const{
    if (row >= n || col >= n)
        throw std::out_of_range(
            "Matrix index out of range"
        );

    return matrix[row][col];
}


std::vector<Complex>& Matrix::operator[](std::size_t row){
    return matrix[row];
}


const std::vector<Complex>& Matrix::operator[](std::size_t row) const{
    return matrix[row];
}


// --------------------------------------------------------
// CONJUGATE
// --------------------------------------------------------

Matrix Matrix::conj() const{
    Matrix result(n);

    for (std::size_t i = 0; i < n; ++i){
        for (std::size_t j = 0; j < n; ++j){
            result.matrix[i][j]
                = std::conj(matrix[i][j]);
        }
    }

    return result;
}


// --------------------------------------------------------
// DAGGER
// --------------------------------------------------------

Matrix Matrix::dagger() const
{
    Matrix result(n);

    for (std::size_t i = 0; i < n; ++i){
        for (std::size_t j = 0; j < n; ++j){
            result.matrix[j][i]
                = std::conj(matrix[i][j]);
        }
    }

    return result;
}


// --------------------------------------------------------
// MATRIX MULTIPLICATION
// --------------------------------------------------------

Matrix Matrix::operator*(const Matrix& other) const
{
    if (n != other.n){
        throw std::invalid_argument(
            "Matrix dimensions do not match"
        );
    }

    Matrix result(n);

    for (std::size_t i = 0; i < n; ++i)
    {
        for (std::size_t j = 0; j < n; ++j)
        {
            for (std::size_t k = 0; k < n; ++k)
            {
                result.matrix[i][j]
                    += matrix[i][k]
                    * other.matrix[k][j];
            }
        }
    }

    return result;
}


// --------------------------------------------------------
// ADDITION
// --------------------------------------------------------

Matrix Matrix::operator+(const Matrix& other) const{
    if (n != other.n)
        throw std::invalid_argument(
            "Matrix dimensions do not match"
        );

    Matrix result(n);

    for (std::size_t i = 0; i < n; ++i)
    {
        for (std::size_t j = 0; j < n; ++j)
        {
            result.matrix[i][j]
                = matrix[i][j]
                + other.matrix[i][j];
        }
    }

    return result;
}


// --------------------------------------------------------
// SUBTRACTION
// --------------------------------------------------------

Matrix Matrix::operator-(const Matrix& other) const
{
    if (n != other.n)
        throw std::invalid_argument(
            "Matrix dimensions do not match"
        );

    Matrix result(n);

    for (std::size_t i = 0; i < n; ++i)
    {
        for (std::size_t j = 0; j < n; ++j)
        {
            result.matrix[i][j]
                = matrix[i][j]
                - other.matrix[i][j];
        }
    }

    return result;
}


// --------------------------------------------------------
// SCALAR MULTIPLICATION
// --------------------------------------------------------

Matrix Matrix::operator*(Complex scalar) const
{
    Matrix result(n);

    for (std::size_t i = 0; i < n; ++i)
    {
        for (std::size_t j = 0; j < n; ++j)
        {
            result.matrix[i][j]
                = matrix[i][j] * scalar;
        }
    }

    return result;
}


// --------------------------------------------------------
// SCALAR DIVISION
// --------------------------------------------------------

Matrix Matrix::operator/(Complex scalar) const
{
    Matrix result(n);

    for (std::size_t i = 0; i < n; ++i)
    {
        for (std::size_t j = 0; j < n; ++j)
        {
            result.matrix[i][j]
                = matrix[i][j] / scalar;
        }
    }

    return result;
}


// --------------------------------------------------------
// OUTPUT
// --------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Matrix& mat){
    for (std::size_t i = 0; i < mat.n; ++i)
    {
        os << "[ ";

        for (std::size_t j = 0; j < mat.n; ++j)
        {
            os << mat.matrix[i][j] << " ";
        }

        os << "]\n";
    }

    return os;
}

Matrix operator*(Complex a, const Matrix& mat){
    return mat * a;
}


Matrix transform(const Matrix& matrix,
                 const Matrix& transformation)
{
    return transformation
         * matrix
         * transformation.dagger();
}