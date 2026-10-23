#include <stdexcept>
#include <cmath>

#include "Matrix.hpp"
#include "State.hpp"
#include "tests.hpp"

namespace tests{

    void clearFolder(const std::filesystem::path& folder)
    {
        if (!std::filesystem::exists(folder))
            return;

        for (const auto& entry : std::filesystem::directory_iterator(folder))
            std::filesystem::remove_all(entry.path());
    }

    void testSteps(
        const std::string& folderName,
        const State& initial,
        double delta,
        double v,
        const std::vector<std::size_t>& stepsList,
        double tmax
    )
    {
        std::filesystem::path dir = "./data";
        std::filesystem::path targetFolder = dir / folderName;
        // clearFolder(targetFolder);
        std::filesystem::create_directories(targetFolder);


        Matrix rho = initial.matrix();

        twoState::HamParams H(delta, v);


        // ---------------------------------------------------------
        // Manifest
        // ---------------------------------------------------------

        std::ofstream manifest(targetFolder / "manifest.csv");

        if (!manifest)
            throw std::runtime_error("Could not create manifest.csv");


        manifest << std::setprecision(15);

        manifest
            << "steps,"
            << "theory_file,"
            << "numeric_file,"
            << "delta,"
            << "v,"
            << "tmax,"
            << "dt,"
            << "a_real,"
            << "a_imag,"
            << "b_real,"
            << "b_imag\n";


        // ---------------------------------------------------------
        // Evolutions
        // ---------------------------------------------------------

        for (std::size_t steps : stepsList)
        {
            std::string theoryFile = "st" + std::to_string(steps) + "_theory.csv";
            std::string numericFile = "st" + std::to_string(steps) + "_numeric.csv";

            twoState::EvolData theory = twoState::theoreticalEvol(rho, H, tmax, steps);
            twoState::EvolData numeric = twoState::numericEvol(rho, H, tmax, steps);

            twoState::saveCSV(targetFolder / theoryFile, theory);

            twoState::saveCSV(targetFolder / numericFile, numeric);

            double dt = tmax / static_cast<double>(steps);

            // -----------------------------------------------------
            // Add entry to manifest
            // -----------------------------------------------------
            manifest
                << steps << ","
                << theoryFile << ","
                << numericFile << ","
                << delta << ","
                << v << ","
                << tmax << ","
                << dt << ","
                << initial[0].real() << ","
                << initial[0].imag() << ","
                << initial[1].real() << ","
                << initial[1].imag()
                << "\n";
        }
    }


    std::vector<double> genLinspace(double start, double stop, std::size_t points){
        if (points<2)
            throw std::invalid_argument("No. of points cannot be less than 2");
        double jump = (stop - start) / static_cast<double>(points-1);
        std::vector<double> values;
        values.reserve(points);
        for (std::size_t i=0; i<points; i++){
            double current = start + i*jump;
            values.push_back(current);
        }
        return values;
    }


    double testError(const Matrix& rho, double Delta, double V,
                            const ErrorTestParams& params, int element) {
        twoState::HamParams Hpar(Delta, V);

        twoState::EvolData theory = theoreticalEvol(rho, Hpar, params.tmax, params.steps);
        twoState::EvolData numeric = numericEvol(rho, Hpar, params.tmax, params.steps);

        if (element < 0 || element > 2)
            throw std::invalid_argument("element must be 0 (p11), 1 (p22), or 2 (p12)");

        double maxError = 0.0;

        for (std::size_t i = 0; i < theory.p1.size(); ++i) {
            double error = 0.0;

            if (element == 0)
                error = std::abs(theory.p1[i].real() - numeric.p1[i].real());

            else if (element == 1)
                error = std::abs(theory.p2[i].real() - numeric.p2[i].real());

            else
                error = std::abs(theory.p12[i] - numeric.p12[i]);

            maxError = std::max(maxError, error);
        }

        return maxError;
    }

    void testErrors_DeltaV(const Matrix& rho,
                                    const std::vector<double>& DeltaValues,
                                    const std::vector<double>& VValues,
                                    const ErrorTestParams& params,
                                    int element,
                                    const std::filesystem::path& filename) {
        if (element < 0 || element > 2)
            throw std::invalid_argument("element must be 0 (p11), 1 (p22), or 2 (p12)");

        std::ofstream file(filename);

        if (!file)
            throw std::runtime_error("Could not open file: " + filename.string());

        file << std::setprecision(15);
        file << "Delta,V,omega,tmax,steps,dt,element,max_error\n";

        const std::size_t total = DeltaValues.size() * VValues.size();
        std::size_t completed = 0;
        int lastPercent = -1;

        for (double Delta : DeltaValues) {
            for (double V : VValues) {
                double omega = std::sqrt(Delta * Delta + V * V);

                double maxError = testError(rho, Delta, V, params, element);

                file << Delta << ","
                    << V << ","
                    << omega << ","
                    << params.tmax << ","
                    << params.steps << ","
                    << params.dt << ","
                    << element << ","
                    << maxError << "\n";


                ++completed;

                int percent = static_cast<int>(100.0 * completed / total);

                if (percent != lastPercent) {
                    constexpr int barWidth = 40;
                    int filled = barWidth * percent / 100;

                    std::cout << "\r[";
                    for (int i = 0; i < barWidth; ++i)
                        std::cout << (i < filled ? '#' : ' ');

                    std::cout << "] " << std::setw(3) << percent << "%  "
                            << completed << "/" << total << std::flush;

                    lastPercent = percent;
                }
            }
        }
        std::cout<<"\n";
    }
}