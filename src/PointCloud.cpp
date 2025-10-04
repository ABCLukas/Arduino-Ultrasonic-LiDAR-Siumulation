#include "PointCloud.hpp"

void PointCloud::add(const Point& pt){
	scannedPoints_.push_back(pt);
}

void PointCloud::clear(){
	scannedPoints_.clear();
}

u_int16_t PointCloud::size(){
	return scannedPoints_.size();
}

std::vector<Point>& PointCloud::getPoints(){
	return scannedPoints_;
}