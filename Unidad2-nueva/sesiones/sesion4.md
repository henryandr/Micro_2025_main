# Sesión 4: Driver GPIO Completo en C

**Duración**: 2 horas  
**Modalidad**: Presencial con desarrollo de driver modular

---

## Objetivos Específicos

1. **Desarrollar** un driver GPIO completo y modular en C
2. **Aplicar** principios de separación de interfaz e implementación (.h y .c)
3. **Integrar** driver GPIO con proyectos existentes (MEF de sesiones anteriores)

---

## Contenidos Temáticos

### 1. Arquitectura de Drivers (15 minutos)

#### ¿Qué es un Driver?

**Definición**: Capa de software que proporciona interfaz de alto nivel para controlar hardware.

**Objetivos de un driver**:
- **Abstracción**: Ocultar complejidad del hardware
- **Reusabilidad**: Código que se puede usar en múltiples proyectos
- **Mantenibilidad**: Fácil de modificar y extender
- **Portabilidad**: Mínimo cambio para otros microcontroladores

**Capas de abstracción**:
```
┌─────────────────────────┐
│   Aplicación (main.c)   │  <- Usa funciones simples
├─────────────────────────┤
│   Driver GPIO (API)     │  <- Interfaz clara
├─────────────────────────┤
│   Registros Hardware    │  <- Acceso directo
└─────────────────────────┘
```

#### Separación Header/Implementation

**Header (.h)**:
- Prototipos de funciones públicas
- Definiciones de constantes
- Definiciones de tipos
- Documentación de API

**Implementation (.c)**:
- Implementación de funciones
- Variables privadas (static)
- Funciones auxiliares privadas

**Ejemplo mínimo**:

```c
// gpio_driver.h
#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

void GPIO_Init(void);
void GPIO_Write(uint8_t pin, uint8_t value);

#endif
```

```c
// gpio_driver.c
#include "gpio_driver.h"

void GPIO_Init(void) {
    // Implementación
}

void GPIO_Write(uint8_t pin, uint8_t value) {
    // Implementación
}
```

### 2. Definiciones de Registros (20 minutos)

#### Archivo stm32f407xx.h

**Estructura GPIO completa**:

```c
#ifndef STM32F407XX_H
#define STM32F407XX_H

#include <stdint.h>

// Direcciones base
#define PERIPH_BASE       0x40000000UL
#define AHB1PERIPH_BASE   (PERIPH_BASE + 0x00020000UL)
#define RCC_BASE          (AHB1PERIPH_BASE + 0x3800UL)
#define GPIOA_BASE        (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE        (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE        (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE        (AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE        (AHB1PERIPH_BASE + 0x1000UL)

// Estructura GPIO
typedef struct {
    volatile uint32_t MODER;     // 0x00
    volatile uint32_t OTYPER;    // 0x04
    volatile uint32_t OSPEEDR;   // 0x08
    volatile uint32_t PUPDR;     // 0x0C
    volatile uint32_t IDR;       // 0x10
    volatile uint32_t ODR;       // 0x14
    volatile uint32_t BSRR;      // 0x18
    volatile uint32_t LCKR;      // 0x1C
    volatile uint32_t AFR[2];    // 0x20-0x24
} GPIO_TypeDef;

// Punteros a GPIOs
#define GPIOA ((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef*)GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef*)GPIOC_BASE)
#define GPIOD ((GPIO_TypeDef*)GPIOD_BASE)
#define GPIOE ((GPIO_TypeDef*)GPIOE_BASE)

// Estructura RCC (solo registros necesarios)
typedef struct {
    volatile uint32_t CR;        // 0x00
    volatile uint32_t PLLCFGR;   // 0x04
    volatile uint32_t CFGR;      // 0x08
    volatile uint32_t CIR;       // 0x0C
    volatile uint32_t AHB1RSTR;  // 0x10
    volatile uint32_t AHB2RSTR;  // 0x14
    volatile uint32_t AHB3RSTR;  // 0x18
    uint32_t RESERVED0;          // 0x1C
    volatile uint32_t APB1RSTR;  // 0x20
    volatile uint32_t APB2RSTR;  // 0x24
    uint32_t RESERVED1[2];       // 0x28-0x2C
    volatile uint32_t AHB1ENR;   // 0x30
} RCC_TypeDef;

#define RCC ((RCC_TypeDef*)RCC_BASE)

#endif
```

### 3. API del Driver GPIO (30 minutos)

#### gpio_driver.h - Header completo

```c
/**
 * @file gpio_driver.h
 * @brief Driver GPIO para STM32F407
 * @author Curso Microprocesadores
 * @date 2025
 */

#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>
#include "stm32f407xx.h"

// Modos de GPIO
typedef enum {
    GPIO_MODE_INPUT  = 0,
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_AF     = 2,
    GPIO_MODE_ANALOG = 3
} GPIO_Mode_t;

// Tipos de salida
typedef enum {
    GPIO_OTYPE_PP = 0,  // Push-Pull
    GPIO_OTYPE_OD = 1   // Open-Drain
} GPIO_OType_t;

// Velocidades
typedef enum {
    GPIO_SPEED_LOW    = 0,
    GPIO_SPEED_MEDIUM = 1,
    GPIO_SPEED_FAST   = 2,
    GPIO_SPEED_HIGH   = 3
} GPIO_Speed_t;

// Pull-up/Pull-down
typedef enum {
    GPIO_PUPD_NONE = 0,
    GPIO_PUPD_PU   = 1,
    GPIO_PUPD_PD   = 2
} GPIO_PUPD_t;

// Estructura de configuración
typedef struct {
    GPIO_TypeDef *port;
    uint8_t       pin;
    GPIO_Mode_t   mode;
    GPIO_OType_t  otype;
    GPIO_Speed_t  speed;
    GPIO_PUPD_t   pupd;
} GPIO_Config_t;

// Funciones públicas

/**
 * @brief Habilita clock del puerto GPIO
 * @param port Puntero al puerto (GPIOA, GPIOB, etc.)
 */
void GPIO_ClockEnable(GPIO_TypeDef *port);

/**
 * @brief Inicializa un pin GPIO con configuración específica
 * @param config Puntero a estructura de configuración
 */
void GPIO_Init(GPIO_Config_t *config);

/**
 * @brief Escribe valor en un pin GPIO
 * @param port Puerto GPIO
 * @param pin Número de pin (0-15)
 * @param value 0 (LOW) o 1 (HIGH)
 */
void GPIO_Write(GPIO_TypeDef *port, uint8_t pin, uint8_t value);

/**
 * @brief Lee valor de un pin GPIO
 * @param port Puerto GPIO
 * @param pin Número de pin (0-15)
 * @return 0 o 1
 */
uint8_t GPIO_Read(GPIO_TypeDef *port, uint8_t pin);

/**
 * @brief Invierte estado de un pin GPIO
 * @param port Puerto GPIO
 * @param pin Número de pin (0-15)
 */
void GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin);

/**
 * @brief Configura modo de un pin
 * @param port Puerto GPIO
 * @param pin Número de pin
 * @param mode Modo (INPUT, OUTPUT, AF, ANALOG)
 */
void GPIO_SetMode(GPIO_TypeDef *port, uint8_t pin, GPIO_Mode_t mode);

/**
 * @brief Configura tipo de salida
 * @param port Puerto GPIO
 * @param pin Número de pin
 * @param otype Tipo (PP o OD)
 */
void GPIO_SetOutputType(GPIO_TypeDef *port, uint8_t pin, GPIO_OType_t otype);

/**
 * @brief Configura velocidad
 * @param port Puerto GPIO
 * @param pin Número de pin
 * @param speed Velocidad
 */
void GPIO_SetSpeed(GPIO_TypeDef *port, uint8_t pin, GPIO_Speed_t speed);

/**
 * @brief Configura pull-up/pull-down
 * @param port Puerto GPIO
 * @param pin Número de pin
 * @param pupd Configuración
 */
void GPIO_SetPullUpDown(GPIO_TypeDef *port, uint8_t pin, GPIO_PUPD_t pupd);

#endif
```

#### gpio_driver.c - Implementación

```c
/**
 * @file gpio_driver.c
 * @brief Implementación del driver GPIO
 */

#include "gpio_driver.h"

void GPIO_ClockEnable(GPIO_TypeDef *port) {
    if (port == GPIOA)
        RCC->AHB1ENR |= (1 << 0);
    else if (port == GPIOB)
        RCC->AHB1ENR |= (1 << 1);
    else if (port == GPIOC)
        RCC->AHB1ENR |= (1 << 2);
    else if (port == GPIOD)
        RCC->AHB1ENR |= (1 << 3);
    else if (port == GPIOE)
        RCC->AHB1ENR |= (1 << 4);
}

void GPIO_SetMode(GPIO_TypeDef *port, uint8_t pin, GPIO_Mode_t mode) {
    // Limpiar bits del pin
    port->MODER &= ~(0x3 << (pin * 2));
    // Establecer nuevo modo
    port->MODER |= (mode << (pin * 2));
}

void GPIO_SetOutputType(GPIO_TypeDef *port, uint8_t pin, GPIO_OType_t otype) {
    if (otype == GPIO_OTYPE_OD)
        port->OTYPER |= (1 << pin);
    else
        port->OTYPER &= ~(1 << pin);
}

void GPIO_SetSpeed(GPIO_TypeDef *port, uint8_t pin, GPIO_Speed_t speed) {
    port->OSPEEDR &= ~(0x3 << (pin * 2));
    port->OSPEEDR |= (speed << (pin * 2));
}

void GPIO_SetPullUpDown(GPIO_TypeDef *port, uint8_t pin, GPIO_PUPD_t pupd) {
    port->PUPDR &= ~(0x3 << (pin * 2));
    port->PUPDR |= (pupd << (pin * 2));
}

void GPIO_Init(GPIO_Config_t *config) {
    // Habilitar clock
    GPIO_ClockEnable(config->port);
    
    // Configurar todos los parámetros
    GPIO_SetMode(config->port, config->pin, config->mode);
    
    if (config->mode == GPIO_MODE_OUTPUT) {
        GPIO_SetOutputType(config->port, config->pin, config->otype);
        GPIO_SetSpeed(config->port, config->pin, config->speed);
    }
    
    GPIO_SetPullUpDown(config->port, config->pin, config->pupd);
}

void GPIO_Write(GPIO_TypeDef *port, uint8_t pin, uint8_t value) {
    if (value)
        port->BSRR = (1 << pin);         // Set
    else
        port->BSRR = (1 << (pin + 16));  // Reset
}

uint8_t GPIO_Read(GPIO_TypeDef *port, uint8_t pin) {
    return (port->IDR & (1 << pin)) ? 1 : 0;
}

void GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin) {
    port->ODR ^= (1 << pin);
}
```

### 4. Uso del Driver (25 minutos)

#### Ejemplo 1: Configuración Simple

```c
#include "gpio_driver.h"

int main(void) {
    // Método 1: Usando estructura de configuración
    GPIO_Config_t led_config = {
        .port  = GPIOA,
        .pin   = 6,
        .mode  = GPIO_MODE_OUTPUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_MEDIUM,
        .pupd  = GPIO_PUPD_NONE
    };
    
    GPIO_Init(&led_config);
    
    // Usar el LED
    while(1) {
        GPIO_Write(GPIOA, 6, 1);
        delay_ms(500);
        GPIO_Write(GPIOA, 6, 0);
        delay_ms(500);
    }
}
```

#### Ejemplo 2: Múltiples GPIOs

```c
void init_all_leds(void) {
    // Configurar múltiples LEDs con misma configuración
    GPIO_Config_t led_config = {
        .port  = GPIOA,
        .mode  = GPIO_MODE_OUTPUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_MEDIUM,
        .pupd  = GPIO_PUPD_NONE
    };
    
    // LED 1 en PA5
    led_config.pin = 5;
    GPIO_Init(&led_config);
    
    // LED 2 en PA6
    led_config.pin = 6;
    GPIO_Init(&led_config);
    
    // LED 3 en PA7
    led_config.pin = 7;
    GPIO_Init(&led_config);
}
```

#### Ejemplo 3: Integración con MEF

```c
#include "gpio_driver.h"

// MEF de sesión 3 ahora usa el driver
void mef_semaforo(void) {
    switch (estado_semaforo) {
        case SEMAFORO_VERDE:
            GPIO_Write(GPIOA, 5, 1);  // Verde ON
            GPIO_Write(GPIOA, 6, 0);  // Amarillo OFF
            GPIO_Write(GPIOA, 7, 0);  // Rojo OFF
            
            if (tiempo_en_estado >= 5000) {
                estado_semaforo = SEMAFORO_AMARILLO;
                tiempo_en_estado = 0;
            }
            break;
            
        case SEMAFORO_AMARILLO:
            GPIO_Write(GPIOA, 5, 0);
            GPIO_Write(GPIOA, 6, 1);
            GPIO_Write(GPIOA, 7, 0);
            
            if (tiempo_en_estado >= 2000) {
                estado_semaforo = SEMAFORO_ROJO;
                tiempo_en_estado = 0;
            }
            break;
            
        case SEMAFORO_ROJO:
            GPIO_Write(GPIOA, 5, 0);
            GPIO_Write(GPIOA, 6, 0);
            GPIO_Write(GPIOA, 7, 1);
            
            if (tiempo_en_estado >= 5000) {
                estado_semaforo = SEMAFORO_VERDE;
                tiempo_en_estado = 0;
            }
            break;
    }
    
    tiempo_en_estado++;
}
```

---

## Actividades

### Actividad 1: Completar Driver (30 minutos)

**Tarea**: Añadir funciones adicionales al driver

**Funciones a implementar**:
```c
// Escribir byte completo a puerto
void GPIO_WritePort(GPIO_TypeDef *port, uint8_t start_pin, uint8_t value);

// Leer byte completo de puerto
uint8_t GPIO_ReadPort(GPIO_TypeDef *port, uint8_t start_pin);

// Configurar función alternativa
void GPIO_SetAF(GPIO_TypeDef *port, uint8_t pin, uint8_t af_num);
```

### Actividad 2: Refactorizar Código Anterior (30 minutos)

**Tarea**: Tomar el código del semáforo de Sesión 3 y refactorizarlo para usar el nuevo driver GPIO.

**Comparar**:
- Líneas de código antes/después
- Legibilidad
- Facilidad de modificación

### Actividad 3: Aplicación con Driver (30 minutos)

**Sistema**: Control de display 7 segmentos

**Requisitos**:
- 8 pines GPIO para segmentos (PA0-PA7)
- Mostrar dígitos 0-9
- Usar driver GPIO
- Crear función `void display_digit(uint8_t digit)`

---

## Evaluación Formativa

### Quiz - Sesión 4

1. **¿Qué va en el archivo .h de un driver?** (2 puntos)
2. **¿Por qué usar estructura de configuración en lugar de múltiples parámetros?** (2 puntos)
3. **¿Qué ventaja tiene usar BSRR en lugar de ODR?** (2 puntos)
4. **¿Cómo se inicializa un pin como entrada con pull-up?** (2 puntos)
5. **¿Por qué declarar variables del driver como `static`?** (2 puntos)

---

## Evidencias de Aprendizaje

**Entregable**: Driver GPIO completo y aplicación de demostración

**Componentes**:
1. `gpio_driver.h` - Header completo
2. `gpio_driver.c` - Implementación
3. `stm32f407xx.h` - Definiciones de hardware
4. `aplicacion_demo.c` - Aplicación que usa el driver
5. `README.md` - Documentación

**La aplicación debe**:
- Usar al menos 5 funciones diferentes del driver
- Controlar mínimo 3 LEDs y 1 botón
- Implementar algún patrón visual

**Evaluación**:
- Driver funcional: 40%
- API bien diseñada: 20%
- Documentación clara: 15%
- Aplicación demo: 15%
- Código limpio: 10%

---

## Material para Casa

### Lectura Previa a Sesión 5
- Concepto de timer/counter
- Prescaler y auto-reload
- Generación de delays precisos

### Ejercicio Opcional
Extender driver GPIO para:
- Configurar múltiples pines con una llamada
- Lock de configuración de pin
- Validación de parámetros

---

## Notas para el Instructor

### Preparación
- Driver de referencia compilado
- Ejemplos de uso listos
- Comparación con HAL de ST (opcional)

### Enfoque
- Enfatizar buenas prácticas de diseño de software
- Mostrar cómo el driver simplifica código de aplicación
- Comparar con código Assembly de Unidad 1

**Próxima sesión**: Configuración de Timer (TIM2)
