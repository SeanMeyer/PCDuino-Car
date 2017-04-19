/* Multiple laser procedure:
 *  1. Wire SHDN (SHutDowN) pins.
 *  2. Disable ALL lasers. (write SHDN pin > LOW)
 *  3. Enable 1 laser. (write SHDN pin > HIGH & call laser.begin())
 *  4. Read value.
 */
/* Note: 
 *  For Arduino Uno, SDA = A5 & SCL = A4
 *      for other boards, see: https://www.arduino.cc/en/Reference/Wire
 */
extern "C" {
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <inttypes.h>
}
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X lasers;
  byte laser1SHDNPin = 7; // SHutDowN pin
  byte laser2SHDNPin = 6;

void setup() {
  printf("starting");
  
  Wire.begin();
  /*
   // Doesn't work
  digitalWrite(laser2SHDNPin, LOW);  // Disable laser2 before...
  digitalWrite(laser1SHDNPin, HIGH); // enabling laser1
  /*
  lasers.begin();
  digitalWrite(laser1SHDNPin, LOW); // enabling laser1
  digitalWrite(laser2SHDNPin, HIGH);  // Disable laser2 before...
  lasers.begin();
  */
}

void loop() {
  printf("\tlooping");
  lasers.begin();
  delay(1);
  printf("laser1: %d\n", lasers.readRange());
  delay(500);
}
