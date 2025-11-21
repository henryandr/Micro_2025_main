# Sesión 5: Configuración de Timer (TIM2) en C

**Duración**: 2 horas  
**Modalidad**: Presencial con práctica de timing preciso

---

## Objetivos Específicos

1. **Configurar** Timer TIM2 para generación de delays precisos
2. **Calcular** valores de prescaler y auto-reload según frecuencia deseada
3. **Integrar** Timer con MEF para base de tiempo precisa

---

## Contenidos Temáticos

### 1. Conceptos de Timer/Counter (20 minutos)

#### ¿Qué es un Timer?

**Definición**: Periférico que cuenta pulsos de reloj para medir tiempo o generar eventos periódicos.

**Componentes principales**:
- **Counter (CNT)**: Registro que se incrementa con cada pulso
- **Prescaler (PSC)**: Divide frecuencia de reloj
- **Auto-Reload (ARR)**: Valor máximo del contador
- **Update Event**: Evento generado cuando CNT alcanza ARR

**Fórmula fundamental**:
```
Frecuencia_Timer = Frecuencia_Clock / (PSC + 1)
Periodo_Overflow = (ARR + 1) / Frecuencia_Timer
```

#### TIM2 en STM32F407

**Características**:
- Timer de 32 bits (contador muy grande: 0 a 4,294,967,295)
- Clock de APB1 (típicamente 16 MHz con HSI)
- Puede generar interrupciones en overflow
- Múltiples modos de operación

### 2. Cálculo de Prescaler y Auto-Reload (25 minutos)

#### Ejemplo 1: Generar Interrupción cada 1ms

**Datos**:
- Clock: 16 MHz
- Periodo deseado: 1 ms (1000 Hz)

**Cálculo**:
```
Frecuencia_Timer = 16,000,000 / (PSC + 1)

Queremos 1000 Hz, entonces:
Frecuencia_Timer = 1000 Hz
16,000,000 / (PSC + 1) = 1000
PSC + 1 = 16,000
PSC = 15,999

Ahora con ARR:
Periodo = (ARR + 1) / 1000 Hz = 0.001 s
ARR + 1 = 1
ARR = 0

Pero ARR=0 es muy pequeño, mejor:
PSC = 159 (divide por 160)
Frecuencia_Timer = 16,000,000 / 160 = 100,000 Hz
ARR = 99 (divide por 100)
Frecuencia_Final = 100,000 / 100 = 1000 Hz ✓
```

#### Código de Configuración

```c
/**
 * @file timer_driver.h
 */
#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <stdint.h>

void Timer_Init(uint16_t prescaler, uint16_t auto_reload);
void Timer_Start(void);
void Timer_Stop(void);
uint32_t Timer_GetValue(void);

#endif
```

```c
/**
 * @file timer_driver.c
 */
#include "timer_driver.h"
#include "stm32f407xx.h"

// Estructura TIM2 (simplificada)
typedef struct {
    volatile uint32_t CR1;       // 0x00
    volatile uint32_t CR2;       // 0x04
    volatile uint32_t SMCR;      // 0x08
    volatile uint32_t DIER;      // 0x0C
    volatile uint32_t SR;        // 0x10
    volatile uint32_t EGR;       // 0x14
    volatile uint32_t CCMR1;     // 0x18
    volatile uint32_t CCMR2;     // 0x1C
    volatile uint32_t CCER;      // 0x20
    volatile uint32_t CNT;       // 0x24
    volatile uint32_t PSC;       // 0x28
    volatile uint32_t ARR;       // 0x2C
} TIM_TypeDef;

#define TIM2_BASE 0x40000000
#define TIM2 ((TIM_TypeDef*)TIM2_BASE)

void Timer_Init(uint16_t prescaler, uint16_t auto_reload) {
    // Habilitar clock TIM2 (APB1ENR bit 0)
    RCC->APB1ENR |= (1 << 0);
    
    // Configurar prescaler
    TIM2->PSC = prescaler;
    
    // Configurar auto-reload
    TIM2->ARR = auto_reload;
    
    // Generar update event para cargar valores
    TIM2->EGR |= (1 << 0);
    
    // Habilitar interrupción de update
    TIM2->DIER |= (1 << 0);  // UIE bit
    
    // Configurar NVIC (IRQ 28 para TIM2)
    uint32_t *NVIC_ISER0 = (uint32_t*)0xE000E100;
    *NVIC_ISER0 |= (1 << 28);
}

void Timer_Start(void) {
    TIM2->CR1 |= (1 << 0);  // CEN bit
}

void Timer_Stop(void) {
    TIM2->CR1 &= ~(1 << 0);
}

uint32_t Timer_GetValue(void) {
    return TIM2->CNT;
}
```

### 3. Integración Timer + MEF (30 minutos)

#### Base de Tiempo Global

```c
// Variable global incrementada por timer cada 1ms
volatile uint32_t millis = 0;

// Handler de interrupción (en archivo separado)
void TIM2_IRQHandler(void) {
    // Verificar flag de update
    if (TIM2->SR & (1 << 0)) {
        millis++;
        
        // Limpiar flag
        TIM2->SR &= ~(1 << 0);
    }
}

// Función auxiliar tipo Arduino
uint32_t get_millis(void) {
    return millis;
}

// Delay bloqueante usando timer
void delay_ms(uint32_t ms) {
    uint32_t start = millis;
    while ((millis - start) < ms);
}
```

#### MEF con Timer

```c
// MEF con timing preciso
typedef enum {
    LED_OFF,
    LED_ON
} EstadoLED_t;

EstadoLED_t estado_led = LED_OFF;
uint32_t ultimo_cambio = 0;

void mef_blink_timer(void) {
    uint32_t tiempo_actual = get_millis();
    
    switch (estado_led) {
        case LED_OFF:
            GPIO_Write(GPIOA, 6, 0);
            
            if ((tiempo_actual - ultimo_cambio) >= 500) {
                estado_led = LED_ON;
                ultimo_cambio = tiempo_actual;
            }
            break;
            
        case LED_ON:
            GPIO_Write(GPIOA, 6, 1);
            
            if ((tiempo_actual - ultimo_cambio) >= 500) {
                estado_led = LED_OFF;
                ultimo_cambio = tiempo_actual;
            }
            break;
    }
}

int main(void) {
    // Inicializar GPIO
    GPIO_Config_t led = {GPIOA, 6, GPIO_MODE_OUTPUT, 
                         GPIO_OTYPE_PP, GPIO_SPEED_MEDIUM, GPIO_PUPD_NONE};
    GPIO_Init(&led);
    
    // Inicializar timer: 1ms tick
    Timer_Init(159, 99);  // 16MHz / 160 / 100 = 1kHz (1ms)
    Timer_Start();
    
    while(1) {
        mef_blink_timer();  // Llamar frecuentemente
    }
}
```

---

## Actividades

### Actividad 1: Calcular Parámetros (20 minutos)

**Calcular PSC y ARR para**:
1. Interrupción cada 10ms
2. Interrupción cada 100μs
3. Interrupción cada 5s

### Actividad 2: MEF con Múltiples Timings (40 minutos)

**Sistema**: 3 LEDs con diferentes frecuencias
- LED1: 1 Hz (500ms ON, 500ms OFF)
- LED2: 2 Hz (250ms ON, 250ms OFF)
- LED3: 0.5 Hz (1s ON, 1s OFF)

**Usar una sola base de tiempo (1ms tick)**

### Actividad 3: Timeout con Timer (30 minutos)

**Sistema**: Botón con timeout
- Si botón presionado > 3s: acción especial
- Si presionado < 3s: acción normal
- Usar timer para medir duración

---

## Evaluación Formativa

### Quiz - Sesión 5

1. **¿Qué hace el prescaler del timer?** (2 puntos)
2. **Si PSC=1599 y CLK=16MHz, ¿cuál es la frecuencia del timer?** (2 puntos)
3. **¿Por qué limpiar el flag SR en el handler de interrupción?** (2 puntos)
4. **¿Qué ventaja tiene usar timer vs delay busy-wait?** (2 puntos)
5. **¿Cómo medir tiempo transcurrido entre dos eventos?** (2 puntos)

---

## Evidencias de Aprendizaje

**Entregable**: Sistema de cronómetro simple

**Especificaciones**:
- Cuenta segundos (0-99)
- Botón START/STOP
- Botón RESET
- Mostrar en 4 LEDs (binario) o display
- Precisión: ±10ms

**Evaluación**:
- Configuración correcta de timer: 30%
- Cálculos correctos: 20%
- Funcionalidad: 30%
- Código limpio: 20%

---

## Material para Casa

### Lectura Sesión 6
- Concepto de multiplexación
- Refresh rate de displays
- Persistencia visual humana

**Próxima sesión**: Multiplexación
