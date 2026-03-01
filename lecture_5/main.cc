#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    // --- Del 1: Kommandolinje-argumenter ---
    std::cout << "--- Del 1: Kommandolinje-argumenter ---" << std::endl;
    for (int i = 1; i < argc; i++) {
        double x = std::stod(argv[i]);
        std::cout << "x = " << x << ", sin(x) = " << std::sin(x) << ", cos(x) = " << std::cos(x) << std::endl;
    }

    // --- Del 2: Standard input (cin) ---
    // Bemærk: Denne del kører kun, hvis du sender data ind (f.eks. via echo eller en pipe)
    std::cout << "\n--- Del 2: Læsning fra stdin ---" << std::endl;
    double x_cin;
    while (std::cin >> x_cin) {
        std::cout << "x = " << x_cin << ", sin(x) = " << std::sin(x_cin) << ", cos(x) = " << std::cos(x_cin) << std::endl;
    }

    // --- Del 3: Fil I/O (Læs fra input.txt -> Skriv til output.txt) ---
    std::cout << "\n--- Del 3: Fil I/O ---" << std::endl;
    std::string input_name = "input.txt";
    std::string output_name = "output.txt";

    std::ifstream instream(input_name);
    std::ofstream outstream(output_name);

    if (instream.is_open() && outstream.is_open()) {
        double x_file;
        while (instream >> x_file) {
            outstream << "x = " << x_file << ", sin(x) = " << std::sin(x_file) << std::endl;
        }
        std::cout << "Data læst fra " << input_name << " og gemt i " << output_name << std::endl;
    } else {
        std::cerr << "Fejl: Kunne ikke åbne filer!" << std::endl;
    }

    return 0;
}