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
  byte laser2SHDNPin = 6;

/*******************************************************************************
* Function name : DetectVL6180X
* Description : DetectVL6180X
* Input : I2CDevAddr
* Output : None
* Return : Failure
*******************************************************************************/
u8 DetectVL6180X (u8 I2CDevAddr)
{
  u8 i, DeviceID;
  for (i=0; i<10; i++)
  {
    DeviceID = I2C_VL6180XReadByte(I2CDevAddr, 0x00);
    Delay(20);
    if (DeviceID == 0xB4)
      {
      return 0;
      }
  }
  return 1;
}

/*******************************************************************************
* Function name : ChangeVL6180XI2CAddr
* Description : ChangeVL6180XI2CAddr
* Input : CurrentI2CDevAddr, NewI2CDevAddr
* Output : None
* Return : Failure
*******************************************************************************/
u8 ChangeVL6180XI2CAddr (u8 CurrentI2CDevAddr, u8 NewI2CDevAddr)
{
  u8 Failure;
  Failure = I2C_VL6180XByteWrite (CurrentI2CDevAddr, 0x0212, NewI2CDevAddr / 2);
  if (Failure != 0x00)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}


void setup() {
  printf("starting");
  
  Wire.begin();
  laser1.begin(0x29);
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
  printf("read device test: %d", 0x52)
  delay(1);
  printf("laser1: %d\n", lasers.readRange());
  delay(500);
}
