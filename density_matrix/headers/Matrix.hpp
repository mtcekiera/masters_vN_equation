#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <stdexcept>

using Complex = std::complex<double>;

class State;

class Matrix{
private:
    std::vector<std::vector<Complex>> matrix;

public:
    std::size_t n;
    
    //  CONSTRUCTORS
    explicit Matrix(size_t n_)
        : matrix(n_, std::vector<Complex>(n_, Complex{0.0, 0.0})),
        n(n_){};

    Matrix(const std::vector<std::vector<Complex>>&);
    Matrix(std::initializer_list<std::initializer_list<Complex>>);


    // CLASS METHODS
    static Matrix fromState(const State&);

    // ACCES
    Complex at(std::size_t, std::size_t) const;
    std::vector<Complex>& operator[](std::size_t);
    const std::vector<Complex>& operator[](std::size_t) const;

    // ACTIONS
    Matrix dagger() const;
    Matrix conj() const;

    // OPERATORS
    Matrix operator*(const Matrix&) const;
    Matrix operator*(const Complex) const;
    Matrix operator/(const Complex) const;

    Matrix operator+(const Matrix&) const;
    Matrix operator-(const Matrix&) const;

    // OUTPUT
    friend std::ostream& operator<<(std::ostream&, const Matrix&);
    friend Matrix operator*(Complex, const Matrix&);
    // friend Matrix operator*(double, const Matrix&);

};