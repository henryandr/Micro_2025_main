# Sesión 7: Interrupciones Externas (EXTI) y NVIC en C

**Duración**: 2 horas  
**Modalidad**: Presencial con práctica de interrupciones

---

## Objetivos Específicos

1. **Configurar** interrupciones externas (EXTI) en pines GPIO
2. **Implementar** handlers de interrupción en C
3. **Integrar** interrupciones con MEF para sistemas reactivos

---

## Contenidos Temáticos

### 1. Interrupciones Externas (EXTI) (25 minutos)

#### Conceptos

**EXTI**: External Interrupt/Event Controller
- Detecta cambios en pines GPIO
- Genera interrupción al CPU
- Puede configurarse para flanco subida, bajada, o ambos

**Ventajas sobre polling**:
- CPU libre para otras tareas
- Respuesta más rápida
- Menor consumo de energía

#### Registros EXTI

```c
typedef struct {
    volatile uint32_t IMR;    // 0x00 - Interrupt Mask
    volatile uint32_t EMR;    // 0x04 - Event Mask
    volatile uint32_t RTSR;   // 0x08 - Rising Trigger Selection
    volatile uint32_t FTSR;   // 0x0C - Falling Trigger Selection
    volatile uint32_t SWIER;  // 0x10 - Software Interrupt Event
    volatile uint32_t PR;     // 0x14 - Pending Register
} EXTI_TypeDef;

#define EXTI_BASE 0x40013C00
#define EXTI ((EXTI_TypeDef*)EXTI_BASE)
```

#### SYSCFG para Mapeo

**Problema**: Múltiples puertos comparten misma línea EXTI
- PA0, PB0, PC0, PD0, PE0 → EXTI0
- PA1, PB1, PC1, PD1, PE1 → EXTI1
- etc.

**Solución**: SYSCFG_EXTICR selecciona qué puerto mapear

```c
typedef struct {
    volatile uint32_t MEMRMP;     // 0x00
    volatile uint32_t PMC;        // 0x04
    volatile uint32_t EXTICR[4];  // 0x08-0x14
} SYSCFG_TypeDef;

#define SYSCFG_BASE 0x40013800
#define SYSCFG ((SYSCFG_TypeDef*)SYSCFG_BASE)
```

### 2. Configuración Paso a Paso (30 minutos)

#### Ejemplo: Botón en PC13 con Interrupción

```c
/**
 * @brief Configurar interrupción en PC13 (flanco de bajada)
 */
void EXTI13_Config(void) {
    // 1. Habilitar clocks
    RCC->AHB1ENR |= (1 << 2);  // GPIOC
    RCC->APB2ENR |= (1 << 14); // SYSCFG
    
    // 2. Configurar pin como entrada con pull-up
    GPIO_Config_t boton = {
        .port = GPIOC,
        .pin = 13,
        .mode = GPIO_MODE_INPUT,
        .pupd = GPIO_PUPD_PU
    };
    GPIO_Init(&boton);
    
    // 3. Mapear PC13 a EXTI13 (SYSCFG_EXTICR4)
    // EXTI13 usa bits 4-7 de EXTICR4
    SYSCFG->EXTICR[3] &= ~(0xF << 4);  // Limpiar
    SYSCFG->EXTICR[3] |= (0x2 << 4);   // 0x2 = Port C
    
    // 4. Configurar EXTI13 para flanco de bajada
    EXTI->FTSR |= (1 << 13);  // Falling trigger
    EXTI->RTSR &= ~(1 << 13); // No rising trigger
    
    // 5. Habilitar interrupción EXTI13
    EXTI->IMR |= (1 << 13);
    
    // 6. Habilitar en NVIC (EXTI15_10_IRQn = 40)
    uint32_t *NVIC_ISER1 = (uint32_t*)0xE000E104;
    *NVIC_ISER1 |= (1 << (40 - 32));
}

/**
 * @brief Handler de interrupción EXTI15_10
 * @note PC13 comparte handler con EXTI10-15
 */
void EXTI15_10_IRQHandler(void) {
    // Verificar si fue EXTI13
    if (EXTI->PR & (1 << 13)) {
        // Tu código aquí
        // Por ejemplo: establecer flag
        boton_presionado_flag = 1;
        
        // IMPORTANTE: Limpiar flag de pending
        EXTI->PR = (1 << 13);  // Write 1 to clear
    }
}
```

### 3. Integración con MEF (30 minutos)

#### Patrón: Flag + Main Loop

```c
// Variable compartida entre ISR y main
volatile uint8_t evento_boton = 0;

void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & (1 << 13)) {
        evento_boton = 1;  // Establecer flag
        EXTI->PR = (1 << 13);
    }
}

// MEF en main
typedef enum {
    ESTADO_IDLE,
    ESTADO_PROCESANDO,
    ESTADO_FINALIZADO
} Estado_t;

void mef_con_interrupciones(void) {
    static Estado_t estado = ESTADO_IDLE;
    
    switch (estado) {
        case ESTADO_IDLE:
            if (evento_boton) {
                evento_boton = 0;  // Limpiar flag
                estado = ESTADO_PROCESANDO;
                // Iniciar procesamiento
            }
            break;
            
        case ESTADO_PROCESANDO:
            // Hacer trabajo
            if (trabajo_completado()) {
                estado = ESTADO_FINALIZADO;
            }
            break;
            
        case ESTADO_FINALIZADO:
            // Mostrar resultado
            estado = ESTADO_IDLE;
            break;
    }
}
```

#### Debouncing con Interrupción

```c
// Combinar EXTI con MEF de debounce
volatile uint8_t evento_interrupcion = 0;
uint32_t tiempo_ultima_interrupcion = 0;

void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & (1 << 13)) {
        uint32_t tiempo_actual = get_millis();
        
        // Debounce por software (30ms)
        if ((tiempo_actual - tiempo_ultima_interrupcion) > 30) {
            evento_interrupcion = 1;
            tiempo_ultima_interrupcion = tiempo_actual;
        }
        
        EXTI->PR = (1 << 13);
    }
}
```

### 4. Prioridades de Interrupciones (15 minutos)

#### Configuración de Prioridad

```c
/**
 * @brief Configurar prioridad de interrupción
 * @param irq_num Número IRQ
 * @param priority Prioridad (0-15, menor = mayor prioridad)
 */
void NVIC_SetPriority(uint8_t irq_num, uint8_t priority) {
    volatile uint8_t *IPR = (volatile uint8_t*)(0xE000E400 + irq_num);
    *IPR = (priority << 4);  // Solo usan 4 bits superiores
}

// Ejemplo
NVIC_SetPriority(40, 2);  // EXTI15_10 con prioridad 2
NVIC_SetPriority(28, 1);  // TIM2 con prioridad 1 (mayor)
```

---

## Actividades

### Actividad 1: Contador con Botón e Interrupción (30 minutos)

**Requisitos**:
- Contador de 0-99
- Botón incrementa (interrupción)
- Display 7 segmentos (multiplexado)
- Debounce en ISR

### Actividad 2: Sistema de Alarma (35 minutos)

**Estados**:
- DESARMADA
- ARMADA
- DISPARADA

**Eventos**:
- Botón armar/desarmar (interrupción)
- Sensor PIR (interrupción)
- Timeout (timer)

### Actividad 3: Múltiples Interrupciones (25 minutos)

**Sistema**: 2 botones, 2 interrupciones
- Botón 1 (PC13): Incrementar
- Botón 2 (PA0): Decrementar
- Mostrar valor en LEDs

---

## Evaluación Formativa

### Quiz - Sesión 7

1. **¿Qué hace el registro EXTI PR?** (2 puntos)
2. **¿Por qué usar SYSCFG_EXTICR?** (2 puntos)
3. **¿Qué pasa si no se limpia el flag en ISR?** (2 puntos)
4. **¿Prioridad 0 o 15 es mayor?** (2 puntos)
5. **¿Cómo comunicar evento de ISR a main?** (2 puntos)

---

## Evidencias de Aprendizaje

**Entregable**: Sistema de control de acceso

**Especificaciones**:
- Botón entrada (interrupción)
- Botón salida (interrupción)
- Contador de personas (0-99)
- Alarma si > capacidad máxima (configurable)
- Display muestra count actual
- LED verde: OK, LED rojo: lleno

**Evaluación**:
- Funcionalidad: 35%
- Interrupciones correctas: 30%
- MEF bien diseñada: 20%
- Código limpio: 15%

---

## Material para Casa

### Preparación Sesión 8
- Revisar todos los conceptos de la unidad
- Preparar proyecto final
- Repasar MEF, GPIO, Timer, EXTI

**Próxima sesión**: Proyecto Integrador y Evaluación Sumativa
