#include <iostream>

int main() {
// 1. Declare the constant double here
    const double m_electron = 9.109e-31;
// 2. Declare your loop counter integer here
    int loop_counter = 10;

    const double g = 9.81; // acceleration due to gravity in m/s^2
    double force_electron = m_electron * g; // calculate the force on the electron

for (int i = 0; i < loop_counter; ++i) {
    std::cout << "Loop iteration: " << i + 1 << std::endl;
    double force_i_electron = i * force_electron; // calculate the force on the electron for this iteration
    std::cout << "The force on " << i << " electrons is: " << force_i_electron << " N" << std::endl;
    }
// 3. Print them out here
    std::cout << "The loop counter is: " << loop_counter << std::endl;
        std::cout << "" << std::endl;
    std::cout << "The mass of the electron is: " << m_electron << " kg" << std::endl;
    std::cout << "The force on the electron due to gravity is: " << force_electron << " N" << std::endl;


    return 0;
}

