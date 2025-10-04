#include "gtest/gtest.h"
#include "../srcNew/LineParser.hpp"

TEST(LineParserTest, CanParsae){
    Point p = LineParser::parse("160,45");
    // Radiant = 45*PI/180 = 0,785398163 
    // x = 160 * sin(0,785398163) 
    // y = 160 * cos(0,785398163) 
  
    
    int x = p.x;
    int y = p.y;

    Point pe = Point{2,160};

    EXPECT_EQ(p,pe);
}