# Sesión 6: Control de Flujo en Assembly y C Bare Metal

**Duración**: 2 horas  
**Modalidad**: Presencial con transición Assembly-C

---

## Objetivos Específicos

1. **Implementar** estructuras de control (if, while, for) en Assembly ARM
2. **Integrar** código Assembly con código C bare metal
3. **Comprender** el startup code y el proceso de inicialización del microcontrolador

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

### 3. Introducción a C Bare Metal (25 minutos)

#### Estructura Mínima de un Proyecto

```
proyecto/
├── startup_stm32f407.s     # Código de inicio
├── linker_script.ld        # Script de enlazado
├── main.c                  # Código principal en C
└── Makefile                # Compilación
```

#### Startup Code (startup_stm32f407.s)

**Propósito**: Inicializar el sistema antes de ejecutar main()

```asm
.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler

.section .isr_vector
    .word _estack           ; Stack pointer inicial
    .word Reset_Handler     ; Reset handler
    ; ... más vectores de interrupción

.section .text
Reset_Handler:
    ; 1. Copiar .data de Flash a RAM
    LDR R0, =_sdata
    LDR R1, =_edata
    LDR R2, =_sidata
copy_data:
    CMP R0, R1
    BGE init_bss
    LDR R3, [R2], #4
    STR R3, [R0], #4
    B copy_data
    
    ; 2. Inicializar .bss a cero
init_bss:
    LDR R0, =_sbss
    LDR R1, =_ebss
    MOV R2, #0
zero_bss:
    CMP R0, R1
    BGE call_main
    STR R2, [R0], #4
    B zero_bss
    
    ; 3. Llamar a main()
call_main:
    BL main
    
    ; 4. Si main retorna, bucle infinito
    B .
```

#### Linker Script (linker_script.ld)

**Propósito**: Definir cómo se organiza el código en memoria

```ld
MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    .isr_vector : {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    .text : {
        . = ALIGN(4);
        *(.text*)
        . = ALIGN(4);
    } > FLASH

    .data : {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        _edata = .;
    } > RAM AT> FLASH

    .bss : {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > RAM
}
```

#### main.c Básico

```c
#include <stdint.h>

// Dirección base de GPIOA
#define GPIOA_BASE  0x40020000
#define RCC_BASE    0x40023800

// Estructura para GPIO
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

// Puntero a estructura GPIOA
#define GPIOA ((GPIO_TypeDef*)GPIOA_BASE)

int main(void)
{
    // Habilitar clock GPIOA (RCC_AHB1ENR bit 0)
    *(volatile uint32_t*)(RCC_BASE + 0x30) |= (1 << 0);
    
    // Configurar PA6 como salida
    GPIOA->MODER &= ~(0x3 << 12);  // Limpiar bits 12-13
    GPIOA->MODER |= (0x1 << 12);   // Establecer a 01 (salida)
    
    // Bucle infinito con toggle
    while(1) {
        GPIOA->ODR ^= (1 << 6);    // Toggle PA6
        
        // Delay simple (busy-wait)
        for(volatile int i=0; i<1000000; i++);
    }
    
    return 0;
}
```

### 4. Interoperabilidad Assembly-C (10 minutos)

#### Llamar Assembly desde C

**delay.s**:
```asm
.syntax unified
.thumb

.global delay_cycles

; void delay_cycles(uint32_t cycles)
; R0 contiene número de ciclos
delay_cycles:
    SUBS R0, R0, #1
    BNE delay_cycles
    BX LR
```

**main.c**:
```c
extern void delay_cycles(uint32_t cycles);

int main(void)
{
    while(1) {
        // Toggle LED
        GPIOA->ODR ^= (1 << 6);
        
        // Llamar función Assembly
        delay_cycles(1000000);
    }
}
```

#### Llamar C desde Assembly

**main.s**:
```asm
.syntax unified
.thumb

.extern configurar_led    ; Función definida en C

.global main
main:
    BL configurar_led     ; Llamar función C
    
bucle:
    ; ... código ...
    B bucle
```

**led.c**:
```c
void configurar_led(void)
{
    // Configuración en C
    *(volatile uint32_t*)0x40023830 |= (1 << 0);  // RCC
    // ...
}
```

---

## Actividades

### Actividad 1: Implementar Estructuras de Control (30 minutos)

**Ejercicio 1**: Sumar números del 1 al 10
```
Resultado esperado: 55
```

**Ejercicio 2**: Encontrar el máximo en un array
```asm
.data
array: .word 5, 12, 3, 18, 7
size: .word 5
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

### Actividad 2: Debug de Código con Error (15 minutos)

**Código con bug**:
```asm
; Contar de 0 a 10
    MOV R0, #0
bucle:
    ADD R0, R0, #1
    CMP R0, #10
    BGT bucle        ; ERROR: debería ser BLE o cambiar condición
    B fin
fin:
```

**Tarea**: Identificar y corregir el error.

### Actividad 3: Primer Programa en C Bare Metal (25 minutos)

**Tarea**: Modificar el main.c de ejemplo para:
1. Configurar dos LEDs (PA6 y PA7)
2. Parpadear alternadamente
3. Usar función de delay en C

**Plantilla proporcionada**, estudiantes completan partes faltantes.

---

## Evaluación Formativa

### Quiz - Sesión 6

1. ¿Qué diferencia hay entre `B` y `BL`? (2 puntos)
2. ¿Qué hace la instrucción `CMP R0, #10`? (2 puntos)
3. ¿Por qué se necesita un startup code? (2 puntos)
4. ¿Qué es `volatile` en C y por qué se usa con registros de hardware? (2 puntos)
5. Escribe un bucle while en Assembly que cuente de 10 a 0 (2 puntos)

---

## Evidencias de Aprendizaje

**Entregable**: Programa funcional en C que:
1. Configure 3 LEDs
2. Los encienda en secuencia con delays
3. Incluya comentarios explicativos
4. Compile sin errores

**Formato**: Archivos main.c y Makefile  
**Evaluación**: Funcionalidad + estilo de código

---

## Notas para el Instructor

### Transición Importante
Esta sesión marca la transición de Assembly puro a C. Enfatizar que:
- C genera código Assembly
- Entender Assembly ayuda a escribir mejor C
- Bare metal significa control total pero más responsabilidad

### Demo Recomendada
- Mostrar compilación paso a paso: .c → .o → .elf → .bin
- Inspeccionar código Assembly generado por gcc (-S flag)

---

**Próxima sesión**: Drivers GPIO y Timer
**Preparación**: Leer Reference Manual sobre GPIO y TIM2
