#pragma once

class Config{
    public:
    const char* serialPort = "/dev/tty/ACM0";
    int baudRate = 9600;
    int scanresolution = 36;
    int windowHeight = 480;
    int windowWidth = 640;
};