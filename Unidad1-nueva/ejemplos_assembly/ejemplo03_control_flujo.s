/**
 * @file ejemplo03_control_flujo.s
 * @brief Ejemplos de control de flujo en Assembly
 * @author Curso Microprocesadores
 * @date 2025
 * 
 * Descripción:
 *   Demuestra estructuras de control: if-else, while, for
 *   
 * Hardware:
 *   - STM32F407VET6
 *   
 * Compilación:
 *   arm-none-eabi-as -mcpu=cortex-m4 -mthumb ejemplo03_control_flujo.s -o ejemplo03.o
 */

.syntax unified
.thumb

.global _start

.section .text

_start:
    ; Ejemplo 1: IF-ELSE simple
    MOV R0, #15
    CMP R0, #10
    BLE else_parte
    
    ; THEN: R0 > 10
    MOV R1, #1
    B fin_if
    
else_parte:
    MOV R1, #2
    
fin_if:
    ; R1 contiene resultado

    ; Ejemplo 2: WHILE loop - sumar 1 a 10
    MOV R0, #1              ; contador
    MOV R1, #0              ; suma
    
while_loop:
    CMP R0, #11
    BGE while_fin
    
    ADD R1, R1, R0          ; suma += contador
    ADD R0, R0, #1          ; contador++
    B while_loop
    
while_fin:
    ; R1 = 55 (suma de 1 a 10)

    ; Ejemplo 3: FOR loop - copiar array
    LDR R0, =origen         ; puntero origen
    LDR R1, =destino        ; puntero destino
    MOV R2, #5              ; contador (5 elementos)
    
for_loop:
    CMP R2, #0
    BEQ for_fin
    
    LDR R3, [R0], #4        ; Leer y post-incrementar
    STR R3, [R1], #4        ; Escribir y post-incrementar
    SUB R2, R2, #1
    B for_loop
    
for_fin:

    ; Bucle infinito
    B .

.section .data
origen:
    .word 1, 2, 3, 4, 5

destino:
    .space 20               ; 5 words = 20 bytes
