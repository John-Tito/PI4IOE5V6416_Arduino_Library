/*
  Basic demo for PI4IOE5V96248 I2C IO expander
  By: Ilia Baranov
  BLueprint Foundry
  Date: March 5th, 2021
  License: BSD 3 Clause
  This example demonstrates how to set and read single pins and sets of pins on the PI4IOE5V96248
  Note that this chip requires all pins to be set every time you write something, so the write pin
  function is only useful for comprehensability. Effeciency wise it's faster to set all pins.

*/
#include <Wire.h>
#include "PI4IOE5V96248.h"

PI4IOE5V96248 io_exp; // Object for communicating with the io expander
const byte PI4IOE5V96248_ADDRESS = 0x23;  // Example PI4IOE5V96248 I2C address, depends on setting for AD0, AD1, AD2

//Example helper function to print out chip stored pin states
void printHex(uint8_t num) {
  char hexCar[2];

  sprintf(hexCar, "%02X", num);
  Serial.print(" 0x");
  Serial.print(hexCar);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(25, 23); //Join I2C bus, set SDA and SCL pins respectively (tested on ESP32, remove pins if using another chip)
  Wire.setClock(400000); //Set speed to 400Khz, chip supports up to 1Mhz

  delay(100); //give system chance to stabilize, likely not needed.

  // Call io_exp.begin(<address>) to initialize the PI4IOE5V96248. Successfully communication = return 1.
  // NOTE: Requires Wire.begin to already have been called.
  if (!io_exp.begin(PI4IOE5V96248_ADDRESS))
  {
    Serial.println("Failed to init PI4IOE5V96248 :(");
    while (1); //loop forever
  }
  Serial.println("PI4IOE5V96248 found! :)");
  Serial.print("Default pin values: ");
  byte * stored = io_exp.returnValue();
  for (int i = 0; i < sizeof(stored); i++) { //example printing initialized 0xFF values
    printHex(stored[i]);
  }
}

void loop() {
  delay(500);
  io_exp.writePin(0, 7, LOW);
  delay(500);
  io_exp.writePin(0, 7, HIGH); // set a single pin on port 0 (pin 5 numerically on the chip)
  delay(500);
  io_exp.writePort(1, 0x00); //set entire port 1 to 0 (pins 6 - 13 numerically on the chip)
}
