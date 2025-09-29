#include <sys/types.h>
#include <vector>
#include "Point.hpp"

class PointCloud{
	public:
		//Add a Point to the Vector
		void add(const Point& pt);
		//Clears the entire Vector
		void clear();
		//Returns the Size of the Vector
		u_int16_t size();
		//Returns the Entire Vector
		std::vector<Point>& getPoints();

	private:
		//Vector where the Points are saved
		std::vector<Point> scannedPoints_;
};
