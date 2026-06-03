# Comparación Assembly vs C - Conceptos y Traducción

Documento que facilita la transición de Unidad 1 (Assembly) a Unidad 2 (C).

---

## 1. Operaciones Básicas

### Asignación de Variable

**Assembly**:
```asm
MOV R0, #5      ; R0 = 5
```

**C**:
```c
uint32_t valor = 5;
// o directamente en registro (optimización)
register uint32_t valor asm("r0") = 5;
```

**Equivalencia**: MOV directo se traduce a asignación simple en C.

---

### Suma

**Assembly**:
```asm
MOV R0, #10
MOV R1, #20
ADD R2, R0, R1  ; R2 = R0 + R1 = 30
```

**C**:
```c
uint32_t a = 10;
uint32_t b = 20;
uint32_t resultado = a + b;  // resultado = 30
```

**Nota**: Compilador asigna registros automáticamente.

---

### Operaciones Bit a Bit

**Assembly**:
```asm
LDR R0, =0xFFFF
MOV R1, #0x00F0
AND R2, R0, R1  ; R2 = R0 & R1
ORR R3, R0, R1  ; R3 = R0 | R1
EOR R4, R0, R1  ; R4 = R0 ^ R1 (XOR)
MVN R5, R0      ; R5 = ~R0 (NOT)
```

**C**:
```c
uint32_t val1 = 0xFFFF;
uint32_t val2 = 0x00F0;
uint32_t and_result = val1 & val2;
uint32_t or_result  = val1 | val2;
uint32_t xor_result = val1 ^ val2;
uint32_t not_result = ~val1;
```

**Mapeo directo**: Operadores C se traducen 1:1 a instrucciones Assembly.

---

## 2. Acceso a Memoria

### Lectura de Memoria

**Assembly**:
```asm
LDR R0, =0x40020000  ; Dirección de GPIOA
LDR R1, [R0]         ; Leer contenido en dirección R0
```

**C**:
```c
uint32_t *direccion = (uint32_t*)0x40020000;
uint32_t valor = *direccion;  // Dereferenciar puntero
```

**Equivalencia**: LDR [Rn] = dereferenciar puntero en C.

---

### Escritura en Memoria

**Assembly**:
```asm
LDR R0, =0x40020000  ; Dirección
MOV R1, #0x12345678  ; Valor
STR R1, [R0]         ; Escribir R1 en [R0]
```

**C**:
```c
uint32_t *direccion = (uint32_t*)0x40020000;
*direccion = 0x12345678;  // Escribir valor
```

**Equivalencia**: STR Rn, [Rm] = *puntero = valor en C.

---

### Acceso a Registros GPIO

**Assembly**:
```asm
; Configurar PA6 como salida
LDR R0, =0x40020000      ; GPIOA_BASE
LDR R1, [R0, #0x00]      ; Leer MODER
BIC R1, R1, #(0x3<<12)   ; Limpiar bits 12-13
ORR R1, R1, #(0x1<<12)   ; Establecer 01
STR R1, [R0, #0x00]      ; Escribir MODER
```

**C**:
```c
// Con estructura
GPIOA->MODER &= ~(0x3 << 12);  // Limpiar bits
GPIOA->MODER |= (0x1 << 12);   // Establecer bits

// Sin estructura (punteros directos)
volatile uint32_t *GPIOA_MODER = (volatile uint32_t*)0x40020000;
*GPIOA_MODER &= ~(0x3 << 12);
*GPIOA_MODER |= (0x1 << 12);
```

**Ventaja C**: Estructuras hacen código más legible.

---

## 3. Estructuras de Control

### Condicional Simple

**Assembly**:
```asm
CMP R0, #5
BEQ igual        ; Branch if equal
BGT mayor        ; Branch if greater
BLT menor        ; Branch if less than
```

**C**:
```c
if (valor == 5) {
    // igual
} else if (valor > 5) {
    // mayor
} else {
    // menor
}
```

**Mapeo**: CMP + Bxx → if/else en C.

---

### Bucle FOR

**Assembly**:
```asm
    MOV R0, #0           ; i = 0
loop:
    CMP R0, #10          ; ¿i < 10?
    BGE end_loop         ; Si i >= 10, salir
    
    ; Cuerpo del bucle
    ; ...
    
    ADD R0, R0, #1       ; i++
    B loop               ; Repetir
end_loop:
```

**C**:
```c
for (int i = 0; i < 10; i++) {
    // Cuerpo del bucle
}
```

**Equivalencia**: Contador + comparación + branch → for loop.

---

### Bucle WHILE

**Assembly**:
```asm
while_loop:
    LDR R0, [R1]         ; Leer condición
    CMP R0, #0
    BEQ end_while        ; Si 0, salir
    
    ; Cuerpo
    ; ...
    
    B while_loop
end_while:
```

**C**:
```c
while (condicion != 0) {
    // Cuerpo
}
```

---

## 4. Funciones

### Llamada a Función

**Assembly**:
```asm
; Preparar parámetros en R0-R3
MOV R0, #10
MOV R1, #20
BL mi_funcion    ; Llamar función
; Resultado en R0

mi_funcion:
    PUSH {LR}        ; Guardar link register
    ADD R0, R0, R1   ; Operación
    POP {PC}         ; Retornar
```

**C**:
```c
uint32_t mi_funcion(uint32_t a, uint32_t b) {
    return a + b;
}

// Llamada
uint32_t resultado = mi_funcion(10, 20);
```

**Convención ARM**: 
- R0-R3: Primeros 4 parámetros
- R0: Valor de retorno
- R4-R11: Preservar si se usan (callee-saved)

---

### Función con Variables Locales

**Assembly**:
```asm
mi_funcion:
    PUSH {R4, R5, LR}    ; Guardar registros
    SUB SP, SP, #8       ; Reservar stack para locales
    
    ; Variable local en [SP, #0]
    MOV R4, #100
    STR R4, [SP, #0]
    
    ; Operar...
    
    ADD SP, SP, #8       ; Liberar stack
    POP {R4, R5, PC}     ; Restaurar y retornar
```

**C**:
```c
uint32_t mi_funcion(void) {
    uint32_t local = 100;  // Compilador maneja stack
    // Operar...
    return local;
}
```

**Ventaja C**: Gestión automática de stack y registros.

---

## 5. Conceptos Avanzados

### Volatilidad

**Assembly**: Todos los accesos a memoria son "volátiles" por naturaleza.

**C**: Debe marcarse explícitamente:
```c
volatile uint32_t *reg = (volatile uint32_t*)0x40020000;
```

**Razón**: Sin `volatile`, compilador puede optimizar y eliminar accesos.

---

### Máscaras de Bits

**Assembly**:
```asm
LDR R0, =0x40020000
LDR R1, [R0]
BIC R1, R1, #(0x3<<12)   ; Limpiar bits
ORR R1, R1, #(0x1<<12)   ; Establecer bits
STR R1, [R0]
```

**C**:
```c
*reg &= ~(0x3 << 12);  // Limpiar
*reg |= (0x1 << 12);   // Establecer
```

**Patrón común**:
1. Limpiar: `&= ~(mask)`
2. Establecer: `|= (value)`
3. Toggle: `^= (mask)`
4. Leer: `& (mask)`

---

## 6. Interrupciones

### Handler de Interrupción

**Assembly**:
```asm
TIM2_IRQHandler:
    PUSH {R0, R1, LR}
    
    ; Leer y limpiar flag
    LDR R0, =TIM2_BASE
    LDR R1, [R0, #0x10]  ; SR
    BIC R1, R1, #1       ; Limpiar UIF
    STR R1, [R0, #0x10]
    
    ; Incrementar contador
    LDR R0, =millis
    LDR R1, [R0]
    ADD R1, R1, #1
    STR R1, [R0]
    
    POP {R0, R1, PC}
```

**C**:
```c
void TIM2_IRQHandler(void) {
    if (TIM2->SR & (1 << 0)) {
        TIM2->SR &= ~(1 << 0);  // Limpiar flag
        millis++;
    }
}
```

**Diferencia clave**: 
- Assembly: Manejo manual de stack
- C: Compilador genera prólogo/epílogo automáticamente

---

## 7. Ejemplo Completo: Blink LED

### Assembly (Unidad 1)

```asm
.syntax unified
.cpu cortex-m4
.thumb

.global main

main:
    ; Habilitar clock GPIOA
    LDR R0, =RCC_AHB1ENR
    LDR R1, [R0]
    ORR R1, R1, #1
    STR R1, [R0]
    
    ; Configurar PA6 como salida
    LDR R0, =GPIOA_MODER
    LDR R1, [R0]
    BIC R1, R1, #(0x3<<12)
    ORR R1, R1, #(0x1<<12)
    STR R1, [R0]
    
loop:
    ; Encender LED
    LDR R0, =GPIOA_BSRR
    MOV R1, #(1<<6)
    STR R1, [R0]
    
    ; Delay
    LDR R2, =1000000
delay1:
    SUBS R2, R2, #1
    BNE delay1
    
    ; Apagar LED
    LDR R0, =GPIOA_BSRR
    MOV R1, #(1<<22)
    STR R1, [R0]
    
    ; Delay
    LDR R2, =1000000
delay2:
    SUBS R2, R2, #1
    BNE delay2
    
    B loop
```

### C (Unidad 2)

```c
#include <stdint.h>

// Definiciones
#define RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef*)0x40020000)

void delay(volatile uint32_t count) {
    while(count--);
}

int main(void) {
    // Habilitar clock GPIOA
    RCC_AHB1ENR |= (1 << 0);
    
    // Configurar PA6 como salida
    GPIOA->MODER &= ~(0x3 << 12);
    GPIOA->MODER |= (0x1 << 12);
    
    while(1) {
        // Encender LED
        GPIOA->BSRR = (1 << 6);
        delay(1000000);
        
        // Apagar LED
        GPIOA->BSRR = (1 << 22);
        delay(1000000);
    }
    
    return 0;
}
```

**Comparación**:
- Assembly: 45 líneas
- C: 35 líneas
- C es más legible y mantenible
- Assembly da control total y puede ser más eficiente

---

## 8. Ventajas y Desventajas

### Assembly

**Ventajas**:
- Control total sobre registros
- Optimización máxima posible
- Tamaño de código predecible
- Útil para debug y comprensión profunda

**Desventajas**:
- Difícil de mantener
- Específico de arquitectura
- Propenso a errores
- Desarrollo lento

### C

**Ventajas**:
- Más legible y mantenible
- Portable (mayormente)
- Desarrollo más rápido
- Menos errores

**Desventajas**:
- Menos control sobre generación de código
- Puede generar código subóptimo
- Requiere entender compilador

---

## 9. Cuándo Usar Cada Uno

**Usar Assembly cuando**:
- Código crítico de timing
- Startup code
- Rutinas muy específicas de hardware
- Optimización extrema necesaria
- Aprendizaje de arquitectura

**Usar C cuando**:
- Desarrollo de aplicaciones
- Lógica de negocio
- Drivers de alto nivel
- Código portable
- Prototipado rápido

**Híbrido (C + Assembly inline)**:
- Operaciones atómicas críticas
- Instrucciones especiales de CPU
- Optimización de hotspots

---

## 10. Consejos para la Transición

1. **Piensa en alto nivel**: C abstrae detalles, enfócate en lógica
2. **Usa estructuras**: Mejor que punteros crudos
3. **Confía en el compilador**: Generalmente hace buen trabajo
4. **Revisa ensamblado generado**: Para entender y optimizar
5. **Usa volatile correctamente**: Para acceso a hardware
6. **Aprovecha funciones**: Modulariza más que en Assembly
7. **Debugger es tu amigo**: Menos tedioso que en Assembly
