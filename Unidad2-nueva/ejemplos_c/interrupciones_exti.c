/**
 * @file interrupciones_exti.c
 * @brief Ejemplo de interrupciones externas (EXTI)
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Configuración de interrupción externa en botón.
 *   Demuestra uso de EXTI, SYSCFG y NVIC.
 * 
 * Hardware:
 *   - STM32F407VET6
 *   - Botón en PC13 (con pull-up)
 *   - LED en PA6
 * 
 * Notas:
 *   - Usa interrupción de flanco de bajada
 *   - Incluye debounce por software en ISR
 */

#include <stdint.h>

/* ========== Estructuras de Hardware ========== */

/* GPIO */
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
#define GPIOC ((GPIO_TypeDef*)0x40020800)

/* EXTI */
typedef struct {
    volatile uint32_t IMR;    /* 0x00: Interrupt mask register */
    volatile uint32_t EMR;    /* 0x04: Event mask register */
    volatile uint32_t RTSR;   /* 0x08: Rising trigger selection register */
    volatile uint32_t FTSR;   /* 0x0C: Falling trigger selection register */
    volatile uint32_t SWIER;  /* 0x10: Software interrupt event register */
    volatile uint32_t PR;     /* 0x14: Pending register */
} EXTI_TypeDef;

#define EXTI_BASE 0x40013C00
#define EXTI ((EXTI_TypeDef*)EXTI_BASE)

/* SYSCFG */
typedef struct {
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR[4];
} SYSCFG_TypeDef;

#define SYSCFG_BASE 0x40013800
#define SYSCFG ((SYSCFG_TypeDef*)SYSCFG_BASE)

/* RCC */
#define RCC_AHB1ENR (*((volatile uint32_t*)0x40023830))
#define RCC_APB2ENR (*((volatile uint32_t*)0x40023844))

/* NVIC */
#define NVIC_ISER1 (*((volatile uint32_t*)0xE000E104))

/* ========== Variables Globales ========== */

/* Flag de evento (compartido entre ISR y main) */
volatile uint8_t boton_presionado = 0;

/* Variables para debounce */
extern uint32_t get_millis(void);  /* Requiere timer configurado */
static uint32_t ultima_interrupcion = 0;

/* ========== Funciones ========== */

/**
 * @brief Configurar interrupción en PC13 (flanco de bajada)
 */
void config_exti13(void) {
    /* 1. Habilitar clocks */
    RCC_AHB1ENR |= (1 << 2);   /* GPIOC */
    RCC_APB2ENR |= (1 << 14);  /* SYSCFG */
    
    /* 2. Configurar PC13 como entrada con pull-up */
    GPIOC->MODER &= ~(0x3 << 26);  /* Input mode */
    GPIOC->PUPDR &= ~(0x3 << 26);
    GPIOC->PUPDR |= (0x1 << 26);   /* Pull-up */
    
    /* 3. Mapear PC13 a EXTI13 usando SYSCFG */
    /* EXTI13 usa bits 4-7 de EXTICR[3] */
    SYSCFG->EXTICR[3] &= ~(0xF << 4);  /* Limpiar */
    SYSCFG->EXTICR[3] |= (0x2 << 4);   /* 0x2 = Port C */
    
    /* 4. Configurar EXTI13 para flanco de bajada */
    EXTI->FTSR |= (1 << 13);   /* Falling trigger enable */
    EXTI->RTSR &= ~(1 << 13);  /* Rising trigger disable */
    
    /* 5. Habilitar interrupción EXTI13 */
    EXTI->IMR |= (1 << 13);
    
    /* 6. Habilitar en NVIC */
    /* EXTI15_10_IRQn = 40, NVIC_ISER1 controla IRQs 32-63 */
    NVIC_ISER1 |= (1 << (40 - 32));
}

/**
 * @brief Configurar LED en PA6
 */
void config_led(void) {
    /* Habilitar clock GPIOA */
    RCC_AHB1ENR |= (1 << 0);
    
    /* Configurar PA6 como salida */
    GPIOA->MODER &= ~(0x3 << 12);
    GPIOA->MODER |= (0x1 << 12);
    
    /* Push-pull, velocidad media */
    GPIOA->OTYPER &= ~(1 << 6);
    GPIOA->OSPEEDR &= ~(0x3 << 12);
    GPIOA->OSPEEDR |= (0x1 << 12);
}

/**
 * @brief Toggle LED
 */
void toggle_led(void) {
    GPIOA->ODR ^= (1 << 6);
}

/**
 * @brief Handler de interrupción EXTI15_10
 * @note PC13 comparte handler con EXTI10-15
 */
void EXTI15_10_IRQHandler(void) {
    /* Verificar si fue EXTI13 (PC13) */
    if (EXTI->PR & (1 << 13)) {
        /* Debounce simple por tiempo */
        uint32_t tiempo_actual = get_millis();
        
        if ((tiempo_actual - ultima_interrupcion) > 30) {
            /* Pulsación válida */
            boton_presionado = 1;
            ultima_interrupcion = tiempo_actual;
            
            /* Acción inmediata: toggle LED */
            toggle_led();
        }
        
        /* IMPORTANTE: Limpiar flag de pending */
        EXTI->PR = (1 << 13);  /* Write 1 to clear */
    }
}

/**
 * @brief Ejemplo de main
 */
int main(void) {
    /* Inicializar timer (necesario para get_millis y debounce) */
    /* ... código de timer ... */
    
    /* Configurar hardware */
    config_led();
    config_exti13();
    
    /* Habilitar interrupciones globales */
    __asm volatile ("cpsie i" : : : "memory");
    
    while(1) {
        /* Verificar flag de evento */
        if (boton_presionado) {
            boton_presionado = 0;  /* Limpiar flag */
            
            /* Hacer algo en respuesta al botón */
            /* Por ejemplo: cambiar estado de MEF, iniciar proceso, etc. */
        }
        
        /* Main loop puede hacer otras tareas */
    }
    
    return 0;
}
