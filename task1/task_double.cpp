#include <iostream>
#include <cmath>
#include <vector>


int main() {
    std::vector<double> sins;
    sins.reserve(10000000);
    float sum = 0;
    for (int i = 0; i < 10000000; i++) {
        sins[i] = sinf((double)i * 2 * M_PI / 10000000.f);
        sum += sinf((double)i * 2 * M_PI / 10000000.f);
    }    
    std::cout << sum << '\n';
    return 0;
}