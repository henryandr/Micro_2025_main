/**
 * @file main_blink.c
 * @brief Primer programa en C bare metal - Blink LED
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Programa básico que hace parpadear un LED usando C bare metal.
 *   Demuestra configuración de GPIO y delay simple.
 * 
 * Hardware:
 *   - STM32F407VET6
 *   - LED en PA6
 * 
 * Compilación:
 *   arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O2 -c main_blink.c
 */

#include <stdint.h>

/* ========== Definiciones de Hardware ========== */

/* Direcciones base de periféricos */
#define PERIPH_BASE       0x40000000UL
#define AHB1PERIPH_BASE   (PERIPH_BASE + 0x00020000UL)
#define RCC_BASE          (AHB1PERIPH_BASE + 0x3800UL)
#define GPIOA_BASE        (AHB1PERIPH_BASE + 0x0000UL)

/* Estructura para GPIO */
typedef struct {
    volatile uint32_t MODER;     /* Offset 0x00: Mode register */
    volatile uint32_t OTYPER;    /* Offset 0x04: Output type register */
    volatile uint32_t OSPEEDR;   /* Offset 0x08: Output speed register */
    volatile uint32_t PUPDR;     /* Offset 0x0C: Pull-up/pull-down register */
    volatile uint32_t IDR;       /* Offset 0x10: Input data register */
    volatile uint32_t ODR;       /* Offset 0x14: Output data register */
    volatile uint32_t BSRR;      /* Offset 0x18: Bit set/reset register */
    volatile uint32_t LCKR;      /* Offset 0x1C: Lock register */
    volatile uint32_t AFR[2];    /* Offset 0x20-0x24: Alternate function registers */
} GPIO_TypeDef;

/* Puntero a GPIOA */
#define GPIOA ((GPIO_TypeDef*)GPIOA_BASE)

/* Registro RCC_AHB1ENR */
#define RCC_AHB1ENR (*((volatile uint32_t*)(RCC_BASE + 0x30)))

/* ========== Funciones ========== */

/**
 * @brief Delay simple usando busy-wait
 * @param count Número de iteraciones
 * @note No es preciso, solo para demostración
 */
void delay(volatile uint32_t count) {
    while(count--);
}

/**
 * @brief Función principal
 * @return Nunca retorna
 */
int main(void) {
    /* 1. Habilitar clock de GPIOA (bit 0 de RCC_AHB1ENR) */
    RCC_AHB1ENR |= (1 << 0);
    
    /* 2. Configurar PA6 como salida (MODER bits 12-13 = 01) */
    GPIOA->MODER &= ~(0x3 << 12);  /* Limpiar bits 12-13 */
    GPIOA->MODER |= (0x1 << 12);   /* Establecer 01 (salida) */
    
    /* 3. Configurar como push-pull (OTYPER bit 6 = 0, ya está por defecto) */
    GPIOA->OTYPER &= ~(1 << 6);
    
    /* 4. Configurar velocidad media (OSPEEDR bits 12-13 = 01) */
    GPIOA->OSPEEDR &= ~(0x3 << 12);
    GPIOA->OSPEEDR |= (0x1 << 12);
    
    /* 5. Sin pull-up/pull-down (PUPDR bits 12-13 = 00) */
    GPIOA->PUPDR &= ~(0x3 << 12);
    
    /* 6. Bucle principal - parpadear LED */
    while(1) {
        /* Encender LED usando BSRR (operación atómica) */
        GPIOA->BSRR = (1 << 6);      /* Set bit 6 */
        delay(1000000);
        
        /* Apagar LED */
        GPIOA->BSRR = (1 << (6 + 16)); /* Reset bit 6 (escribir en bit 22) */
        delay(1000000);
    }
    
    return 0;  /* Nunca se alcanza */
}
