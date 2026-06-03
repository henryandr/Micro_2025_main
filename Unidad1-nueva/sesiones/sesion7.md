# Sesión 7: Configuración Avanzada de GPIO en Assembly

**Duración**: 2 horas  
**Modalidad**: Presencial con práctica intensiva de hardware

---

## Objetivos Específicos

1. **Configurar** pines GPIO con todas sus opciones (modo, tipo, velocidad, pull-up/down) en Assembly
2. **Implementar** "driver" modular en Assembly para control de GPIO
3. **Aplicar** técnicas de lectura de datasheet para configurar registros de periféricos

---

## Contenidos Temáticos

### 1. Registros de GPIO en STM32F407 (30 minutos)

#### Mapa de Memoria de GPIO

**Direcciones base**:
```
GPIOA: 0x40020000
GPIOB: 0x40020400
GPIOC: 0x40020800
GPIOD: 0x40020C00
GPIOE: 0x40021000
```

#### Registros de GPIO (Offsets desde base)

| Registro | Offset | Función |
|----------|--------|---------|
| MODER    | 0x00   | Modo del pin (input/output/AF/analog) |
| OTYPER   | 0x04   | Tipo de salida (push-pull/open-drain) |
| OSPEEDR  | 0x08   | Velocidad de salida |
| PUPDR    | 0x0C   | Pull-up/Pull-down |
| IDR      | 0x10   | Input Data Register (lectura) |
| ODR      | 0x14   | Output Data Register (escritura) |
| BSRR     | 0x18   | Bit Set/Reset Register (atómico) |
| LCKR     | 0x1C   | Lock Register |
| AFRL     | 0x20   | Alternate Function Low (pins 0-7) |
| AFRH     | 0x24   | Alternate Function High (pins 8-15) |

#### Registro MODER (Mode Register)

**Formato**: 2 bits por pin (32 bits total = 16 pines)
```
Bits 31-30: Pin 15
Bits 29-28: Pin 14
...
Bits 1-0:   Pin 0
```

**Valores**:
- `00`: Entrada (input)
- `01`: Salida de propósito general (output)
- `10`: Función alternativa (AF)
- `11`: Modo analógico

#### Registro OTYPER (Output Type Register)

**Formato**: 1 bit por pin
```
Bit 15: Pin 15
...
Bit 0:  Pin 0
```

**Valores**:
- `0`: Push-pull (puede generar 0 y 1)
- `1`: Open-drain (solo puede generar 0, necesita pull-up externo para 1)

#### Registro OSPEEDR (Output Speed Register)

**Formato**: 2 bits por pin
```
00: Low speed (2 MHz)
01: Medium speed (25 MHz)
10: Fast speed (50 MHz)
11: High speed (100 MHz)
```

#### Registro PUPDR (Pull-up/Pull-down Register)

**Formato**: 2 bits por pin
```
00: Sin pull-up ni pull-down
01: Pull-up habilitado
10: Pull-down habilitado
11: Reservado
```

### 2. Configuración Completa en Assembly (45 minutos)

#### Paso 1: Habilitar Clock del Periférico

**Todos los periféricos requieren clock habilitado**

```asm
; Habilitar clock de GPIOA
; RCC_AHB1ENR está en 0x40023830

.equ RCC_AHB1ENR, 0x40023830

enable_gpioa_clock:
    LDR R0, =RCC_AHB1ENR
    LDR R1, [R0]
    ORR R1, R1, #(1 << 0)    ; Bit 0 para GPIOA
    STR R1, [R0]
    BX LR
```

#### Función Genérica para Habilitar Clock

```asm
; void gpio_clock_enable(uint8_t port_number)
; R0 = número de puerto (0=A, 1=B, 2=C, etc.)

.global gpio_clock_enable
gpio_clock_enable:
    LDR R1, =RCC_AHB1ENR
    LDR R2, [R1]
    MOV R3, #1
    LSL R3, R3, R0           ; Desplazar 1 por port_number posiciones
    ORR R2, R2, R3
    STR R2, [R1]
    BX LR
```

#### Paso 2: Configurar Modo del Pin

```asm
; void gpio_set_mode(GPIO_TypeDef *port, uint8_t pin, uint8_t mode)
; R0 = dirección base del puerto
; R1 = número de pin (0-15)
; R2 = modo (0=input, 1=output, 2=AF, 3=analog)

.global gpio_set_mode
gpio_set_mode:
    PUSH {R4, R5, LR}
    
    ; Calcular desplazamiento: pin * 2
    LSL R3, R1, #1           ; R3 = pin * 2
    
    ; Crear máscara para limpiar: ~(0x3 << (pin*2))
    MOV R4, #0x3
    LSL R4, R4, R3           ; R4 = 0x3 << (pin*2)
    MVN R4, R4               ; R4 = ~(0x3 << (pin*2))
    
    ; Leer MODER
    LDR R5, [R0, #0x00]
    
    ; Limpiar bits
    AND R5, R5, R4
    
    ; Establecer nuevo modo
    LSL R2, R2, R3           ; mode << (pin*2)
    ORR R5, R5, R2
    
    ; Escribir MODER
    STR R5, [R0, #0x00]
    
    POP {R4, R5, PC}
```

#### Paso 3: Configurar Tipo de Salida

```asm
; void gpio_set_output_type(GPIO_TypeDef *port, uint8_t pin, uint8_t type)
; R0 = dirección base del puerto
; R1 = número de pin (0-15)
; R2 = tipo (0=push-pull, 1=open-drain)

.global gpio_set_output_type
gpio_set_output_type:
    PUSH {R4, LR}
    
    ; Leer OTYPER
    LDR R3, [R0, #0x04]
    
    ; Crear máscara para el pin
    MOV R4, #1
    LSL R4, R4, R1           ; R4 = 1 << pin
    
    CMP R2, #0
    BEQ set_pushpull
    
set_opendrain:
    ; Establecer bit (open-drain)
    ORR R3, R3, R4
    B write_otyper
    
set_pushpull:
    ; Limpiar bit (push-pull)
    MVN R4, R4
    AND R3, R3, R4
    
write_otyper:
    STR R3, [R0, #0x04]
    
    POP {R4, PC}
```

#### Paso 4: Configurar Velocidad

```asm
; void gpio_set_speed(GPIO_TypeDef *port, uint8_t pin, uint8_t speed)
; R0 = dirección base del puerto
; R1 = número de pin (0-15)
; R2 = velocidad (0=low, 1=medium, 2=fast, 3=high)

.global gpio_set_speed
gpio_set_speed:
    PUSH {R4, R5, LR}
    
    ; Calcular desplazamiento: pin * 2
    LSL R3, R1, #1
    
    ; Máscara para limpiar
    MOV R4, #0x3
    LSL R4, R4, R3
    MVN R4, R4
    
    ; Leer, modificar, escribir OSPEEDR
    LDR R5, [R0, #0x08]
    AND R5, R5, R4
    LSL R2, R2, R3
    ORR R5, R5, R2
    STR R5, [R0, #0x08]
    
    POP {R4, R5, PC}
```

#### Paso 5: Configurar Pull-up/Pull-down

```asm
; void gpio_set_pupd(GPIO_TypeDef *port, uint8_t pin, uint8_t pupd)
; R0 = dirección base del puerto
; R1 = número de pin (0-15)
; R2 = pupd (0=none, 1=pull-up, 2=pull-down)

.global gpio_set_pupd
gpio_set_pupd:
    PUSH {R4, R5, LR}
    
    LSL R3, R1, #1
    
    MOV R4, #0x3
    LSL R4, R4, R3
    MVN R4, R4
    
    LDR R5, [R0, #0x0C]
    AND R5, R5, R4
    LSL R2, R2, R3
    ORR R5, R5, R2
    STR R5, [R0, #0x0C]
    
    POP {R4, R5, PC}
```

### 3. Operaciones de Lectura/Escritura (20 minutos)

#### Escribir en un Pin (usando BSRR)

**BSRR es atómico - no necesita read-modify-write**

```asm
; void gpio_write(GPIO_TypeDef *port, uint8_t pin, uint8_t value)
; R0 = dirección base del puerto
; R1 = número de pin
; R2 = valor (0 o 1)

.global gpio_write
gpio_write:
    CMP R2, #0
    BEQ write_zero
    
write_one:
    ; Set: bits 0-15 de BSRR
    MOV R2, #1
    LSL R2, R2, R1
    STR R2, [R0, #0x18]      ; BSRR
    BX LR
    
write_zero:
    ; Reset: bits 16-31 de BSRR
    MOV R2, #1
    LSL R2, R2, R1
    LSL R2, R2, #16          ; Desplazar a bits 16-31
    STR R2, [R0, #0x18]
    BX LR
```

#### Leer un Pin

```asm
; uint8_t gpio_read(GPIO_TypeDef *port, uint8_t pin)
; R0 = dirección base del puerto
; R1 = número de pin
; Retorno: R0 = 0 o 1

.global gpio_read
gpio_read:
    LDR R2, [R0, #0x10]      ; Leer IDR
    LSR R2, R2, R1           ; Desplazar a derecha
    AND R0, R2, #1           ; Aislar bit menos significativo
    BX LR
```

#### Toggle de un Pin

```asm
; void gpio_toggle(GPIO_TypeDef *port, uint8_t pin)
; R0 = dirección base del puerto
; R1 = número de pin

.global gpio_toggle
gpio_toggle:
    PUSH {R4, LR}
    
    ; Leer ODR
    LDR R2, [R0, #0x14]
    
    ; XOR con máscara del pin
    MOV R3, #1
    LSL R3, R3, R1
    EOR R2, R2, R3
    
    ; Escribir ODR
    STR R2, [R0, #0x14]
    
    POP {R4, PC}
```

### 4. Driver Completo en Assembly (15 minutos)

#### Inicialización Completa de un Pin

```asm
; void gpio_init_output(GPIO_TypeDef *port, uint8_t pin)
; Inicializa pin como salida con configuración estándar:
; - Modo: Output
; - Tipo: Push-pull
; - Velocidad: Medium
; - PUPD: None

.global gpio_init_output
gpio_init_output:
    PUSH {R4, R5, LR}
    MOV R4, R0               ; Guardar port
    MOV R5, R1               ; Guardar pin
    
    ; 1. Habilitar clock (asumiendo GPIOA=0, GPIOB=1, etc.)
    ; Simplificado: siempre habilitar GPIOA
    LDR R0, =0x40023830
    LDR R1, [R0]
    ORR R1, R1, #(1 << 0)
    STR R1, [R0]
    
    ; 2. Configurar modo = output (0x1)
    MOV R0, R4
    MOV R1, R5
    MOV R2, #1               ; Output mode
    BL gpio_set_mode
    
    ; 3. Configurar tipo = push-pull (0x0)
    MOV R0, R4
    MOV R1, R5
    MOV R2, #0               ; Push-pull
    BL gpio_set_output_type
    
    ; 4. Configurar velocidad = medium (0x1)
    MOV R0, R4
    MOV R1, R5
    MOV R2, #1               ; Medium speed
    BL gpio_set_speed
    
    ; 5. Configurar PUPD = none (0x0)
    MOV R0, R4
    MOV R1, R5
    MOV R2, #0               ; No pull
    BL gpio_set_pupd
    
    POP {R4, R5, PC}
```

---

## Actividades

### Actividad 1: Lectura de Datasheet (20 minutos)

**Tarea**: En grupos de 2-3, consultar el STM32F407 Reference Manual:
1. Identificar el offset del registro LCKR
2. Explicar para qué sirve y cómo se usa
3. Escribir pseudocódigo para "bloquear" un pin

### Actividad 2: Implementar Configuración Completa (40 minutos)

**Ejercicio**: Escribir programa en Assembly que:
1. Configure PA6 como salida push-pull, velocidad high, sin pull
2. Configure PA7 como entrada con pull-up
3. Si PA7 está en LOW, encender PA6
4. Si PA7 está en HIGH, apagar PA6

**Estructura sugerida**:
```asm
.syntax unified
.thumb

.equ GPIOA_BASE, 0x40020000

.global main

main:
    ; Configurar PA6
    LDR R0, =GPIOA_BASE
    MOV R1, #6
    BL gpio_init_output
    
    ; Configurar PA7
    ; (implementar)
    
bucle_principal:
    ; Leer PA7
    ; Si LOW, encender PA6
    ; Si HIGH, apagar PA6
    
    B bucle_principal
```

### Actividad 3: Driver Genérico (40 minutos)

**Tarea**: Crear archivo `gpio_driver.s` con:
- `gpio_init_output(port, pin)`
- `gpio_init_input(port, pin, pupd)`
- `gpio_write(port, pin, value)`
- `gpio_read(port, pin)`
- `gpio_toggle(port, pin)`

**Requisitos**:
- Comentarios claros en cada función
- Preservar registros según AAPCS
- Validación opcional de parámetros

---

## Evaluación Formativa

### Quiz - Sesión 7

1. ¿Por qué es necesario habilitar el clock de un periférico antes de usarlo? (2 puntos)
2. ¿Qué ventaja tiene usar BSRR en lugar de ODR para cambiar un pin? (2 puntos)
3. ¿Cuándo se debe usar configuración open-drain en lugar de push-pull? (2 puntos)
4. ¿Qué registro se usa para leer el estado de un pin configurado como entrada? (2 puntos)
5. Escribe en Assembly la configuración de PB5 como salida push-pull, velocidad low (2 puntos)

---

## Evidencias de Aprendizaje

**Entregable**: Archivo `gpio_driver.s` completo que:
1. Implemente todas las funciones especificadas
2. Funcione correctamente con hardware (demostración en clase)
3. Incluya comentarios explicativos
4. Siga convenciones AAPCS
5. Compile sin errores ni warnings

**Formato**: Archivo .s con código fuente  
**Evaluación**: 
- Funcionalidad correcta: 50%
- Estructura y organización: 20%
- Comentarios y documentación: 15%
- Eficiencia del código: 15%

---

## Material para Casa

### Lectura Previa a Sesión 8
- Concepto de interrupciones
- NVIC (Nested Vectored Interrupt Controller)
- Tipos de interrupciones en Cortex M-4
- Prioridades de interrupciones

### Ejercicio Opcional
Implementar función que:
- Configure 8 LEDs (puerto completo)
- Muestre un patrón tipo "Knight Rider" (ida y vuelta)
- Use delays parametrizables

---

## Notas para el Instructor

### Enfoque Pedagógico
- Énfasis en lectura activa de datasheet
- Relacionar cada bit con comportamiento observable en hardware
- Debugging con osciloscopio/analizador lógico si disponible

### Demos Importantes
1. Mostrar diferencia entre push-pull y open-drain con LEDs
2. Demostrar pull-up/pull-down con botones
3. Mostrar timing con diferentes velocidades (si posible con osciloscopio)

### Problemas Comunes
- Olvidar habilitar clock (el periférico no responde)
- Confundir offset de registros
- No preservar registros en funciones (violación AAPCS)
- Usar ODR en lugar de BSRR (race conditions en sistemas más complejos)

---

**Próxima sesión**: Interrupciones Básicas en Assembly  
**Preparación**: Revisar conceptos de interrupciones y NVIC
