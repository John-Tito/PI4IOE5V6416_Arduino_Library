/*
  Basic demo for PI4IOE5V6416 I2C IO expander
  By: John Tito
  Date: 06/19, 2022
  License: BSD 3 Clause
  This example demonstrates how to set and read single pins and sets of pins on
  the PI4IOE5V6416 Note that this chip requires all pins to be set every time
  you write something, so the write pin function is only useful for
  comprehensability. Effeciency wise it's faster to set all pins.

*/
#include "PI4IOE5V6416.h"
#include <Arduino.h>
#include <Wire.h>
#include <pins_arduino.h>

#define I2C_DEV_ADDR 0x55

uint32_t i = 0;
PI4IOE5V6416 ioExpand;

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(true);

  Wire.begin(26, 27, 400000U);
  uint8_t res = ioExpand.begin(0x40);
  if (0 != res) {
    Serial.printf("err when init iic:%d,\n", res);
    while (1) {
      delay(5000);
    }
  }
  res |= ioExpand.setOutputPortConfiguration(0x03);

  res |= ioExpand.setPortOutputDriveStrength(0, 0x00ff);
  res |= ioExpand.setPortOutputDriveStrength(1, 0x0000);

  res |= ioExpand.setPortInvert(0, 0x00);
  res |= ioExpand.setPortInvert(1, 0x00);

  res |= ioExpand.setInterruptMask(0, 0x00);
  res |= ioExpand.setInterruptMask(1, 0x00);

  res |= ioExpand.setPortInputLatch(0, 0x00);
  res |= ioExpand.setPortInputLatch(1, 0x00);

  res |= ioExpand.setPortPullUpDownEn(0, 0x0F);
  res |= ioExpand.setPortPullUpDownEn(1, 0x00);

  res |= ioExpand.setPortPullUpDownSel(0, 0x0f);
  res |= ioExpand.setPortPullUpDownSel(1, 0x00);

  res |= ioExpand.setPortDirection(0, 0xF0);
  res |= ioExpand.setPortDirection(1, 0xFF);

  res |= ioExpand.setPortOutputValue(0, 0xFF);
  res |= ioExpand.setPortOutputValue(1, 0xFF);

  if (0 != res) {
    Serial.printf("err when init iic:%d,\n", res);
    while (1) {
      delay(5000);
    }
  }

  Serial.printf(" init done,\n");
}

uint8_t value = 0, count = 0;
void loop() {
  while (1) {
    delay(1000);
    uint8_t res = ioExpand.getPortInputValue(0, &value);
    res |= ioExpand.setPortOutputValue(0, (count++) & 0x0f);

    if (0 == res) {
      Serial.printf("input value:%d,%d\n", count, value);
    } else {
      Serial.printf("read err:%d\n", res);
    }
  }
}
