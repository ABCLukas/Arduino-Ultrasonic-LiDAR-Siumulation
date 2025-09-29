#include "PointCloud.hpp"
#include <string>
#include <sys/types.h>

class LineParser{
    public:
        //Extracts Angle and Distance from Arduino output
        Point parse(std::string input);

        //Calculates Y Position
        u_int16_t getY(u_int16_t angle,u_int16_t distance);

        //Calculates X Position
        u_int16_t getX(u_int16_t angle,u_int16_t distance);
};