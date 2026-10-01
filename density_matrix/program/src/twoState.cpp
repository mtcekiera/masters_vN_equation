#include "../headers/twoState.hpp"

namespace twoState{
    
    // ============================================================
    // transformMatrix 
    // ============================================================
    Matrix transformMatrix(
        const Matrix& matrix,
        const Matrix& transformation
    )
    {
        return transformation * matrix * transformation.dagger();
    }

    // ============================================================
    // generate S matrix
    // ============================================================
    Matrix generateSMatrix(
        double omega, 
        double delta, 
        double v
    )
    {
        const double denominator = std::sqrt(2.0*omega * (omega+delta));
        return Matrix{
            {omega + delta, -v},
            {v, omega + delta},
        } / denominator;
    }


    // ============================================================
    // U diagonal
    // ============================================================
    Matrix UDiag(double t, HamParams Hpar)
    {
        const Complex j{0.0, 1.0};
        return Matrix{
            {std::exp(-j*Hpar.Omega*t), 0.0},
            {0.0, std::exp(j*Hpar.Omega*t)}
        };
    }




    // ============================================================
    // U bare
    // ============================================================
    Matrix UBare(
        double t, 
        HamParams Hpar
    )
    {
        return transformMatrix(UDiag(t, Hpar), Hpar.SMatrix);
    }


    // ============================================================
    // Analytic transformation by time
    // ============================================================
    
    Matrix transformByTime(const Matrix& matrix0, double t, HamParams Hpar){
        return transformMatrix(matrix0, UBare(t,Hpar));
    }

    // ============================================================
    // Theoretical evolution
    // ============================================================

    EvolData theoreticalEvol(
        const Matrix& rho0, HamParams Hpar, double tMax, std::size_t steps
    ){
        if (steps == 0)
            throw std::invalid_argument(
                "Number of steps must be greater than zero"
            );
        double dt = tMax / static_cast<double>(steps);

        EvolData result(steps);


        // t = 0
        result.t.push_back(0.0);
        result.p1.push_back(rho0[0][0]);
        result.p2.push_back(rho0[1][1]);
        result.p12.push_back(rho0[0][1]);


        for (std::size_t i = 1; i <= steps; ++i)
        {
            const double t = dt * static_cast<double>(i);

            const Matrix rho = transformByTime(rho0, t, Hpar);

            result.t.push_back(t);
            result.p1.push_back(rho[0][0]);
            result.p2.push_back(rho[1][1]);
            result.p12.push_back(rho[0][1]);
        }


        return result;
    }
    
    
    // ============================================================
    // rho time derivative (von Neumann equation)
    // ============================================================
    Matrix drhoDt(const Matrix& rho, const Matrix& H, double hbar=1.0){
        const Complex j{0.0, 1.0};
        return (-j/hbar) * (H*rho - rho*H);
    }


    // ============================================================
    // RK4 step
    // ============================================================
    Matrix rk4Step(double dt, const Matrix& rho, const Matrix& H){
        const Matrix k1 = drhoDt(rho,                   H);
        const Matrix k2 = drhoDt(rho + k1 * (0.5 * dt), H);
        const Matrix k3 = drhoDt(rho + k2 * (0.5 * dt), H);
        const Matrix k4 = drhoDt(rho + k3 * dt,         H);

        return rho + (k1 + 2.0*k2 + 2.0*k3 + k4) * (dt / 6.0);
    }



    // ============================================================
    // Numerical evolution
    // ============================================================

    EvolData numericEvol(
        const Matrix& rho0, HamParams Hpar, double tMax, std::size_t steps
    ){
        if (steps == 0)
            throw std::invalid_argument(
                "Number of steps must be greater than zero"
            );
        
            const Matrix H{
                {Hpar.Delta, Hpar.V     },
                {Hpar.V,    -Hpar.Delta }
            };

            const double dt = tMax / static_cast<double>(steps);

            Matrix rho = rho0;

            EvolData result(steps);


        // t = 0
        result.t.push_back(0.0);
        result.p1.push_back(rho0[0][0]);
        result.p2.push_back(rho0[1][1]);
        result.p12.push_back(rho0[0][1]);


        for (std::size_t i = 1; i <= steps; ++i)
        {
            const double t = static_cast<double>(i) * dt;

            rho = rk4Step(dt, rho, H);

            result.t.push_back(t);
            result.p1.push_back(rho[0][0]);
            result.p2.push_back(rho[1][1]);
            result.p12.push_back(rho[0][1]);
        }

        return result;
    }

    

    // ============================================================
    // File saving
    // ============================================================
    void saveCSV(const std::string& filename, const EvolData& data){
        if (
            data.t.size() != data.p1.size()
            || data.t.size() != data.p2.size()
            || data.t.size() != data.p12.size()
        )
        {
            throw std::invalid_argument(
                "EvolutionData vectors have different sizes"
            );
        }


        std::ofstream file(filename);


        if (!file)
        {
            throw std::runtime_error(
                "Could not open file: " + filename
            );
        }


        // Preserve reasonable numerical precision
        file << std::setprecision(15);


        // Header
        file
            << "t,"
            << "p1_real,p1_imag,"
            << "p2_real,p2_imag,"
            << "p12_real,p12_imag\n";


        for (std::size_t i = 0; i < data.t.size(); ++i)
        {
            file
                << data.t[i] << ","

                << data.p1[i].real() << ","
                << data.p1[i].imag() << ","

                << data.p2[i].real() << ","
                << data.p2[i].imag() << ","

                << data.p12[i].real() << ","
                << data.p12[i].imag()

                << "\n";
        }
    }

}
