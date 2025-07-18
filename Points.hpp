#ifndef POINTS_HPP
#define POINTS_HPP

#include <vector>

struct Point {
    int x;
    int y;
};

// Declare the vector as extern here
extern std::vector<Point> scannedPoints;

#endif
