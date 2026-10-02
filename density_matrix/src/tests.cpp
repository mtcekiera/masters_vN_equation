#include "Matrix.hpp"
#include "State.hpp"
#include "twoState.hpp"
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
        clearFolder(targetFolder);
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
}