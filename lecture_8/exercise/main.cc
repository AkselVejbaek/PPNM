#include <iostream>
#include <vector>
#include <thread>
#include <string>

// Struct til at sende data til hver tråd
struct data { 
    int a, b; 
    double sum; 
};

// Funktionen som hver tråd skal køre
void harm(data& arg) {
    int a = arg.a;
    int b = arg.b;
    double sum = 0;
    for (int i = a; i < b; i++) {
        sum += 1.0 / i;
    }
    arg.sum = sum; // Skriv kun til delt hukommelse én gang til sidst
}

int main(int argc, char* argv[]) {
    int nthreads = 1;
    int nterms = (int)1e8; // Standard værdi

    // Læs argumenter fra kommandolinjen
    for (int i = 0; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-threads" && i + 1 < argc) nthreads = std::stoi(argv[i + 1]);
        if (arg == "-terms" && i + 1 < argc) nterms = std::stoi(argv[i + 1]);
    }

    // Forbered data-objekter
    std::vector<data> params(nthreads);
    for (int i = 0; i < nthreads; i++) {
        params[i].a = 1 + (nterms / nthreads) * i;
        params[i].b = 1 + (nterms / nthreads) * (i + 1);
    }
    params[nthreads - 1].b = nterms + 1; // Justér sidste interval

    // Opret og kør tråde
    std::vector<std::thread> threads;
    threads.reserve(nthreads);

    for (int i = 0; i < nthreads; i++) {
        // std::ref er nødvendig for at sende referencer til tråde
        threads.emplace_back(harm, std::ref(params[i]));
    }

    // Vent på at alle tråde er færdige (Join)
    for (auto &t : threads) {
        t.join();
    }

    // Beregn den totale sum
    double total = 0;
    for (const auto &p : params) {
        total += p.sum;
    }

    std::cout << "N_threads: " << nthreads << ", Sum: " << total << std::endl;

    return 0;
}