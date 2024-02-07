#include <iostream>
#include <cmath>
#include <vector>


int main() {
    std::vector<float> sins;
    sins.reserve(10000000);
    float sum = 0;
    for (int i = 0; i < 10000000; i++) {
        sins[i] = sinf((float)i * 2 * M_PI / 10000000.f);
        sum += sinf((float)i * 2 * M_PI / 10000000.f);
    }    
    std::cout << sum << '\n';
    return 0;
}