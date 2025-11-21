/**
 * @file ejemplo04_funciones.s
 * @brief Ejemplos de funciones y manejo de stack
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Demuestra creación de funciones, paso de parámetros,
 *   valores de retorno y uso del stack
 *   
 * Hardware:
 *   - STM32F407VET6
 *   
 * Compilación:
 *   arm-none-eabi-as -mcpu=cortex-m4 -mthumb ejemplo04_funciones.s -o ejemplo04.o
 */

.syntax unified
.thumb

.global _start

.section .text

_start:
    ; Ejemplo 1: Función simple suma
    MOV R0, #10
    MOV R1, #20
    BL suma                 ; Llamar función
    ; R0 contiene 30

    ; Ejemplo 2: Función con más parámetros
    MOV R0, #5
    MOV R1, #3
    MOV R2, #2
    BL calcular             ; (a + b) * c
    ; R0 contiene 16

    ; Ejemplo 3: Función recursiva - factorial
    MOV R0, #5
    BL factorial
    ; R0 contiene 120

    B .

/**
 * @brief Suma dos números
 * @param R0 primer número
 * @param R1 segundo número
 * @return R0 suma
 */
suma:
    ADD R0, R0, R1
    BX LR

/**
 * @brief Calcula (a + b) * c
 * @param R0 = a
 * @param R1 = b
 * @param R2 = c
 * @return R0 = resultado
 */
calcular:
    ADD R0, R0, R1          ; a + b
    MUL R0, R0, R2          ; (a+b) * c
    BX LR

/**
 * @brief Calcula factorial de n
 * @param R0 = n
 * @return R0 = n!
 */
factorial:
    PUSH {R4, LR}           ; Guardar registros
    
    CMP R0, #1
    BLE factorial_base
    
    ; Caso recursivo
    MOV R4, R0              ; Guardar n
    SUB R0, R0, #1          ; n - 1
    BL factorial            ; factorial(n-1)
    MUL R0, R4, R0          ; n * factorial(n-1)
    B factorial_fin
    
factorial_base:
    MOV R0, #1              ; 0! = 1! = 1
    
factorial_fin:
    POP {R4, PC}            ; Restaurar y retornar
