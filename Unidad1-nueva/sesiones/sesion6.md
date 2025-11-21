# Sesión 6: Control de Flujo y Funciones en Assembly

**Duración**: 2 horas  
**Modalidad**: Presencial con práctica intensiva de Assembly

---

## Objetivos Específicos

1. **Implementar** estructuras de control complejas (if, while, for) en Assembly ARM
2. **Desarrollar** funciones con paso de parámetros y manejo de stack
3. **Aplicar** control de flujo en problemas prácticos de programación

---

## Contenidos Temáticos

### 1. Instrucciones de Salto y Condicionales (30 minutos)

#### Saltos Incondicionales

```asm
B label             ; Branch (salto) a 'label'
BL label            ; Branch with Link (llama función, guarda retorno en LR)
BX R0               ; Branch and Exchange (salta a dirección en R0)
```

#### Instrucciones de Comparación

```asm
CMP R0, R1          ; Compara R0 con R1 (hace R0 - R1, actualiza flags)
CMN R0, R1          ; Compare Negative (R0 + R1, actualiza flags)
TST R0, R1          ; Test (R0 AND R1, actualiza flags)
TEQ R0, R1          ; Test Equivalence (R0 XOR R1)
```

#### Saltos Condicionales (basados en flags)

| Instrucción | Condición | Flags | Significado |
|-------------|-----------|-------|-------------|
| BEQ | Equal | Z=1 | Si igual (cero) |
| BNE | Not Equal | Z=0 | Si no igual |
| BGT | Greater Than | Z=0, N=V | Si mayor (con signo) |
| BGE | Greater or Equal | N=V | Si mayor o igual |
| BLT | Less Than | N≠V | Si menor |
| BLE | Less or Equal | Z=1 o N≠V | Si menor o igual |
| BHI | Higher | C=1, Z=0 | Si mayor (sin signo) |
| BLS | Lower or Same | C=0 o Z=1 | Si menor o igual (sin signo) |
| BCS/BHS | Carry Set | C=1 | Si hay acarreo |
| BCC/BLO | Carry Clear | C=0 | Si no hay acarreo |

### 2. Estructuras de Control en Assembly (35 minutos)

#### IF-THEN-ELSE

**Pseudocódigo**:
```
if (R0 > 10)
    R1 = 1
else
    R1 = 2
```

**Assembly**:
```asm
    MOV R0, #15         ; R0 = 15 (ejemplo)
    CMP R0, #10         ; Comparar R0 con 10
    BLE else_parte      ; Si R0 <= 10, ir a else
    
    ; THEN parte
    MOV R1, #1
    B fin_if
    
else_parte:
    MOV R1, #2
    
fin_if:
    ; Continuar...
```

#### WHILE Loop

**Pseudocódigo**:
```
while (R0 > 0)
{
    R0 = R0 - 1
}
```

**Assembly**:
```asm
while_inicio:
    CMP R0, #0          ; Comparar R0 con 0
    BLE while_fin       ; Si R0 <= 0, salir del bucle
    
    SUB R0, R0, #1      ; R0 = R0 - 1
    B while_inicio      ; Volver al inicio
    
while_fin:
    ; Continuar...
```

#### FOR Loop

**Pseudocódigo**:
```
for (i=0; i<10; i++)
{
    suma = suma + i
}
```

**Assembly**:
```asm
    MOV R0, #0          ; i = 0 (contador)
    MOV R1, #0          ; suma = 0
    
for_inicio:
    CMP R0, #10         ; i < 10?
    BGE for_fin         ; Si i >= 10, salir
    
    ADD R1, R1, R0      ; suma = suma + i
    ADD R0, R0, #1      ; i++
    B for_inicio        ; Repetir
    
for_fin:
    ; R1 contiene la suma
```

#### DO-WHILE Loop

**Pseudocódigo**:
```
do {
    R0 = R0 - 1
} while (R0 > 0)
```

**Assembly**:
```asm
do_inicio:
    SUB R0, R0, #1      ; R0 = R0 - 1
    CMP R0, #0          ; Comparar con 0
    BGT do_inicio       ; Si R0 > 0, repetir
    
    ; Continuar...
```

### 3. Funciones en Assembly (30 minutos)

#### Convención de Llamadas ARM (AAPCS)

**Registros para parámetros**:
- R0-R3: Primeros 4 parámetros
- Stack: Parámetros adicionales
- R0: Valor de retorno
- LR (R14): Dirección de retorno

#### Función Simple

```asm
; int suma(int a, int b)
; Parámetros: R0 = a, R1 = b
; Retorno: R0 = a + b
.global suma
suma:
    ADD R0, R0, R1      ; R0 = R0 + R1
    BX LR               ; Retornar
```

**Uso**:
```asm
    MOV R0, #5          ; Primer parámetro
    MOV R1, #3          ; Segundo parámetro
    BL suma             ; Llamar función
    ; R0 contiene 8
```

#### Función con Variables Locales (Uso de Stack)

```asm
; int factorial(int n)
; Calcula n!
.global factorial
factorial:
    PUSH {R4, LR}       ; Guardar registros
    
    CMP R0, #1          ; if (n <= 1)
    BLE caso_base
    
    ; n > 1: guardar n y llamar recursivamente
    MOV R4, R0          ; Guardar n en R4
    SUB R0, R0, #1      ; n - 1
    BL factorial        ; factorial(n-1)
    MUL R0, R4, R0      ; n * factorial(n-1)
    B fin_factorial
    
caso_base:
    MOV R0, #1          ; retornar 1
    
fin_factorial:
    POP {R4, PC}        ; Restaurar y retornar
```

#### Función con Múltiples Parámetros

```asm
; void swap(int *a, int *b)
; Intercambia dos valores en memoria
.global swap
swap:
    LDR R2, [R0]        ; R2 = *a
    LDR R3, [R1]        ; R3 = *b
    STR R3, [R0]        ; *a = R3 (valor de b)
    STR R2, [R1]        ; *b = R2 (valor de a)
    BX LR
```

### 4. Manipulación Avanzada de GPIO en Assembly (25 minutos)

#### Configuración Completa de un Pin

```asm
; Configurar PA6 como salida con velocidad media
.syntax unified
.thumb

; Direcciones base
.equ RCC_AHB1ENR, 0x40023830
.equ GPIOA_BASE,  0x40020000

.global config_pa6
config_pa6:
    PUSH {R4, LR}
    
    ; 1. Habilitar clock GPIOA
    LDR R0, =RCC_AHB1ENR
    LDR R1, [R0]
    ORR R1, R1, #(1 << 0)
    STR R1, [R0]
    
    ; 2. Configurar MODER (bits 12-13 = 01 para salida)
    LDR R0, =GPIOA_BASE
    LDR R1, [R0, #0x00]     ; Leer MODER
    BIC R1, R1, #(0x3 << 12) ; Limpiar bits 12-13
    ORR R1, R1, #(0x1 << 12) ; Establecer 01
    STR R1, [R0, #0x00]     ; Escribir MODER
    
    ; 3. Configurar OTYPER (push-pull = 0)
    LDR R1, [R0, #0x04]     ; Leer OTYPER
    BIC R1, R1, #(1 << 6)   ; Limpiar bit 6
    STR R1, [R0, #0x04]
    
    ; 4. Configurar OSPEEDR (velocidad media = 01)
    LDR R1, [R0, #0x08]     ; Leer OSPEEDR
    BIC R1, R1, #(0x3 << 12)
    ORR R1, R1, #(0x1 << 12)
    STR R1, [R0, #0x08]
    
    ; 5. Configurar PUPDR (sin pull = 00)
    LDR R1, [R0, #0x0C]
    BIC R1, R1, #(0x3 << 12)
    STR R1, [R0, #0x0C]
    
    POP {R4, PC}

; Función para toggle de LED
.global led_toggle
led_toggle:
    LDR R0, =GPIOA_BASE
    LDR R1, [R0, #0x14]     ; Leer ODR
    EOR R1, R1, #(1 << 6)   ; Toggle bit 6
    STR R1, [R0, #0x14]     ; Escribir ODR
    BX LR

; Función para encender LED
.global led_on
led_on:
    LDR R0, =GPIOA_BASE
    MOV R1, #(1 << 6)
    STR R1, [R0, #0x18]     ; BSRR set
    BX LR

; Función para apagar LED
.global led_off
led_off:
    LDR R0, =GPIOA_BASE
    MOV R1, #(1 << 22)      ; bit 6 + 16 = reset
    STR R1, [R0, #0x18]     ; BSRR reset
    BX LR
```

#### Delay en Assembly

```asm
; void delay_ms(uint32_t ms)
; Delay aproximado (asumiendo 16MHz)
.global delay_ms
delay_ms:
    PUSH {R4, LR}
    MOV R4, R0              ; Guardar ms
    
delay_loop:
    CMP R4, #0
    BEQ delay_fin
    
    ; Inner loop: ~1ms @ 16MHz (ajustar según necesidad)
    LDR R1, =5333           ; Ciclos para ~1ms
inner_loop:
    SUBS R1, R1, #1
    BNE inner_loop
    
    SUBS R4, R4, #1
    B delay_loop
    
delay_fin:
    POP {R4, PC}
```

---

## Actividades

### Actividad 1: Implementar Estructuras de Control (40 minutos)

**Ejercicio 1**: Sumar números del 1 al 10
```
Resultado esperado en R0: 55
Implementar usando un bucle for
```

**Ejercicio 2**: Encontrar el máximo en un array
```asm
.data
array: .word 5, 12, 3, 18, 7
size: .word 5
max: .word 0

.text
; Escribir código para encontrar el máximo y guardarlo en 'max'
```

**Ejercicio 3**: Implementar if-else anidado
```
if (R0 > 50) {
    if (R0 > 75)
        R1 = 3
    else
        R1 = 2
} else {
    R1 = 1
}
```

### Actividad 2: Desarrollo de Funciones (30 minutos)

**Ejercicio 1**: Función potencia
```asm
; int potencia(int base, int exponente)
; Calcular base^exponente sin usar recursión
```

**Ejercicio 2**: Función para copiar array
```asm
; void copiar_array(int *origen, int *destino, int n)
; Copiar n elementos de origen a destino
```

### Actividad 3: Programa Completo de Parpadeo (30 minutos)

**Tarea**: Crear un programa completo en Assembly que:
1. Configure PA6, PA7, PA8 como salidas
2. Los haga parpadear en secuencia (uno a la vez)
3. Use funciones modulares (config_gpio, led_on, led_off, delay)
4. Incluya comentarios explicativos

**Estructura sugerida**:
```asm
.syntax unified
.thumb

.global main

main:
    BL config_todos_leds
    
bucle_principal:
    ; Encender PA6
    MOV R0, #6
    BL led_on
    LDR R0, =500
    BL delay_ms
    
    ; Apagar PA6
    MOV R0, #6
    BL led_off
    
    ; ... continuar con PA7 y PA8 ...
    
    B bucle_principal
```

---

## Evaluación Formativa

### Quiz - Sesión 6

1. ¿Qué diferencia hay entre `B` y `BL`? ¿Para qué se usa cada uno? (2 puntos)
2. ¿Qué hace la instrucción `CMP R0, #10` y cómo afecta los flags? (2 puntos)
3. ¿Qué registros se usan para pasar parámetros a funciones según AAPCS? (2 puntos)
4. ¿Por qué es necesario usar PUSH y POP en funciones que llaman otras funciones? (2 puntos)
5. Escribe un bucle while en Assembly que cuente de 10 a 0 y guarde el resultado en R1 (2 puntos)

---

## Evidencias de Aprendizaje

**Entregable**: Programa funcional en Assembly que:
1. Configure 3 LEDs en pines diferentes
2. Los encienda en secuencia con delays
3. Use al menos 3 funciones (config, toggle/on/off, delay)
4. Incluya comentarios explicativos en cada sección
5. Compile sin errores con arm-none-eabi-as

**Formato**: Archivo .s con código completo  
**Evaluación**: Funcionalidad (50%) + estructura modular (30%) + comentarios (20%)

---

## Material para Casa

### Lectura Previa a Sesión 7
- Reference Manual STM32F407: Capítulo GPIO (secciones 8.1-8.4)
- Concepto de registros de periféricos
- Mapa de memoria de STM32F407

### Ejercicio Opcional
Implementar en Assembly una función que:
- Reciba un array de enteros
- Ordene el array (bubble sort)
- Retorne el array ordenado

---

## Notas para el Instructor

### Enfoque de la Sesión
Esta sesión consolida el conocimiento de Assembly antes de la transición a periféricos. Enfatizar:
- Importancia de funciones modulares
- Convenciones de llamada (crucial para debugging)
- Relación entre código Assembly y manipulación de hardware

### Demos Recomendadas
1. Mostrar ejecución paso a paso de función con stack usando debugger
2. Demostrar cómo el código modular facilita el debugging
3. Mostrar diferencia de timing entre delays implementados de diferentes formas

### Errores Comunes
- Olvidar PUSH/POP de LR en funciones que llaman otras funciones
- Confundir BX con B (uno usa registro, otro label)
- No alinear stack correctamente (debe ser múltiplo de 8)

---

**Próxima sesión**: Configuración Avanzada de GPIO en Assembly  
**Preparación**: Revisar datasheet de GPIO, especialmente registros MODER, OTYPER, OSPEEDR, PUPDR
