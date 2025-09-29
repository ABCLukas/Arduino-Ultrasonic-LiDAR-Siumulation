#include <Ultrasonic.h>
#include <Stepper.h>
#include<Wire.h>

#define trig 6
#define echo 5

int angle = 10;
int curAngle = 0;
uint16_t distance = 0;
const int stepsPerRevolution = 2048;
int direction = 1; //1 forward -1 backwards

String output = "";


Stepper stepper(stepsPerRevolution, 8, 10, 9, 11);
Ultrasonic sr04(echo,trig);

void rotateByAngle(float angle) {
  int steps = angle / 360.0 * stepsPerRevolution;
  stepper.step(steps);
}

void setup() {
  Serial.begin(9600);
  stepper.setSpeed(5);
}

void loop() {
  rotateByAngle(angle*direction);
  delay(1000);
  distance = sr04.read();
  delay(16);

  output = String(curAngle)+","+String(distance);
  Serial.println(output);
  output="";
  
  curAngle += 10;
  
  if(curAngle>=360 || curAngle <= 0.0){
    direction *= -1;
    curAngle = constrain(curAngle, 0, 360);
    delay(1000);
  }
}
