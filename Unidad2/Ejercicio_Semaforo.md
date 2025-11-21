# Adicional: Implementación FSM - Semáforo

## 📤 Tarea para subir al repositorio

A continuación te mostraré como implementar la máquina de estados de un semáforo. El reto consiste en realizar las siguientes tareas:

- El siguiente código fue implementado para el microcontrolador
- Comentar el código presentado, explicando lo que hace cada función o cada parte importante del código. Aprovecha la explicación del profesor para realizar este paso. Pregunta lo que no entiendas.
- Se espera que los tiempos de duración de cada estado sean los siguientes:
    - Rojo: 5 s
    - Rojo - Amarillo: 3 s
    - Verde: 5 s
    - Amarillo: 2 s
- Corrige el funcionamiento del código, pues como te darás cuenta, actualmente, no está funcionando como debería.
- Describe cuáles fueron los cambios que implementaste en el código y cuál fue el objetivo.
- Guarda en el repositorio esta actividad.

# Implementación del ejemplo

A continuación te mostraré el análisis y la implementación de una máquina de estados que simula el control de un semáforo.  La figura 1 es una representación de la manera como se debería controlar el sistema.

![Figura 1. Comportamiento del semáforo](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/855129db-3108-44e9-a7b9-7777b8bd1c3a/image.png)

Figura 1. Comportamiento del semáforo

## Estrategia para la implementación del código

A continuación, te presento una tabla que representa la estrategia de definición de una máquina de estados. Esta tabla comienza con la definición de la dirección base de la máquina, seguida de las entradas, estados y salidas del sistema. La tabla está organizada de manera que se detalla cómo cada elemento se alinea en la memoria y cómo se define dentro del sistema.

| **Elemento** | **Descripción** | **Dirección/Offset** | **Comentarios** |
| --- | --- | --- | --- |
| **Base_maquina_0** | Dirección base de la máquina de estados | `0x20001000` | La dirección base en la memoria RAM |
| **var_estado_M0** | Variable de estado actual de la máquina | Base_maquina_0 + `0x0` | Almacena el estado actual (ej. `0`, `1`, `2`, `3`) |
| **entrada_tiempo_M0** | Variable de tiempo transcurrido | Base_maquina_0 + `0x4` | Incrementado por el manejador de SysTick |
| **dir_tabla_estados** | Dirección de la tabla de estados | `0x20001008` | Tabla que contiene las direcciones de las funciones de estado |
| **estado_rojo** | Dirección de la función de estado "Rojo" | `0x20002000` | Inicio de la función de estado `estado_rojo` |
| **estado_rojo_amarillo** | Dirección de la función de estado "Rojo-Amarillo" | `0x20002020` | Inicio de la función de estado `estado_rojo_amarillo` |
| **estado_verde** | Dirección de la función de estado "Verde" | `0x20002040` | Inicio de la función de estado `estado_verde` |
| **estado_amarillo** | Dirección de la función de estado "Amarillo" | `0x20002060` | Inicio de la función de estado `estado_amarillo` |
| **entrada_sensor** | Variable que almacena la entrada del sensor de tiempo (SysTick) | Base_maquina_0 + `0x8` | Actualiza el tiempo transcurrido en la máquina |
| **GPIOB_PSOR** | Dirección de la salida para encender los LEDs | `0x400FF044` | Salida de los LEDs conectados en PTB12, PTB13, PTB14 |
| **GPIOB_PCOR** | Dirección de la salida para apagar los LEDs | `0x400FF048` | Salida para apagar los LEDs |

### **Explicación de la Tabla:**

- **Base_maquina_0**: Es la dirección base en la memoria donde se empieza a organizar la máquina de estados. Todas las variables y estructuras de la máquina están alineadas en memoria a partir de esta dirección base.
- **var_estado_M0**: Almacena el estado actual de la máquina (por ejemplo, `0` para "Rojo", `1` para "Rojo-Amarillo", etc.). Es una variable que se actualiza en cada transición de estado.
- **entrada_tiempo_M0**: Es la variable que cuenta el tiempo transcurrido en milisegundos. Se actualiza típicamente por el manejador de la interrupción SysTick.
- **dir_tabla_estados**: Es una tabla que contiene las direcciones de las funciones de estado. La máquina de estados usa esta tabla para determinar la siguiente función de estado a ejecutar.
- **estado_rojo, estado_rojo_amarillo, etc.**: Estas son las direcciones de las funciones de estado individuales que representan los diferentes estados en la máquina de estados.
- **entrada_sensor**: Puede representar cualquier entrada adicional, como un sensor externo. Es un ejemplo para mostrar cómo manejar entradas en el sistema.
- **GPIOB_PSOR y GPIOB_PCOR**: Direcciones para los registros de salida en GPIOB, utilizados para encender y apagar los LEDs correspondientes a los estados del semáforo.

### **Uso Práctico:**

Esta tabla se puede utilizar para planificar y organizar el código Assembly de una máquina de estados, asegurando que todas las variables, funciones y recursos estén bien definidos y alineados en memoria. Esta organización facilita la escritura, depuración y mantenimiento del código en un entorno de sistemas embebidos.

# Programa principal

```nasm
  #include "definitions.h"

  .syntax unified
  .global _start
  .extern estado_semaforo  
  .text

  .thumb_func

_start:
    bl PTB_init         
    bl gpioB_init       
    bl systick_config   

    // Máquina de estados
    ldr r4, =Base_maquina_0
    mov r1, #ROJO                     
    str r1, [r4, #var_estado_M0]
    mov r2, #0
    str r2, [r4, entrada_tiempo_M0]   

    // Bucle principal
loop_principal:
    bl estado_semaforo                
    b loop_principal                  

// Configuraciones iniciales
gpioB_init:    
    ldr r0, =GPIOB_PDDR               
    ldr r1, [r0]
    orr r1, r1, #(1 << LED_ROJO)      
    orr r1, r1, #(1 << LED_AMARILLO)  
    orr r1, r1, #(1 << LED_VERDE)     
    str r1, [r0]                      

    ldr r0, =GPIOB_PSOR
    mov r1, #(1 << LED_ROJO)
    str r1, [r0]                      
    mov r1, #(1 << LED_AMARILLO)
    str r1, [r0]                      
    mov r1, #(1 << LED_VERDE)   
    str r1, [r0]                    
    bx  lr

    // Subrutina de configuración de periféricos
PTB_init:
    // Habilitar el reloj para el puerto B
    ldr r0, =PCC_PORTB                 
    ldr r1, [r0]                       
    orr r1, r1, #PCC_PORTB_CGC         
    str r1, [r0]                       

    // Configurar el puerto
    ldr r0, =PORTB_PCR12               
    ldr r1, [r0]                       
    bic r1, r1, #(0x7 << 8)            
    orr r1, r1, #(MUX_GPIO << 8)       
    str r1, [r0]                       

    // Configurar el puerto
    ldr r0, =PORTB_PCR13               
    ldr r1, [r0]
    bic r1, r1, #(0x7 << 8)
    orr r1, r1, #(MUX_GPIO << 8)
    str r1, [r0]

    // Configurar el puerto
    ldr r0, =PORTB_PCR14               
    ldr r1, [r0]
    bic r1, r1, #(0x7 << 8)
    orr r1, r1, #(MUX_GPIO << 8)
    str r1, [r0]

    // Retornar de la subrutina
    bx lr

systick_config:
   // Configurar SysTick 
    ldr r0, =SYST_RVR
    ldr r1, =SYSTICK_RELOAD_1MS
    str r1, [r0]                      

    ldr r0, =SYST_CVR
    mov r1, #0
    str r1, [r0]                      

    ldr r0, =SYST_CSR
    mov r1, #(SYSTICK_ENABLE | SYSTICK_TICKINT | SYSTICK_CLKSOURCE)
    str r1, [r0]                      // Habilitar el SysTick, la interrupción y seleccionar el reloj del procesador
    bx  lr
```

# Máquina de estados (FSM)

```nasm
  
   #include "definitions.h"

  .syntax unified
  .global estado_semaforo
  .text

  .align 2                // Alinear la tabla de direcciones a 4 bytes (2^2 = 4)
// Lista de direcciones de los estados
dir_tabla_estados:
  .long estado_rojo           //0
  .long estado_rojo_amarillo  //1
  .long estado_verde          //2
  .long estado_amarillo       //3

    .thumb_func

estado_semaforo:
    push {lr}
    ldr r4, =Base_maquina_0
    ldr r0, [r4, #var_estado_M0]   
    lsl r0, #2                     
    ldr r4, =dir_tabla_estados     
    ldr r1, [r4, r0]               
    bx r1                          

    .thumb_func
estado_rojo:
    ldr r4, =Base_maquina_0
    ldr r0, [r4, #entrada_tiempo_M0]  
    ldr r5, =TIEMPO_ROJO              
    cmp r0, r5
    blt fin_estado                    

    // Salidas
    ldr r0, =GPIOB_PCOR
    mov r1, #(1 << LED_VERDE)
    str r1, [r0]                      

    ldr r0, =GPIOB_PSOR
    mov r1, #(1 << LED_AMARILLO)
    str r1, [r0]                      
    mov r1, #(1 << LED_ROJO)
    str r1, [r0]                      

    // Cambiar al siguiente estado 
    mov r1, #ROJO_AMARILLO
    str r1, [r4, #var_estado_M0]
    pop {lr}
    bx lr

    .thumb_func
estado_rojo_amarillo:
    ldr r4, =Base_maquina_0
    ldr r0, [r4, #entrada_tiempo_M0]  
    ldr r5, =TIEMPO_ROJO_AMARILLO     
    cmp r0, r5
    blt fin_estado                    

    // Configura la salida
    ldr r0, =GPIOB_PSOR
    mov r1, #(1 << LED_ROJO) | (1 << LED_AMARILLO)
    str r1, [r0]                      

    ldr r0, =GPIOB_PSOR
    mov r1, #(1 << LED_VERDE)
    str r1, [r0]                      

    // Cambiar al siguiente estado 
    mov r1, #VERDE
    str r1, [r4, #var_estado_M0]
    mov r2, #0
    str r2, [r4, entrada_tiempo_M0]   
    pop {lr}
    bx lr

    .thumb_func
estado_verde:
    ldr r4, =Base_maquina_0
    ldr r0, [r4, #entrada_tiempo_M0]  
    ldr r5, =TIEMPO_VERDE             
    cmp r0, r5
    blt fin_estado                    

    // Configura salida
    ldr r0, =GPIOB_PSOR
    mov r1, #(1 << LED_ROJO)
    str r1, [r0]                      

    ldr r0, =GPIOB_PCOR
    mov r1, #(1 << LED_VERDE)
    str r1, [r0]                      
    mov r1, #(1 << LED_AMARILLO)
    str r1, [r0]                      

    // Cambiar al siguiente estado 
    mov r1, #AMARILLO
    str r1, [r4, #var_estado_M0]
    mov r2, #0
    str r2, [r4, entrada_tiempo_M0]   
    pop {lr}
    bx lr

    .thumb_func
estado_amarillo:
    ldr r4, =Base_maquina_0
    ldr r0, [r4, #entrada_tiempo_M0]  
    ldr r5, =TIEMPO_AMARILLO          
    cmp r0, r5
    blt fin_estado                    

    // Configurar salida
    ldr r0, =GPIOB_PCOR
    mov r1, #(1 << LED_AMARILLO)
    str r1, [r0]                      

    ldr r0, =GPIOB_PCOR
    mov r1, #(1 << LED_ROJO)
    str r1, [r0]                      
    mov r1, #(1 << LED_VERDE)
    str r1, [r0]          
                
    // Cambiar al siguiente estado 
    mov r1, #ROJO
    str r1, [r4, #var_estado_M0]
    mov r2, #0
    str r2, [r4, entrada_tiempo_M0]   
    pop {lr}
    bx lr

fin_estado:
    pop {lr}
    bx lr
```

# Interrupción SysTick

```nasm
  .syntax unified
  .global SysTick_Handler
  .text

  .equ Base_maquina_0, 0x20001000      // Dirección base compartida
  .equ entrada_tiempo_M0, 4            // Offset para la entrada de tiempo transcurrido
  .thumb_func

SysTick_Handler:
    push {r4}
    ldr r4, =Base_maquina_0
    ldr r0, [r4, #entrada_tiempo_M0]  // Leer la variable de tiempo transcurrido
    add r0, r0, #1                    // Incrementar en 1 (1 ms ha transcurrido)
    str r0, [r4, #entrada_tiempo_M0]  // Guardar el valor actualizado
    pop {r4}
    bx lr                             // Retornar de la interrupción
```

# Constantes simbólicas

```nasm
    .text
    
    // Direcciones de los registros SysTick
    .equ SYSTICK_BASE, 0xE000E010        // Base del SysTick
    .equ SYST_CSR, (SYSTICK_BASE + 0x0)  // SysTick Control and Status Register
    .equ SYST_RVR, (SYSTICK_BASE + 0x4)  // SysTick Reload Value Register
    .equ SYST_CVR, (SYSTICK_BASE + 0x8)  // SysTick Current Value Register

    .equ SYSTICK_ENABLE, 0x1             // Bit para habilitar el SysTick
    .equ SYSTICK_TICKINT, 0x2            // Bit para habilitar la interrupción del SysTick
    .equ SYSTICK_CLKSOURCE, 0x4          // Bit para seleccionar el reloj del procesador

    .equ SYSTICK_RELOAD_1MS, 48000-1     // Valor para recargar el SysTick cada 1 ms (suponiendo un reloj de 48 MHz)

    .equ Base_maquina_0, 0x20001000      // Dirección base compartida
    .equ var_estado_M0, 0                // Offset para la variable de estado
    .equ entrada_tiempo_M0, 4            // Offset para la entrada de tiempo transcurrido

    // Direcciones de los registros GPIO (Ejemplo para Kinetis K64)
    .equ GPIOB_PDDR, 0x400FF054          // Registro de dirección de datos del puerto B
    .equ GPIOB_PDOR, 0x400FF040          // Registro de salida de datos del puerto B
    .equ GPIOB_PTOR, 0x400FF04C          // Registro de alternancia de datos del puerto B
    .equ GPIOB_PSOR, 0x400FF044          // Registro de establecer bits de salida en puerto B
    .equ GPIOB_PCOR, 0x400FF048          // Registro de limpiar bits de salida en puerto B

    // Definición de los registros y valores
    .equ PCC_BASE, 0x40065000              // Base del PCC (Peripheral Clock Control)
    .equ PCC_PORTB, (PCC_BASE + 0x128)      // Offset para el PCC del puerto B
    .equ PORTB_BASE, 0x4004A000            // Base del PORTB
    .equ PORTB_PCR12, (PORTB_BASE + 0x30)  // PCR para PTB12
    .equ PORTB_PCR13, (PORTB_BASE + 0x34)  // PCR para PTB13
    .equ PORTB_PCR14, (PORTB_BASE + 0x38)  // PCR para PTB14
    .equ MUX_GPIO, 0x1                     // Configuración Mux para GPIO (Alternativa 1)
    .equ PCC_PORTB_CGC, (1 << 30)          // Bit para habilitar el reloj del puerto B

    // Bits correspondientes a los LEDs
    .equ LED_ROJO, 12                    // LED Rojo conectado a PTB12
    .equ LED_AMARILLO, 13                // LED Amarillo conectado a PTB13
    .equ LED_VERDE, 14                   // LED Verde conectado a PTB14

    // Bits correspondientes a los LEDs
    .equ LED_ROJO, 12                    // LED Rojo conectado a PTB12
    .equ LED_AMARILLO, 13                // LED Amarillo conectado a PTB13
    .equ LED_VERDE, 14                   // LED Verde conectado a PTB14

    // Definición de los tiempos en ciclos de reloj (ajustar según la frecuencia del microcontrolador)
    .equ TIEMPO_ROJO, 3000               
    .equ TIEMPO_ROJO_AMARILLO, 5000      
    .equ TIEMPO_VERDE, 2000              
    .equ TIEMPO_AMARILLO, 3000           

    // Definición de los estados
    .equ ROJO, 0
    .equ ROJO_AMARILLO, 1
    .equ VERDE, 2
    .equ AMARILLO, 3
```

fin