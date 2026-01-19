#include <random>

int randomNumber() {

    // Making rng static ensures that it stays the same
    // Between different invocations of the function
    static std::mt19937 rng(rand());

    int val = rng();

    return abs(val);
}
