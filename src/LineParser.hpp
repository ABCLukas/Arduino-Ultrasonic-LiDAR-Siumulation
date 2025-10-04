#pragma once
#include "Point.hpp"
#include <string>
#include <sys/types.h>

class LineParser{
    public:
        //Extracts Angle and Distance from Arduino output
        static Point parse(std::string input);        
};