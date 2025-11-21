/**
 * @file mef_simple.c
 * @brief Ejemplo simple de Máquina de Estados Finitos en C
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   MEF básica de 2 estados para LED parpadeante.
 *   Demuestra uso de enum y switch-case para implementar MEF.
 * 
 * Hardware:
 *   - STM32F407VET6
 *   - LED en PA6
 * 
 * Notas:
 *   - Requiere función get_millis() que retorna tiempo en ms
 *   - Llamar mef_blink() frecuentemente desde main loop
 */

#include <stdint.h>

/* Estructura GPIO (definir o incluir desde stm32f407xx.h) */
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

/* Estados de la MEF */
typedef enum {
    LED_APAGADO,
    LED_ENCENDIDO
} EstadoLED_t;

/* Variables globales de la MEF */
static EstadoLED_t estado_actual = LED_APAGADO;
static uint32_t tiempo_ultimo_cambio = 0;

/* Constantes de timing (milisegundos) */
#define TIEMPO_ON_MS  500
#define TIEMPO_OFF_MS 500

/* ========== Funciones ========== */

/**
 * @brief Obtener tiempo actual en milisegundos
 * @return Tiempo en ms desde inicio
 * @note Esta función debe ser implementada con timer (ver timer_driver.c)
 */
extern uint32_t get_millis(void);

/**
 * @brief Máquina de Estados Finitos para LED parpadeante
 * @note Llamar esta función frecuentemente desde main loop
 */
void mef_blink(void) {
    uint32_t tiempo_actual = get_millis();
    uint32_t tiempo_en_estado = tiempo_actual - tiempo_ultimo_cambio;
    
    switch (estado_actual) {
        case LED_APAGADO:
            /* Acción: mantener LED apagado */
            GPIOA->BSRR = (1 << (6 + 16));  /* PA6 OFF */
            
            /* Verificar transición: después de TIEMPO_OFF_MS */
            if (tiempo_en_estado >= TIEMPO_OFF_MS) {
                estado_actual = LED_ENCENDIDO;
                tiempo_ultimo_cambio = tiempo_actual;
            }
            break;
            
        case LED_ENCENDIDO:
            /* Acción: mantener LED encendido */
            GPIOA->BSRR = (1 << 6);   /* PA6 ON */
            
            /* Verificar transición: después de TIEMPO_ON_MS */
            if (tiempo_en_estado >= TIEMPO_ON_MS) {
                estado_actual = LED_APAGADO;
                tiempo_ultimo_cambio = tiempo_actual;
            }
            break;
            
        default:
            /* Estado inválido - recuperación */
            estado_actual = LED_APAGADO;
            tiempo_ultimo_cambio = tiempo_actual;
            break;
    }
}

/**
 * @brief Ejemplo de main loop usando la MEF
 */
int main(void) {
    /* Inicializar hardware (GPIO, Timer) */
    /* ... código de inicialización ... */
    
    while(1) {
        /* Llamar la MEF continuamente */
        mef_blink();
        
        /* Otras tareas pueden ejecutarse aquí */
    }
    
    return 0;
}
