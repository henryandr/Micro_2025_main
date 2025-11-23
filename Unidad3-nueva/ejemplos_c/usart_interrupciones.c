/**
 * @file usart_interrupciones.c
 * @brief Ejemplo de USART con interrupciones y buffer circular
 * @details Implementa TX/RX por interrupciones con buffers circulares
 */

#include "stm32f4xx.h"
#include <stdint.h>

#define BUFFER_SIZE 256

// Buffer circular para RX
typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} RingBuffer_t;

RingBuffer_t usart2_rx_buffer;
RingBuffer_t usart2_tx_buffer;

// Prototipos
void USART2_Init_IT(void);
int RingBuffer_Write(RingBuffer_t *rb, uint8_t data);
int RingBuffer_Read(RingBuffer_t *rb);

int main(void) {
    USART2_Init_IT();
    
    while (1) {
        // El CPU está libre para otras tareas
        // La comunicación ocurre en segundo plano vía interrupciones
        int byte = RingBuffer_Read(&usart2_rx_buffer);
        if (byte >= 0) {
            // Echo del byte recibido
            RingBuffer_Write(&usart2_tx_buffer, (uint8_t)byte);
            USART2->CR1 |= USART_CR1_TXEIE;  // Habilitar TX interrupt
        }
    }
}

/**
 * @brief Handler de interrupción USART2
 */
void USART2_IRQHandler(void) {
    // RX interrupt
    if (USART2->SR & USART_SR_RXNE) {
        uint8_t data = USART2->DR;
        RingBuffer_Write(&usart2_rx_buffer, data);
    }
    
    // TX interrupt
    if ((USART2->SR & USART_SR_TXE) && (USART2->CR1 & USART_CR1_TXEIE)) {
        int data = RingBuffer_Read(&usart2_tx_buffer);
        if (data >= 0) {
            USART2->DR = (uint8_t)data;
        } else {
            USART2->CR1 &= ~USART_CR1_TXEIE;  // Deshabilitar TX interrupt
        }
    }
}

void USART2_Init_IT(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    
    // Configurar pines (ver ejemplo básico)
    GPIOA->MODER &= ~(0xF << (2*2));
    GPIOA->MODER |= (0xA << (2*2));
    GPIOA->AFR[0] &= ~(0xFF << (2*4));
    GPIOA->AFR[0] |= (0x77 << (2*4));
    
    // Configurar USART
    USART2->BRR = 42000000 / 115200;
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
    USART2->CR1 |= USART_CR1_RXNEIE;  // Habilitar RX interrupt
    USART2->CR1 |= USART_CR1_UE;
    
    // Configurar NVIC
    NVIC_SetPriority(USART2_IRQn, 5);
    NVIC_EnableIRQ(USART2_IRQn);
}

int RingBuffer_Write(RingBuffer_t *rb, uint8_t data) {
    uint16_t next = (rb->head + 1) % BUFFER_SIZE;
    if (next == rb->tail) return -1;  // Full
    rb->buffer[rb->head] = data;
    rb->head = next;
    return 0;
}

int RingBuffer_Read(RingBuffer_t *rb) {
    if (rb->head == rb->tail) return -1;  // Empty
    uint8_t data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % BUFFER_SIZE;
    return data;
}
