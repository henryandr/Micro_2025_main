/**
 * @file gpio_driver.c
 * @brief Implementación del driver GPIO
 * @author Curso Microprocesadores
 * @date 2025
 */

#include "gpio_driver.h"

/* RCC - Reset and Clock Control */
#define RCC_AHB1ENR (*((volatile uint32_t*)0x40023830))

/* ========== Funciones Públicas ========== */

void GPIO_ClockEnable(GPIO_TypeDef *port) {
    if (port == GPIOA)
        RCC_AHB1ENR |= (1 << 0);
    else if (port == GPIOB)
        RCC_AHB1ENR |= (1 << 1);
    else if (port == GPIOC)
        RCC_AHB1ENR |= (1 << 2);
    else if (port == GPIOD)
        RCC_AHB1ENR |= (1 << 3);
    else if (port == GPIOE)
        RCC_AHB1ENR |= (1 << 4);
}

void GPIO_SetMode(GPIO_TypeDef *port, uint8_t pin, GPIO_Mode_t mode) {
    /* Limpiar bits del pin (2 bits por pin) */
    port->MODER &= ~(0x3 << (pin * 2));
    /* Establecer nuevo modo */
    port->MODER |= (mode << (pin * 2));
}

void GPIO_SetOutputType(GPIO_TypeDef *port, uint8_t pin, GPIO_OType_t otype) {
    if (otype == GPIO_OTYPE_OD)
        port->OTYPER |= (1 << pin);
    else
        port->OTYPER &= ~(1 << pin);
}

void GPIO_SetSpeed(GPIO_TypeDef *port, uint8_t pin, GPIO_Speed_t speed) {
    /* Limpiar bits */
    port->OSPEEDR &= ~(0x3 << (pin * 2));
    /* Establecer velocidad */
    port->OSPEEDR |= (speed << (pin * 2));
}

void GPIO_SetPullUpDown(GPIO_TypeDef *port, uint8_t pin, GPIO_PUPD_t pupd) {
    /* Limpiar bits */
    port->PUPDR &= ~(0x3 << (pin * 2));
    /* Establecer pull-up/pull-down */
    port->PUPDR |= (pupd << (pin * 2));
}

void GPIO_Init(GPIO_Config_t *config) {
    /* Habilitar clock del puerto */
    GPIO_ClockEnable(config->port);
    
    /* Configurar modo */
    GPIO_SetMode(config->port, config->pin, config->mode);
    
    /* Si es salida, configurar parámetros adicionales */
    if (config->mode == GPIO_MODE_OUTPUT) {
        GPIO_SetOutputType(config->port, config->pin, config->otype);
        GPIO_SetSpeed(config->port, config->pin, config->speed);
    }
    
    /* Configurar pull-up/pull-down */
    GPIO_SetPullUpDown(config->port, config->pin, config->pupd);
}

void GPIO_Write(GPIO_TypeDef *port, uint8_t pin, uint8_t value) {
    if (value)
        port->BSRR = (1 << pin);         /* Set bit */
    else
        port->BSRR = (1 << (pin + 16));  /* Reset bit */
}

uint8_t GPIO_Read(GPIO_TypeDef *port, uint8_t pin) {
    return (port->IDR & (1 << pin)) ? 1 : 0;
}

void GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin) {
    port->ODR ^= (1 << pin);
}

void GPIO_SetAlternateFunction(GPIO_TypeDef *port, uint8_t pin, uint8_t af_num) {
    /* AFR[0] para pines 0-7, AFR[1] para pines 8-15 */
    uint8_t reg_index = pin / 8;        /* 0 o 1 */
    uint8_t bit_position = (pin % 8) * 4;  /* Posición dentro del registro */
    
    /* Limpiar bits de la función alternativa */
    port->AFR[reg_index] &= ~(0xF << bit_position);
    /* Establecer nueva función alternativa */
    port->AFR[reg_index] |= (af_num << bit_position);
}
