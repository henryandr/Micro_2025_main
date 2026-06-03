/**
 * @file can_driver.h
 * @brief Driver API for CAN peripheral
 * @details Bare metal driver for STM32F407
 */

#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <stdint.h>

// Function prototypes
void CAN_Init(uint32_t config);
int CAN_Transmit(uint8_t *data, uint16_t length);
int CAN_Receive(uint8_t *data, uint16_t length);

#endif // CAN_DRIVER_H
