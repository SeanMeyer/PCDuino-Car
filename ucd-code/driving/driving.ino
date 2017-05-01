extern "C" {
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <inttypes.h>
}
#include "Adafruit_VL6180X.h"

#include <core.h>

const int trig = 3;
const int echo = 4;

#define pinI1 8   // pins 1 + 2 is the right motor. 1 low, 2 high is forwards.
#define pinI2 11
#define pinI3 12  // pins 3 + 4 is the left motor.  3 low, 4 high is forwards.
#define pinI4 13
#define speedpinA 9
#define speedpinB 10

#define speed 25

Adafruit_VL6180X laser1;
byte laser1SHDNPin = 1;

Adafruit_VL6180X laser2;
byte laser2SHDNPin = 7;

void forward()
{
  analogWrite(speedpinA, speed);
  analogWrite(speedpinB, speed);
  digitalWrite(pinI4, LOW);
  digitalWrite(pinI3, HIGH);
  digitalWrite(pinI2, LOW);
  digitalWrite(pinI1, HIGH);
}

void startCar()
{
  forward();
}

void stopCar()
{
  digitalWrite(speedpinA, LOW);
  digitalWrite(speedpinB, LOW);
}

void setMotor(char motor, int newSpeed)
{
  switch(motor) {
    case 'L':
    case 'l':
      analogWrite(speedpinA, newSpeed);
      break;
    case 'R':
    case 'r':
      analogWrite(speedpinB, newSpeed);
      break;
  }
  return;
}

void backward()
{
  analogWrite(speedpinA, speed);
  analogWrite(speedpinB, speed);
  digitalWrite(pinI4, HIGH);
  digitalWrite(pinI3, LOW);
  digitalWrite(pinI2, HIGH);
  digitalWrite(pinI1, LOW);
}

void left()
{
  analogWrite(speedpinA, speed);
  analogWrite(speedpinB, speed);
  digitalWrite(pinI1, LOW);
  digitalWrite(pinI2, HIGH);
  digitalWrite(pinI4, LOW);
  digitalWrite(pinI3, HIGH);
}

void right()
{
  analogWrite(speedpinA, speed);
  analogWrite(speedpinB, speed);
  digitalWrite(pinI1, HIGH);
  digitalWrite(pinI2, LOW);
  digitalWrite(pinI4, HIGH);
  digitalWrite(pinI3, LOW);
}

long microsecondsToInches(long microseconds)
{
  return (microseconds / 74 / 2);
}

long microsecondsToCentimeters(long microseconds)
{
  return (microseconds / 29 / 2);
}

int getUltraDistance(void)
{
  long duration;
  int cm;
  digitalWrite(trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH, 1000000);
  cm = microsecondsToCentimeters(duration);
  return cm;
}

int getDistance(char laser)
{
  int mm;
  switch(laser) {
    case 'L':
    case 'l':
      return laser2.readRange() / 10;
      break;
    case 'R':
    case 'r':
      return laser1.readRange() / 10;
      break;
    case 'F':
    case 'f':
      return getUltraDistance();
  }
}

void setup() {
  printf("starting");

  // Motor setup
  pinMode(pinI1, OUTPUT);
  pinMode(pinI2, OUTPUT);
  pinMode(speedpinA, OUTPUT);
  pinMode(pinI3, OUTPUT);
  pinMode(pinI4, OUTPUT);
  pinMode(speedpinB, OUTPUT);

  // Ultrasonic setup
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delay(20);
  
  // Laser setup
  pinMode(laser1SHDNPin, OUTPUT);
  pinMode(laser2SHDNPin, OUTPUT);
  
  Wire.begin();
  digitalWrite(laser1SHDNPin, HIGH);
  digitalWrite(laser2SHDNPin, LOW);
  laser1.begin(0x84);

  digitalWrite(laser2SHDNPin, HIGH);
  laser2.begin(0x86);
}

void loop() {
  printf("looping \n");
  printf("Left: %d\n", getDistance('L'));
  printf("Right: %d\n", getDistance('R'));
  printf("Front: %d\n", getDistance('F'));
  delay(500);
}