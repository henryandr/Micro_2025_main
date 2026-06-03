/**
 * @file usart_basic.c
 * @brief Ejemplo básico de USART: transmisión y recepción por polling
 * 
 * Este programa demuestra el uso básico del USART2 del STM32F407.
 * Implementa un echo: todo lo que se recibe se reenvía de vuelta.
 * 
 * Conexiones:
 * - PA2: USART2 TX
 * - PA3: USART2 RX
 * 
 * Baudrate: 115200
 * Data bits: 8
 * Parity: None
 * Stop bits: 1
 * 
 * @author Generated for Unidad 3
 * @date 2025
 */

#include "stm32f4xx.h"
#include <stdio.h>

// Prototipos de funciones
void SystemClock_Config(void);
void USART2_Init(uint32_t baudrate);
void USART2_SendByte(uint8_t data);
uint8_t USART2_ReceiveByte(void);
void USART2_SendString(const char *str);
void delay_ms(uint32_t ms);

/**
 * @brief Función principal
 */
int main(void) {
    // Configuración del sistema
    SystemClock_Config();
    
    // Inicializar USART2 a 115200 bps
    USART2_Init(115200);
    
    // Mensaje de bienvenida
    USART2_SendString("\r\n");
    USART2_SendString("==================================\r\n");
    USART2_SendString("  USART2 Echo Test - Polling Mode\r\n");
    USART2_SendString("==================================\r\n");
    USART2_SendString("Escriba cualquier texto...\r\n\r\n");
    
    // Loop principal: echo de caracteres
    while (1) {
        // Esperar y recibir un byte
        uint8_t received_byte = USART2_ReceiveByte();
        
        // Hacer echo del byte recibido
        USART2_SendByte(received_byte);
        
        // Si es Enter, agregar salto de línea
        if (received_byte == '\r') {
            USART2_SendByte('\n');
        }
    }
}

/**
 * @brief Inicializa el USART2
 * @param baudrate Velocidad de comunicación deseada
 * 
 * Configuración:
 * - APB1 Clock: 42 MHz
 * - 8 bits de datos
 * - Sin paridad
 * - 1 bit de stop
 */
void USART2_Init(uint32_t baudrate) {
    // 1. Habilitar clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Clock GPIOA
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Clock USART2
    
    // 2. Configurar pines GPIO (PA2=TX, PA3=RX)
    GPIOA->MODER &= ~(0xF << (2*2));  // Limpiar bits de PA2 y PA3
    GPIOA->MODER |= (0xA << (2*2));   // Modo Alternate Function
    
    GPIOA->AFR[0] &= ~(0xFF << (2*4));  // Limpiar AF de PA2 y PA3
    GPIOA->AFR[0] |= (0x77 << (2*4));   // AF7 (USART2)
    
    // 3. Configurar USART2
    USART2->CR1 = 0;  // Limpiar registro de control
    
    // Calcular BRR para el baudrate deseado
    // BRR = fCK / (16 * baudrate)
    // fCK = APB1 = 42 MHz
    uint32_t usartdiv = (42000000 + (baudrate/2)) / baudrate;
    USART2->BRR = usartdiv;
    
    // Habilitar TX y RX
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
    
    // Habilitar USART
    USART2->CR1 |= USART_CR1_UE;
}

/**
 * @brief Transmite un byte por USART2 (polling)
 * @param data Byte a transmitir
 */
void USART2_SendByte(uint8_t data) {
    // Esperar que el registro de transmisión esté vacío
    while (!(USART2->SR & USART_SR_TXE));
    
    // Escribir el dato
    USART2->DR = data;
}

/**
 * @brief Recibe un byte por USART2 (polling)
 * @return Byte recibido
 */
uint8_t USART2_ReceiveByte(void) {
    // Esperar que haya un dato disponible
    while (!(USART2->SR & USART_SR_RXNE));
    
    // Leer y retornar el dato
    return (uint8_t)(USART2->DR & 0xFF);
}

/**
 * @brief Transmite un string por USART2
 * @param str Puntero al string (terminado en null)
 */
void USART2_SendString(const char *str) {
    while (*str) {
        USART2_SendByte(*str++);
    }
}

/**
 * @brief Configuración del reloj del sistema
 * Configura el sistema para correr a 168 MHz usando el PLL
 */
void SystemClock_Config(void) {
    // Esta función configuraría el PLL y los clocks
    // Para simplificar, asumimos que ya está configurado
    // o usamos el reloj interno HSI
    
    // Configurar Flash latency para 168 MHz
    FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
    
    // Aquí iría la configuración completa del PLL
    // Por ahora usamos configuración por defecto
}

/**
 * @brief Delay simple en milisegundos
 * @param ms Cantidad de milisegundos a esperar
 * @note Esta es una implementación simple, no precisa
 */
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 4000; i++) {
        __asm("nop");
    }
}
