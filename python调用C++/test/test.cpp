#include "test.h"

extern "C" {
    char* test(char* n) {
        std::cout << "The C++ input char is " << n;
        return n;
    }
}

