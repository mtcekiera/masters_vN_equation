#pragma once

#include <complex>
#include <vector>
#include <cmath>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <fstream>

#include "Matrix.hpp"
#include "State.hpp"


namespace twoState{
    using Complex = std::complex<double>;

    //// structures 


    struct EvolData{
        std::vector<double> t;
        std::vector<Complex> p1;
        std::vector<Complex> p2;
        std::vector<Complex> p12;

        EvolData(std::size_t steps){
            t.reserve(steps + 1);
            p1.reserve(steps + 1);
            p2.reserve(steps + 1);
            p12.reserve(steps + 1);
        }
    };

    //// transformation 
    Matrix transformMatrix(const Matrix& matrix, const Matrix& transformation);

    //// S matrix generator
    Matrix generateSMatrix(double omega, double delta, double v);

    //hamiltonian data structure
    struct HamParams{
        double Delta;
        double V;
        double Omega;
        Matrix SMatrix; // S transformation matrix

        HamParams(double Delta_, double V_)
        : Delta(Delta_), 
        V(V_), 
        Omega(std::sqrt( Delta_*Delta_ + V_*V_ )),
        SMatrix(generateSMatrix(Omega, Delta, V))
        {}
    };

    // rest of theoretical
    Matrix UDiag(double t, HamParams);
    Matrix UBare(double t, HamParams);
    Matrix transformByTime(
        const Matrix& matrix0, double t, HamParams
    );
    EvolData theoreticalEvol(
        const Matrix& rho0, HamParams, double tMax, std::size_t steps
    );
    

    //// numerical 
    Matrix drhoDt(const Matrix& rho, const Matrix& H, double hbar);
    Matrix rk4Step(double dt, const Matrix& rho, const Matrix& H);
    EvolData numericEvol(
        const Matrix& rho0, HamParams, double tMax, std::size_t steps
    );

    //// files
    void saveCSV(const std::string& filename, const EvolData& data);
}