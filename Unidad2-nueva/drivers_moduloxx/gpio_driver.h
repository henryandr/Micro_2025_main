/**
 * @file gpio_driver.h
 * @brief Driver GPIO completo para STM32F407
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Driver modular para control de GPIO en C bare metal.
 *   Proporciona API de alto nivel para configuración y control de pines.
 */

#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>

/* ========== Definiciones de Hardware ========== */

/* Estructura GPIO */
typedef struct {
    volatile uint32_t MODER;     /* 0x00: Mode register */
    volatile uint32_t OTYPER;    /* 0x04: Output type register */
    volatile uint32_t OSPEEDR;   /* 0x08: Output speed register */
    volatile uint32_t PUPDR;     /* 0x0C: Pull-up/pull-down register */
    volatile uint32_t IDR;       /* 0x10: Input data register */
    volatile uint32_t ODR;       /* 0x14: Output data register */
    volatile uint32_t BSRR;      /* 0x18: Bit set/reset register */
    volatile uint32_t LCKR;      /* 0x1C: Lock register */
    volatile uint32_t AFR[2];    /* 0x20-0x24: Alternate function registers */
} GPIO_TypeDef;

/* Punteros a puertos GPIO */
#define GPIOA ((GPIO_TypeDef*)0x40020000)
#define GPIOB ((GPIO_TypeDef*)0x40020400)
#define GPIOC ((GPIO_TypeDef*)0x40020800)
#define GPIOD ((GPIO_TypeDef*)0x40020C00)
#define GPIOE ((GPIO_TypeDef*)0x40021000)

/* ========== Enumeraciones y Tipos ========== */

/* Modos de GPIO */
typedef enum {
    GPIO_MODE_INPUT  = 0,   /* Entrada */
    GPIO_MODE_OUTPUT = 1,   /* Salida */
    GPIO_MODE_AF     = 2,   /* Función alternativa */
    GPIO_MODE_ANALOG = 3    /* Modo analógico */
} GPIO_Mode_t;

/* Tipos de salida */
typedef enum {
    GPIO_OTYPE_PP = 0,      /* Push-Pull */
    GPIO_OTYPE_OD = 1       /* Open-Drain */
} GPIO_OType_t;

/* Velocidades de salida */
typedef enum {
    GPIO_SPEED_LOW    = 0,  /* 2 MHz */
    GPIO_SPEED_MEDIUM = 1,  /* 25 MHz */
    GPIO_SPEED_FAST   = 2,  /* 50 MHz */
    GPIO_SPEED_HIGH   = 3   /* 100 MHz */
} GPIO_Speed_t;

/* Pull-up/Pull-down */
typedef enum {
    GPIO_PUPD_NONE = 0,     /* Sin pull-up ni pull-down */
    GPIO_PUPD_PU   = 1,     /* Pull-up */
    GPIO_PUPD_PD   = 2      /* Pull-down */
} GPIO_PUPD_t;

/* Estructura de configuración */
typedef struct {
    GPIO_TypeDef *port;     /* Puerto GPIO */
    uint8_t       pin;      /* Número de pin (0-15) */
    GPIO_Mode_t   mode;     /* Modo */
    GPIO_OType_t  otype;    /* Tipo de salida */
    GPIO_Speed_t  speed;    /* Velocidad */
    GPIO_PUPD_t   pupd;     /* Pull-up/Pull-down */
} GPIO_Config_t;

/* ========== Funciones Públicas ========== */

/**
 * @brief Habilita el clock del puerto GPIO
 * @param port Puntero al puerto (GPIOA, GPIOB, etc.)
 */
void GPIO_ClockEnable(GPIO_TypeDef *port);

/**
 * @brief Inicializa un pin GPIO con configuración específica
 * @param config Puntero a estructura de configuración
 */
void GPIO_Init(GPIO_Config_t *config);

/**
 * @brief Configura el modo de un pin
 * @param port Puerto GPIO
 * @param pin Número de pin (0-15)
 * @param mode Modo (INPUT, OUTPUT, AF, ANALOG)
 */
void GPIO_SetMode(GPIO_TypeDef *port, uint8_t pin, GPIO_Mode_t mode);

/**
 * @brief Configura el tipo de salida
 * @param port Puerto GPIO
 * @param pin Número de pin (0-15)
 * @param otype Tipo (PP o OD)
 */
void GPIO_SetOutputType(GPIO_TypeDef *port, uint8_t pin, GPIO_OType_t otype);

/**
 * @brief Configura la velocidad de salida
 * @param port Puerto GPIO
 * @param pin Número de pin (0-15)
 * @param speed Velocidad
 */
void GPIO_SetSpeed(GPIO_TypeDef *port, uint8_t pin, GPIO_Speed_t speed);

/**
 * @brief Configura pull-up/pull-down
 * @param port Puerto GPIO
 * @param pin Número de pin (0-15)
 * @param pupd Configuración
 */
void GPIO_SetPullUpDown(GPIO_TypeDef *port, uint8_t pin, GPIO_PUPD_t pupd);

/**
 * @brief Escribe un valor en un pin GPIO
 * @param port Puerto GPIO
 * @param pin Número de pin (0-15)
 * @param value 0 (LOW) o 1 (HIGH)
 */
void GPIO_Write(GPIO_TypeDef *port, uint8_t pin, uint8_t value);

/**
 * @brief Lee el valor de un pin GPIO
 * @param port Puerto GPIO
 * @param pin Número de pin (0-15)
 * @return 0 o 1
 */
uint8_t GPIO_Read(GPIO_TypeDef *port, uint8_t pin);

/**
 * @brief Invierte el estado de un pin GPIO
 * @param port Puerto GPIO
 * @param pin Número de pin (0-15)
 */
void GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin);

/**
 * @brief Configura la función alternativa de un pin
 * @param port Puerto GPIO
 * @param pin Número de pin (0-15)
 * @param af_num Número de función alternativa (0-15)
 */
void GPIO_SetAlternateFunction(GPIO_TypeDef *port, uint8_t pin, uint8_t af_num);

#endif /* GPIO_DRIVER_H */
