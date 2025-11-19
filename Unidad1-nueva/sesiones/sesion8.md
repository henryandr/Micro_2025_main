# Sesión 8: Interrupciones y Evaluación Sumativa

**Duración**: 2 horas  
**Modalidad**: Presencial con evaluación práctica

---

## Objetivos Específicos

1. **Configurar** y manejar interrupciones básicas (EXTI, TIM2, SysTick)
2. **Implementar** mini-driver con manejo de interrupciones
3. **Demostrar** competencias adquiridas en la Unidad 1 mediante evaluación sumativa

---

## Contenidos Temáticos

### 1. Conceptos de Interrupciones (20 minutos)

#### ¿Qué es una Interrupción?

Una **interrupción** es un evento que suspende la ejecución normal del programa para atender un evento urgente.

**Flujo**:
```
1. CPU ejecutando main()
2. Evento ocurre (timer overflow, pin cambia, etc.)
3. CPU guarda contexto automáticamente
4. CPU ejecuta ISR (Interrupt Service Routine)
5. ISR termina con instrucción de retorno especial
6. CPU restaura contexto y continúa main()
```

#### NVIC (Nested Vectored Interrupt Controller)

**Características**:
- Hasta 240 interrupciones externas
- 16 niveles de prioridad (configurable a 4, 8 o 16 niveles)
- Anidación automática
- Vector table en memoria

**Registros principales**:
- **ISER** (Interrupt Set-Enable Register): Habilitar interrupción
- **ICER** (Interrupt Clear-Enable Register): Deshabilitar interrupción
- **ISPR** (Interrupt Set-Pending Register): Forzar pending
- **IPR** (Interrupt Priority Register): Configurar prioridad

### 2. Configuración de Interrupciones (30 minutos)

#### Ejemplo 1: SysTick (Interrupción de Sistema)

**SysTick**: Timer de sistema de 24 bits, parte del core ARM

```c
#define SYSTICK_BASE  0xE000E010

typedef struct {
    volatile uint32_t CTRL;    // Control and Status
    volatile uint32_t LOAD;    // Reload Value
    volatile uint32_t VAL;     // Current Value
    volatile uint32_t CALIB;   // Calibration Value
} SysTick_TypeDef;

#define SysTick ((SysTick_TypeDef*)SYSTICK_BASE)

void SysTick_Init(uint32_t ticks)
{
    SysTick->LOAD = ticks - 1;              // Valor de recarga
    SysTick->VAL = 0;                       // Limpiar valor actual
    SysTick->CTRL = 0x07;                   // Habilitar, interrupción, clock del procesador
}

// Handler (ISR)
void SysTick_Handler(void)
{
    // Código a ejecutar cada interrupción
    // Esta función se ejecuta automáticamente
    static uint32_t counter = 0;
    counter++;
    
    if (counter >= 1000) {
        GPIO_Toggle(GPIOA, 6);
        counter = 0;
    }
}
```

#### Ejemplo 2: Interrupción de Timer (TIM2)

```c
void TIM2_IRQ_Init(void)
{
    // 1. Configurar timer (PSC, ARR)
    Timer_Init(16000, 1000);  // 1 kHz, overflow cada 1s
    
    // 2. Habilitar interrupción de update en timer
    TIM2->DIER |= (1 << 0);  // UIE bit
    
    // 3. Configurar prioridad en NVIC (opcional)
    // NVIC_SetPriority(TIM2_IRQn, 3);
    
    // 4. Habilitar interrupción en NVIC
    NVIC->ISER[0] |= (1 << 28);  // TIM2 es interrupción 28
}

void TIM2_IRQHandler(void)
{
    // Verificar si fue interrupción de update
    if (TIM2->SR & (1 << 0)) {
        // Limpiar flag
        TIM2->SR &= ~(1 << 0);
        
        // Acción
        GPIO_Toggle(GPIOA, 6);
    }
}
```

#### Ejemplo 3: Interrupción Externa (EXTI)

**EXTI**: Interrupciones en cambios de pines GPIO

```c
void EXTI_Init(uint8_t pin)
{
    // 1. Configurar GPIO como entrada
    GPIOA->MODER &= ~(0x3 << (pin * 2));
    
    // 2. Habilitar clock de SYSCFG
    RCC->APB2ENR |= (1 << 14);
    
    // 3. Conectar pin a línea EXTI
    SYSCFG->EXTICR[pin/4] &= ~(0xF << ((pin%4)*4));
    SYSCFG->EXTICR[pin/4] |= (0x0 << ((pin%4)*4));  // 0 = GPIOA
    
    // 4. Configurar tipo de disparo
    EXTI->RTSR |= (1 << pin);   // Rising edge trigger
    EXTI->FTSR |= (1 << pin);   // Falling edge trigger (ambos)
    
    // 5. Desenmascarar interrupción
    EXTI->IMR |= (1 << pin);
    
    // 6. Habilitar en NVIC
    if (pin < 5)
        NVIC->ISER[0] |= (1 << (6 + pin));
    // ... otros casos
}

void EXTI0_IRQHandler(void)
{
    // Verificar pending
    if (EXTI->PR & (1 << 0)) {
        // Limpiar flag (escribir 1)
        EXTI->PR |= (1 << 0);
        
        // Acción
        GPIO_Toggle(GPIOA, 7);
    }
}
```

### 3. Vector Table (10 minutos)

La **vector table** es un arreglo de punteros a funciones que el hardware consulta cuando ocurre una interrupción.

**Ubicación**: Inicio de la memoria Flash (0x08000000 en STM32)

**Primeras entradas**:
```
Offset  | Vector
--------|------------------
0x00    | Initial SP value
0x04    | Reset Handler
0x08    | NMI Handler
0x0C    | HardFault Handler
0x10    | MemManage Handler
...     | ...
0x3C    | SVCall
...     | ...
0x68    | SysTick Handler
0x6C    | WWDG IRQ
...     | ...
0xB8    | TIM2 IRQ (0x28 = 40 = interrupción 28)
```

**Definición en startup**:
```asm
.section .isr_vector
vector_table:
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    ; ... más handlers
    .word SysTick_Handler
    .word WWDG_IRQHandler
    .word TIM2_IRQHandler
    ; ...
```

---

## Actividades

### Actividad 1: SysTick con LED (20 minutos)

**Objetivo**: Implementar parpadeo de LED usando interrupción SysTick.

**Requisitos**:
1. Configurar SysTick para interrupción cada 1 ms
2. En el handler, contar hasta 500 y toggle LED
3. Main() debe estar en bucle infinito vacío

**Verificación**: LED parpadea cada 500 ms sin código explícito en main.

### Actividad 2: Botón con Interrupción Externa (15 minutos)

**Objetivo**: Responder a presión de botón mediante EXTI.

**Requisitos**:
1. Configurar pin como entrada con pull-up
2. Configurar EXTI en falling edge
3. En handler, toggle LED diferente

**Verificación**: LED cambia estado inmediatamente al presionar botón.

### Actividad 3: Preparación para Evaluación (15 minutos)

**Revisión de conceptos clave**:
- Arquitecturas (Harvard, RISC)
- Registros (R0-R15, APSR)
- Instrucciones básicas (MOV, LDR, STR, ADD, B)
- Configuración de GPIO
- Timer y delays
- Interrupciones

**Preguntas abiertas para discusión**.

---

## Evaluación Sumativa (40 minutos)

### Proyecto Final: Sistema de Control con Múltiples Periféricos

**Enunciado**:

Implementar un sistema que:

1. **GPIO**: Configure 2 LEDs y 1 botón
2. **Timer**: Use TIM2 para generar delay preciso
3. **Interrupción**: Use SysTick o EXTI
4. **Comportamiento**:
   - LED1 parpadea cada 1 segundo (timer)
   - LED2 cambia estado con botón (interrupción)
   - Al presionar botón, LED1 detiene por 5 segundos, luego continúa

**Entregables**:
- Código fuente (.c y .h)
- Breve documento (1 página) explicando:
  - Arquitectura del código (qué hace cada módulo)
  - Decisiones técnicas (por qué usaste X en vez de Y)
  - Dificultades encontradas y soluciones
  
**Criterios de Evaluación** (ver evaluaciones/rubricas.md):
- Funcionalidad (40%)
- Código (30%): Claridad, comentarios, organización
- Documentación (20%): Explicación técnica
- Conceptos (10%): Respuestas a preguntas orales sobre arquitectura

**Tiempo**: 40 minutos para implementación + 5 min presentación oral individual

### Preguntas Conceptuales (Durante Presentación)

**Cada estudiante responderá 3 preguntas aleatorias**:

1. ¿Qué diferencia hay entre Harvard y von Neumann?
2. ¿Por qué ARM usa RISC?
3. ¿Para qué sirve el registro LR?
4. ¿Qué hace la instrucción `LDR R0, [R1, #4]`?
5. ¿Por qué se debe habilitar el clock de un periférico?
6. ¿Qué es el NVIC?
7. ¿Cuál es la ventaja de usar interrupciones sobre polling?
8. ¿Qué hace el prescaler de un timer?

---

## Cierre de la Unidad 1 (20 minutos)

### Recapitulación

**Lo que aprendimos**:
- ✅ Arquitecturas de procesadores
- ✅ ARM Cortex-M4: componentes, registros, memoria
- ✅ Assembly ARM: instrucciones, estructuras de control
- ✅ C bare metal: startup, drivers
- ✅ GPIO y Timer: configuración completa
- ✅ Interrupciones: NVIC, handlers

### Proyección a Unidad 2

**Próximos temas**:
- Comunicación serial (UART, I2C, SPI)
- ADC y PWM
- DMA
- Proyectos más complejos

### Feedback del Curso

**Breve encuesta**:
1. ¿Qué tema fue más claro?
2. ¿Qué tema necesita refuerzo?
3. ¿El ritmo fue adecuado?
4. Sugerencias de mejora

---

## Material Complementario

### Para Unidad 2

**Lectura previa**:
- Protocolo UART: conceptos básicos
- ADC: conversión analógica-digital

### Recursos de Repaso

- Grabaciones de sesiones (si disponibles)
- Ejemplos de código en repositorio
- Guías de referencia rápida

---

## Indicadores de Logro de la Unidad

| Competencia | Nivel Esperado al Finalizar |
|-------------|----------------------------|
| Arquitecturas | Explica diferencias y justifica por qué ARM usa cada una |
| Assembly | Escribe programas simples con control de flujo y funciones |
| C bare metal | Configura periféricos sin HAL |
| Drivers | Desarrolla driver modular con funciones de init y control |
| Interrupciones | Implementa ISR y configura NVIC |
| Depuración | Identifica errores de configuración leyendo datasheet |

---

## Notas para el Instructor

### Gestión del Tiempo

- Evaluación sumativa: 40 min (puede extenderse si es necesario)
- Presentaciones: 5 min por estudiante (ajustar según tamaño de clase)
- Si clase grande, considerar evaluación en dos bloques

### Evaluación Justa

- Rúbrica clara compartida con estudiantes
- Considerar diferentes niveles (básico cumple requisitos, avanzado añade extras)
- Feedback constructivo inmediato

### Cierre Positivo

- Reconocer progreso de estudiantes
- Enfatizar que lo aprendido es base sólida para desarrollo embebido
- Motivar para Unidad 2

---

**Felicitaciones por completar la Unidad 1!** 🎉

**Próxima Unidad**: Periféricos Avanzados y Comunicación
