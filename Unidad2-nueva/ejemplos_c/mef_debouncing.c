/**
 * @file mef_debouncing.c
 * @brief MEF para antirrebote de botón (debouncing)
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Implementación de MEF de 4 estados para eliminar rebotes
 *   mecánicos de un botón. Valida pulsaciones con tiempo de debounce.
 * 
 * Hardware:
 *   - STM32F407VET6
 *   - Botón en PC13 (con pull-up interno)
 * 
 * Notas:
 *   - Llamar procesar_debounce() cada 1ms desde interrupción de timer
 *   - Usar obtener_estado_boton() para leer estado estable
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

#define GPIOC ((GPIO_TypeDef*)0x40020800)

/* ========== Definiciones de la MEF ========== */

/* Estados de la MEF de debouncing */
typedef enum {
    BTN_LIBERADO,
    BTN_DEBOUNCE_PRESS,
    BTN_PRESIONADO,
    BTN_DEBOUNCE_RELEASE
} EstadoBoton_t;

/* Variables de la MEF */
static EstadoBoton_t estado_boton = BTN_LIBERADO;
static uint32_t tiempo_en_estado = 0;
static uint8_t boton_estable = 0;  /* Salida: 0=liberado, 1=presionado */

/* Constantes */
#define TIEMPO_DEBOUNCE_MS 30
#define PIN_BOTON 13  /* PC13 */

/* ========== Funciones Privadas ========== */

/**
 * @brief Leer estado físico del botón (con ruido)
 * @return 1 si presionado, 0 si liberado
 * @note Asume botón activo bajo con pull-up (0=presionado, 1=liberado)
 */
static uint8_t leer_boton_fisico(void) {
    /* Leer IDR y invertir lógica (activo bajo) */
    uint8_t estado = (GPIOC->IDR & (1 << PIN_BOTON)) ? 0 : 1;
    return estado;
}

/* ========== Funciones Públicas ========== */

/**
 * @brief Obtener estado estable del botón (salida de MEF)
 * @return 1 si presionado, 0 si liberado
 */
uint8_t obtener_estado_boton(void) {
    return boton_estable;
}

/**
 * @brief Procesar MEF de antirrebote
 * @note DEBE llamarse cada 1ms desde interrupción de timer
 */
void procesar_debounce(void) {
    uint8_t boton_actual = leer_boton_fisico();
    
    switch (estado_boton) {
        case BTN_LIBERADO:
            /* Salida del estado */
            boton_estable = 0;
            
            /* Transición: detectar posible pulsación */
            if (boton_actual == 1) {
                estado_boton = BTN_DEBOUNCE_PRESS;
                tiempo_en_estado = 0;
            }
            break;
            
        case BTN_DEBOUNCE_PRESS:
            /* Mantener salida anterior durante debounce */
            boton_estable = 0;
            
            /* Esperar tiempo de debounce */
            if (tiempo_en_estado >= TIEMPO_DEBOUNCE_MS) {
                if (boton_actual == 1) {
                    /* Pulsación confirmada */
                    estado_boton = BTN_PRESIONADO;
                } else {
                    /* Fue un glitch, volver a liberado */
                    estado_boton = BTN_LIBERADO;
                }
                tiempo_en_estado = 0;
            }
            break;
            
        case BTN_PRESIONADO:
            /* Salida del estado */
            boton_estable = 1;
            
            /* Transición: detectar posible liberación */
            if (boton_actual == 0) {
                estado_boton = BTN_DEBOUNCE_RELEASE;
                tiempo_en_estado = 0;
            }
            break;
            
        case BTN_DEBOUNCE_RELEASE:
            /* Mantener salida anterior durante debounce */
            boton_estable = 1;
            
            /* Esperar tiempo de debounce */
            if (tiempo_en_estado >= TIEMPO_DEBOUNCE_MS) {
                if (boton_actual == 0) {
                    /* Liberación confirmada */
                    estado_boton = BTN_LIBERADO;
                } else {
                    /* Fue un glitch, volver a presionado */
                    estado_boton = BTN_PRESIONADO;
                }
                tiempo_en_estado = 0;
            }
            break;
            
        default:
            /* Estado inválido - recuperación */
            estado_boton = BTN_LIBERADO;
            tiempo_en_estado = 0;
            boton_estable = 0;
            break;
    }
    
    tiempo_en_estado++;
}

/**
 * @brief Inicializar hardware para botón
 */
void init_boton(void) {
    /* Habilitar clock GPIOC (bit 2 de RCC_AHB1ENR) */
    volatile uint32_t *RCC_AHB1ENR = (volatile uint32_t*)0x40023830;
    *RCC_AHB1ENR |= (1 << 2);
    
    /* Configurar PC13 como entrada (MODER bits 26-27 = 00) */
    GPIOC->MODER &= ~(0x3 << (PIN_BOTON * 2));
    
    /* Configurar pull-up (PUPDR bits 26-27 = 01) */
    GPIOC->PUPDR &= ~(0x3 << (PIN_BOTON * 2));
    GPIOC->PUPDR |= (0x1 << (PIN_BOTON * 2));
}

/**
 * @brief Ejemplo de uso en main
 */
int main(void) {
    /* Inicializar hardware */
    init_boton();
    /* ... inicializar timer para llamar procesar_debounce() cada 1ms ... */
    
    while(1) {
        /* Leer estado estable del botón */
        if (obtener_estado_boton()) {
            /* Botón presionado - hacer algo */
        } else {
            /* Botón liberado */
        }
    }
    
    return 0;
}
