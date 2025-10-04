#include "LineParser.hpp"
#include <cmath>
#include <sys/types.h>

Point LineParser::parse(std::string input){

    size_t commaPos = input.find(',');

	int angle = std::stoi(input.substr(0, commaPos));
	int distance = std::stoi(input.substr(commaPos + 1));

	//Gets The Y Position
	double angleRadiant = angle * M_PI / 180.0;
	u_int16_t x=  distance* sin(angleRadiant);

	//Gets The X Position
	u_int16_t y =  distance * cos(angleRadiant);

	return Point{x,y};
}
