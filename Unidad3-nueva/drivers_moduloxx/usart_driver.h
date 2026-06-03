/**
 * @file usart_driver.h
 * @brief Driver API for USART peripheral
 * @details Bare metal driver for STM32F407
 */

#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include <stdint.h>

// Function prototypes
void USART_Init(uint32_t config);
int USART_Transmit(uint8_t *data, uint16_t length);
int USART_Receive(uint8_t *data, uint16_t length);

#endif // USART_DRIVER_H
