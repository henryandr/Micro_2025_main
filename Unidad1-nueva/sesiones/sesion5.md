# Sesión 5: Operaciones Aritméticas y Lógicas

**Duración**: 2 horas  
**Modalidad**: Presencial con práctica intensiva

---

## Objetivos Específicos

1. **Utilizar** instrucciones aritméticas (ADD, SUB, MUL, DIV) en programas Assembly
2. **Aplicar** operaciones lógicas y de bits para configuración de registros
3. **Implementar** funciones simples respetando convenciones de llamada ARM (AAPCS)

---

## Contenidos Temáticos

### 1. Operaciones Aritméticas (25 minutos)

#### ADD - Suma
```asm
ADD R0, R1, R2      ; R0 = R1 + R2
ADD R0, R0, #1      ; R0 = R0 + 1 (incremento)
ADDS R0, R1, R2     ; Suma con actualización de flags
```

#### SUB - Resta
```asm
SUB R0, R1, R2      ; R0 = R1 - R2
SUB R0, R0, #1      ; R0 = R0 - 1 (decremento)
SUBS R0, R1, R2     ; Resta con actualización de flags
```

#### MUL - Multiplicación
```asm
MUL R0, R1, R2      ; R0 = R1 * R2 (32 bits x 32 bits = 32 bits bajos)
```

#### UDIV / SDIV - División
```asm
UDIV R0, R1, R2     ; R0 = R1 / R2 (sin signo)
SDIV R0, R1, R2     ; R0 = R1 / R2 (con signo)
```

**Nota**: No hay instrucción de módulo; se calcula como: `mod = dividendo - (cociente * divisor)`

#### Operaciones con Flags

Cuando se usa sufijo 'S' (ADDS, SUBS), se actualizan flags:

```asm
ADDS R0, R1, R2     ; R0 = R1 + R2, actualiza N, Z, C, V

; Ejemplo con flags:
MOV R0, #0xFFFFFFFF  ; R0 = -1 (con signo) o máximo (sin signo)
ADDS R0, R0, #1      ; R0 = 0, Z=1 (cero), C=1 (acarreo)
```

### 2. Operaciones Lógicas (25 minutos)

#### AND - Y lógico
```asm
AND R0, R1, R2      ; R0 = R1 & R2 (bit a bit)
AND R0, R0, #0x0F   ; Máscarar nibble bajo (mantener bits 0-3)
```

**Uso típico**: Leer bits específicos (máscara)

#### ORR - O lógico
```asm
ORR R0, R1, R2      ; R0 = R1 | R2
ORR R0, R0, #0x01   ; Establecer bit 0 a 1
```

**Uso típico**: Establecer bits a 1

#### EOR - XOR lógico
```asm
EOR R0, R1, R2      ; R0 = R1 ^ R2
EOR R0, R0, R0      ; R0 = 0 (limpiar registro eficientemente)
```

**Uso típico**: Invertir bits específicos

#### BIC - Bit Clear (AND NOT)
```asm
BIC R0, R1, R2      ; R0 = R1 & ~R2 (limpiar bits en R2)
BIC R0, R0, #0x01   ; Limpiar bit 0 (poner a 0)
```

**Uso típico**: Limpiar bits específicos

#### MVN - Move NOT
```asm
MVN R0, R1          ; R0 = ~R1 (complemento bit a bit)
```

### 3. Operaciones de Desplazamiento (20 minutos)

#### LSL - Logical Shift Left (desplazamiento lógico izquierda)
```asm
LSL R0, R1, #2      ; R0 = R1 << 2 (equivale a multiplicar por 4)
```

#### LSR - Logical Shift Right (desplazamiento lógico derecha)
```asm
LSR R0, R1, #2      ; R0 = R1 >> 2 (equivale a dividir por 4, sin signo)
```

#### ASR - Arithmetic Shift Right (desplazamiento aritmético derecha)
```asm
ASR R0, R1, #2      ; R0 = R1 >> 2 (preserva signo)
```

#### ROR - Rotate Right
```asm
ROR R0, R1, #4      ; Rotar R1 a la derecha 4 bits
```

**Tabla de Uso**:

| Operación | Uso Típico | Ejemplo |
|-----------|------------|---------|
| LSL | Multiplicar por 2^n | `LSL R0, R0, #3` = x8 |
| LSR | Dividir por 2^n (sin signo) | `LSR R0, R0, #2` = /4 |
| ASR | Dividir por 2^n (con signo) | `ASR R0, R0, #1` = /2 |
| ROR | Rotación de bits | Cifrado, checksum |

### 4. Aplicación: Configuración de Registros (15 minutos)

#### Ejemplo: Configurar GPIO

Supongamos que queremos configurar PTA6 como salida sin modificar otros pines:

```asm
; Registro GPIOA_MODER (cada pin usa 2 bits)
; Pin 6: bits 12-13
; Queremos: 01 (salida)

LDR R0, =0x40020000     ; Base GPIOA
LDR R1, [R0, #0]        ; Leer MODER actual

; Paso 1: Limpiar bits 12-13 (poner a 00)
BIC R1, R1, #(0b11 << 12)

; Paso 2: Establecer bits 12-13 a 01
ORR R1, R1, #(0b01 << 12)

; Paso 3: Escribir de vuelta
STR R1, [R0, #0]
```

**Patrón general**:
1. BIC para limpiar bits (máscara de 1s)
2. ORR para establecer bits (valor deseado)

### 5. Convenciones de Llamada (AAPCS) (15 minutos)

**ARM Architecture Procedure Call Standard**:

```
Registros de argumentos: R0-R3
  R0: Primer argumento y valor de retorno
  R1-R3: Argumentos adicionales

Registros preservados (callee-saved): R4-R11
  Función debe preservar estos (PUSH al inicio, POP al salir)

Registros no preservados (caller-saved): R0-R3, R12
  Función puede modificarlos libremente

Stack Pointer (SP): Debe preservarse
Link Register (LR): Contiene dirección de retorno
```

#### Ejemplo de Función

```asm
; Función: sumar(a, b) -> a + b
; Entrada: R0 = a, R1 = b
; Salida: R0 = resultado

sumar:
    ADD R0, R0, R1      ; R0 = R0 + R1
    BX LR               ; Retornar

; Llamada desde otro código:
main:
    MOV R0, #10
    MOV R1, #20
    BL sumar            ; Llama función, LR = dirección de retorno
    ; Aquí R0 = 30
```

#### Función con Variables Locales

```asm
; Función compleja que preserva registros
multiplicar_por_tres:
    PUSH {R4, LR}       ; Preservar R4 y LR
    
    MOV R4, R0          ; Usar R4 como temporal
    LSL R1, R0, #1      ; R1 = R0 * 2
    ADD R0, R1, R4      ; R0 = (R0*2) + R0 = R0*3
    
    POP {R4, PC}        ; Restaurar R4 y retornar (PC = LR)
```

---

## Actividades

### Actividad 1: Calculadora Básica (35 minutos)

**Objetivo**: Implementar operaciones aritméticas.

**Tarea**: Escribir programa que calcule:
```
resultado = (a + b) * c / d
donde a=10, b=5, c=3, d=3
```

**Plantilla**:
```asm
.syntax unified
.thumb

.global main
main:
    MOV R0, #10         ; a
    MOV R1, #5          ; b
    MOV R2, #3          ; c
    MOV R3, #3          ; d
    
    ; Tu código aquí
    ; R0 debe contener el resultado final
    
    B .

```

**Solución**:
```asm
    ADD R0, R0, R1      ; R0 = a + b = 15
    MUL R0, R0, R2      ; R0 = 15 * 3 = 45
    UDIV R0, R0, R3     ; R0 = 45 / 3 = 15
```

### Actividad 2: Manipulación de Bits - Práctica con GPIO (30 minutos)

**Objetivo**: Practicar operaciones de bits para configuración.

**Escenario**: Configurar GPIOA pins 6, 7, 8 como:
- Pin 6: Salida (01)
- Pin 7: Entrada (00)
- Pin 8: Función alternativa (10)

**Código guiado**:
```asm
.equ GPIOA_BASE, 0x40020000
.equ GPIOA_MODER, 0x00

main:
    ; 1. Cargar dirección base
    LDR R0, =GPIOA_BASE
    
    ; 2. Leer valor actual
    LDR R1, [R0, #GPIOA_MODER]
    
    ; 3. Limpiar bits para pins 6, 7, 8 (bits 12-17)
    ; Máscara: 0b111111 << 12 = 0x3F000
    LDR R2, =0x3F000
    BIC R1, R1, R2
    
    ; 4. Establecer configuración deseada
    ; Pin 6: 01 en bits 12-13
    ; Pin 7: 00 en bits 14-15 (ya están en 0)
    ; Pin 8: 10 en bits 16-17
    ; Valor: 0b10_00_01 << 12 = 0x21000
    LDR R2, =0x21000
    ORR R1, R1, R2
    
    ; 5. Escribir de vuelta
    STR R1, [R0, #GPIOA_MODER]
    
    B .
```

### Actividad 3: Crear Función Reutilizable (15 minutos)

**Objetivo**: Aplicar AAPCS.

**Tarea**: Escribir función `maximo(a, b)` que retorne el mayor de dos números.

**Plantilla**:
```asm
; Entrada: R0 = a, R1 = b
; Salida: R0 = max(a, b)
maximo:
    ; Tu código aquí
    BX LR

main:
    MOV R0, #15
    MOV R1, #20
    BL maximo
    ; R0 debe ser 20
    B .
```

**Solución**:
```asm
maximo:
    CMP R0, R1          ; Comparar R0 con R1
    BGE fin             ; Si R0 >= R1, ya está en R0
    MOV R0, R1          ; Sino, R0 = R1
fin:
    BX LR
```

---

## Evaluación Formativa

### Quiz - Sesión 5

1. ¿Qué hace `LSL R0, R0, #2`? (2 puntos)
2. ¿Cuál es la diferencia entre `UDIV` y `SDIV`? (2 puntos)
3. ¿Cómo se limpia el bit 3 de un registro sin afectar otros bits? (2 puntos)
4. ¿Qué registros deben preservarse en una función según AAPCS? (2 puntos)
5. Escribe código que multiplique R0 por 5 sin usar MUL (2 puntos)

---

## Evidencias de Aprendizaje

**Entregable**: Programa que implemente función `potencia(base, exponente)` donde exponente es pequeño (0-10).

```asm
; Ejemplo: potencia(2, 3) = 8
; Usar bucle con multiplicaciones sucesivas
```

**Formato**: Archivo .s con comentarios  
**Evaluación**: Funcionalidad + claridad de código

---

## Notas para el Instructor

### Puntos Clave
- Operaciones de bits son fundamentales para configuración de periféricos
- AAPCS permite interoperabilidad con C
- Desplazamientos son más eficientes que multiplicación/división

### Demo en Vivo
- Mostrar cómo configurar un pin GPIO paso a paso
- Depurar con inspección de registros

---

**Próxima sesión**: Control de Flujo
**Preparación**: Leer sobre instrucciones de salto (B, BL, BEQ, BNE, etc.)
