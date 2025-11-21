/**
 * @file multiplexacion_display.c
 * @brief Multiplexación de displays 7 segmentos
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Multiplexación de 4 displays 7 segmentos para mostrar números.
 *   Usa MEF para ciclar entre displays evitando flicker.
 * 
 * Hardware:
 *   - STM32F407VET6
 *   - Segmentos a-g en PA0-PA6
 *   - Punto decimal en PA7
 *   - Selección displays en PB0-PB3
 * 
 * Notas:
 *   - Llamar mef_multiplexacion() cada 1-2ms
 *   - Displays cátodo común (selección activa en bajo)
 */

#include <stdint.h>

/* Estructuras GPIO */
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
#define GPIOB ((GPIO_TypeDef*)0x40020400)

/* ========== Definiciones ========== */

/* Estados de multiplexación */
typedef enum {
    DISPLAY_1,
    DISPLAY_2,
    DISPLAY_3,
    DISPLAY_4
} EstadoDisplay_t;

/* Variables globales */
static EstadoDisplay_t display_actual = DISPLAY_1;
static uint8_t digitos[4] = {0, 0, 0, 0};  /* Valores a mostrar (0-9) */

/* Tabla de codificación 7 segmentos (cátodo común) */
/*      a
 *     ---
 *  f |   | b
 *     -g-
 *  e |   | c
 *     ---
 *      d    dp
 */
const uint8_t tabla_7seg[10] = {
    0x3F,  /* 0: abcdef   */
    0x06,  /* 1: bc       */
    0x5B,  /* 2: abdeg    */
    0x4F,  /* 3: abcdg    */
    0x66,  /* 4: bcfg     */
    0x6D,  /* 5: acdfg    */
    0x7D,  /* 6: acdefg   */
    0x07,  /* 7: abc      */
    0x7F,  /* 8: abcdefg  */
    0x6F   /* 9: abcdfg   */
};

/* ========== Funciones ========== */

/**
 * @brief Escribir patrón a segmentos (PA0-PA7)
 * @param patron Byte con patrón de segmentos
 */
static void escribir_segmentos(uint8_t patron) {
    /* Escribir cada bit al pin correspondiente */
    for (int i = 0; i < 8; i++) {
        if (patron & (1 << i)) {
            GPIOA->BSRR = (1 << i);          /* Set bit */
        } else {
            GPIOA->BSRR = (1 << (i + 16));   /* Reset bit */
        }
    }
}

/**
 * @brief Seleccionar display activo (PB0-PB3)
 * @param display Número de display (0-3)
 * @note Displays cátodo común: 0=activo, 1=inactivo
 */
static void seleccionar_display(uint8_t display) {
    /* Desactivar todos (escribir 1) */
    GPIOB->BSRR = (0xF << 0);  /* PB0-PB3 = 1 */
    
    /* Activar el seleccionado (escribir 0) */
    GPIOB->BSRR = (1 << (display + 16));  /* Reset bit */
}

/**
 * @brief MEF de multiplexación
 * @note Llamar cada 1-2ms para evitar flicker
 */
void mef_multiplexacion(void) {
    uint8_t digito;
    uint8_t patron;
    
    switch (display_actual) {
        case DISPLAY_1:
            digito = digitos[0];
            patron = tabla_7seg[digito];
            seleccionar_display(0);
            escribir_segmentos(patron);
            display_actual = DISPLAY_2;
            break;
            
        case DISPLAY_2:
            digito = digitos[1];
            patron = tabla_7seg[digito];
            seleccionar_display(1);
            escribir_segmentos(patron);
            display_actual = DISPLAY_3;
            break;
            
        case DISPLAY_3:
            digito = digitos[2];
            patron = tabla_7seg[digito];
            seleccionar_display(2);
            escribir_segmentos(patron);
            display_actual = DISPLAY_4;
            break;
            
        case DISPLAY_4:
            digito = digitos[3];
            patron = tabla_7seg[digito];
            seleccionar_display(3);
            escribir_segmentos(patron);
            display_actual = DISPLAY_1;
            break;
    }
}

/**
 * @brief Actualizar valor mostrado en displays
 * @param valor Valor de 0 a 9999
 */
void actualizar_displays(uint16_t valor) {
    /* Limitar a 9999 */
    if (valor > 9999) valor = 9999;
    
    /* Separar dígitos */
    digitos[0] = (valor / 1000) % 10;  /* Miles */
    digitos[1] = (valor / 100) % 10;   /* Centenas */
    digitos[2] = (valor / 10) % 10;    /* Decenas */
    digitos[3] = valor % 10;           /* Unidades */
}

/**
 * @brief Inicializar hardware para displays
 */
void init_displays(void) {
    /* Habilitar clocks */
    volatile uint32_t *RCC_AHB1ENR = (volatile uint32_t*)0x40023830;
    *RCC_AHB1ENR |= (1 << 0);  /* GPIOA */
    *RCC_AHB1ENR |= (1 << 1);  /* GPIOB */
    
    /* Configurar PA0-PA7 como salidas (segmentos) */
    GPIOA->MODER &= ~0xFFFF;     /* Limpiar bits 0-15 */
    GPIOA->MODER |= 0x5555;      /* Establecer 01 para cada pin */
    
    /* Configurar PB0-PB3 como salidas (selección) */
    GPIOB->MODER &= ~0xFF;       /* Limpiar bits 0-7 */
    GPIOB->MODER |= 0x55;        /* Establecer 01 para cada pin */
    
    /* Desactivar todos los displays inicialmente */
    GPIOB->BSRR = (0xF << 0);    /* PB0-PB3 = 1 */
}

/**
 * @brief Ejemplo de uso
 */
int main(void) {
    /* Inicializar hardware */
    init_displays();
    
    /* Inicializar timer para llamar mef_multiplexacion() cada 1ms */
    /* ... código de timer ... */
    
    /* Mostrar número 1234 */
    actualizar_displays(1234);
    
    uint16_t contador = 0;
    
    while(1) {
        /* Actualizar contador cada segundo */
        /* (delay_ms requiere timer configurado) */
        // delay_ms(1000);
        // contador++;
        // if (contador > 9999) contador = 0;
        // actualizar_displays(contador);
    }
    
    return 0;
}
