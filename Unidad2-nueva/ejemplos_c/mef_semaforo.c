/**
 * @file mef_semaforo.c
 * @brief Semáforo simple implementado con MEF
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Semáforo de 3 estados (Verde, Amarillo, Rojo) con tiempos definidos.
 *   Demuestra MEF de Moore para control secuencial.
 * 
 * Hardware:
 *   - STM32F407VET6
 *   - LED Verde en PA5
 *   - LED Amarillo en PA6
 *   - LED Rojo en PA7
 * 
 * Notas:
 *   - Llamar mef_semaforo() cada 1ms desde interrupción de timer
 */

#include <stdint.h>

/* Estructura GPIO */
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef*)0x40020000)

/* ========== Definiciones de la MEF ========== */

/* Estados del semáforo */
typedef enum {
    SEMAFORO_VERDE,
    SEMAFORO_AMARILLO,
    SEMAFORO_ROJO
} EstadoSemaforo_t;

/* Variables globales */
static EstadoSemaforo_t estado_actual = SEMAFORO_VERDE;
static uint32_t tiempo_en_estado = 0;

/* Tiempos en milisegundos */
#define TIEMPO_VERDE     5000   /* 5 segundos */
#define TIEMPO_AMARILLO  2000   /* 2 segundos */
#define TIEMPO_ROJO      5000   /* 5 segundos */

/* Pines de LEDs */
#define PIN_VERDE    5
#define PIN_AMARILLO 6
#define PIN_ROJO     7

/* ========== Funciones ========== */

/**
 * @brief Actualizar LEDs según estado actual
 */
static void actualizar_leds(void) {
    /* Apagar todos los LEDs primero */
    GPIOA->BSRR = (1 << (PIN_VERDE + 16));     /* Verde OFF */
    GPIOA->BSRR = (1 << (PIN_AMARILLO + 16));  /* Amarillo OFF */
    GPIOA->BSRR = (1 << (PIN_ROJO + 16));      /* Rojo OFF */
    
    /* Encender LED según estado */
    switch (estado_actual) {
        case SEMAFORO_VERDE:
            GPIOA->BSRR = (1 << PIN_VERDE);
            break;
            
        case SEMAFORO_AMARILLO:
            GPIOA->BSRR = (1 << PIN_AMARILLO);
            break;
            
        case SEMAFORO_ROJO:
            GPIOA->BSRR = (1 << PIN_ROJO);
            break;
    }
}

/**
 * @brief Máquina de Estados Finitos del semáforo
 * @note Llamar cada 1ms desde interrupción de timer
 */
void mef_semaforo(void) {
    /* Actualizar salidas (LEDs) según estado */
    actualizar_leds();
    
    /* Evaluar transiciones */
    switch (estado_actual) {
        case SEMAFORO_VERDE:
            if (tiempo_en_estado >= TIEMPO_VERDE) {
                estado_actual = SEMAFORO_AMARILLO;
                tiempo_en_estado = 0;
            }
            break;
            
        case SEMAFORO_AMARILLO:
            if (tiempo_en_estado >= TIEMPO_AMARILLO) {
                estado_actual = SEMAFORO_ROJO;
                tiempo_en_estado = 0;
            }
            break;
            
        case SEMAFORO_ROJO:
            if (tiempo_en_estado >= TIEMPO_ROJO) {
                estado_actual = SEMAFORO_VERDE;
                tiempo_en_estado = 0;
            }
            break;
            
        default:
            /* Estado inválido - recuperación */
            estado_actual = SEMAFORO_VERDE;
            tiempo_en_estado = 0;
            break;
    }
    
    tiempo_en_estado++;
}

/**
 * @brief Inicializar hardware del semáforo
 */
void init_semaforo(void) {
    /* Habilitar clock GPIOA */
    volatile uint32_t *RCC_AHB1ENR = (volatile uint32_t*)0x40023830;
    *RCC_AHB1ENR |= (1 << 0);
    
    /* Configurar PA5, PA6, PA7 como salidas */
    /* Limpiar bits de MODER */
    GPIOA->MODER &= ~((0x3 << (PIN_VERDE * 2)) | 
                      (0x3 << (PIN_AMARILLO * 2)) | 
                      (0x3 << (PIN_ROJO * 2)));
    
    /* Establecer como salidas (01) */
    GPIOA->MODER |= ((0x1 << (PIN_VERDE * 2)) | 
                     (0x1 << (PIN_AMARILLO * 2)) | 
                     (0x1 << (PIN_ROJO * 2)));
    
    /* Configurar como push-pull (por defecto en 0) */
    GPIOA->OTYPER &= ~((1 << PIN_VERDE) | (1 << PIN_AMARILLO) | (1 << PIN_ROJO));
    
    /* Configurar velocidad media */
    GPIOA->OSPEEDR &= ~((0x3 << (PIN_VERDE * 2)) | 
                        (0x3 << (PIN_AMARILLO * 2)) | 
                        (0x3 << (PIN_ROJO * 2)));
    GPIOA->OSPEEDR |= ((0x1 << (PIN_VERDE * 2)) | 
                       (0x1 << (PIN_AMARILLO * 2)) | 
                       (0x1 << (PIN_ROJO * 2)));
}

/**
 * @brief Ejemplo de main
 */
int main(void) {
    /* Inicializar hardware */
    init_semaforo();
    
    /* Inicializar timer para llamar mef_semaforo() cada 1ms */
    /* ... código de timer ... */
    
    while(1) {
        /* Main loop puede hacer otras tareas */
    }
    
    return 0;
}
