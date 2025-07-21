#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "Points.hpp"

//vector that saves the scanned Points
std::vector<Point> scannedPoints;

void addPoint(int x, int y){
	scannedPoints.emplace_back(Point{x,y});
}

int getX(int angle,int distance ){
	double angleRadiant = angle * M_PI / 180.0;
	return distance * cos(angleRadiant);
}

int getY(int angle,int distance){
	double angleRadiant = angle * M_PI / 180.0;
	return distance* sin(angleRadiant);
}

//deconstruct given Line and add it to the vector
void deconstructLine(std::string line){

	size_t commaPos = line.find(',');
	if (commaPos == std::string::npos) return;

	int angle = std::stoi(line.substr(0, commaPos));
	int distance = std::stoi(line.substr(commaPos + 1));
	
	addPoint(getX(angle,distance),getY(angle,distance));
}

int readSerialData() {
	const char* portname = "/dev/ttyACM0";//can vary

	int fd = open(portname, O_RDONLY | O_NOCTTY | O_SYNC);
		if (fd < 0) {
		std::cerr << "ERROR opening " << portname << ": " << strerror(errno) << "\n";
		return 1;
	}

	struct termios tty;
	memset(&tty, 0, sizeof tty);

	if (tcgetattr(fd, &tty) != 0) {
		std::cerr << "ERROR from tcgetattr: " << strerror(errno) << "\n";
		close(fd);
		return 1;
	}

	cfsetospeed(&tty, B9600);
	cfsetispeed(&tty, B9600);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag |= CLOCAL;
	tty.c_cflag |= CREAD;
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_lflag = 0;
	tty.c_oflag = 0;
	tty.c_cc[VMIN]  = 1;
	tty.c_cc[VTIME] = 0;

	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		std::cerr << "ERROR from tcsetattr: " << strerror(errno) << "\n";
		close(fd);
		return 1;
	}


	scannedPoints.clear();
	scannedPoints.reserve(36);
	std::string line;
	char ch;

	while (true) {

		int n = read(fd, &ch, 1);
		if (n > 0) {
			if (ch == '\n') {
				if(!line.empty()){
				std::cout << "line: " << line << std::endl;
				deconstructLine(line);
				line.clear();
			} 
		}else {
			line+=ch;
		}
	} else if (n < 0) {
		std::cerr << "Error reading: " << strerror(errno) << "\n";
		break;
		}

	if(scannedPoints.size()>36) break;
	}

	close(fd);
	return 0;
}

