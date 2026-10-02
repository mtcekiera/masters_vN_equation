#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <stdexcept>
#include <iomanip>

#include "Matrix.hpp"
#include "State.hpp"
#include "twoState.hpp"
#include "tests.hpp"


int main(){
    std::vector<std::size_t> STEPS({100, 316, 1000, 3162, 10000});
    tests::testSteps("a1b1E1V2", State({1, 1}), 1.0, 2.0, STEPS, 10.0);
    return 0;
}