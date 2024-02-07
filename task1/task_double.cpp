#include <iostream>
#include <cmath>
#include <vector>
#define mult 10000000

int main() {
    std::vector<double> sins;
    sins.reserve(mult);
    double sum = 0;
    for (int i = 0; i < mult; i++) {
        sins[i] = sin((double)i * 2 * M_PI / mult);
        sum += sin((double)i * 2 * M_PI / mult);
    }
    std::cout << sum << '\n';
    return 0;
}