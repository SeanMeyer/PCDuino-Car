#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X vl = Adafruit_VL6180X();

void setup() {  
  printf("Adafruit VL6180x test! \n");
  if (! vl.begin()) {
    printf("Failed to find sensor \n");
    while (1);
  }
  printf("Sensor found! \n");
}

void loop() {
  float lux = vl.readLux(VL6180X_ALS_GAIN_5);

  printf("Lux: "); printf("%E", lux);
  
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    printf("Range: "); printf("%d \n", range);
  }

  // Some error occurred, print it out!
  
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    printf("System error \n");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    printf("ECE failure \n");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    printf("No convergence \n");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    printf("Ignoring range \n");
  }
  else if (status == VL6180X_ERROR_SNR) {
    printf("Signal/Noise error \n");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    printf("Raw reading underflow \n");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    printf("Raw reading overflow \n");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    printf("Range reading underflow \n");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    printf("Range reading overflow \n");
  }
  delay(50);
}
