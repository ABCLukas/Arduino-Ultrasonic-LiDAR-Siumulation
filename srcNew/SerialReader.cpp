#include "SerialReader.hpp"
#include "LineParser.hpp"
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <termios.h>
#include <unistd.h>

SerialReader::SerialReader(const char* portName,int baudrate) 
    : portName_(portName),baudrate_(baudrate){

};

bool SerialReader::openCon(){
    opened_=true;

    fd_ = open(portName_,O_RDONLY | O_NOCTTY | O_SYNC);

    if(fd_ < 0){
        std::cerr << "Error Opening" << portName_ << ": " << strerror(errno) << "\n"; 
        opened_ = false;
        return false;
    }
    return true;

    struct termios tty;
    memset(&tty,0,sizeof tty);

    if(tcgetattr(fd_, &tty)){
        std::cerr << "Error from tcgetattr: " << strerror(errno) << "\n";
        close(fd_);
        opened_ = false;
        return false;
    }

    cfsetospeed(&tty, baudrate_);
    cfsetispeed(&tty, baudrate_);

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

    if (tcsetattr(fd_, TCSANOW, &tty) != 0) {
		std::cerr << "Error from tcsetattr: " << strerror(errno) << "\n";
		close(fd_);
        opened_ = false;
		return 1;
	}
};

void SerialReader::closeCon(){
    opened_ = false;
    close(fd_);
}

bool SerialReader::isOpen(){
    return opened_;
}

void SerialReader::readPoints(PointCloud& Cloud){
    if(isOpen()){
        Cloud.clear();
        std::string line;
        char buffer;

        while (true) {
    	    int n = read(fd_, &buffer, 1);
        
            if(n > 0) {
		    	if(buffer == '\n'&&!line.empty()) {
                    //readLine
                Cloud.add(LineParser::parse(line));
    			line.clear();
    		}else line+=buffer;
    	} else if (n < 0) {
    		std::cerr << "Error reading: " << strerror(errno) << "\n";
    		break;
    	}

	    if(Cloud.size()>36) break;
	}
        
    }else{
        std::cerr << "Error from readPoints" << strerror(errno) << "\n";
    }
}