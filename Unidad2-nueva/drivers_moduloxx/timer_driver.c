/**
 * @file timer_driver.c
 * @brief Implementación del driver Timer
 * @author Curso Microprocesadores
 * @date 2025
 */

#include "timer_driver.h"

/* RCC y NVIC */
#define RCC_APB1ENR (*((volatile uint32_t*)0x40023840))
#define NVIC_ISER0  (*((volatile uint32_t*)0xE000E100))

/* Variable global de milisegundos */
static volatile uint32_t millis_count = 0;

/* ========== Funciones Públicas ========== */

void Timer_Init(uint16_t prescaler, uint16_t auto_reload) {
    /* Habilitar clock de TIM2 (APB1ENR bit 0) */
    RCC_APB1ENR |= (1 << 0);
    
    /* Configurar prescaler */
    TIM2->PSC = prescaler;
    
    /* Configurar auto-reload */
    TIM2->ARR = auto_reload;
    
    /* Generar update event para cargar valores */
    TIM2->EGR |= (1 << 0);  /* UG bit */
    
    /* Limpiar flag de update */
    TIM2->SR &= ~(1 << 0);  /* UIF bit */
    
    /* Habilitar interrupción de update */
    TIM2->DIER |= (1 << 0);  /* UIE bit */
    
    /* Habilitar TIM2 en NVIC (IRQ 28) */
    NVIC_ISER0 |= (1 << 28);
}

void Timer_Start(void) {
    TIM2->CR1 |= (1 << 0);  /* CEN bit */
}

void Timer_Stop(void) {
    TIM2->CR1 &= ~(1 << 0);
}

uint32_t Timer_GetCount(void) {
    return TIM2->CNT;
}

uint32_t Timer_GetMillis(void) {
    return millis_count;
}

void Timer_DelayMs(uint32_t ms) {
    uint32_t start = millis_count;
    while ((millis_count - start) < ms);
}

void Timer_IRQHandler(void) {
    /* Verificar si es interrupción de update */
    if (TIM2->SR & (1 << 0)) {
        /* Incrementar contador de milisegundos */
        millis_count++;
        
        /* Limpiar flag */
        TIM2->SR &= ~(1 << 0);
    }
}

/**
 * @brief Handler de interrupción real (debe estar definido)
 * @note Este es el nombre esperado por el vector table
 */
void TIM2_IRQHandler(void) {
    Timer_IRQHandler();
}
