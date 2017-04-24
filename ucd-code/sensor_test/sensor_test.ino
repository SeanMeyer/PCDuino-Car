extern "C" {
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <inttypes.h>
}
#include "Adafruit_VL6180X.h"

  Adafruit_VL6180X laser1;
  byte laser1SHDNPin = 1;

  Adafruit_VL6180X laser2;
  byte laser2SHDNPin = 2;

void setup() {
  printf("starting");
  
  pinMode(laser1SHDNPin, OUTPUT);
  pinMode(laser2SHDNPin, OUTPUT);
  
  Wire.begin();

  // Initialize laser 1
  digitalWrite(laser1SHDNPin, HIGH);
  digitalWrite(laser2SHDNPin, LOW);
  laser1.begin(0x84);

  // Initialize laser 2
  digitalWrite(laser2SHDNPin, HIGH);
  laser2.begin(0x86);
}

void loop() {
  printf("looping \n");
  printf("laser1: %d\n", laser1.readRange());
  printf("laster2: %d\n", laser2.readRange());
  delay(500);
}