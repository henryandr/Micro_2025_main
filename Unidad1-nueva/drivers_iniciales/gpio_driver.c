/**
 * @file gpio_driver.c
 * @brief Implementación del driver GPIO
 */

#include "gpio_driver.h"

#define RCC_AHB1ENR (*((volatile uint32_t*)0x40023830))

/**
 * @brief Habilita el clock de un puerto GPIO
 * @param port Puntero al puerto (GPIOA, GPIOB, etc.)
 */
void GPIO_ClockEnable(GPIO_TypeDef *port)
{
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

/**
 * @brief Inicializa un pin GPIO con configuración por defecto
 * @param port Puerto GPIO
 * @param pin Número de pin (0-15)
 * @param mode Modo (INPUT, OUTPUT, AF, ANALOG)
 */
void GPIO_Init(GPIO_TypeDef *port, uint8_t pin, uint8_t mode)
{
    /* Habilitar clock */
    GPIO_ClockEnable(port);
    
    /* Configurar modo */
    GPIO_SetMode(port, pin, mode);
    
    /* Si es salida, configurar como push-pull, velocidad media, sin pull */
    if (mode == GPIO_MODE_OUTPUT) {
        GPIO_SetOutputType(port, pin, GPIO_OTYPE_PP);
        GPIO_SetSpeed(port, pin, GPIO_SPEED_MEDIUM);
        GPIO_SetPullUpDown(port, pin, GPIO_PUPD_NONE);
    }
}

/**
 * @brief Escribe valor en un pin
 * @param port Puerto GPIO
 * @param pin Número de pin
 * @param value 0 (LOW) o 1 (HIGH)
 */
void GPIO_Write(GPIO_TypeDef *port, uint8_t pin, uint8_t value)
{
    if (value)
        port->BSRR = (1 << pin);        /* Set bit */
    else
        port->BSRR = (1 << (pin + 16)); /* Reset bit */
}

/**
 * @brief Lee valor de un pin
 * @param port Puerto GPIO
 * @param pin Número de pin
 * @return 0 o 1
 */
uint8_t GPIO_Read(GPIO_TypeDef *port, uint8_t pin)
{
    return (port->IDR & (1 << pin)) ? 1 : 0;
}

/**
 * @brief Invierte el estado de un pin
 * @param port Puerto GPIO
 * @param pin Número de pin
 */
void GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin)
{
    port->ODR ^= (1 << pin);
}

/**
 * @brief Configura el modo de un pin
 * @param port Puerto GPIO
 * @param pin Número de pin
 * @param mode Modo (00=input, 01=output, 10=AF, 11=analog)
 */
void GPIO_SetMode(GPIO_TypeDef *port, uint8_t pin, uint8_t mode)
{
    port->MODER &= ~(0x3 << (pin * 2));
    port->MODER |= (mode << (pin * 2));
}

/**
 * @brief Configura el tipo de salida
 * @param port Puerto GPIO
 * @param pin Número de pin
 * @param type 0=push-pull, 1=open-drain
 */
void GPIO_SetOutputType(GPIO_TypeDef *port, uint8_t pin, uint8_t type)
{
    if (type)
        port->OTYPER |= (1 << pin);
    else
        port->OTYPER &= ~(1 << pin);
}

/**
 * @brief Configura la velocidad del pin
 * @param port Puerto GPIO
 * @param pin Número de pin
 * @param speed 00=low, 01=medium, 10=fast, 11=high
 */
void GPIO_SetSpeed(GPIO_TypeDef *port, uint8_t pin, uint8_t speed)
{
    port->OSPEEDR &= ~(0x3 << (pin * 2));
    port->OSPEEDR |= (speed << (pin * 2));
}

/**
 * @brief Configura pull-up/pull-down
 * @param port Puerto GPIO
 * @param pin Número de pin
 * @param pupd 00=none, 01=pull-up, 10=pull-down
 */
void GPIO_SetPullUpDown(GPIO_TypeDef *port, uint8_t pin, uint8_t pupd)
{
    port->PUPDR &= ~(0x3 << (pin * 2));
    port->PUPDR |= (pupd << (pin * 2));
}
