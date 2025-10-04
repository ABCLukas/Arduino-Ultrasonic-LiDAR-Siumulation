#include <string>
#include "Config.hpp"
#include "PointCloud.hpp"
#include "SerialReader.hpp"
#include "Renderer.hpp"

int main(int argc,char* argv[]){
    Config cfg;
    std::string port = cfg.serialPort;
    SerialReader sr(cfg.serialPort,cfg.baudRate);
    
    PointCloud pointcloud;
    Renderer renderer(cfg.windowWidth,cfg.windowHeight);

    sr.openCon();

    sr.readPoints(pointcloud);
    
    renderer.drawBackground();
    renderer.renderPoints(pointcloud);
    renderer.pollEvents();    

    return 0;
}