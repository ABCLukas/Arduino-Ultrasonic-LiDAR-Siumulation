#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <array>
#include <math.h>

std::array<int,36> measuredValues;//initialize and write to

void deconstructJson(std::string line){

	size_t commaPos = line.find(',');
	if (commaPos == std::string::npos) return;

	int angle = std::stoi(line.substr(0, commaPos));
	int distance = std::stoi(line.substr(commaPos + 1));

	if (angle >= -1 && angle < 36) {
		measuredValues[(angle/10)]=distance;
	}
}

int getX(int angle,int distance ){
	double angleRadiant = angle * M_PI / 180.0;
	return distance * cos(angleRadiant);
}

int getY(int angle,int distance){
	double angleRadiant = angle * M_PI / 180.0;
	return distance* sin(angleRadiant);
}

std::array<int,36> getMeasuredValues(){
	return measuredValues;
}


int main() {
	for (int i = 0; i<36;i++) {
		measuredValues[i]=-1;	
	}

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

	char buf[256];
	int idx = 0;

	//std::cout << "Waiting for data...\n";

	while (true) {
		char ch;
		int n = read(fd, &ch, 1);
		if (n > 0) {
			if (ch == '\n') {
			buf[idx] = '\0';
			std::cout << "Received line: " << buf << std::endl;
				
			//deconstructJson(buf);
			idx = 0;
		} else if (idx < sizeof(buf) - 1) {
			buf[idx++] = ch;
		}
		//test
		for(int i = 0; i<36;i++){
			std::cout << i*10 << " - " << measuredValues[i] << std::endl;
		}
	} else if (n < 0) {
		std::cerr << "Error reading: " << strerror(errno) << "\n";
		break;
		}
	}

	close(fd);
	return 0;
}

