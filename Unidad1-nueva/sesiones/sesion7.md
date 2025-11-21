# Sesión 7: Desarrollo de Drivers GPIO y Timer

**Duración**: 2 horas  
**Modalidad**: Presencial con práctica de hardware

---

## Objetivos Específicos

1. **Desarrollar** driver básico para GPIO con funciones de inicialización y control
2. **Configurar** Timer básico (TIM2) para generación de delays precisos
3. **Aplicar** técnicas de lectura de datasheet para configurar periféricos

---

## Contenidos Temáticos

### 1. Configuración Completa de GPIO (40 minutos)

#### Paso 1: Habilitar Clock del Periférico

**Todos los periféricos en STM32 requieren clock habilitado primero**

```c
// RCC_AHB1ENR - Bit 0 para GPIOA
#define RCC_AHB1ENR  (*((volatile uint32_t*)0x40023830))

void gpio_enable_clock(GPIO_TypeDef *port)
{
    if (port == GPIOA)
        RCC_AHB1ENR |= (1 << 0);
    else if (port == GPIOB)
        RCC_AHB1ENR |= (1 << 1);
    // ... otros puertos
}
```

#### Paso 2: Configurar Modo del Pin (MODER)

**Modos posibles**:
- 00: Entrada
- 01: Salida
- 10: Función alternativa
- 11: Analógico

```c
void gpio_set_mode(GPIO_TypeDef *port, uint8_t pin, uint8_t mode)
{
    port->MODER &= ~(0x3 << (pin * 2));      // Limpiar bits
    port->MODER |= (mode << (pin * 2));      // Establecer modo
}

// Uso
gpio_set_mode(GPIOA, 6, 0x1);  // PA6 como salida
```

#### Paso 3: Configurar Tipo de Salida (OTYPER)

**Tipos**:
- 0: Push-pull (puede generar HIGH y LOW)
- 1: Open-drain (solo pull-down, necesita pull-up externo)

```c
void gpio_set_output_type(GPIO_TypeDef *port, uint8_t pin, uint8_t type)
{
    if (type)
        port->OTYPER |= (1 << pin);      // Open-drain
    else
        port->OTYPER &= ~(1 << pin);     // Push-pull
}
```

#### Paso 4: Configurar Velocidad (OSPEEDR)

**Velocidades**:
- 00: Low speed
- 01: Medium speed
- 10: Fast speed
- 11: High speed

```c
void gpio_set_speed(GPIO_TypeDef *port, uint8_t pin, uint8_t speed)
{
    port->OSPEEDR &= ~(0x3 << (pin * 2));
    port->OSPEEDR |= (speed << (pin * 2));
}
```

#### Paso 5: Configurar Pull-up/Pull-down (PUPDR)

**Opciones**:
- 00: No pull-up, no pull-down
- 01: Pull-up
- 10: Pull-down
- 11: Reservado

```c
void gpio_set_pupd(GPIO_TypeDef *port, uint8_t pin, uint8_t pupd)
{
    port->PUPDR &= ~(0x3 << (pin * 2));
    port->PUPDR |= (pupd << (pin * 2));
}
```

#### Driver GPIO Completo (gpio_driver.h/c)

**gpio_driver.h**:
```c
#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>

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

#define GPIOA ((GPIO_TypeDef*)0x40020000)
#define GPIOB ((GPIO_TypeDef*)0x40020400)

// Funciones públicas
void GPIO_Init(GPIO_TypeDef *port, uint8_t pin);
void GPIO_Write(GPIO_TypeDef *port, uint8_t pin, uint8_t value);
uint8_t GPIO_Read(GPIO_TypeDef *port, uint8_t pin);
void GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin);

#endif
```

**gpio_driver.c**:
```c
#include "gpio_driver.h"

#define RCC_AHB1ENR  (*((volatile uint32_t*)0x40023830))

void GPIO_Init(GPIO_TypeDef *port, uint8_t pin)
{
    // 1. Habilitar clock
    if (port == GPIOA)
        RCC_AHB1ENR |= (1 << 0);
    
    // 2. Configurar como salida
    port->MODER &= ~(0x3 << (pin * 2));
    port->MODER |= (0x1 << (pin * 2));
    
    // 3. Push-pull
    port->OTYPER &= ~(1 << pin);
    
    // 4. Velocidad media
    port->OSPEEDR &= ~(0x3 << (pin * 2));
    port->OSPEEDR |= (0x1 << (pin * 2));
    
    // 5. Sin pull-up/down
    port->PUPDR &= ~(0x3 << (pin * 2));
}

void GPIO_Write(GPIO_TypeDef *port, uint8_t pin, uint8_t value)
{
    if (value)
        port->BSRR = (1 << pin);         // Set bit
    else
        port->BSRR = (1 << (pin + 16));  // Reset bit
}

uint8_t GPIO_Read(GPIO_TypeDef *port, uint8_t pin)
{
    return (port->IDR & (1 << pin)) ? 1 : 0;
}

void GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin)
{
    port->ODR ^= (1 << pin);
}
```

### 2. Configuración de Timer Básico (TIM2) (40 minutos)

#### Conceptos de Timer

**Timer**: Contador que incrementa con cada pulso de reloj

**Prescaler (PSC)**: Divide la frecuencia de reloj
```
Frecuencia_Timer = Frecuencia_Clock / (PSC + 1)
```

**Auto-Reload Register (ARR)**: Valor máximo del contador
```
Periodo = (ARR + 1) / Frecuencia_Timer
```

#### Ejemplo de Cálculo

**Objetivo**: Generar un evento cada 1 segundo con reloj de 16 MHz

```
Queremos: 1 segundo
Reloj: 16,000,000 Hz

Opción 1: PSC = 15999, ARR = 999
Frecuencia_Timer = 16,000,000 / 16,000 = 1,000 Hz
Periodo = 1,000 / 1,000 = 1 segundo ✓

Opción 2: PSC = 15, ARR = 999,999
Frecuencia_Timer = 16,000,000 / 16 = 1,000,000 Hz
Periodo = 1,000,000 / 1,000,000 = 1 segundo ✓
```

#### Driver Timer (timer_driver.h/c)

**timer_driver.h**:
```c
#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <stdint.h>

void Timer_Init(uint16_t prescaler, uint32_t period);
void Timer_Start(void);
void Timer_Stop(void);
uint32_t Timer_GetCounter(void);
void Delay_ms(uint32_t ms);

#endif
```

**timer_driver.c**:
```c
#include "timer_driver.h"

#define RCC_APB1ENR  (*((volatile uint32_t*)0x40023840))
#define TIM2_BASE    0x40000000

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    uint32_t RESERVED;
    volatile uint32_t CCR[4];
} TIM_TypeDef;

#define TIM2 ((TIM_TypeDef*)TIM2_BASE)

void Timer_Init(uint16_t prescaler, uint32_t period)
{
    // 1. Habilitar clock TIM2
    RCC_APB1ENR |= (1 << 0);
    
    // 2. Configurar prescaler
    TIM2->PSC = prescaler - 1;
    
    // 3. Configurar auto-reload
    TIM2->ARR = period - 1;
    
    // 4. Generar evento de actualización
    TIM2->EGR |= (1 << 0);
    
    // 5. Habilitar timer
    TIM2->CR1 |= (1 << 0);
}

void Timer_Start(void)
{
    TIM2->CR1 |= (1 << 0);  // CEN bit
}

void Timer_Stop(void)
{
    TIM2->CR1 &= ~(1 << 0);
}

uint32_t Timer_GetCounter(void)
{
    return TIM2->CNT;
}

void Delay_ms(uint32_t ms)
{
    // Suponiendo timer configurado para 1kHz (1ms por tick)
    uint32_t start = TIM2->CNT;
    while ((TIM2->CNT - start) < ms);
}
```

### 3. Integración: LED Parpadeante con Timer (10 minutos)

```c
#include "gpio_driver.h"
#include "timer_driver.h"

int main(void)
{
    // Inicializar LED en PA6
    GPIO_Init(GPIOA, 6);
    
    // Inicializar timer para 1ms por tick
    // PSC = 16000 → 1kHz, ARR = máximo (para cuenta libre)
    Timer_Init(16000, 0xFFFFFFFF);
    
    while (1)
    {
        GPIO_Toggle(GPIOA, 6);
        Delay_ms(500);  // 500 ms
    }
    
    return 0;
}
```

---

## Actividades

### Actividad 1: Lectura de Datasheet (25 minutos)

**Objetivo**: Extraer información del Reference Manual.

**Tarea**: En grupos, completar tabla:

| Registro | Dirección | Bits relevantes | Función |
|----------|-----------|----------------|---------|
| RCC_AHB1ENR | | | |
| GPIOA_MODER | | | |
| TIM2_CR1 | | | |
| TIM2_PSC | | | |

**Herramienta**: Reference Manual RM0090

### Actividad 2: Implementar Patrón de LEDs (35 minutos)

**Tarea**: Crear secuencia con 3 LEDs:
```
LED1: ON  - LED2: OFF - LED3: OFF  (500ms)
LED1: OFF - LED2: ON  - LED3: OFF  (500ms)
LED1: OFF - LED2: OFF - LED3: ON   (500ms)
Repetir
```

**Usar**: Drivers GPIO y Timer creados

### Actividad 3: Debug de Configuración Incorrecta (10 minutos)

**Escenario**: LED no enciende. Código proporcionado tiene error intencional.

**Errores posibles**:
1. Clock no habilitado
2. Pin configurado como entrada en vez de salida
3. Registro BSRR usado incorrectamente

**Tarea**: Identificar y corregir.

---

## Evaluación Formativa

### Quiz - Sesión 7

1. ¿Por qué es necesario habilitar el clock de un periférico? (2 puntos)
2. ¿Qué hace el registro BSRR y por qué es preferible sobre ODR para cambiar un pin? (2 puntos)
3. ¿Cómo se calcula el periodo de un timer dado PSC y ARR? (3 puntos)
4. ¿Qué diferencia hay entre push-pull y open-drain? (2 puntos)
5. Nombra los 5 registros principales de GPIO (1 punto)

---

## Evidencias de Aprendizaje

**Entregable**: Driver completo y funcional que:
1. Configure GPIO con todas las opciones (modo, tipo, velocidad, pull-up/down)
2. Incluya funciones para leer y escribir pines
3. Implemente delay preciso con timer
4. Tenga comentarios explicando cada registro

**Formato**: Archivos .c y .h separados
**Evaluación**: Funcionalidad (60%) + Documentación (40%)

---

## Notas para el Instructor

### Hardware Necesario
- Tarjeta STM32F407
- LEDs externos y resistencias (si LEDs integrados insuficientes)
- Osciloscopio o analizador lógico (opcional para verificar timing)

### Puntos Clave
- Lectura de datasheet es habilidad crítica
- Separación de driver de aplicación (buena práctica)
- Drivers reutilizables para proyectos futuros

### Problemas Comunes
- Olvidar habilitar clock (falla #1)
- Calcular mal prescaler/ARR
- No esperar a que timer inicie después de configuración

---

**Próxima sesión**: Interrupciones y Evaluación Final
**Preparación**: Leer sobre NVIC e interrupciones en Cortex-M4
