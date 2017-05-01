extern "C" {
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <inttypes.h>
}
#include "Adafruit_VL6180X.h"
#include <core.h>
#include <math.h>

const int trig = 3;
const int echo = 4;

#define pinI1 8   // pins 1 + 2 is the right motor. 1 low, 2 high is forwards.
#define pinI2 11
#define pinI3 12  // pins 3 + 4 is the left motor.  3 low, 4 high is forwards.
#define pinI4 13
#define speedpinA 9
#define speedpinB 10

#define speed 30
int leftSpeed = speed;
int rightSpeed = speed;

Adafruit_VL6180X laser1;
byte laser1SHDNPin = 1;

Adafruit_VL6180X laser2;
byte laser2SHDNPin = 7;

int getMotor(char motor) {
  switch(motor) {
    case 'L':
    case 'l':
      return leftSpeed;
    case 'R':
    case 'r':
      return rightSpeed;
  }
}

void forward()
{
  analogWrite(speedpinA, speed);
  analogWrite(speedpinB, speed);
  digitalWrite(pinI4, LOW);
  digitalWrite(pinI3, HIGH);
  digitalWrite(pinI2, LOW);
  digitalWrite(pinI1, HIGH);
}


void setMotor(char motor, int newSpeed)
{
  switch(motor) {
    case 'L':
    case 'l':
      analogWrite(speedpinB, newSpeed);
      leftSpeed = newSpeed;
      break;
    case 'R':
    case 'r':
      analogWrite(speedpinA, newSpeed);
      rightSpeed = newSpeed;
      break;
  }
  return;
}

void startCar()
{
  forward();
}

void stopCar()
{
  setMotor('L', 0);
  setMotor('R', 0);
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

double difference(int left, int right) {
    double total = left + right;
    int sub;
    if (left == right) {
        return 0;
    } else if (left >= right) {
        sub = left - right;
    } else {
        sub = right - left;
    }
    return ((double) sub / total);
}

void driveStraight() {
    double percentDif;
    int leftLaser = getDistance('l');
    int rightLaser = getDistance('r');
    percentDif = difference(leftLaser, rightLaser);
    char motor1 = 'l';
    char motor2 = 'r';
    if (leftLaser > rightLaser) {
        motor1 = 'r';
        motor2 = 'l';
    }

    //Stop if no wall
    if (leftLaser == 25 || rightLaser == 25) {
      stopCar();
      while(1) {
        delay(50);
      }
    }

    //Limit motor changes
    if (getMotor(motor1) >= 90) {
      setMotor(motor1, 89);
    } else if (getMotor(motor2) >= 90) {
      setMotor(motor2, 89);
    }
    //Drive straight
    else {
      if (percentDif > 0.25) {
        setMotor( motor1, (int) round(getMotor(motor1)*(1 + (percentDif / 2))));
        setMotor( motor2, (int) round(getMotor(motor2)*(1 - (percentDif / 2))));  
      } else {
        startCar();
      }
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

  startCar();
}

void loop() {
  printf("Left: %d  Right: %d,  Front: %d\n\n", getDistance('L'), getDistance('R'), getDistance('F'));
  printf("          Motor Left: %d   Motor Right: %d\n\n", getMotor('L'), getMotor('R'));
  if (getDistance('f') <= 20) {
    printf("Stopping Motors\n");
    stopCar();
    return;
  } else {
    driveStraight();
  }
  delay(100);
}