# Sesión 8: Conceptos de Interrupciones y Evaluación Sumativa

**Duración**: 2 horas  
**Modalidad**: Presencial con evaluación práctica

---

## Objetivos Específicos

1. **Comprender** el concepto de interrupciones y su importancia en sistemas embebidos
2. **Identificar** los componentes del NVIC y su configuración básica
3. **Demostrar** competencias adquiridas en la Unidad 1 mediante evaluación sumativa

---

## Contenidos Temáticos

### 1. Conceptos Fundamentales de Interrupciones (30 minutos)

#### ¿Qué es una Interrupción?

Una **interrupción** es un evento que suspende temporalmente la ejecución del programa principal para atender un evento prioritario.

**Analogías**:
- Es como una alarma que interrumpe tu actividad actual
- Como un timbre que te obliga a pausar lo que haces para atender

#### Flujo de Ejecución con Interrupciones

```
1. CPU ejecutando código principal (main)
2. Evento externo ocurre (timer expira, pin cambia estado, etc.)
3. CPU termina instrucción actual
4. CPU guarda contexto automáticamente (R0-R3, R12, LR, PC, xPSR)
5. CPU busca dirección del Handler en Vector Table
6. CPU ejecuta Handler (ISR - Interrupt Service Routine)
7. Handler ejecuta código necesario
8. Handler retorna con instrucción especial (BX LR con LR especial)
9. CPU restaura contexto automáticamente
10. CPU continúa con código principal
```

#### Tipos de Interrupciones

**1. Excepciones del Sistema (Sistema ARM)**
- Reset
- NMI (Non-Maskable Interrupt)
- Hard Fault
- Memory Management Fault
- Bus Fault
- Usage Fault
- SVCall (Supervisor Call)
- PendSV
- SysTick

**2. Interrupciones de Periféricos (Específicas de STM32)**
- EXTI0-EXTI15 (pines GPIO)
- TIM2-TIM5 (timers)
- USART1-USART3 (comunicación serial)
- SPI1-SPI3
- I2C1-I2C3
- ADC
- DMA
- Y muchas más... (hasta 240 interrupciones posibles)

### 2. NVIC - Nested Vectored Interrupt Controller (35 minutos)

#### Características del NVIC

**Capacidades**:
- Manejo de hasta 240 interrupciones externas
- 16 niveles de prioridad (configurable: 4, 8 o 16 niveles)
- Soporte para anidación automática (interrupciones de mayor prioridad pueden interrumpir a las de menor)
- Activación/desactivación individual de interrupciones
- Estado de pending para interrupciones que llegaron pero no se han atendido

#### Registros Principales del NVIC

**Direcciones base**:
```
NVIC_BASE: 0xE000E100
```

**Registros importantes**:

| Registro | Dirección | Función |
|----------|-----------|---------|
| ISER0-ISER7 | 0xE000E100 | Interrupt Set-Enable Registers |
| ICER0-ICER7 | 0xE000E180 | Interrupt Clear-Enable Registers |
| ISPR0-ISPR7 | 0xE000E200 | Interrupt Set-Pending Registers |
| ICPR0-ICPR7 | 0xE000E280 | Interrupt Clear-Pending Registers |
| IPR0-IPR59  | 0xE000E400 | Interrupt Priority Registers |

#### Habilitar una Interrupción

**Ejemplo: Habilitar interrupción TIM2 (IRQ 28)**

```asm
; TIM2 es IRQ número 28
; ISER0 maneja IRQs 0-31
; Para habilitar: ISER0 |= (1 << 28)

.equ NVIC_ISER0, 0xE000E100

enable_tim2_irq:
    LDR R0, =NVIC_ISER0
    LDR R1, [R0]
    ORR R1, R1, #(1 << 28)
    STR R1, [R0]
    BX LR
```

#### Deshabilitar una Interrupción

```asm
.equ NVIC_ICER0, 0xE000E180

disable_tim2_irq:
    LDR R0, =NVIC_ICER0
    MOV R1, #(1 << 28)
    STR R1, [R0]             ; Escribir 1 deshabilita
    BX LR
```

#### Configurar Prioridad

**Formato de IPR**: Cada registro IPR contiene prioridades de 4 interrupciones (8 bits cada una)

```asm
; Configurar prioridad de TIM2 (IRQ 28)
; IPR7 contiene IRQs 28-31
; IRQ 28 está en bits 0-7 de IPR7

.equ NVIC_IPR7, 0xE000E41C

set_tim2_priority:
    ; R0 = prioridad deseada (0-15, valores altos = menor prioridad)
    
    LDR R1, =NVIC_IPR7
    LDR R2, [R1]
    
    ; Limpiar bits 4-7 (prioridad usa bits superiores)
    BIC R2, R2, #0xF0
    
    ; Establecer nueva prioridad (desplazar a bits 4-7)
    LSL R0, R0, #4
    ORR R2, R2, R0
    
    STR R2, [R1]
    BX LR
```

### 3. Vector Table y Handlers (25 minutos)

#### Vector Table

La **tabla de vectores** es un array de direcciones que el CPU consulta cuando ocurre una interrupción.

**Ubicación**: Dirección 0x00000000 (o 0x08000000 en Flash de STM32)

**Formato**:
```asm
.syntax unified
.thumb

.section .isr_vector, "a"
.type vector_table, %object

vector_table:
    .word _estack              ; 0: Stack pointer inicial
    .word Reset_Handler        ; 1: Reset
    .word NMI_Handler          ; 2: NMI
    .word HardFault_Handler    ; 3: Hard Fault
    ; ... más excepciones del sistema ...
    .word 0                    ; 15: Reservado
    
    ; Interrupciones externas (STM32 específicas)
    .word WWDG_IRQHandler      ; 16: Watchdog
    .word PVD_IRQHandler       ; 17: PVD
    ; ...
    .word TIM2_IRQHandler      ; 44: TIM2 (16 + 28)
    ; ... más interrupciones ...
```

#### Implementación de Handler

**Handler básico**:
```asm
.global TIM2_IRQHandler
.type TIM2_IRQHandler, %function

TIM2_IRQHandler:
    PUSH {R4, LR}              ; Guardar registros (R0-R3 ya guardados por hardware)
    
    ; Código del handler
    ; Ejemplo: Toggle LED
    LDR R0, =GPIOA_BASE
    MOV R1, #6
    BL gpio_toggle
    
    ; Limpiar flag de interrupción en periférico
    ; (cada periférico tiene su propio registro de flags)
    LDR R0, =TIM2_BASE
    LDR R1, [R0, #0x10]        ; SR register
    BIC R1, R1, #(1 << 0)      ; Limpiar UIF
    STR R1, [R0, #0x10]
    
    POP {R4, PC}               ; Retornar (LR contiene valor especial EXC_RETURN)
```

#### Handler por Defecto

```asm
; Handler por defecto para interrupciones no implementadas
.global Default_Handler
.type Default_Handler, %function

Default_Handler:
    B .                        ; Bucle infinito
```

#### Asignación de Handlers No Implementados

```asm
; Crear alias para interrupciones no usadas
.weak WWDG_IRQHandler
.thumb_set WWDG_IRQHandler, Default_Handler

.weak PVD_IRQHandler
.thumb_set PVD_IRQHandler, Default_Handler

; ... etc. para todas las interrupciones
```

### 4. Consideraciones Importantes (10 minutos)

#### Reglas para Handlers (ISR)

1. **Ser rápido**: El handler debe ejecutar lo mínimo necesario
2. **No usar polling loops**: Nunca usar delays o esperas en un handler
3. **Comunicación con main**: Usar flags globales para comunicar eventos
4. **Limpiar flags**: Siempre limpiar el flag de interrupción del periférico

#### Variables Compartidas

```asm
.data
.global interrupt_flag
interrupt_flag:
    .word 0                    ; Flag compartido entre ISR y main

.text
TIM2_IRQHandler:
    PUSH {R4, LR}
    
    ; Establecer flag
    LDR R0, =interrupt_flag
    MOV R1, #1
    STR R1, [R0]
    
    ; Limpiar flag de timer
    ; ...
    
    POP {R4, PC}

main:
    ; ...
bucle:
    LDR R0, =interrupt_flag
    LDR R1, [R0]
    CMP R1, #0
    BEQ bucle
    
    ; Procesar evento
    MOV R1, #0
    STR R1, [R0]               ; Limpiar flag
    
    ; ... hacer algo ...
    
    B bucle
```

#### Reentrancia y Atomicidad

**Problema**: Variables compartidas pueden corromperse

**Solución**: Deshabilitar interrupciones temporalmente para operaciones críticas

```asm
; Deshabilitar interrupciones globalmente
CPSID I                        ; Clear PRIMASK (disable interrupts)

; Sección crítica
; ...

; Habilitar interrupciones
CPSIE I                        ; Set PRIMASK (enable interrupts)
```

---

## Actividades

### Actividad 1: Análisis de Código (20 minutos)

**Código con problemas**:
```asm
TIM2_IRQHandler:
    PUSH {LR}
    
    ; Toggle LED
    LDR R0, =GPIOA_BASE
    LDR R1, [R0, #0x14]
    EOR R1, R1, #(1 << 6)
    STR R1, [R0, #0x14]
    
    ; Delay (MAL - no hacer esto en ISR!)
    MOV R2, #1000000
delay_loop:
    SUBS R2, R2, #1
    BNE delay_loop
    
    POP {PC}
```

**Preguntas**:
1. ¿Qué problemas tiene este handler?
2. ¿Qué falta hacer antes de retornar?
3. ¿Cómo lo mejorarías?

### Actividad 2: Configuración de NVIC (25 minutos)

**Tarea**: Escribir funciones en Assembly para:
1. Habilitar interrupción EXTI0 (IRQ 6)
2. Deshabilitar interrupción EXTI0
3. Configurar prioridad de EXTI0 a nivel 5

**Plantilla**:
```asm
.equ NVIC_ISER0, 0xE000E100
.equ NVIC_ICER0, 0xE000E180
.equ NVIC_IPR1, 0xE000E404

; Implementar aquí
```

---

## Evaluación Sumativa

### Formato de Evaluación (90 minutos)

#### Parte 1: Teórica (30 minutos, 30%)

**10 preguntas de opción múltiple** (10 puntos):
- Arquitectura ARM Cortex M-4
- Registros y su uso
- Instrucciones Assembly
- Conceptos de periféricos

**5 preguntas cortas** (10 puntos):
- Explicar proceso de configuración de GPIO
- Diferenciar entre push-pull y open-drain
- Explicar qué hace una secuencia de instrucciones
- Describir flujo de interrupción
- Justificar uso de registros específicos

Ver archivo `evaluaciones/sumativa.md` para preguntas específicas.

#### Parte 2: Práctica (60 minutos, 70%)

**Proyecto**: Implementar sistema de control con LEDs y botón

**Requisitos**:
1. Configurar 4 LEDs (PA5, PA6, PA7, PA8) como salidas
2. Configurar 1 botón (PC13) como entrada con pull-up
3. Implementar patrón de LEDs que avance cuando se presione el botón
4. Código debe estar modularizado en funciones
5. Incluir comentarios explicativos

**Estructura sugerida**:
```asm
.syntax unified
.thumb

.global main

main:
    BL init_leds
    BL init_button
    
    MOV R4, #0                 ; Contador de patrón
    
bucle:
    ; Leer botón
    BL read_button
    CMP R0, #0
    BNE bucle                  ; Esperar release
    
wait_press:
    BL read_button
    CMP R0, #0
    BEQ wait_press
    
    ; Botón presionado
    ADD R4, R4, #1
    AND R4, R4, #0x0F          ; Mantener en rango 0-15
    
    ; Mostrar patrón
    MOV R0, R4
    BL show_pattern
    
    ; Debounce
    LDR R0, =50
    BL delay_ms
    
    B bucle

; Implementar funciones auxiliares
init_leds:
    ; ...
    BX LR

init_button:
    ; ...
    BX LR

read_button:
    ; ...
    BX LR

show_pattern:
    ; R0 = patrón (0-15)
    ; Mostrar en LEDs
    ; ...
    BX LR
```

**Rúbrica** (ver `evaluaciones/rubricas.md`):
- Funcionalidad correcta: 35 puntos
- Código estructurado y modular: 15 puntos
- Comentarios y claridad: 10 puntos
- Configuración correcta de periféricos: 10 puntos

---

## Material de Cierre

### Resumen de Unidad 1

**Lo que hemos aprendido**:
1. ✅ Arquitecturas de procesadores (Harvard, von Neumann, RISC, CISC)
2. ✅ Arquitectura interna de ARM Cortex M-4
3. ✅ Programación en Assembly ARM (instrucciones Thumb-2)
4. ✅ Control de flujo y funciones
5. ✅ Configuración de GPIO en Assembly
6. ✅ Conceptos fundamentales de interrupciones

**Habilidades desarrolladas**:
- Lectura e interpretación de datasheets
- Manipulación de registros a bajo nivel
- Programación modular en Assembly
- Debugging de problemas de hardware/software

### Preparación para Unidad 2

**Lo que veremos en Unidad 2**:
- Transición de Assembly a C bare metal
- Máquinas de Estados Finitos (MEF)
- Desarrollo de drivers completos en C
- Periféricos avanzados (UART, Timer avanzado)
- Interrupciones en C
- Proyectos integradores

**Brecha entre Unidad 1 y 2**:
- Unidad 1: Fundamentos en Assembly
- Unidad 2: Aplicación práctica en C
- **Justificación**: Assembly da comprensión profunda del hardware, C permite desarrollo más rápido y mantenible

---

## Notas para el Instructor

### Gestión del Tiempo
- Teoría de interrupciones: 30 min
- NVIC y configuración: 35 min
- Handlers y ejemplos: 25 min
- Evaluación sumativa: 90 min

### Criterios de Aprobación
- Mínimo 60% en evaluación sumativa
- Asistencia mínima 80% (6 de 8 sesiones)
- Entrega de evidencias de al menos 6 sesiones

### Feedback Post-Evaluación
- Revisión inmediata de parte teórica (si tiempo permite)
- Código práctico: feedback escrito en 2-3 días
- Sesión de retroalimentación grupal en siguiente clase

---

**Fin de Unidad 1**  
**Próxima unidad**: Máquinas de Estados Finitos y Programación en C
