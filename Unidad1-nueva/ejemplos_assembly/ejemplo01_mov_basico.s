/**
 * Ejemplo 1: Instrucciones Básicas de Movimiento
 * 
 * Descripción:
 *   Este ejemplo demuestra el uso de instrucciones MOV básicas
 *   para transferir datos entre registros y cargar valores inmediatos.
 *
 * Conceptos:
 *   - MOV: Mover datos entre registros
 *   - Valores inmediatos (#valor)
 *   - Nomenclatura de registros (R0-R12)
 *
 * Target: ARM Cortex-M4
 * Toolchain: arm-none-eabi-gcc
 */

.syntax unified        /* Sintaxis unificada ARM/Thumb */
.cpu cortex-m4        /* CPU objetivo */
.thumb                /* Código Thumb (obligatorio en Cortex-M) */

.global main          /* Hacer main visible externamente */

.text                 /* Sección de código */

main:
    /* Cargar valores inmediatos en registros */
    MOV R0, #10       /* R0 = 10 */
    MOV R1, #20       /* R1 = 20 */
    MOV R2, #30       /* R2 = 30 */
    
    /* Copiar valor de un registro a otro */
    MOV R3, R0        /* R3 = R0 (R3 ahora es 10) */
    MOV R4, R1        /* R4 = R1 (R4 ahora es 20) */
    
    /* Mover valor hexadecimal */
    MOV R5, #0xFF     /* R5 = 255 (hexadecimal) */
    
    /* Limpiar registro (poner en cero) */
    MOV R6, #0        /* R6 = 0 */
    
    /* Bucle infinito - detener ejecución aquí */
    B .               /* Saltar a sí mismo (bucle infinito) */

/* Fin del programa */
.end
