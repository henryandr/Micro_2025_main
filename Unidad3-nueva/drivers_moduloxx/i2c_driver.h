/**
 * @file i2c_driver.h
 * @brief Driver API for I2C peripheral
 * @details Bare metal driver for STM32F407
 */

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>

// Function prototypes
void I2C_Init(uint32_t config);
int I2C_Transmit(uint8_t *data, uint16_t length);
int I2C_Receive(uint8_t *data, uint16_t length);

#endif // I2C_DRIVER_H
