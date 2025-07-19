#include <iostream>
#include "Points.hpp"

// Declare the existing functions you implemented elsewhere
extern int startSDL(int argc, char* argv[]);
extern int readSerialData();

int main(int argc, char* argv[]) {
    // Read data from Arduino (fills scannedPoints)
    if (readSerialData() != 0) {
        std::cerr << "Failed to read serial data.\n";
        return 1;
    }

    return startSDL(argc, argv);
}

