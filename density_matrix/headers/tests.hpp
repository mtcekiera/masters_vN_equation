#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <stdexcept>
#include <iomanip>

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
} // namespace tests
