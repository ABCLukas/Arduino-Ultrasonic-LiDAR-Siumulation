#pragma once

#include "PointCloud.hpp"

class SerialReader{
    public:
        //Constructor
        SerialReader(const char* portName,int baudrate);

        //Opens Serial connection, return false if fails
        bool openCon();

        //closes Serial Connection
        void closeCon();

        //checks if Connection is Open
        bool isOpen();

        //Reads, Parses and adds them to PointCloud
        void readPoints(PointCloud& Cloud,int scanresolution);

    private:
        //file descriptor for serial port
        int fd_;
        bool opened_;    
        const char* portName_;
        int baudrate_;
};