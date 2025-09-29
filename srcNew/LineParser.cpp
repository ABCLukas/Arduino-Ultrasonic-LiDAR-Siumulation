#include "LineParser.hpp"
#include <cmath>
#include <sys/types.h>

Point LineParser::parse(std::string input){

    size_t commaPos = input.find(',');

	int angle = std::stoi(input.substr(0, commaPos));
	int distance = std::stoi(input.substr(commaPos + 1));
	
	return Point{getX(angle,distance),getY(angle,distance)};
}

int getX(int angle,int distance ){
	double angleRadiant = angle * M_PI / 180.0;
	return distance * cos(angleRadiant);
}

int getY(int angle,int distance){
	double angleRadiant = angle * M_PI / 180.0;
	return distance* sin(angleRadiant);
}