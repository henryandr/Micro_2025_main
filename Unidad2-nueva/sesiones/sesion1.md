# Sesión 1: Transición de Assembly a C y Estructura de Proyectos Bare Metal

**Duración**: 2 horas  
**Modalidad**: Presencial con práctica guiada

---

## Objetivos Específicos

1. **Comprender** las diferencias entre programación en Assembly y C para sistemas embebidos
2. **Identificar** los componentes esenciales de un proyecto C bare metal (startup code, linker script, Makefile)
3. **Implementar** primer programa en C bare metal que configure y controle GPIO

---

## Contenidos Temáticos

### 1. Repaso y Transición desde Unidad 1 (15 minutos)

#### Lo que Aprendimos en Unidad 1

**Logros**:
- ✅ Arquitectura ARM Cortex M-4 a profundidad
- ✅ Programación en Assembly (instrucciones, funciones, control de flujo)
- ✅ Configuración de GPIO en Assembly
- ✅ Conceptos de interrupciones y NVIC
- ✅ Lectura de datasheets

**¿Por qué ahora C?**

| Aspecto | Assembly | C |
|---------|----------|---|
| **Control** | Total, cada instrucción visible | Alto, pero con abstracciones |
| **Velocidad de desarrollo** | Lenta | Rápida (5-10x más código en mismo tiempo) |
| **Legibilidad** | Baja para código complejo | Alta |
| **Mantenibilidad** | Difícil | Fácil |
| **Portabilidad** | Nula (específico del procesador) | Moderada (recompilar para otro MCU) |
| **Depuración** | Compleja | Más sencilla (herramientas mejores) |
| **Tamaño de código** | Óptimo si se optimiza bien | Comparable con -O2 o -O3 |

**Conclusión**: Assembly da comprensión profunda, C permite construcción de sistemas más complejos.

### 2. C para Sistemas Embebidos vs C Estándar (20 minutos)

#### Diferencias Clave

**C Estándar** (PC, con sistema operativo):
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Hello World\n");
    int *ptr = malloc(100 * sizeof(int));
    free(ptr);
    return 0;
}
```

**C Bare Metal** (microcontrolador, sin OS):
```c
#include <stdint.h>

int main(void) {
    // Configurar hardware directamente
    *(volatile uint32_t*)0x40023830 |= (1 << 0);  // Clock
    *(volatile uint32_t*)0x40020000 |= (1 << 12); // GPIO
    
    while(1) {
        // Bucle infinito (nunca retorna)
    }
}
```

#### Restricciones en C Bare Metal

**NO disponible**:
- ❌ `printf()` (a menos que se implemente redirección a UART)
- ❌ `malloc()/free()` (no hay gestor de memoria dinámica)
- ❌ Sistema de archivos (`fopen()`, `fread()`, etc.)
- ❌ Bibliotecas estándar completas

**SÍ disponible**:
- ✅ Tipos de datos estándar (`uint8_t`, `uint32_t`, etc.)
- ✅ Operadores (aritméticos, lógicos, bit-wise)
- ✅ Estructuras de control (`if`, `while`, `for`, `switch`)
- ✅ Funciones
- ✅ Punteros (cruciales para acceso a registros)
- ✅ `struct`, `enum`, `union`
- ✅ Macros del preprocesador

#### Palabra Clave Crítica: `volatile`

**¿Por qué `volatile`?**

El compilador optimiza código asumiendo que las variables solo cambian cuando el programa las modifica. Pero en sistemas embebidos:
- Hardware puede cambiar registros
- Interrupciones pueden cambiar variables

**Sin `volatile`**:
```c
uint32_t *reg = (uint32_t*)0x40020014;  // GPIO ODR
*reg = 0x01;  // Escribir
*reg = 0x01;  // Compilador puede ELIMINAR esta línea (optimización)
```

**Con `volatile`**:
```c
volatile uint32_t *reg = (volatile uint32_t*)0x40020014;
*reg = 0x01;  // Escribir
*reg = 0x01;  // Compilador NO elimina (puede haber cambio externo)
```

**Regla**: SIEMPRE usar `volatile` para:
1. Punteros a registros de hardware
2. Variables compartidas entre main e ISR
3. Variables que pueden cambiar por hardware

### 3. Estructura de un Proyecto C Bare Metal (30 minutos)

#### Componentes Esenciales

```
proyecto/
├── src/
│   ├── main.c              # Código principal
│   └── system_init.c       # Inicialización del sistema
├── inc/
│   ├── stm32f407xx.h       # Definiciones de registros
│   └── system_init.h       # Header de inicialización
├── startup/
│   └── startup_stm32f407.s # Código de inicio (Assembly)
├── linker_script.ld        # Script de enlazado
├── Makefile                # Automatización de compilación
└── README.md               # Documentación
```

#### 1. Startup Code (startup_stm32f407.s)

**Propósito**: Ejecutar antes de `main()` para inicializar el sistema

**Tareas del startup code**:
1. Definir stack pointer inicial
2. Definir vector table (direcciones de handlers de interrupciones)
3. Copiar sección `.data` de Flash a RAM
4. Inicializar sección `.bss` a cero
5. Llamar a `main()`
6. Si `main()` retorna (no debería), bucle infinito

**Estructura básica**:
```asm
.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler

/* Vector Table */
.section .isr_vector, "a"
vector_table:
    .word _estack           /* 0: Stack pointer inicial */
    .word Reset_Handler     /* 1: Reset handler */
    .word NMI_Handler       /* 2: NMI */
    .word HardFault_Handler /* 3: Hard Fault */
    /* ... más vectores ... */

/* Reset Handler */
.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    /* 1. Copiar .data de Flash a RAM */
    ldr r0, =_sdata         /* Inicio de .data en RAM */
    ldr r1, =_edata         /* Fin de .data en RAM */
    ldr r2, =_sidata        /* Inicio de .data en Flash */
    b copy_data_check
copy_data_loop:
    ldr r3, [r2], #4        /* Leer de Flash, incrementar */
    str r3, [r0], #4        /* Escribir a RAM, incrementar */
copy_data_check:
    cmp r0, r1
    blo copy_data_loop
    
    /* 2. Inicializar .bss a cero */
    ldr r0, =_sbss
    ldr r1, =_ebss
    mov r2, #0
    b init_bss_check
init_bss_loop:
    str r2, [r0], #4
init_bss_check:
    cmp r0, r1
    blo init_bss_loop
    
    /* 3. Llamar a main() */
    bl main
    
    /* 4. Si retorna, bucle infinito */
    b .

/* Handlers por defecto */
.section .text.Default_Handler
Default_Handler:
    b .

/* Alias para handlers no implementados */
.weak NMI_Handler
.thumb_set NMI_Handler, Default_Handler

.weak HardFault_Handler
.thumb_set HardFault_Handler, Default_Handler
```

#### 2. Linker Script (linker_script.ld)

**Propósito**: Indicar al enlazador cómo organizar código y datos en memoria

**Estructura**:
```ld
/* Regiones de memoria del STM32F407 */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Definir tamaño de stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Secciones */
SECTIONS
{
    /* Vector table al inicio de Flash */
    .isr_vector : {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH
    
    /* Código en Flash */
    .text : {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
        _etext = .;
    } > FLASH
    
    /* Datos inicializados: en Flash, copiados a RAM */
    .data : {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT> FLASH
    
    _sidata = LOADADDR(.data);
    
    /* Datos no inicializados: en RAM, inicializados a 0 */
    .bss : {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM
}
```

#### 3. Archivo de Definiciones (stm32f407xx.h)

**Propósito**: Definir direcciones base y estructuras de registros

**Ejemplo**:
```c
#ifndef STM32F407XX_H
#define STM32F407XX_H

#include <stdint.h>

/* Direcciones base de periféricos */
#define PERIPH_BASE       0x40000000UL
#define AHB1PERIPH_BASE   (PERIPH_BASE + 0x00020000UL)
#define RCC_BASE          (AHB1PERIPH_BASE + 0x3800UL)
#define GPIOA_BASE        (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE        (AHB1PERIPH_BASE + 0x0400UL)

/* Estructura GPIO */
typedef struct {
    volatile uint32_t MODER;     /* Offset 0x00 */
    volatile uint32_t OTYPER;    /* Offset 0x04 */
    volatile uint32_t OSPEEDR;   /* Offset 0x08 */
    volatile uint32_t PUPDR;     /* Offset 0x0C */
    volatile uint32_t IDR;       /* Offset 0x10 */
    volatile uint32_t ODR;       /* Offset 0x14 */
    volatile uint32_t BSRR;      /* Offset 0x18 */
    volatile uint32_t LCKR;      /* Offset 0x1C */
    volatile uint32_t AFR[2];    /* Offset 0x20-0x24 */
} GPIO_TypeDef;

/* Estructura RCC */
typedef struct {
    volatile uint32_t CR;        /* Offset 0x00 */
    volatile uint32_t PLLCFGR;   /* Offset 0x04 */
    volatile uint32_t CFGR;      /* Offset 0x08 */
    volatile uint32_t CIR;       /* Offset 0x0C */
    /* ... más registros ... */
    volatile uint32_t AHB1ENR;   /* Offset 0x30 */
} RCC_TypeDef;

/* Punteros a periféricos */
#define GPIOA ((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef*)GPIOB_BASE)
#define RCC   ((RCC_TypeDef*)RCC_BASE)

#endif /* STM32F407XX_H */
```

#### 4. Makefile

**Propósito**: Automatizar compilación y enlazado

**Ejemplo básico**:
```makefile
# Toolchain
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Flags
CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
         -O2 -Wall -Wextra -g -I./inc

LDFLAGS = -T linker_script.ld -Wl,-Map=output.map

# Archivos fuente
C_SRCS = $(wildcard src/*.c)
ASM_SRCS = startup/startup_stm32f407.s

# Archivos objeto
C_OBJS = $(C_SRCS:.c=.o)
ASM_OBJS = $(ASM_SRCS:.s=.o)

# Target principal
all: programa.bin

# Enlazar
programa.elf: $(C_OBJS) $(ASM_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

# Generar binario
%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

# Compilar C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ensamblar
%.o: %.s
	$(AS) -mcpu=cortex-m4 -mthumb $< -o $@

# Limpiar
clean:
	rm -f $(C_OBJS) $(ASM_OBJS) *.elf *.bin *.map

.PHONY: all clean
```

### 4. Primer Programa en C (30 minutos)

#### main.c - Blink LED

```c
/**
 * @file main.c
 * @brief Primer programa en C bare metal - Blink LED
 * @author Curso Microprocesadores
 * @date 2025
 */

#include "stm32f407xx.h"

/**
 * @brief Delay simple (busy-wait)
 * @param count Número de iteraciones
 */
void delay(volatile uint32_t count) {
    while(count--);
}

/**
 * @brief Función principal
 */
int main(void) {
    /* 1. Habilitar clock de GPIOA (bit 0 de RCC_AHB1ENR) */
    RCC->AHB1ENR |= (1 << 0);
    
    /* 2. Configurar PA6 como salida */
    /* Limpiar bits 12-13 (MODER6) */
    GPIOA->MODER &= ~(0x3 << 12);
    /* Establecer 01 (salida) */
    GPIOA->MODER |= (0x1 << 12);
    
    /* 3. Configurar como push-pull (por defecto, OTYPER bit 6 = 0) */
    GPIOA->OTYPER &= ~(1 << 6);
    
    /* 4. Configurar velocidad media (OSPEEDR bits 12-13 = 01) */
    GPIOA->OSPEEDR &= ~(0x3 << 12);
    GPIOA->OSPEEDR |= (0x1 << 12);
    
    /* 5. Sin pull-up/pull-down (PUPDR bits 12-13 = 00) */
    GPIOA->PUPDR &= ~(0x3 << 12);
    
    /* 6. Bucle principal - parpadear LED */
    while(1) {
        /* Encender LED (usar BSRR para operación atómica) */
        GPIOA->BSRR = (1 << 6);      /* Set bit 6 */
        delay(1000000);
        
        /* Apagar LED */
        GPIOA->BSRR = (1 << 22);     /* Reset bit 6 (6+16) */
        delay(1000000);
    }
    
    return 0;  /* Nunca se alcanza */
}
```

#### Comparación con Assembly

**Assembly** (Unidad 1):
```asm
; Configurar PA6 como salida
LDR R0, =0x40020000      ; GPIOA_BASE
LDR R1, [R0, #0x00]      ; Leer MODER
BIC R1, R1, #(0x3 << 12) ; Limpiar
ORR R1, R1, #(0x1 << 12) ; Establecer
STR R1, [R0, #0x00]      ; Escribir
```

**C**:
```c
GPIOA->MODER &= ~(0x3 << 12);
GPIOA->MODER |= (0x1 << 12);
```

**¿Qué genera el compilador de C?**
```asm
; Código Assembly generado por gcc -O2
ldr r3, =0x40020000      ; Equivalente a Assembly manual
ldr r2, [r3, #0]
bic r2, r2, #12288       ; ~(0x3 << 12)
orr r2, r2, #4096        ; (0x1 << 12)
str r2, [r3, #0]
```

**Conclusión**: Código C bien optimizado genera Assembly casi idéntico al escrito manualmente.

---

## Actividades

### Actividad 1: Comparar Código Assembly y C (15 minutos)

**Tarea**: Dado el siguiente código Assembly de Unidad 1, escribir la versión equivalente en C.

**Assembly**:
```asm
; Encender LED en PA5
LDR R0, =0x40020000
MOV R1, #(1 << 5)
STR R1, [R0, #0x18]      ; BSRR
```

**Respuesta esperada**:
```c
GPIOA->BSRR = (1 << 5);
```

### Actividad 2: Configurar Proyecto desde Cero (30 minutos)

**Tarea**: Siguiendo la plantilla proporcionada, crear un proyecto C bare metal que:
1. Configure PA5, PA6, PA7 como salidas
2. Los encienda en secuencia (uno a la vez)
3. Use delay entre cambios

**Plantilla proporcionada**:
- `stm32f407xx.h`
- `startup_stm32f407.s`
- `linker_script.ld`
- `Makefile`

**Estudiantes completan**: `main.c`

### Actividad 3: Debugging de Código (20 minutos)

**Código con error**:
```c
#include "stm32f407xx.h"

int main(void) {
    /* Habilitar clock GPIOA */
    RCC->AHB1ENR |= (1 << 0);
    
    /* Configurar PA6 como salida */
    uint32_t *moder = (uint32_t*)0x40020000;  // ERROR: falta volatile
    *moder &= ~(0x3 << 12);
    *moder |= (0x1 << 12);
    
    while(1) {
        *moder ^= (1 << 6);  // ERROR: debería usar ODR o BSRR, no MODER
        delay(1000000);
    }
}
```

**Preguntas**:
1. ¿Qué errores tiene el código?
2. ¿Por qué es importante `volatile`?
3. ¿Cuál es la forma correcta de toggle de LED?

---

## Evaluación Formativa

### Quiz - Sesión 1

1. ¿Qué palabra clave se debe usar siempre con punteros a registros de hardware? (2 puntos)
   - a) `const`
   - b) `static`
   - c) `volatile` ✅
   - d) `extern`

2. ¿Qué hace el startup code ANTES de llamar a `main()`? (2 puntos)
   - Copiar `.data` de Flash a RAM y inicializar `.bss` ✅

3. ¿Por qué `printf()` no está disponible por defecto en C bare metal? (2 puntos)
   - No hay sistema operativo ni biblioteca estándar completa ✅

4. ¿Qué registro se debe usar para operaciones atómicas de set/reset de pines GPIO? (2 puntos)
   - a) ODR
   - b) IDR
   - c) BSRR ✅
   - d) MODER

5. Escribe en C la línea que habilita el clock de GPIOB (bit 1 de RCC_AHB1ENR) (2 puntos)
   - `RCC->AHB1ENR |= (1 << 1);` ✅

---

## Evidencias de Aprendizaje

**Entregable**: Proyecto C bare metal funcional que:
1. Compile sin errores ni warnings
2. Configure 3 LEDs (PA5, PA6, PA7)
3. Los encienda en secuencia con delays
4. Incluya comentarios en cada paso
5. Use estructuras correctamente (ej: `GPIOA->MODER`)

**Formato**: 
- Carpeta comprimida con todos los archivos (.c, .h, .s, .ld, Makefile)
- README.md con instrucciones de compilación

**Evaluación**:
- Compilación exitosa: 30%
- Funcionalidad correcta: 40%
- Uso correcto de `volatile` y estructuras: 20%
- Comentarios y claridad: 10%

---

## Material para Casa

### Lectura Previa a Sesión 2
- Material sobre Máquinas de Estados Finitos (MEF)
- Concepto de estados y transiciones
- Ejemplos de sistemas con estados (semáforo, lavadora, cajero automático)

### Video recomendado (opcional)
- "Finite State Machines Explained" (10 minutos)

### Ejercicio Opcional
Modificar el programa para que los LEDs parpadeen con diferentes frecuencias:
- PA5: rápido (250ms)
- PA6: medio (500ms)
- PA7: lento (1000ms)

---

## Notas para el Instructor

### Preparación
- Tener proyecto de ejemplo completamente funcional
- Probar compilación en máquina del laboratorio
- Tener datasheet de STM32F407 disponible

### Demos Importantes
1. Compilar paso a paso (gcc, ld, objcopy)
2. Ver código Assembly generado por C (`gcc -S`)
3. Depurar con gdb (si tiempo permite)

### Errores Comunes
- Olvidar `volatile`
- Confundir offsets de registros
- No habilitar clock del periférico
- Usar ODR en lugar de BSRR (potencial race condition)

### Flexibilidad
- Si el grupo tiene dificultades con C, dedicar más tiempo a repaso
- Si van rápido, introducir conceptos de siguiente sesión

---

**Próxima sesión**: Máquinas de Estados Finitos - Teoría y Diseño  
**Preparación**: Pensar en ejemplos de sistemas con estados del mundo real
