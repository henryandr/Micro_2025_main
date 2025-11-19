/**
 * Ejemplo 2: Operaciones Aritméticas
 * 
 * Descripción:
 *   Demuestra operaciones aritméticas básicas: suma, resta, 
 *   multiplicación y división.
 *
 * Conceptos:
 *   - ADD: Suma
 *   - SUB: Resta
 *   - MUL: Multiplicación
 *   - UDIV: División sin signo
 *
 * Target: ARM Cortex-M4
 */

.syntax unified
.cpu cortex-m4
.thumb

.global main

.text

main:
    /* Inicializar valores */
    MOV R0, #15       /* R0 = 15 */
    MOV R1, #10       /* R1 = 10 */
    MOV R2, #3        /* R2 = 3 */
    
    /* Suma: R3 = R0 + R1 */
    ADD R3, R0, R1    /* R3 = 15 + 10 = 25 */
    
    /* Resta: R4 = R0 - R1 */
    SUB R4, R0, R1    /* R4 = 15 - 10 = 5 */
    
    /* Multiplicación: R5 = R0 * R2 */
    MUL R5, R0, R2    /* R5 = 15 * 3 = 45 */
    
    /* División: R6 = R0 / R2 */
    UDIV R6, R0, R2   /* R6 = 15 / 3 = 5 */
    
    /* Operaciones compuestas */
    ADD R7, R0, #5    /* R7 = R0 + 5 = 20 */
    SUB R8, R3, #10   /* R8 = R3 - 10 = 15 */
    
    /* Calcular: (a + b) * c */
    MOV R0, #10       /* a = 10 */
    MOV R1, #5        /* b = 5 */
    MOV R2, #3        /* c = 3 */
    
    ADD R3, R0, R1    /* R3 = a + b = 15 */
    MUL R4, R3, R2    /* R4 = (a+b) * c = 45 */
    
    /* Bucle infinito */
    B .

.end
