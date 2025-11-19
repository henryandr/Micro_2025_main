/**
 * @file gpio_driver.h
 * @brief Driver básico para GPIO en STM32F407
 * @author Curso Microprocesadores
 * @date 2025
 */

#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>

/* Estructura GPIO */
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

/* Punteros a puertos GPIO */
#define GPIOA ((GPIO_TypeDef*)0x40020000)
#define GPIOB ((GPIO_TypeDef*)0x40020400)
#define GPIOC ((GPIO_TypeDef*)0x40020800)
#define GPIOD ((GPIO_TypeDef*)0x40020C00)
#define GPIOE ((GPIO_TypeDef*)0x40021000)

/* Modos de GPIO */
#define GPIO_MODE_INPUT     0x00
#define GPIO_MODE_OUTPUT    0x01
#define GPIO_MODE_AF        0x02
#define GPIO_MODE_ANALOG    0x03

/* Tipos de salida */
#define GPIO_OTYPE_PP       0  /* Push-pull */
#define GPIO_OTYPE_OD       1  /* Open-drain */

/* Velocidades */
#define GPIO_SPEED_LOW      0x00
#define GPIO_SPEED_MEDIUM   0x01
#define GPIO_SPEED_FAST     0x02
#define GPIO_SPEED_HIGH     0x03

/* Pull-up/Pull-down */
#define GPIO_PUPD_NONE      0x00
#define GPIO_PUPD_PU        0x01
#define GPIO_PUPD_PD        0x02

/* Funciones públicas */
void GPIO_ClockEnable(GPIO_TypeDef *port);
void GPIO_Init(GPIO_TypeDef *port, uint8_t pin, uint8_t mode);
void GPIO_Write(GPIO_TypeDef *port, uint8_t pin, uint8_t value);
uint8_t GPIO_Read(GPIO_TypeDef *port, uint8_t pin);
void GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin);
void GPIO_SetMode(GPIO_TypeDef *port, uint8_t pin, uint8_t mode);
void GPIO_SetOutputType(GPIO_TypeDef *port, uint8_t pin, uint8_t type);
void GPIO_SetSpeed(GPIO_TypeDef *port, uint8_t pin, uint8_t speed);
void GPIO_SetPullUpDown(GPIO_TypeDef *port, uint8_t pin, uint8_t pupd);

#endif /* GPIO_DRIVER_H */
