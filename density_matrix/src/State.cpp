#include "Matrix.hpp"
#include "State.hpp"

// using Complex = std::complex<double>;

// --------------------------------------------------------
// CONSTRUCTORS
// --------------------------------------------------------

State::State(const std::vector<Complex>& vector_)
    : vector(vector_), n(vector_.size())
{
    if(vector.empty())
        throw std::invalid_argument("State cannot be empty");

    normalize();
}

State::State(const std::initializer_list<Complex> values)
    : vector(values), n(values.size())
{
    if(vector.empty())
        throw std::invalid_argument("State cannot be empty");

    normalize();
}


// --------------------------------------------------------
// METHODS
// --------------------------------------------------------


void State::normalize(){ // all normalized using L^2
    double norm = 0.0;

    for (const auto& value : vector)
        norm += std::norm(value);
    
    norm = std::sqrt(norm);

    if (norm == 0.0)
        throw std::invalid_argument("Cannot normalize a zero state");

    for (auto& value : vector)
        value /= norm;
}

Complex State::at(std::size_t i) const{
    if (i>=n)
        throw std::invalid_argument("State index out of range");
    return vector[i];
} 

Complex& State::operator[](std::size_t i){
    return vector[i];
}

const Complex& State::operator[](std::size_t i) const{
    return vector[i];
}

const std::vector<Complex>& State::getVector() const{
    return vector;
}

std::ostream& operator<<(std::ostream& os, const State& state){
    os << "[";

    for (std::size_t i = 0; i < state.n; ++i)
    {
        os << state.vector[i];

        if (i + 1 < state.n)
            os << ", ";
    }

    os << "]";

    return os;
}




Matrix State::matrix() const
{
    return Matrix::fromState(*this);
}