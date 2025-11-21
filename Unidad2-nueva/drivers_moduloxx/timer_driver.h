/**
 * @file timer_driver.h
 * @brief Driver Timer (TIM2) para STM32F407
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Driver para TIM2 con funciones de configuración, inicio/parada,
 *   y base de tiempo en milisegundos.
 */

#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <stdint.h>

/* ========== Definiciones de Hardware ========== */

/* Estructura TIM2 */
typedef struct {
    volatile uint32_t CR1;       /* 0x00: Control register 1 */
    volatile uint32_t CR2;       /* 0x04: Control register 2 */
    volatile uint32_t SMCR;      /* 0x08: Slave mode control register */
    volatile uint32_t DIER;      /* 0x0C: DMA/Interrupt enable register */
    volatile uint32_t SR;        /* 0x10: Status register */
    volatile uint32_t EGR;       /* 0x14: Event generation register */
    volatile uint32_t CCMR1;     /* 0x18: Capture/compare mode register 1 */
    volatile uint32_t CCMR2;     /* 0x1C: Capture/compare mode register 2 */
    volatile uint32_t CCER;      /* 0x20: Capture/compare enable register */
    volatile uint32_t CNT;       /* 0x24: Counter */
    volatile uint32_t PSC;       /* 0x28: Prescaler */
    volatile uint32_t ARR;       /* 0x2C: Auto-reload register */
    uint32_t RESERVED;           /* 0x30: Reserved */
    volatile uint32_t CCR1;      /* 0x34: Capture/compare register 1 */
    volatile uint32_t CCR2;      /* 0x38: Capture/compare register 2 */
    volatile uint32_t CCR3;      /* 0x3C: Capture/compare register 3 */
    volatile uint32_t CCR4;      /* 0x40: Capture/compare register 4 */
} TIM_TypeDef;

#define TIM2 ((TIM_TypeDef*)0x40000000)

/* ========== Funciones Públicas ========== */

/**
 * @brief Inicializa TIM2 con prescaler y auto-reload específicos
 * @param prescaler Valor del prescaler (PSC)
 * @param auto_reload Valor del auto-reload (ARR)
 * 
 * @note Para generar tick de 1ms con clock de 16MHz:
 *       prescaler = 159, auto_reload = 99
 *       Frecuencia = 16,000,000 / 160 / 100 = 1000 Hz (1ms)
 */
void Timer_Init(uint16_t prescaler, uint16_t auto_reload);

/**
 * @brief Inicia el timer
 */
void Timer_Start(void);

/**
 * @brief Detiene el timer
 */
void Timer_Stop(void);

/**
 * @brief Obtiene el valor actual del contador
 * @return Valor de CNT
 */
uint32_t Timer_GetCount(void);

/**
 * @brief Obtiene tiempo en milisegundos desde el inicio
 * @return Tiempo en ms
 */
uint32_t Timer_GetMillis(void);

/**
 * @brief Delay bloqueante en milisegundos
 * @param ms Milisegundos a esperar
 */
void Timer_DelayMs(uint32_t ms);

/**
 * @brief Handler de interrupción TIM2
 * @note Debe ser llamado por TIM2_IRQHandler
 */
void Timer_IRQHandler(void);

#endif /* TIMER_DRIVER_H */
