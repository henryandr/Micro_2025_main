/**
 * @file irq_handler.c
 * @brief Implementación de funciones de manejo de interrupciones
 * @author Curso Microprocesadores
 * @date 2025
 */

#include "irq_handler.h"

/* NVIC Registers */
#define NVIC_ISER_BASE ((volatile uint32_t*)0xE000E100)  /* Interrupt Set-Enable */
#define NVIC_ICER_BASE ((volatile uint32_t*)0xE000E180)  /* Interrupt Clear-Enable */
#define NVIC_IPR_BASE  ((volatile uint8_t*)0xE000E400)   /* Interrupt Priority */

void IRQ_SetPriority(uint8_t irq_num, uint8_t priority) {
    /* Escribir en IPR (cada IRQ tiene 8 bits, pero solo usan 4 superiores) */
    NVIC_IPR_BASE[irq_num] = (priority << 4);
}

void IRQ_EnableIRQ(uint8_t irq_num) {
    /* Determinar registro (cada registro controla 32 IRQs) */
    uint8_t reg_index = irq_num / 32;
    uint8_t bit_position = irq_num % 32;
    
    /* Set bit en ISER */
    NVIC_ISER_BASE[reg_index] = (1 << bit_position);
}

void IRQ_DisableIRQ(uint8_t irq_num) {
    /* Determinar registro */
    uint8_t reg_index = irq_num / 32;
    uint8_t bit_position = irq_num % 32;
    
    /* Set bit en ICER (escribir 1 deshabilita) */
    NVIC_ICER_BASE[reg_index] = (1 << bit_position);
}
