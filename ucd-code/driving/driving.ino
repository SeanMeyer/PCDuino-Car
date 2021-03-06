extern "C" {
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <inttypes.h>
}
#include "Adafruit_VL6180X.h"
#include <core.h>
#include <math.h>
#include <iostream>
#include <string>

const int trig = 3;
const int echo = 4;

#define pinI1 8   // pins 1 + 2 is the right motor. 1 low, 2 high is forwards.
#define pinI2 11
#define pinI3 12  // pins 3 + 4 is the left motor.  3 low, 4 high is forwards.
#define pinI4 13
#define speedpinA 9
#define speedpinB 10

char fromPython[100];
char directions[] = {'l','f','f','f','r', 'f','f', 'l','f','r','f', 'f', 'l', 'r', 'f'};


int speed = 29;
int leftSpeed = speed;
int rightSpeed = speed;
int minMove = 27;
int globalTurnTime = 0;
int globalForwardTime = 0;

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
      //printf("Setting left motor\n");
      if (newSpeed < 0 && (leftSpeed > 0 || leftSpeed == 0)) {
        //printf("Setting motor to backwards\n");
        analogWrite(speedpinB, 0);
        delay(5);
        digitalWrite(pinI4, HIGH);
        digitalWrite(pinI3, LOW);
      } else if (newSpeed > 0 && (leftSpeed < 0 || leftSpeed == 0)) {
        //printf("Setting motor to forwards\n");
        analogWrite(speedpinB, 0);
        delay(5);
        digitalWrite(pinI4, LOW);
        digitalWrite(pinI3, HIGH);
      }
      //printf("Doing the analog write\n");
      analogWrite(speedpinB, abs(newSpeed));
      leftSpeed = newSpeed;
      return;
    case 'R':
    case 'r':
      //printf("Setting right motor\n");
      if (newSpeed < 0 && (rightSpeed > 0 || rightSpeed == 0)) {
        //printf("Setting motor to backwards\n");
        analogWrite(speedpinA, 0);
        delay(5);
        digitalWrite(pinI2, HIGH);
        digitalWrite(pinI1, LOW);
      } else if (newSpeed > 0 && (rightSpeed < 0 || rightSpeed == 0)) {
        //printf("Setting motor to forwards\n");
        analogWrite(speedpinA, 0);
        delay(5);
        digitalWrite(pinI2, LOW);
        digitalWrite(pinI1, HIGH);
      }
      //printf("Doing the analog write\n");
      analogWrite(speedpinA, abs(newSpeed));
      rightSpeed = newSpeed;
      return;
  }
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

long microsecondsToMillimeters(long microseconds)
{
  return (microseconds / 2.9411 / 2);
}

int getUltraDistance(void)
{
  long duration;
  int mm;
  digitalWrite(trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH, 1000000);
  mm = microsecondsToCentimeters(duration);
  return mm;
}


int getDistance(char laser)
{
  int mm;
  int reading[3];
  switch(laser) {
    case 'L':
    case 'l':
      reading[0] = laser2.readRange();
      delay(5);
      reading[1] = laser2.readRange();
      delay(5);
      reading[2] = laser2.readRange();
      return ( round(reading[0] + reading[1] + reading[2]) / 3) / 10;
      break;
    case 'R':
    case 'r':
      reading[0] = laser1.readRange();
      delay(5);
      reading[1] = laser1.readRange();
      delay(5);
      reading[2] = laser1.readRange();
      return ( round(reading[0] + reading[1] + reading[2]) / 3) / 10;
      break;
    case 'F':
    case 'f':
      return getUltraDistance();
  }
}
int getDistance2(char laser)
{
  int mm;
  switch(laser) {
    case 'L':
    case 'l':
      return (round(laser2.readRange() / 10));
      break;
    case 'R':
    case 'r':
      return ( round(laser1.readRange() / 10));
      break;
    case 'F':
    case 'f':
      return getUltraDistance();
  }
  return 0;
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

void rotate(char forward, int time) {
    setMotor( forward, minMove);
    setMotor( other(forward), -minMove);
    delay (time);
}

bool shouldRun(int lDist, int rDist) {
  if (lDist != 25 || rDist != 25) {
    return true;
  } else {
    return false;
  }
}

void performTurn(char direction) {
    int turnTime = 575;
    int forwardTime = 800;
    switch(direction) {
      case 'L':
      case 'l':
        rotate('r', turnTime);
        break;
      case 'R':
      case 'r':
        rotate('l', turnTime);
        break;
      case 'F':
      case 'f':
       break;
    }
    delay(50);
    equalPower(speed);
    delay(forwardTime);
    equalPower(0);
}

void fixRotation() {
    int lDist = getDistance('l');
    int rDist = getDistance('r');
    int fDist;
    //printf("Left: %d   Right: %d\n", lDist, rDist);
    char smaller = getSmaller(lDist, rDist);
    int count = 0;

    //Straighten out.
    int smallerDist = getDistance(smaller);
    int lastSmallerDist;
    int rotationDelay = 40;
    printf("Attempting to rotate\n");
    printf("     Left: %d, Right: %d, smaller: %c\n", lDist, rDist, smaller);
    do {
        count = count + 1;
        rotate(smaller, rotationDelay);
        lastSmallerDist = smallerDist;
        smallerDist = getDistance(smaller);
        fDist = getDistance('f');
        printf("         %cDist: %d, last-%cDist: %d, fDist: %d\n", smaller, smallerDist, smaller, lastSmallerDist, fDist);
    } while ((smallerDist <= lastSmallerDist || fDist < 5) && count < 45);
    equalPower(0);
    delay(20);
    if (getDistance('l') == 25 || getDistance('r') == 25) {
      printf("infinite rotate distance, moving forwards a bit\n");
      equalPower(speed);
      delay(250);
      fixRotation();
    }
    //rotate(other(smaller), rotationDelay * 2);
    //equalPower(0);
    /*
    lastSmallerDist = smallerDist;
    printf("Entering second while\n");
    do {
        rotate(other(smaller), rotationDelay);
        lastSmallerDist = smallerDist;
        smallerDist = getDistance(smaller);
    } while (smallerDist <= lastSmallerDist);
    rotate(smaller, rotationDelay);
    equalPower(0);
    */
}

void centerCar() {
    int lDist = getDistance('l');
    int rDist = getDistance('r');
    double lastDiff, currDiff;
    char smaller = getSmaller(lDist, rDist);

    printf("In centerCar.\n");
    currDiff = difference(lDist, rDist);
    if (currDiff > 0.30) {
      printf("      Car needs to be centered.\n");
      //do {
          setMotor(smaller, round(minMove * 1.25));
          setMotor(other(smaller), minMove);
          delay(500);
          setMotor(smaller, minMove);
          setMotor(other(smaller), round(minMove * 1.25));
          delay(300);
          lastDiff = currDiff;
          currDiff = getDiff();
      //} while (lastDiff > currDiff);
      printf("      Done centering car.\n");
      equalPower(0);
    }
    printf("Exit center car.\n");
}

void driveFoward() {
    printf("Entering drive forward function\n");
    int run = 0;
    int fDist;
    int lDist = getDistance('l');
    int rDist = getDistance('r');
    int lMotor, rMotor, smotor;
    double lastDiff = 0;
    double currDiff = difference(lDist, rDist);
    char smaller;

    //Initial Call
    //  Do things if (and only if) the car is not centered/straight
    if ( currDiff >= 0.30 || lDist > 20 || rDist > 20) {
        //Lets try to correct.
        printf("Running fixRotation\n");
        fixRotation();
        /* centerCar() is untested. Don't run it ever.'
        if ( getDiff() > 0.50 ) {
            printf("Running centerCar\n");
            centerCar();
        }
        */
    }


    //Main go.
    printf("Attempting to drive foward.\n");
    equalPower(0);
    delay(50);
    equalPower(speed);
    printf("Go.\n");
    printf ("Left: %d Right: %d Fowrd: %d -- ML: %d MR: %d \n\n", lDist, rDist, fDist, lMotor, rMotor);
    int avgRuns = 4;
    double diffs[avgRuns];  //Save the difference each run
    int lDists[avgRuns];    //Save lDist every run
    int rDists[avgRuns];    //..
    int fDists[avgRuns];    //..
    bool recentAdjustment = false;
    int numAvgRuns = 0;
    int adjustmentMagic[2] = {10, 50};  //Multiply difference by, 2 for difference differences
    double lastDriftAmount = 0;
    double driftChange = 0;
    do {
        //Initilize variables for use
        fDist = getDistance('f');
        lDist = getDistance('l');
        rDist = getDistance('r');
        lMotor = getMotor('l');
        rMotor = getMotor('r');

        diffs[run] = difference(lDist, rDist);
        lDists[run] = lDist;
        rDists[run] = rDist;
        fDists[run] = fDist;

        if (run == avgRuns - 1) {
            int i;        //use for for loops
            
            //Get an average of the front readings
            int favg = 0; //front reading averaged.
            for (i = 0; i < avgRuns; i++) {
                favg = favg + fDists[i];
            }
            favg = round(favg / avgRuns);
            if (favg <= 20) {   //if the front readings are low, stop
                printf("Front <= 20, Stopping. its %d \n", favg);
                stopCar();
                return;
            } else {             //Nothing in front, lets go.
                run = 0;
                printf ("\n Left: %d Right: %d Fowrd: %d -- ML: %d MR: %d \n", lDist, rDist, favg, lMotor, rMotor);
                //If the difference is trending upwords (car is getting worse)
                double x = 0;   //Average of the first 1/2 of the differences
                double y = 0;   //Average of the 2nd 1/2 of the differences
                for (i = 0; i < avgRuns / 2; i++) {
                    x = x + diffs[i];
                }
                for (i = avgRuns / 2; i < avgRuns; i++) {
                    y = y + diffs[i];
                }
                x = x / (avgRuns / 2);
                y = y / (avgRuns / 2);
                printf("----Drift Amount: %f \n", (y - x));
                if (lDists[avgRuns-1] < 5 || rDists[avgRuns-1] < 5) {          //If the lasers are very off, run fixrotate
                    printf("One of the lasers is to small -----Run fixrotate \n");
                    equalPower(0);
                    delay(20);
                    equalPower(-speed);
                    delay(350);
                    equalPower(0);
                    delay(20);
                    fixRotation();
                    run = 0;
                    recentAdjustment = false;
                    if (getDistance('l') < 5 || getDistance('r') < 5) {
                      printf("Rotate didn't fix things... Center car? \n");
                      //centerCar();
                    }
                    equalPower(speed);
                } else if (y - x > 0.03 && !recentAdjustment && ( (y - x) > lastDriftAmount)) {
                    printf("--Fixing Drift %f -> %f \n", x, y);
                    smaller = getSmaller( ((lDists[(avgRuns - 2)] + lDists[(avgRuns - 1)]) / 2), ((rDists[(avgRuns - 2)] + rDists[(avgRuns - 1)]) /2));
                    printf("---Increase %c by %d \n", smaller, (int) ceil(adjustmentMagic[0] * y));
                    if (y - x > 0.300)
                        setMotor(smaller, getMotor(smaller) + ceil(adjustmentMagic[1] * y));
                    else
                        setMotor(smaller, getMotor(smaller) + 3);
                    recentAdjustment = true;
                    numAvgRuns = 0;
                } else if (recentAdjustment) { //&& numAvgRuns < 1) {
                  if (y < 0.15 || (y-x) > (lastDriftAmount + .1)) {
                    printf("UNDO ADJUSTMENT. \n");
                    recentAdjustment = false;
                    equalPower(speed);
                  } else if (abs( (y-x) - lastDriftAmount) > driftChange) {
                    printf("ADJUSTMENT NOT WORKING. \n");
                    equalPower(0);
                    delay(20);
                    fixRotation();
                    delay(20);
                    recentAdjustment = false;
                    equalPower(speed);
                  }
                  numAvgRuns++;
                  //recentAdjustment = false;
                  //equalPower(speed);
                } /*else if (recentAdjustment) {
                  recentAdjustment = false;
                  equalPower(speed);
                }*/
                driftChange = abs( (y-x) - lastDriftAmount); 
                lastDriftAmount = y - x;
            }
            run = -1;
        }
        run++;
        delay(2);
    } while (lDist != 25 && rDist != 25);
    stopCar();
    printf("Stoped because inf laser reading. \n");
    return;
}

void performAction(char action) {
    switch(action) {
      case 'L':
      case 'l':
        performTurn('l');
        break;
      case 'R':
      case 'r':
        performTurn('r');
        break;
      case 'F':
      case 'f':
        fixRotation();
        performTurn('f');
        delay(25);
        driveFoward();
        break;
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
  stopCar();
  //start by setting minspeed and speed
  std::cout << "Enter minMove (27): ";
  std::cin >> minMove;
  std::cin.clear();
  std::cin.ignore(256, '\n');
  std::cout << "Enter speed (29): ";
  std::cin >> speed;
  std::cin.clear();
  std::cin.ignore(256, '\n');


  //This is for getting output from python
  FILE *p = popen("python \"/home/linaro/PCDuino-Car/MapCreation-Search code/driver.py\"", "r");
  if (p != NULL) {
      while(fgets(fromPython, sizeof(fromPython), p) != NULL) {
          printf("From Python: %s \n", fromPython);
      }
  }
}

void loop() {
    stopCar();

    char userInput;
    std::cout << "Enter command character (l, r, f): ";
    std::cin >> userInput;
    std::cin.clear();
    std::cin.ignore(256, '\n');
    /*
    int turnTime, forwardTime;
    std::cout << "Enter turn time: ";
    std::cin >> turnTime;
    globalTurnTime = turnTime;
    std::cin.clear();
    std::cin.ignore(256, '\n');
    std::cout << "Enter forward time: ";
    std::cin >> forwardTime;
    globalForwardTime = forwardTime;
    std::cin.clear();
    std::cin.ignore(256, '\n');
    */
    //performAction(userInput);
    //driveFoward();
    //performTurn('l');


    /*
    char * pch;
    pch = strtok(fromPython, ",");
    while (pch != NULL) {
      printf("Doing: %s from python.", pch);
      performAction(*pch);
    }
    */
    int dirLength = sizeof(directions) / sizeof(directions[0]);
    int leftsDoneCount = 0;
    for(int i = 0; i < dirLength; i++) {
      printf("Next will be %c \n", directions[i]);
      //std::cin >> userInput;
      delay(50);
      if (getDistance('l') == 25 && leftsDoneCount < 3) {
        performAction('l');
        leftsDoneCount = leftsDoneCount + 1;
      } else {
        performAction('f');
      }
      delay(200);
    }
}