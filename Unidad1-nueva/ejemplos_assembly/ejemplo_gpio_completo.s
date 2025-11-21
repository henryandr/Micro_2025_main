/**
 * @file ejemplo_gpio_completo.s
 * @brief Ejemplo completo de uso del driver GPIO
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Programa completo que usa el driver GPIO para controlar LEDs.
 *   Demuestra configuración y control de pines.
 *   
 * Hardware:
 *   - STM32F407VET6
 *   - LEDs en PA5, PA6, PA7
 *   
 * Compilación:
 *   arm-none-eabi-as -mcpu=cortex-m4 -mthumb gpio_driver.s -o gpio_driver.o
 *   arm-none-eabi-as -mcpu=cortex-m4 -mthumb ejemplo_gpio_completo.s -o main.o
 *   arm-none-eabi-ld -T linker_script.ld gpio_driver.o main.o -o programa.elf
 */

.syntax unified
.thumb

/* Importar funciones del driver */
.extern gpio_init_output
.extern gpio_write
.extern gpio_toggle

.equ GPIOA_BASE, 0x40020000

.global _start

.section .text

_start:
    BL main
    B .                      /* Bucle infinito si main retorna */

/**
 * @brief Función principal
 */
main:
    PUSH {LR}
    
    /* Inicializar LEDs */
    LDR R0, =GPIOA_BASE
    MOV R1, #5
    BL gpio_init_output      /* PA5 */
    
    LDR R0, =GPIOA_BASE
    MOV R1, #6
    BL gpio_init_output      /* PA6 */
    
    LDR R0, =GPIOA_BASE
    MOV R1, #7
    BL gpio_init_output      /* PA7 */
    
bucle_principal:
    /* Secuencia de LEDs */
    
    /* Encender PA5 */
    LDR R0, =GPIOA_BASE
    MOV R1, #5
    MOV R2, #1
    BL gpio_write
    LDR R0, =500
    BL delay_ms
    
    /* Apagar PA5 */
    LDR R0, =GPIOA_BASE
    MOV R1, #5
    MOV R2, #0
    BL gpio_write
    
    /* Encender PA6 */
    LDR R0, =GPIOA_BASE
    MOV R1, #6
    MOV R2, #1
    BL gpio_write
    LDR R0, =500
    BL delay_ms
    
    /* Apagar PA6 */
    LDR R0, =GPIOA_BASE
    MOV R1, #6
    MOV R2, #0
    BL gpio_write
    
    /* Encender PA7 */
    LDR R0, =GPIOA_BASE
    MOV R1, #7
    MOV R2, #1
    BL gpio_write
    LDR R0, =500
    BL delay_ms
    
    /* Apagar PA7 */
    LDR R0, =GPIOA_BASE
    MOV R1, #7
    MOV R2, #0
    BL gpio_write
    
    B bucle_principal

/**
 * @brief Delay aproximado en milisegundos
 * @param R0 milisegundos a esperar
 * @note Asume reloj de 16MHz (HSI). Ajustar constante según necesidad.
 */
delay_ms:
    PUSH {R4, LR}
    MOV R4, R0               /* Guardar ms */
    
delay_outer:
    CMP R4, #0
    BEQ delay_fin
    
    /* Inner loop: ~1ms @ 16MHz */
    LDR R1, =5333            /* Ajustar según timing real */
delay_inner:
    SUBS R1, R1, #1
    BNE delay_inner
    
    SUBS R4, R4, #1
    B delay_outer
    
delay_fin:
    POP {R4, PC}
