/**
 * @file spi_driver.h
 * @brief Driver API for SPI peripheral
 * @details Bare metal driver for STM32F407
 */

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <stdint.h>

// Function prototypes
void SPI_Init(uint32_t config);
int SPI_Transmit(uint8_t *data, uint16_t length);
int SPI_Receive(uint8_t *data, uint16_t length);

#endif // SPI_DRIVER_H
