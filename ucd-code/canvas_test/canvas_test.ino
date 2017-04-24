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

  Adafruit_VL6180X laser1;
  byte laser1GPIOPin = 3;
  byte laser1SHDNPin = 4;

  Adafruit_VL6180X laser2;
  byte laser2SHDNPin = 5;

void setup() {
  printf("starting");
  
  Wire.begin();

  // Initialize laser 1
  digitalWrite(laser2SHDNPin, LOW);
  digitalWrite(laser1SHDNPin, HIGH);
  laser1.begin(0x80);

  // Initialize laser 2
  digitalWrite(laser1SHDNPin, LOW);
  digitalWrite(laser2SHDNPin, HIGH);
  laser2.begin(0x82);

  // Turn all lasers on
  digitalWrite(laser1SHDNPin, HIGH);

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
  printf("looping \n");
  delay(1);
  printf("laser1: %d\n", laser1.readRange());
  printf("laser2: %d\n", laser2.readRange());
  delay(1500);
}
