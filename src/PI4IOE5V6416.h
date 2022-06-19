/*
  PI4IOE5V6416.h
  PI4IOE5V6416 I2C IO expander Header File
  By: John Tito
  Date: 06/19, 2022
  License: BSD 3 Clause
*/

#include "Arduino.h"

#ifndef PI4IOE5V6416_H
#define PI4IOE5V6416_H

// 基类
class I2CDeviceBase {
private: // These private functions are not available to Arduino sketches.
  uint8_t deviceAddress; // I2C Address of the device

public: // These public functions are available to Arduino sketches.;
  I2CDeviceBase();
  uint8_t begin(uint8_t address = 0x40); // Example PI4IOE5V6416 I2C address,
                                         // depends on setting for ADDR
  uint8_t setAll(uint8_t byteAddress, uint8_t value);
  uint8_t getAll(uint8_t byteAddress, uint8_t *buf, uint8_t *count);
  uint8_t setSingle(uint8_t byteAddress, uint8_t pinNumber, uint8_t state);
  uint8_t getSingle(uint8_t byteAddress, uint8_t pinNumber, uint8_t *ret);
};

class PI4IOE5V6416 : private I2CDeviceBase {
private: // These private functions are not available to Arduino sketches.
  static const uint8_t MAX_PORT_COUNT = 2;

  static const uint8_t PI4IOE5V6416_REG_R_INPUTPORT0 = 0x00;
  static const uint8_t PI4IOE5V6416_REG_R_INPUTPORT1 = 0x01;

  static const uint8_t PI4IOE5V6416_REG_RW_OUTPUTPORT0 = 0x02;
  static const uint8_t PI4IOE5V6416_REG_RW_OUTPUTPORT1 = 0x03;

  static const uint8_t PI4IOE5V6416_REG_RW_POLINVPORT0 = 0x04;
  static const uint8_t PI4IOE5V6416_REG_RW_POLINVPORT1 = 0x05;

  // 1: input,0:output
  static const uint8_t PI4IOE5V6416_REG_RW_CONFIGPORT0 = 0x06;
  static const uint8_t PI4IOE5V6416_REG_RW_CONFIGPORT1 = 0x07;

  static const uint8_t PI4IOE5V6416_REG_RW_OUTPUTDRIVESTRENGTH00 = 0x40;
  static const uint8_t PI4IOE5V6416_REG_RW_OUTPUTDRIVESTRENGTH01 = 0x41;
  static const uint8_t PI4IOE5V6416_REG_RW_OUTPUTDRIVESTRENGTH10 = 0x42;
  static const uint8_t PI4IOE5V6416_REG_RW_OUTPUTDRIVESTRENGTH11 = 0x43;

  static const uint8_t PI4IOE5V6416_REG_RW_INPUTLATCHEN0 = 0x44;
  static const uint8_t PI4IOE5V6416_REG_RW_INPUTLATCHEN1 = 0x45;

  // 1: enable,0:disable
  static const uint8_t PI4IOE5V6416_REG_RW_PUPDEN0 = 0x46;
  static const uint8_t PI4IOE5V6416_REG_RW_PUPDEN1 = 0x47;
  // 1: pull-up,0:pull-down
  static const uint8_t PI4IOE5V6416_REG_RW_PUPDSEL0 = 0x48;
  static const uint8_t PI4IOE5V6416_REG_RW_PUPDSEL1 = 0x49;

  static const uint8_t PI4IOE5V6416_REG_RW_INTERRUPTMASK0 = 0X4A;
  static const uint8_t PI4IOE5V6416_REG_RW_INTERRUPTMASK1 = 0x4B;

  static const uint8_t PI4IOE5V6416_REG_R_INTERRUPSTATUS0 = 0X4C;
  static const uint8_t PI4IOE5V6416_REG_R_INTERRUPSTATUS1 = 0x4D;

  static const uint8_t PI4IOE5V6416_REG_RW_OUTPUTPORTCONFIGURATION = 0x4F;

public: // These public functions are available to Arduino sketches.
  PI4IOE5V6416();
  uint8_t begin(uint8_t address);

  uint8_t getPortInputValue(uint8_t port, uint8_t *value);

  uint8_t getPortOutputValue(uint8_t port, uint8_t *value);
  uint8_t setPortOutputValue(uint8_t port, uint8_t value);

  uint8_t getPortInvert(uint8_t port, uint8_t *value);
  uint8_t setPortInvert(uint8_t port, uint8_t value);

  uint8_t getPortDirection(uint8_t port, uint8_t *value);
  uint8_t setPortDirection(uint8_t port, uint8_t value);

  uint8_t getPortOutputDriveStrength(uint8_t port, uint16_t *value);
  uint8_t setPortOutputDriveStrength(uint8_t port, uint16_t value);

  uint8_t getPortInputLatch(uint8_t port, uint8_t *value);
  uint8_t setPortInputLatch(uint8_t port, uint8_t value);

  uint8_t getPortPullUpDownEn(uint8_t port, uint8_t *value);
  uint8_t setPortPullUpDownEn(uint8_t port, uint8_t value);

  uint8_t getPortPullUpDownSel(uint8_t port, uint8_t *value);
  uint8_t setPortPullUpDownSel(uint8_t port, uint8_t value);

  uint8_t getInterruptMask(uint8_t port, uint8_t *value);
  uint8_t setInterruptMask(uint8_t port, uint8_t value);

  uint8_t getInterruptStatus(uint8_t port, uint8_t *value);

  uint8_t getOutputPortConfiguration(uint8_t *value);
  uint8_t setOutputPortConfiguration(uint8_t value);
};     // PI4IOE5V6416 class
#endif // PI4IOE5V6416_H
