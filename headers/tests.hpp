#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include "twoState.hpp"

class State;
class Matrix;

namespace tests
{
    void clearFolder(const std::filesystem::path& folder);
        void testSteps(
        const std::string& folderName,
        const State& initial,
        double delta,
        double v,
        const std::vector<std::size_t>& stepsList,
        double tmax
    );
    std::vector<double> genLinspace(double start, double stop, std::size_t points);
    struct ErrorTestParams {
        const double tmax;
        const std::size_t steps;
        const double dt;

        ErrorTestParams(double tmax_, std::size_t steps_)
            : tmax(tmax_), steps(steps_), dt(tmax_ / steps_) {}
    };

    double testError(const Matrix& rho, double Delta, double V, const ErrorTestParams& params, int element);

    void testErrors_DeltaV(const Matrix& rho,
                        const std::vector<double>& DeltaValues,
                        const std::vector<double>& VValues,
                        const ErrorTestParams& params,
                       int element,
                        const std::filesystem::path& filename);

} // namespace tests
