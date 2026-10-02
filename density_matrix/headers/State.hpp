#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <stdexcept>
#include <initializer_list>

using Complex = std::complex<double>;

class Matrix;

class State{
private:
    std::vector<Complex> vector;

public:
    std::size_t n;

    // --------------------------------------------------------
    // CONSTRUCTORS
    // --------------------------------------------------------

    State(const std::vector<Complex>&);
    State(const std::initializer_list<Complex>);


    // --------------------------------------------------------
    // METHODS
    // --------------------------------------------------------

    void normalize();

    Complex at(std::size_t) const;

    const std::vector<Complex>& getVector() const;

    Complex& operator[](std::size_t);

    const Complex& operator[](std::size_t) const;

    friend std::ostream& operator<<(std::ostream&, const State&);

    Matrix matrix() const; 
};