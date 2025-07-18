#include <Ultrasonic.h>
#include <Servo.h>
#include<Wire.h>

#define trig 11
#define echo 10

uint16_t angle = 0;
uint16_t distance = 0;
uint16_t list[36];

String output = "";

Servo servo;
Ultrasonic sr04(echo,trig);


void setup() {
  Serial.begin(9600);
  servo.attach(9);
}

void loop() {
  servo.write(angle);
  delay(300);
  distance = sr04.read();
  delay(16);
  output = String(angle)+","+String(distance);

  Serial.println(output);
  
  angle += 10;
  if(angle>=360){
    angle=0;
  }
}
