#include <Servo.h>
#include<Wire.h>
#include <SR04.h>

#define trig 10
#define echo 11

uint16_t angle = 0;
uint16_t distance = 0;

String jsonOutput = "";

Servo servo;
SR04 sr04 = SR04(echo,trig);

void setup() {
  Serial.begin(9600);
  servo.attach(9);
}

void loop() {
  servo.write(angle);
  distance = sr04.Distance();

  jsonOutput = "{\"angle\":"+String(angle)+"\"distance\":"+String(distance)+"}";

  Serial.println(jsonOutput);
  
  angle += 10;
  if(angle>=360){
    angle=0;
  }
}