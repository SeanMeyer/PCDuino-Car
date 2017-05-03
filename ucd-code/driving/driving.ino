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
int minMove = 25;

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
      printf("Setting left motor\n");
      if (newSpeed < 0 && leftSpeed > 0) {
        printf("Setting motor to backwards\n");
        analogWrite(speedpinB, 0);
        delay(50);
        digitalWrite(pinI4, HIGH);
        digitalWrite(pinI3, LOW);
      } else if (newSpeed > 0 && leftSpeed < 0) {
        printf("Setting motor to forwards\n");
        analogWrite(speedpinB, 0);
        delay(50);
        digitalWrite(pinI4, LOW);
        digitalWrite(pinI3, HIGH);
      }
      printf("Doing the analog write\n");
      analogWrite(speedpinB, abs(newSpeed));
      leftSpeed = newSpeed;
    case 'R':
    case 'r':
      printf("Setting right motor\n");
      if (newSpeed < 0 && rightSpeed > 0) {
        printf("Setting motor to backwards\n");
        analogWrite(speedpinA, 0);
        delay(50);
        digitalWrite(pinI2, HIGH);
        digitalWrite(pinI1, LOW);
      } else if (newSpeed > 0 && rightSpeed < 0) {
        printf("Setting motor to forwards\n");
        analogWrite(speedpinA, 0);
        delay(50);
        digitalWrite(pinI2, LOW);
        digitalWrite(pinI1, HIGH);
      }
      printf("Doing the analog write\n");
      analogWrite(speedpinA, abs(newSpeed));
      rightSpeed = newSpeed;
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

double getDiff() {
    return difference(getDistance('l'), getDistance('r'));
}

void equalPower(int power) {
    setMotor('l', power);
    setMotor('r', power);
    return;
}

char other(char side) {
    if (side == 'l') {
        return 'r';
    } else {
        return 'l';
    }
}

char getSmaller(int l, int r) {
    if (l < r) {
        return 'l';
    } else {
        return 'r';
    }
}

void rotate(char foward, int time) {
    equalPower(0);
    setMotor( foward, minMove);
    setMotor( other(foward), - minMove);
    delay (time);
    equalPower(0);
}

void fixRotation() {
    int lDist = getDistance('l');
    int rDist = getDistance('r');
    printf("Left: %d   Right: %d\n", lDist, rDist);
    char smaller = getSmaller(lDist, rDist);

    //Straighten out.
    int smallerDist = getDistance(smaller);
    int lastSmallerDist;
    printf("Entering first while\n");
    do {
        rotate(smaller, 50);
        lastSmallerDist = smallerDist;
        smallerDist = getDistance(smaller);
    } while (smallerDist <= lastSmallerDist);
    rotate(other(smaller), 50);
    lastSmallerDist = smallerDist;
    printf("Entering second while\n");
    do {
        rotate(other(smaller), 50);
        lastSmallerDist = smallerDist;
        smallerDist = getDistance(smaller);
    } while (smallerDist <= lastSmallerDist);
    rotate(smaller, 50);
}

void centerCar() {
    int lDist = getDistance('l');
    int rDist = getDistance('r');
    double lastDiff, currDiff;
    char smaller = getSmaller(lDist, rDist);

    currDiff = difference(lDist, rDist);
    do {
        setMotor(smaller, round(minMove * 1.25));
        setMotor(other(smaller), minMove);
        delay(100);
        setMotor(smaller, minMove);
        setMotor(other(smaller), round(minMove * 1.25));
        stopCar();
        lastDiff = currDiff;
        currDiff = getDiff();
    } while (lastDiff > currDiff);
}

void driveFoward() {
    printf("Entering drive forward function\n");
    int run = 4;
    int fDist;
    int lDist = getDistance('l');
    int rDist = getDistance('r');
    int lMotor, rMotor, smotor;
    double lastDiff;
    double currDiff = difference(lDist, rDist);
    char smaller;

    //Initial Call
    //  Do things if (and only if) the car is not centered/straight
    if ( currDiff >= 0.15 || lDist > 20 || rDist > 20) {
        //Lets try to correct.
        printf("Running fixRotation\n");
        fixRotation();
        printf("Running centerCar\n");
        centerCar();
    }


    //Main go.
    run = 4;
    startCar();
    do {
        //Initilize variables for use
        fDist = getDistance('f');
        lDist = getDistance('l');
        rDist = getDistance('r');
        lMotor = getMotor('l');
        rMotor = getMotor('r');
        ++run;
        if (run == 5) {
            printf ("Left: %d Right: %d Fowrd: %d -- ML: %d MR: %d", lDist, rDist, fDist, lMotor, rMotor);
            run = 0;
        }
        if (fDist <= 10) {
            printf("Front <= 10, Stopping");
            stopCar();
            return;
        /* Option #1
        } else {
            currDiff = difference(lDist, rDist);
            delay (100);
            lastDiff = currDiff;
            currDiff = getDiff();
            smaller = getSmaller(getDistance('l'), getDistance('r'));
            if (currDiff - lastDiff >= 0.05) {
                
                sdist = getDistance(smaller)
                do {
                    setMotor(smaller, getMotor(smaller) + 1);
                    setMotor(other(smaller), getMotor(other(smaller)) - 1);
                    delay(50);
                }while(sdist > getDistance(smaller));
            } else if (currDiff > 0.20) {
                smaller = getSmaller(lDist, rDist);
                do {
                    smotor = getMotor(smaller);
                    setMotor(smaller, round(getMotor(smaller) * 1.25));
                    delay(50);
                    setMotor(smaller, smotor);
                    smotor = getMotor(other(smaller));
                    setMotor(other(smaller), round(getMotor(other(smaller)) * 1.25));
                    delay(50);
                    setMotor(other(smaller), smotor);
                    lastDiff = currDiff;
                    currDiff = getDiff();
                } while(currDiff < lastDiff);
            }
        }
        */
        /*
        } else {
            smaller= getSmaller(lDist, rDist);
            int sdist = getDistance(smaller);
            delay(50);
            if (sdist > getDistance(smaller)) {
                sdist = getDistance(smaller)
                do {
                    setMotor(smaller, getMotor(smaller) + 1);
                    setMotor(other(smaller), getMotor(other(smaller)) - 1);
                    delay(50);
                }while(sdist > getDistance(smaller));
            }
        }
        */
        }
    } while (lDist != 25 || rDist != 25);
    stopCar();
    printf("Stoped because inf laser reading");
    return;
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
  delay(50);

  digitalWrite(pinI4, LOW);
  digitalWrite(pinI3, HIGH);
  digitalWrite(pinI2, LOW);
  digitalWrite(pinI1, HIGH);
  delay(50);

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
  delay(50);
  digitalWrite(laser2SHDNPin, LOW);
  delay(50);
  laser1.begin(0x84);
  delay(50);

  digitalWrite(laser2SHDNPin, HIGH);
  delay(50);
  laser2.begin(0x86);
  delay(50);
}

void loop() {
    //stopCar();
    int lDist = getDistance('l');
    int rDist = getDistance('r');
    printf("Left: %d   Right: %d\n", lDist, rDist);
    //this while is loop
    //driveFoward();
    setMotor('l', 40);
    setMotor('r', 40);
    printf("Going forwards at 30\n");
    delay(3000);
    setMotor('l', 40);
    setMotor('r', -40);
    printf("Left forward at 30, right backwards at 30\n");
    while(1) {
      delay(5000);
        //Stop doing stuff
    }
}