/*
  PI4IOE5V6416.cpp
  PI4IOE5V6416 I2C IO expander Source File
  By: John Tito
  Date: 06/19, 2022
  License: BSD 3 Clause
*/

#include "PI4IOE5V6416.h"
#include "Arduino.h"
#include <Wire.h>

// init function, fills port value array with 0xFF
I2CDeviceBase::I2CDeviceBase() {
  // fill state array with HIGH on init, as this is state of chip on power on
  deviceAddress = 0;
}

// communication check, simply checks if chip responds with Ack
uint8_t I2CDeviceBase::begin(uint8_t address) {
  // Store the received parameter into member variables

  deviceAddress = address >> 1;

  Wire.beginTransmission(deviceAddress);
  return Wire.endTransmission();
}

uint8_t I2CDeviceBase::setAll(uint8_t byteAddress, uint8_t value) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(byteAddress);
  Wire.write(value);
  return Wire.endTransmission();
}

uint8_t I2CDeviceBase::getAll(uint8_t byteAddress, uint8_t *buf,
                              uint8_t *count) {

  Wire.beginTransmission(deviceAddress);
  Wire.write(byteAddress);
  if (ESP_OK == Wire.endTransmission(false)) {

    uint8_t _count = 0;

    if (Wire.requestFrom(deviceAddress, 1U, true) > 0) {

      while (Wire.available()) {
        *buf = Wire.read();
        _count++;
      }

      if (NULL != count) {
        *count = _count;
      }
      return 0;
    }
    return 254;
  }
  return 255;
}

uint8_t I2CDeviceBase::setSingle(uint8_t byteAddress, uint8_t pinNumber,
                                 uint8_t state) {
  uint8_t oldValue = 0;
  if (0 == getAll(byteAddress, &oldValue, NULL) && (pinNumber <= 7)) {
    if (state)
      oldValue &= ~(1 << pinNumber);
    else
      oldValue |= (1 << pinNumber);

    return setAll(byteAddress, oldValue);
  } else
    return 255;
}

uint8_t I2CDeviceBase::getSingle(uint8_t byteAddress, uint8_t pinNumber,
                                 uint8_t *ret) {
  uint8_t oldValue = 0;
  if (0 == getAll(byteAddress, &oldValue, NULL) && (pinNumber <= 7)) {
    oldValue &= (1 << pinNumber);
    if (oldValue)
      *ret = 1;
    else
      *ret = 0;

    return 0;
  } else
    return 255;
}

PI4IOE5V6416::PI4IOE5V6416() { I2CDeviceBase(); }

uint8_t PI4IOE5V6416::begin(uint8_t address) {
  if (0x40 != (address & 0xFD))
    return 255;

  return I2CDeviceBase::begin(address);
  // return Wire.endTransmission();
}

uint8_t PI4IOE5V6416::getPortInputValue(uint8_t port, uint8_t *value) {
  if (port < MAX_PORT_COUNT) {
    return getAll(port + PI4IOE5V6416_REG_R_INPUTPORT0, value, NULL);
  }
  return 255;
}

uint8_t PI4IOE5V6416::getPortOutputValue(uint8_t port, uint8_t *value) {
  if (port < MAX_PORT_COUNT) {
    return getAll(port + PI4IOE5V6416_REG_RW_OUTPUTPORT0, value, NULL);
  }
  return 255;
}

uint8_t PI4IOE5V6416::setPortOutputValue(uint8_t port, uint8_t value) {
  if (port < MAX_PORT_COUNT) {
    return setAll(port + PI4IOE5V6416_REG_RW_OUTPUTPORT0, value);
  }
  return 255;
}

uint8_t PI4IOE5V6416::getPortInvert(uint8_t port, uint8_t *value) {
  if (port < MAX_PORT_COUNT) {
    return getAll(port + PI4IOE5V6416_REG_RW_POLINVPORT0, value, NULL);
  }
  return 255;
}

uint8_t PI4IOE5V6416::setPortInvert(uint8_t port, uint8_t value) {
  if (port < MAX_PORT_COUNT) {
    return setAll(port + PI4IOE5V6416_REG_RW_POLINVPORT0, value);
  }
  return 255;
}

uint8_t PI4IOE5V6416::getPortDirection(uint8_t port, uint8_t *value) {
  if (port < MAX_PORT_COUNT) {
    return getAll(port + PI4IOE5V6416_REG_RW_CONFIGPORT0, value, NULL);
  }
  return 255;
}

uint8_t PI4IOE5V6416::setPortDirection(uint8_t port, uint8_t value) {
  if (port < MAX_PORT_COUNT) {
    return setAll(port + PI4IOE5V6416_REG_RW_CONFIGPORT0, value);
  }
  return 255;
}

uint8_t PI4IOE5V6416::getPortOutputDriveStrength(uint8_t port,
                                                 uint16_t *value) {
  uint8_t _value[2];
  if (port < MAX_PORT_COUNT) {
    if (0 == getAll(port + PI4IOE5V6416_REG_RW_OUTPUTDRIVESTRENGTH00,
                    &_value[0], NULL)) {
      if (0 == getAll(port + PI4IOE5V6416_REG_RW_OUTPUTDRIVESTRENGTH01,
                      &_value[1], NULL)) {
        *value = (((uint16_t)((uint16_t)_value[0]) << 8) & (uint16_t)0xff00) |
                 (((uint16_t)_value[1]) & (uint16_t)0x00ff);
        return 0;
      }
    }
  }
  return 255;
}

uint8_t PI4IOE5V6416::setPortOutputDriveStrength(uint8_t port, uint16_t value) {
  uint8_t _value[2];
  if (port < MAX_PORT_COUNT) {
    if (0 == setAll(port * 2 + PI4IOE5V6416_REG_RW_OUTPUTDRIVESTRENGTH00,
                    (uint8_t)(value & 0xff))) {
      if (0 == setAll(port * 2 + PI4IOE5V6416_REG_RW_OUTPUTDRIVESTRENGTH01,
                      (uint8_t)((value >> 8) & 0xff))) {
        return 0;
      }
    }
  }
  return 255;
}

uint8_t PI4IOE5V6416::getPortInputLatch(uint8_t port, uint8_t *value) {
  if (port < MAX_PORT_COUNT) {
    return getAll(port + PI4IOE5V6416_REG_RW_INPUTLATCHEN0, value, NULL);
  }
  return 255;
}

uint8_t PI4IOE5V6416::setPortInputLatch(uint8_t port, uint8_t value) {
  if (port < MAX_PORT_COUNT) {
    return setAll(port + PI4IOE5V6416_REG_RW_INPUTLATCHEN0, value);
  }
  return 255;
}

uint8_t PI4IOE5V6416::getPortPullUpDownEn(uint8_t port, uint8_t *value) {
  if (port < MAX_PORT_COUNT) {
    return getAll(port + PI4IOE5V6416_REG_RW_PUPDEN0, value, NULL);
  }
  return 255;
}

uint8_t PI4IOE5V6416::setPortPullUpDownEn(uint8_t port, uint8_t value) {
  if (port < MAX_PORT_COUNT) {
    return setAll(port + PI4IOE5V6416_REG_RW_PUPDEN0, value);
  }
  return 255;
}

uint8_t PI4IOE5V6416::getPortPullUpDownSel(uint8_t port, uint8_t *value) {
  if (port < MAX_PORT_COUNT) {
    return getAll(port + PI4IOE5V6416_REG_RW_PUPDSEL0, value, NULL);
  }
  return 255;
}

uint8_t PI4IOE5V6416::setPortPullUpDownSel(uint8_t port, uint8_t value) {
  if (port < MAX_PORT_COUNT) {
    return setAll(port + PI4IOE5V6416_REG_RW_PUPDSEL0, value);
  }
  return 255;
}

uint8_t PI4IOE5V6416::getInterruptMask(uint8_t port, uint8_t *value) {
  if (port < MAX_PORT_COUNT) {
    return getAll(port + PI4IOE5V6416_REG_RW_INTERRUPTMASK0, value, NULL);
  }
  return 255;
}

uint8_t PI4IOE5V6416::setInterruptMask(uint8_t port, uint8_t value) {
  if (port < MAX_PORT_COUNT) {
    return setAll(port + PI4IOE5V6416_REG_RW_INTERRUPTMASK0, value);
  }
  return 255;
}

uint8_t PI4IOE5V6416::getInterruptStatus(uint8_t port, uint8_t *value) {
  if (port < MAX_PORT_COUNT) {
    return getAll(port + PI4IOE5V6416_REG_R_INTERRUPSTATUS0, value, NULL);
  }
  return 255;
}

uint8_t PI4IOE5V6416::getOutputPortConfiguration(uint8_t *value) {
  return getAll(PI4IOE5V6416_REG_RW_OUTPUTPORTCONFIGURATION, value, NULL);
}

uint8_t PI4IOE5V6416::setOutputPortConfiguration(uint8_t value) {
  return setAll(PI4IOE5V6416_REG_RW_OUTPUTPORTCONFIGURATION, value);
}
