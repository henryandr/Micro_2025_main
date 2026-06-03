/**
 * @file gpio_driver.s
 * @brief Driver GPIO básico en Assembly para STM32F407
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Driver modular para control de GPIO en Assembly puro.
 *   Implementa funciones para configuración y control de pines.
 *   
 * Hardware:
 *   - STM32F407VET6
 *   
 * Funciones públicas:
 *   - gpio_clock_enable(port_num)
 *   - gpio_set_mode(port, pin, mode)
 *   - gpio_set_output_type(port, pin, type)
 *   - gpio_set_speed(port, pin, speed)
 *   - gpio_set_pupd(port, pin, pupd)
 *   - gpio_write(port, pin, value)
 *   - gpio_read(port, pin)
 *   - gpio_toggle(port, pin)
 *   - gpio_init_output(port, pin)
 *   
 * Compilación:
 *   arm-none-eabi-as -mcpu=cortex-m4 -mthumb gpio_driver.s -o gpio_driver.o
 */

.syntax unified
.thumb

/* ========== Constantes ========== */

.equ RCC_AHB1ENR,  0x40023830

.equ GPIOA_BASE,   0x40020000
.equ GPIOB_BASE,   0x40020400
.equ GPIOC_BASE,   0x40020800
.equ GPIOD_BASE,   0x40020C00
.equ GPIOE_BASE,   0x40021000

/* Offsets de registros GPIO */
.equ GPIO_MODER,   0x00
.equ GPIO_OTYPER,  0x04
.equ GPIO_OSPEEDR, 0x08
.equ GPIO_PUPDR,   0x0C
.equ GPIO_IDR,     0x10
.equ GPIO_ODR,     0x14
.equ GPIO_BSRR,    0x18

/* ========== Funciones Públicas ========== */

.global gpio_clock_enable
.global gpio_set_mode
.global gpio_set_output_type
.global gpio_set_speed
.global gpio_set_pupd
.global gpio_write
.global gpio_read
.global gpio_toggle
.global gpio_init_output

.section .text

/**
 * @brief Habilita el clock de un puerto GPIO
 * @param R0 número de puerto (0=A, 1=B, 2=C, 3=D, 4=E)
 */
gpio_clock_enable:
    LDR R1, =RCC_AHB1ENR
    LDR R2, [R1]
    MOV R3, #1
    LSL R3, R3, R0           /* 1 << port_num */
    ORR R2, R2, R3
    STR R2, [R1]
    BX LR

/**
 * @brief Configura el modo de un pin
 * @param R0 dirección base del puerto
 * @param R1 número de pin (0-15)
 * @param R2 modo (0=input, 1=output, 2=AF, 3=analog)
 */
gpio_set_mode:
    PUSH {R4, R5, LR}
    
    /* Calcular desplazamiento: pin * 2 */
    LSL R3, R1, #1
    
    /* Crear máscara para limpiar */
    MOV R4, #0x3
    LSL R4, R4, R3
    MVN R4, R4               /* ~(0x3 << (pin*2)) */
    
    /* Leer-Modificar-Escribir */
    LDR R5, [R0, #GPIO_MODER]
    AND R5, R5, R4           /* Limpiar bits */
    LSL R2, R2, R3           /* mode << (pin*2) */
    ORR R5, R5, R2           /* Establecer nuevo modo */
    STR R5, [R0, #GPIO_MODER]
    
    POP {R4, R5, PC}

/**
 * @brief Configura el tipo de salida
 * @param R0 dirección base del puerto
 * @param R1 número de pin (0-15)
 * @param R2 tipo (0=push-pull, 1=open-drain)
 */
gpio_set_output_type:
    PUSH {R4, LR}
    
    LDR R3, [R0, #GPIO_OTYPER]
    MOV R4, #1
    LSL R4, R4, R1           /* 1 << pin */
    
    CMP R2, #0
    BEQ set_pushpull
    
    /* Open-drain: establecer bit */
    ORR R3, R3, R4
    B write_otyper
    
set_pushpull:
    /* Push-pull: limpiar bit */
    MVN R4, R4
    AND R3, R3, R4
    
write_otyper:
    STR R3, [R0, #GPIO_OTYPER]
    POP {R4, PC}

/**
 * @brief Configura la velocidad del pin
 * @param R0 dirección base del puerto
 * @param R1 número de pin (0-15)
 * @param R2 velocidad (0=low, 1=medium, 2=fast, 3=high)
 */
gpio_set_speed:
    PUSH {R4, R5, LR}
    
    LSL R3, R1, #1
    MOV R4, #0x3
    LSL R4, R4, R3
    MVN R4, R4
    
    LDR R5, [R0, #GPIO_OSPEEDR]
    AND R5, R5, R4
    LSL R2, R2, R3
    ORR R5, R5, R2
    STR R5, [R0, #GPIO_OSPEEDR]
    
    POP {R4, R5, PC}

/**
 * @brief Configura pull-up/pull-down
 * @param R0 dirección base del puerto
 * @param R1 número de pin (0-15)
 * @param R2 pupd (0=none, 1=pull-up, 2=pull-down)
 */
gpio_set_pupd:
    PUSH {R4, R5, LR}
    
    LSL R3, R1, #1
    MOV R4, #0x3
    LSL R4, R4, R3
    MVN R4, R4
    
    LDR R5, [R0, #GPIO_PUPDR]
    AND R5, R5, R4
    LSL R2, R2, R3
    ORR R5, R5, R2
    STR R5, [R0, #GPIO_PUPDR]
    
    POP {R4, R5, PC}

/**
 * @brief Escribe un valor en un pin (usando BSRR para operación atómica)
 * @param R0 dirección base del puerto
 * @param R1 número de pin (0-15)
 * @param R2 valor (0 o 1)
 */
gpio_write:
    CMP R2, #0
    BEQ write_zero
    
write_one:
    /* Set: bits 0-15 */
    MOV R2, #1
    LSL R2, R2, R1
    STR R2, [R0, #GPIO_BSRR]
    BX LR
    
write_zero:
    /* Reset: bits 16-31 */
    MOV R2, #1
    LSL R2, R2, R1
    LSL R2, R2, #16
    STR R2, [R0, #GPIO_BSRR]
    BX LR

/**
 * @brief Lee el valor de un pin
 * @param R0 dirección base del puerto
 * @param R1 número de pin (0-15)
 * @return R0 valor leído (0 o 1)
 */
gpio_read:
    LDR R2, [R0, #GPIO_IDR]
    LSR R2, R2, R1           /* Desplazar a derecha */
    AND R0, R2, #1           /* Aislar bit menos significativo */
    BX LR

/**
 * @brief Invierte el estado de un pin
 * @param R0 dirección base del puerto
 * @param R1 número de pin (0-15)
 */
gpio_toggle:
    PUSH {R4, LR}
    
    LDR R2, [R0, #GPIO_ODR]
    MOV R3, #1
    LSL R3, R3, R1
    EOR R2, R2, R3           /* XOR para toggle */
    STR R2, [R0, #GPIO_ODR]
    
    POP {R4, PC}

/**
 * @brief Inicializa un pin como salida con configuración estándar
 *        (Push-pull, velocidad media, sin pull-up/down)
 * @param R0 dirección base del puerto
 * @param R1 número de pin (0-15)
 */
gpio_init_output:
    PUSH {R4, R5, LR}
    MOV R4, R0               /* Guardar port */
    MOV R5, R1               /* Guardar pin */
    
    /* Habilitar clock - simplificado: solo GPIOA */
    LDR R0, =RCC_AHB1ENR
    LDR R1, [R0]
    ORR R1, R1, #(1 << 0)
    STR R1, [R0]
    
    /* Configurar modo = output */
    MOV R0, R4
    MOV R1, R5
    MOV R2, #1               /* Output mode */
    BL gpio_set_mode
    
    /* Configurar tipo = push-pull */
    MOV R0, R4
    MOV R1, R5
    MOV R2, #0
    BL gpio_set_output_type
    
    /* Configurar velocidad = medium */
    MOV R0, R4
    MOV R1, R5
    MOV R2, #1
    BL gpio_set_speed
    
    /* Configurar PUPD = none */
    MOV R0, R4
    MOV R1, R5
    MOV R2, #0
    BL gpio_set_pupd
    
    POP {R4, R5, PC}
