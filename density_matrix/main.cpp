#include <iostream>

#include "program/headers/Matrix.hpp"
#include "program/headers/State.hpp"
#include "program/headers/twoState.hpp"

int main(){
    State psi({1, 0});
    Matrix rho = psi.matrix();
    twoState::HamParams H(0, 1);
    twoState::EvolData theory = twoState::theoreticalEvol(rho, H, 5, 500);
    twoState::EvolData num = twoState::numericEvol(rho, H, 5, 500);
    twoState::saveCSV("data/th_a1b0E1V0.csv", theory);
    twoState::saveCSV("data/num_a1b0E1V0.csv", theory);

    return 0;
}