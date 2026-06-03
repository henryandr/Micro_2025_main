/**
 * @file irq_handler.h
 * @brief Manejo de interrupciones
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Funciones auxiliares para manejo de interrupciones.
 *   Incluye habilitar/deshabilitar interrupciones globales.
 */

#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

#include <stdint.h>

/**
 * @brief Habilita interrupciones globales
 */
static inline void IRQ_Enable(void) {
    __asm volatile ("cpsie i" : : : "memory");
}

/**
 * @brief Deshabilita interrupciones globales
 */
static inline void IRQ_Disable(void) {
    __asm volatile ("cpsid i" : : : "memory");
}

/**
 * @brief Configura prioridad de una interrupción
 * @param irq_num Número de IRQ
 * @param priority Prioridad (0-15, menor número = mayor prioridad)
 */
void IRQ_SetPriority(uint8_t irq_num, uint8_t priority);

/**
 * @brief Habilita una interrupción específica en NVIC
 * @param irq_num Número de IRQ
 */
void IRQ_EnableIRQ(uint8_t irq_num);

/**
 * @brief Deshabilita una interrupción específica en NVIC
 * @param irq_num Número de IRQ
 */
void IRQ_DisableIRQ(uint8_t irq_num);

#endif /* IRQ_HANDLER_H */
