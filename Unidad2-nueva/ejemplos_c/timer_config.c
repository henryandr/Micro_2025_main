/**
 * @file timer_config.c
 * @brief Configuración de Timer TIM2 para base de tiempo
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Configuración de TIM2 para generar interrupciones cada 1ms.
 *   Proporciona base de tiempo para MEF y funciones de delay.
 * 
 * Hardware:
 *   - STM32F407VET6
 *   - Clock: 16 MHz (HSI)
 * 
 * Notas:
 *   - Handler TIM2_IRQHandler debe estar definido
 *   - Habilita interrupción en NVIC
 */

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

#define TIM2_BASE 0x40000000
#define TIM2 ((TIM_TypeDef*)TIM2_BASE)

/* RCC */
#define RCC_APB1ENR (*((volatile uint32_t*)0x40023840))

/* NVIC */
#define NVIC_ISER0 (*((volatile uint32_t*)0xE000E100))

/* ========== Variables Globales ========== */

/* Contador de milisegundos (incrementado en interrupción) */
volatile uint32_t millis = 0;

/* ========== Funciones ========== */

/**
 * @brief Inicializar TIM2 para generar tick de 1ms
 * @param prescaler Valor del prescaler (PSC register)
 * @param auto_reload Valor del auto-reload (ARR register)
 * 
 * Cálculo para 1ms con clock de 16MHz:
 * Frecuencia_Timer = 16,000,000 / (PSC + 1)
 * Frecuencia_Overflow = Frecuencia_Timer / (ARR + 1)
 * Para 1ms (1000 Hz): PSC = 159, ARR = 99
 * 16,000,000 / 160 / 100 = 1000 Hz
 */
void timer_init(uint16_t prescaler, uint16_t auto_reload) {
    /* 1. Habilitar clock de TIM2 (APB1ENR bit 0) */
    RCC_APB1ENR |= (1 << 0);
    
    /* 2. Configurar prescaler */
    TIM2->PSC = prescaler;
    
    /* 3. Configurar auto-reload */
    TIM2->ARR = auto_reload;
    
    /* 4. Generar update event para cargar valores */
    TIM2->EGR |= (1 << 0);  /* UG bit */
    
    /* 5. Limpiar flag de update */
    TIM2->SR &= ~(1 << 0);  /* UIF bit */
    
    /* 6. Habilitar interrupción de update */
    TIM2->DIER |= (1 << 0);  /* UIE bit */
    
    /* 7. Habilitar TIM2 en NVIC (IRQ 28) */
    NVIC_ISER0 |= (1 << 28);
}

/**
 * @brief Iniciar el timer
 */
void timer_start(void) {
    TIM2->CR1 |= (1 << 0);  /* CEN bit */
}

/**
 * @brief Detener el timer
 */
void timer_stop(void) {
    TIM2->CR1 &= ~(1 << 0);
}

/**
 * @brief Obtener valor actual del contador
 * @return Valor de CNT
 */
uint32_t timer_get_count(void) {
    return TIM2->CNT;
}

/**
 * @brief Obtener tiempo en milisegundos desde inicio
 * @return Tiempo en ms
 */
uint32_t get_millis(void) {
    return millis;
}

/**
 * @brief Delay bloqueante en milisegundos
 * @param ms Milisegundos a esperar
 * @note Usa busy-wait, bloquea ejecución
 */
void delay_ms(uint32_t ms) {
    uint32_t start = millis;
    while ((millis - start) < ms);
}

/**
 * @brief Handler de interrupción TIM2
 * @note DEBE estar definido con este nombre exacto
 */
void TIM2_IRQHandler(void) {
    /* Verificar si es interrupción de update */
    if (TIM2->SR & (1 << 0)) {
        /* Incrementar contador de milisegundos */
        millis++;
        
        /* Limpiar flag de interrupción */
        TIM2->SR &= ~(1 << 0);
    }
}

/**
 * @brief Ejemplo de inicialización en main
 */
void timer_ejemplo_init(void) {
    /* Inicializar TIM2 para tick de 1ms */
    /* Clock: 16 MHz, PSC = 159, ARR = 99 */
    /* 16,000,000 / 160 / 100 = 1000 Hz (1ms) */
    timer_init(159, 99);
    
    /* Iniciar timer */
    timer_start();
}

/**
 * @brief Ejemplo de uso
 */
int main(void) {
    /* Inicializar timer */
    timer_ejemplo_init();
    
    while(1) {
        /* Obtener tiempo actual */
        uint32_t tiempo = get_millis();
        
        /* Delay de 1 segundo */
        delay_ms(1000);
        
        /* Hacer algo cada segundo */
    }
    
    return 0;
}
