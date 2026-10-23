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
    
    // State initial{1, 1};
    // Matrix rho = initial.matrix();
    // std::vector<std::size_t> STEPS({100, 316, 1000, 3162, 10000});
    // double Delta = 1.0;
    // double V = 1.0;
    double tmax = 10.0;
    // tests::testSteps("TEST_a1b1_D1V1", initial, Delta, V, STEPS, tmax);


    State initial1{0.8, 0.6};
    State initial2{1, 1};
    Matrix rho1 = initial1.matrix();
    Matrix rho2 = initial2.matrix();

    double grid_start = 0.0;
    double grid_end = 50; ////////
    std::size_t tiles = 101;
    std::vector<double> deltas = tests::genLinspace(grid_start, grid_end, tiles);
    std::vector<double> vs = tests::genLinspace(grid_start, grid_end, tiles);

    std::size_t chosen_steps = 1000; // for tmax=10 -> dt=1e-2
    tests::ErrorTestParams params{tmax, chosen_steps};

    // tests::testErrors_DeltaV(
    //     rho1,
    //     deltas,
    //     vs,
    //     params,
    //     0,
    //     "./data/TEST_a08b06_D1V2/DV_test_p1.csv"
    // );
    // tests::testErrors_DeltaV(
    //     rho1,
    //     deltas,
    //     vs,
    //     params,
    //     1,
    //     "./data/TEST_a08b06_D1V2/DV_test_p2.csv"
    // );
    // tests::testErrors_DeltaV(
    //     rho1,
    //     deltas,
    //     vs,
    //     params,
    //     2,
    //     "./data/TEST_a08b06_D1V2/DV_test_p12.csv"
    // );
    tests::testErrors_DeltaV(
        rho2,
        deltas,
        vs,
        params,
        0,
        "./data/TEST_a1b1_D1V1/DV_test_p1.csv"
    );
    // tests::testErrors_DeltaV(
    //     rho,
    //     deltas,
    //     vs,
    //     params,
    //     1,
    //     "./data/TEST_a1b1_D1V1/DV_test_p2.csv"
    // );

    // tests::testErrors_DeltaV(
    //     rho,
    //     deltas,
    //     vs,
    //     params,
    //     2,
    //     "./data/TEST_a1b1_D1V1/DV_test_p12.csv"
    // );

    return 0;
}