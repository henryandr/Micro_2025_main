# Sesión 4: Conjunto de Registros y Primeras Instrucciones Assembly

**Duración**: 2 horas  
**Modalidad**: Presencial con práctica en computadora

---

## Objetivos Específicos

Al finalizar esta sesión, el estudiante será capaz de:

1. **Identificar** todos los registros del ARM Cortex-M4 y su propósito
2. **Escribir** programas simples en Assembly usando instrucciones de movimiento
3. **Utilizar** las directivas básicas de ensamblador (.syntax, .global, .text, .data)

---

## Contenidos Temáticos

### 1. Registros de Propósito General (25 minutos)

#### R0 - R12: Registros de Uso General

**R0 - R3**: Registros de argumentos y resultados
- Parámetros de función (según AAPCS - ARM Architecture Procedure Call Standard)
- R0: Primer parámetro y valor de retorno
- R1-R3: Parámetros adicionales
- No se preservan automáticamente en llamadas a función

**R4 - R11**: Registros preservados
- Deben preservarse en llamadas a función (callee-saved)
- Útiles para variables locales que persisten entre llamadas

**R12 (IP)**: Intra-Procedure-call scratch register
- Uso temporal en llamadas
- No se preserva

#### R13 - R15: Registros Especiales

**R13 (SP) - Stack Pointer**:
- Apunta al tope del stack
- Dos versiones: MSP (Main) y PSP (Process)
- Decrece al hacer PUSH, crece al hacer POP
- Debe mantenerse alineado a 8 bytes (AAPCS)

**R14 (LR) - Link Register**:
- Almacena dirección de retorno al llamar función
- Permite retornar con `BX LR`
- En interrupciones, contiene EXC_RETURN (valor especial)

**R15 (PC) - Program Counter**:
- Apunta a instrucción actual + 4 (por pipeline)
- Se modifica con instrucciones de salto (B, BL, BX)
- Lectura directa da dirección de instrucción + offset

### 2. Registros de Estado (20 minutos)

#### APSR (Application Program Status Register)

Flags de condición (bits 31-28):

| Bit | Nombre | Significado |
|-----|--------|-------------|
| 31 | N | Negative (resultado negativo) |
| 30 | Z | Zero (resultado cero) |
| 29 | C | Carry (acarreo en aritmética sin signo) |
| 28 | V | oVerflow (desbordamiento en aritmética con signo) |
| 27 | Q | Saturación en DSP |

**Uso de flags**:
```
Ejemplo: CMP R0, R1  ; Compara R0 con R1 (R0 - R1)

Si R0 > R1:  Z=0, N=0 (resultado positivo)
Si R0 == R1: Z=1, N=x (resultado cero)
Si R0 < R1:  Z=0, N=1 (resultado negativo)
```

#### IPSR (Interrupt Program Status Register)

- Bits 0-8: Número de excepción/interrupción activa
- 0 = Thread mode (no hay interrupción)
- 1-15 = Excepciones del sistema
- 16+ = Interrupciones externas

#### EPSR (Execution Program Status Register)

- Bit 24 (T): Thumb state (siempre 1 en Cortex-M)
- Bits 15-10, 26-25: IT block state (para instrucciones condicionales)

#### Registros de Máscara

- **PRIMASK**: Desactiva todas las interrupciones excepto NMI y HardFault
- **FAULTMASK**: Desactiva todas las interrupciones incluyendo HardFault
- **BASEPRI**: Desactiva interrupciones con prioridad menor o igual

### 3. Instrucciones Básicas de Movimiento (25 minutos)

#### MOV - Mover Datos entre Registros

```asm
MOV R0, R1           ; R0 = R1 (copia contenido)
MOV R2, #42          ; R2 = 42 (valor inmediato)
MOV R3, #0xFF        ; R3 = 255 (hexadecimal)
```

**Limitaciones**: Inmediatos limitados a 8 bits + rotación o 16 bits con MOVW

#### MOVW / MOVT - Mover 16 bits

```asm
MOVW R0, #0x1234     ; R0 = 0x00001234 (lower 16 bits)
MOVT R0, #0x5678     ; R0 = 0x56781234 (upper 16 bits)
```

**Uso**: Cargar constantes de 32 bits en dos instrucciones

#### LDR - Cargar desde Memoria

```asm
LDR R0, =0x20000000  ; R0 = dirección 0x20000000 (pseudo-instrucción)
LDR R1, [R0]         ; R1 = contenido en dirección apuntada por R0
LDR R2, [R0, #4]     ; R2 = contenido en (R0 + 4)
LDR R3, [R0, #8]!    ; R3 = contenido en (R0 + 8), luego R0 = R0 + 8 (pre-index)
LDR R4, [R0], #4     ; R4 = contenido en R0, luego R0 = R0 + 4 (post-index)
```

#### STR - Almacenar en Memoria

```asm
STR R1, [R0]         ; Almacenar R1 en dirección apuntada por R0
STR R2, [R0, #4]     ; Almacenar R2 en (R0 + 4)
STR R3, [R0, #8]!    ; Almacenar R3 en (R0 + 8), luego R0 = R0 + 8
```

#### LDRB / STRB - Byte (8 bits)

```asm
LDRB R0, [R1]        ; Cargar 1 byte (8 bits) desde [R1]
STRB R2, [R3]        ; Almacenar byte bajo de R2 en [R3]
```

#### LDRH / STRH - Half-word (16 bits)

```asm
LDRH R0, [R1]        ; Cargar 2 bytes (16 bits) desde [R1]
STRH R2, [R3]        ; Almacenar 16 bits bajos de R2 en [R3]
```

### 4. Directivas de Ensamblador (15 minutos)

#### Directivas Básicas

```asm
.syntax unified      ; Usar sintaxis unificada ARM/Thumb
.cpu cortex-m4       ; Especificar procesador objetivo
.thumb               ; Generar código Thumb (obligatorio en Cortex-M)
```

#### Secciones

```asm
.text                ; Sección de código (ejecutable)
.data                ; Sección de datos inicializados (RAM)
.bss                 ; Sección de datos no inicializados (RAM)
```

#### Símbolos y Etiquetas

```asm
.global _start       ; Hacer _start visible externamente
.equ VALOR, 100      ; Definir constante VALOR = 100

_start:              ; Etiqueta (dirección en código)
    MOV R0, #10
    B bucle          ; Saltar a etiqueta 'bucle'
    
bucle:
    SUB R0, R0, #1
    CMP R0, #0
    BNE bucle        ; Si R0 != 0, saltar a 'bucle'
```

#### Datos en Memoria

```asm
.data
variable:
    .word 0x12345678  ; 32 bits (4 bytes)
    .half 0x1234      ; 16 bits (2 bytes)  
    .byte 0x12        ; 8 bits (1 byte)
    
string:
    .asciz "Hola"     ; String terminado en null
    
array:
    .word 1, 2, 3, 4, 5  ; Array de 5 elementos de 32 bits
```

---

## Actividades

### Actividad 1: Primer Programa en Assembly (30 minutos)

**Objetivo**: Escribir y comprender un programa Assembly básico.

**Código a analizar**:

```asm
.syntax unified
.cpu cortex-m4
.thumb

.global _start

.text
_start:
    MOV R0, #10        ; R0 = 10
    MOV R1, #20        ; R1 = 20
    MOV R2, #30        ; R2 = 30
    
    LDR R3, =resultado ; R3 = dirección de 'resultado'
    STR R0, [R3]       ; Guardar R0 en resultado[0]
    STR R1, [R3, #4]   ; Guardar R1 en resultado[4]
    STR R2, [R3, #8]   ; Guardar R2 en resultado[8]
    
    LDR R4, [R3]       ; Cargar resultado[0] en R4
    LDR R5, [R3, #4]   ; Cargar resultado[4] en R5
    
    B .                ; Bucle infinito (detener aquí)

.data
resultado:
    .space 12          ; Reservar 12 bytes (3 words)
```

**Tareas**:
1. Ejecutar en simulador o hardware
2. Inspeccionar valores de registros R0-R5 después de cada instrucción
3. Verificar contenido de memoria en dirección 'resultado'
4. Modificar: Cambiar valores iniciales y observar resultados

### Actividad 2: Micro-Lab - Manipulación de Registros (20 minutos)

**Objetivo**: Practicar uso de registros.

**Ejercicio**: Escribir código que:
1. Cargue valor 100 en R0
2. Copie R0 a R1
3. Cargue valor 50 en R2
4. Almacene R1 y R2 en memoria consecutiva

**Plantilla**:
```asm
.syntax unified
.thumb

.global main
main:
    ; Tu código aquí
    
    B .

.data
memoria:
    .space 8
```

**Solución esperada**:
```asm
main:
    MOV R0, #100
    MOV R1, R0
    MOV R2, #50
    LDR R3, =memoria
    STR R1, [R3]
    STR R2, [R3, #4]
    B .
```

### Actividad 3: Lectura de Código - Identificar Errores (15 minutos)

**Objetivo**: Desarrollar habilidad de lectura de código Assembly.

**Código con error**:
```asm
.syntax unified
.thumb
.global main

main:
    MOV R0, #10
    MOV R1, #20
    MOV [R2], R0      ; ERROR: Sintaxis incorrecta
    B .
```

**Errores a identificar**:
1. `MOV [R2], R0` - No se puede MOV directo a memoria
2. R2 no está inicializado antes de usarse como dirección

**Corrección**:
```asm
main:
    MOV R0, #10
    MOV R1, #20
    LDR R2, =destino  ; Cargar dirección primero
    STR R0, [R2]      ; Usar STR para almacenar
    B .
    
.data
destino:
    .word 0
```

---

## Material Complementario

### Para la Próxima Sesión

**Lectura obligatoria**:
- Operaciones aritméticas en ARM: ADD, SUB, MUL, DIV
- Operaciones lógicas: AND, OR, EOR, BIC
- Desplazamientos: LSL, LSR, ASR, ROR

**Práctica**:
- Escribir 3 programas cortos (5-10 líneas) usando MOV, LDR, STR

---

## Evaluación Formativa

### Quiz Formativo - Sesión 4

1. ¿Qué hace la instrucción `MOV R0, R1`? (1 punto)
2. ¿Cuál es la diferencia entre MOV y LDR? (2 puntos)
3. ¿Para qué se usa el registro LR? (2 puntos)
4. ¿Qué flags se modifican con la instrucción CMP? (2 puntos)
5. Escribe código que almacene el valor 42 en la dirección 0x20000000 (3 puntos)

**Total**: 10 puntos

---

## Evidencias de Aprendizaje

### Entregable

**Programa en Assembly** que:
1. Cargue tres valores diferentes en R0, R1, R2
2. Los almacene en un array en memoria
3. Los vuelva a cargar en R3, R4, R5
4. Incluya comentarios explicando cada instrucción

**Formato**: Archivo .s con comentarios  
**Entrega**: Próxima sesión  
**Criterio**: Código funcional + comentarios claros

---

## Notas para el Instructor

### Configuración Necesaria
- Simulador ARM o hardware STM32
- Toolchain arm-none-eabi-gcc instalado
- Ejemplos pre-compilados listos para mostrar

### Puntos Clave
- Enfatizar que MOV es entre registros, LDR/STR son con memoria
- Explicar que `LDR R0, =valor` es pseudo-instrucción (el ensamblador la expande)
- Aclarar diferencia entre dirección y contenido

### Troubleshooting
- Si simulador no disponible: usar papel y lápiz para seguir código
- Tener ejemplos funcionando previamente probados

---

**Próxima sesión**: Operaciones Aritméticas y Lógicas  
**Preparación**: Lectura sobre instrucciones ADD, SUB, AND, OR
