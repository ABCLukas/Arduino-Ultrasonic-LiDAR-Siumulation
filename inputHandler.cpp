#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <array>
#include <math.h>

std::array<std::array<int,2>,255> measuredValues;//initialize and write to

void deconstructJson(std::string buffer){
	std::array<int,2> point; // 0=distance, 1=angle

	std::string search_key = "\"distance\":";
	int pos = buffer.find(search_key);

	search_key = "\"angle\":";
	int angle = buffer.find(search_key);
	int angleRad = angle * M_PI / 180.0;
	
	for (size_t i = 0; i < measuredValues.size(); ++i) {
		if (measuredValues[i][0] == -1 && measuredValues[i][1] == -1) {
			measuredValues[pos][angleRad];
		break;
		}
	}

}

int getX(std::array<int,2> arr){
	return arr[0]* cos(arr[1]);
}

int getY(std::array<int,2> arr){
	return arr[0]* sin(arr[1]);
}

std::array<std::array<int,2>,255> getMeasuredValues(){
	return measuredValues;
}


int main() {
	for (auto& pair : measuredValues) {
		pair = {-1, -1};
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
			//std::cout << "Received line: " << buf << std::endl;
		
			deconstructJson(buf);


			idx = 0;
		} else if (idx < sizeof(buf) - 1) {
			buf[idx++] = ch;
		}
	} else if (n < 0) {
		std::cerr << "Error reading: " << strerror(errno) << "\n";
		break;
		}
	}

	close(fd);
	return 0;
}

