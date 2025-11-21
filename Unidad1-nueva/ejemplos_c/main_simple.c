/**
 * @file main_simple.c
 * @brief Ejemplo simple de programa bare metal en C
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Programa mínimo que configura un LED y lo hace parpadear.
 *   Demuestra conceptos básicos de programación bare metal.
 * 
 * Hardware:
 *   - STM32F407VET6
 *   - LED en PA6
 * 
 * Compilación:
 *   arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O2 -c main_simple.c
 */

#include <stdint.h>

/* Direcciones base de periféricos */
#define RCC_BASE        0x40023800
#define GPIOA_BASE      0x40020000

/* Estructura para GPIO */
typedef struct {
    volatile uint32_t MODER;     /* Offset 0x00 */
    volatile uint32_t OTYPER;    /* Offset 0x04 */
    volatile uint32_t OSPEEDR;   /* Offset 0x08 */
    volatile uint32_t PUPDR;     /* Offset 0x0C */
    volatile uint32_t IDR;       /* Offset 0x10 */
    volatile uint32_t ODR;       /* Offset 0x14 */
    volatile uint32_t BSRR;      /* Offset 0x18 */
    volatile uint32_t LCKR;      /* Offset 0x1C */
    volatile uint32_t AFR[2];    /* Offset 0x20-0x24 */
} GPIO_TypeDef;

/* Puntero a GPIOA */
#define GPIOA ((GPIO_TypeDef*)GPIOA_BASE)

/* Registro RCC_AHB1ENR */
#define RCC_AHB1ENR (*((volatile uint32_t*)(RCC_BASE + 0x30)))

/**
 * @brief Delay simple (busy-wait)
 * @param count Número de iteraciones
 * @note No es preciso, solo para demostración
 */
void delay(volatile uint32_t count)
{
    while(count--);
}

/**
 * @brief Función principal
 */
int main(void)
{
    /* 1. Habilitar clock de GPIOA (bit 0 de RCC_AHB1ENR) */
    RCC_AHB1ENR |= (1 << 0);
    
    /* 2. Configurar PA6 como salida */
    /* Limpiar bits 12-13 (modo de PA6) */
    GPIOA->MODER &= ~(0x3 << 12);
    /* Establecer 01 (salida) */
    GPIOA->MODER |= (0x1 << 12);
    
    /* 3. Configurar como push-pull (por defecto, ya está en 0) */
    GPIOA->OTYPER &= ~(1 << 6);
    
    /* 4. Configurar velocidad media */
    GPIOA->OSPEEDR &= ~(0x3 << 12);
    GPIOA->OSPEEDR |= (0x1 << 12);
    
    /* 5. Sin pull-up/pull-down */
    GPIOA->PUPDR &= ~(0x3 << 12);
    
    /* 6. Bucle principal - parpadear LED */
    while(1)
    {
        /* Encender LED (bit 6 = 1) */
        GPIOA->BSRR = (1 << 6);
        delay(1000000);
        
        /* Apagar LED (bit 6+16 = reset) */
        GPIOA->BSRR = (1 << (6 + 16));
        delay(1000000);
        
        /* Alternativa con toggle */
        /* GPIOA->ODR ^= (1 << 6); */
    }
    
    return 0;  /* Nunca se alcanza */
}
