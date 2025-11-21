# Módulo de Interrupciones NVIC

**Uso de Interrupciones en Microcontroladores ARM Cortex-M4**

# **Introducción**

Las interrupciones son eventos que ocurren de manera imprevista durante la ejecución del programa principal de un microcontrolador, interrumpiendo su flujo normal de ejecución para atender una tarea específica de mayor prioridad. Estas tareas pueden ser generadas por hardware o software y permiten al microcontrolador responder de manera rápida a eventos externos o internos. 

Las interrupciones pueden originarse tanto por `hardware` como por `software`. Aquellas generadas por el **hardware** provienen de eventos físicos en el propio **hardware**, como cambios en las entradas o salidas, así como la finalización de actividades específicas (eventos de hardware). Estas interrupciones pueden ser enmascarables, lo que significa que su ejecución puede ser controlada mediante un sistema de habilitación o deshabilitación (interrupt enable, IE), y se rigen por prioridades, pudiendo ser pospuestas en su atención. Se identifican comúnmente como **IRQ** o INT. Por otro lado, están las interrupciones No Enmascarables (**NMI**), que se atienden siempre independientemente de las condiciones de ejecución del programa.

Por otro lado, las interrupciones generadas por el **software** surgen de instrucciones que invocan la ejecución de una subrutina específica del sistema (eventos de software). Estas interrupciones se pueden clasificar en dos categorías: Interrupciones de software normales, que son activadas o provocadas por instrucciones de software, conocidas como **SWI**. Por otro lado, encontramos las **Excepciones**, que se originan a partir de errores en el código o en los procesos de una instrucción, como por ejemplo la división por cero.

# Modos de operación del procesador

En los microcontroladores ARM Cortex-M4, el procesador puede operar en dos modos principales: *Thread Mode y Handler Mode*.

## **Thread Mode:**

En el modo Thread, el procesador está ejecutando el código principal del programa. Durante este modo, el procesador puede ser interrumpido por eventos externos, como interrupciones de hardware, pero también puede ejecutar interrupciones pendientes de menor prioridad mientras continúa ejecutando el hilo principal del programa.

### **Ejemplo:**

Mientras el procesador está ejecutando el bucle principal del programa en el modo Thread, puede ser interrumpido por una interrupción de pulsación de un botón. La interrupción se manejará en el modo Handler, pero una vez que se complete el manejo de la interrupción, el procesador volverá al modo Thread para continuar ejecutando el bucle principal del programa.

## **Handler Mode:**

En el modo `Handler`, el procesador está ejecutando código en respuesta a una interrupción o una excepción. Durante este modo, el procesador deshabilita las interrupciones normales para evitar que se interrumpa el manejo de la interrupción actual. Este modo se utiliza para manejar eventos críticos, como interrupciones de hardware o excepciones graves.

### **Ejemplo:**

Cuando se produce una interrupción del temporizador SysTick, el procesador cambia al modo Handler y ejecuta el código definido en la función SysTick_Handler para manejar la interrupción.

## Estados de Excepción

Los estados de excepción son una parte fundamental en el funcionamiento de los sistemas informáticos, incluyendo microcontroladores y procesadores. Estos estados reflejan la situación de una excepción en un momento dado y cómo está siendo gestionada por el sistema. A continuación, se describen los cuatro estados principales de excepción:

**1. Inactivo:** En este estado, no hay ninguna excepción ocurriendo. El sistema se encuentra en su funcionamiento normal, ejecutando el código principal del programa o realizando tareas rutinarias. No hay interrupciones ni eventos anómalos que requieran la atención del procesador.

**2. Pendiente:** Cuando una excepción se dispara, pero aún no ha sido atendida por el sistema, entra en el estado pendiente. Esto significa que se ha detectado una situación excepcional, como una interrupción de hardware o una excepción de software, pero el procesador aún no ha comenzado a manejarla.

**3. Activo:** Una vez que el sistema comienza a manejar una excepción, esta entra en el estado activo. Durante este estado, el procesador está ejecutando el código de manejo de la excepción correspondiente, ya sea en modo Handler o en modo Thread, dependiendo del tipo de excepción y del diseño del sistema.

**4. Activo y Pendiente:** En algunos casos, una excepción puede estar en estado activo y pendiente simultáneamente. Esto puede ocurrir cuando se reciben múltiples excepciones del mismo tipo antes de que el sistema pueda manejarlas por completo. En este caso, el procesador atiende una excepción mientras que otras permanecen en estado pendiente hasta que puedan ser procesadas en secuencia.

Estos estados de excepción son esenciales para garantizar el funcionamiento seguro y confiable de los sistemas informáticos. Proporcionan una forma estructurada y ordenada de manejar eventos inesperados, permitiendo al procesador responder eficientemente a situaciones anómalas y garantizar la integridad del sistema.

## Tratamiento de una interrupción

Un sistema embebido real podría tener muchas fuentes de interrupción y normalmente cada fuente de interrupción tiene un nivel de prioridad asociado. Muchas arquitecturas de procesadores admiten el anidamiento de interrupciones, lo que significa que durante la ejecución de una rutina de servicio de interrupción (**ISR**) de baja prioridad, una interrupción de alta prioridad puede interrumpirla y la **ISR** de baja prioridad se suspende, y se reanuda cuando la **ISR** de alta prioridad haya completado. Observa la figura 1 de la manera cómo actúan las interrupciones anidadas. 

![Figura 1. Gestión de interrupciones anidadas. Fuente: ARM Comunity: https://community.arm.com/](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/196ab71d-b677-4c76-bc42-45bff715eea5/Untitled.png)

Figura 1. Gestión de interrupciones anidadas. Fuente: ARM Comunity: https://community.arm.com/

## Context Saving

Cuando un microcontrolador atiende una interrupción, es crucial que pueda guardar y restaurar el contexto del programa que se estaba ejecutando antes de que ocurriera la interrupción. El contexto del programa incluye información como los registros del procesador, el estado de la pila y la dirección de retorno, entre otros. Esto se debe a que, al atender una interrupción, el microcontrolador necesita cambiar temporalmente su flujo de ejecución para manejar la interrupción y luego regresar al estado previo para continuar con la ejecución normal del programa.

Para gestionar esto, el microcontrolador realiza los siguientes pasos:

1. **Guardar el Estado Actual del Proceso:**
Antes de comenzar a ejecutar la rutina de interrupción, el microcontrolador guarda el estado actual del proceso en la pila (Stack). Esto incluye los registros del procesador, como el contador de programa (PC), los registros de propósito general, el registro de estado (PSR), entre otros. Como puedes observar en la figura 2, este es el orden en el que se guardan los registros en el Stack. Es de resaltar que si el módulo de punto flotante del procesador está siendo usado, más registros serán guardados como contexto.

![Figura 2. Context saving. Registros que se guardan en el Stack](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/3670df88-52f4-4f18-86d8-8aa90370c58d/image.png)

Figura 2. Context saving. Registros que se guardan en el Stack

1. **Cambiar al Modo de Manejador (Handler Mode):**
El microcontrolador cambia al modo de manejo de interrupciones (Handler Mode) para comenzar a ejecutar la rutina de interrupción. Durante este modo, se pueden ejecutar instrucciones privilegiadas y acceder a recursos críticos del sistema.
2. **Ejecutar la Rutina de Interrupción:**
El microcontrolador ejecuta la rutina de interrupción correspondiente para manejar la causa de la interrupción.
3. **Restaurar el Estado Anterior del Proceso:**
Una vez que se ha completado la rutina de interrupción, el microcontrolador restaura el estado previo del proceso desde la pila. Esto implica recuperar los registros del procesador y la dirección de retorno almacenados previamente.
4. **Volver al Modo de Usuario (User Mode):**
Finalmente, el microcontrolador vuelve al modo de usuario (User Mode) y continúa la ejecución del programa principal desde donde se interrumpió.

## 📝 Ejercicio

Consulta en qué consiste la `latencia` de las interrupciones. 

## **Módulo NVIC de ARM:**

El Nested Vectored Interrupt Controller (NVIC) es un módulo presente en los microcontroladores ARM Cortex-M4 que se encarga de gestionar las interrupciones. Permite configurar la prioridad de las interrupciones, habilitar/deshabilitar interrupciones específicas y manejar el vector de interrupción para dirigir el flujo de ejecución al controlador de interrupción correspondiente.

## Mapa de Interrupciones

El mapa de interrupciones es una lista en las primeras posiciones de la memoria. Cada posición contiene un apuntador a la dirección de memoria donde queda el Interrupt Handler especifico.

En la parte superior de la siguiente tabla, puede observar las primeras interrupciones, las cuales son específicas de ARM y son generadas por la CPU. En la primera columna se observa la dirección de memoria asignada. Observa la descripción de las fuentes de estos primeros vectores:

- **Initial Stack Pointer**: Este es el puntero de pila inicial, que se utiliza durante la inicialización del sistema. Al iniciar el procesador, este valor es cargado en el registro del Stack Pointer (SP) para gestionar el espacio de memoria de la pila.
- **Initial Program Counter**: Este vector contiene la dirección del punto de entrada del programa (instrucción inicial). Al iniciar, el procesador cargará este valor en el registro del Program Counter (PC), indicando la dirección de la primera instrucción que debe ejecutar.
- **Non-maskable Interrupt (NMI)**: Es una interrupción no enmascarable, lo que significa que no puede ser ignorada ni deshabilitada por el software. Se utiliza generalmente para eventos críticos como fallos de hardware graves.
- **Hard Fault**: Esta excepción se activa cuando ocurre un fallo grave que no puede manejarse por otros mecanismos de falla. Suele deberse a errores de acceso a memoria o problemas de bus.
- **MemManage Fault**: Esta interrupción se genera cuando hay un error relacionado con la protección de la memoria, como acceder a una región de memoria no permitida.
- **Bus Fault**: Se activa cuando se produce un error en el acceso al bus, como cuando el procesador intenta acceder a una dirección de memoria no válida o a un dispositivo externo que no responde.
- **Usage Fault**: Este vector se activa cuando ocurre un error relacionado con el uso incorrecto del procesador, como intentar ejecutar una instrucción indefinida o realizar una operación inválida.

<aside>
🔥

**Importante**: 
Debes buscar esta tabla en el **manual específico del microcontrolador** que estés utilizando, para determinar las posiciones de las interrupciones que son específicas del fabricante. Las que define ARM, deberían ser las mismas para toda la comunidad. 

</aside>

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/4f7a8c2f-894c-4189-92c9-376e0cefca21/Untitled.png)

![Figura 3. Fragmento de la tabla de asignación de vectores de interrupción. La tabla completa e puede consultar en el manual de usuario del microcontrolador (NXP en este caso), Capítulo 4.](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/a3000ed8-1db1-42f9-8bb5-cba656ede330/Untitled.png)

Figura 3. Fragmento de la tabla de asignación de vectores de interrupción. La tabla completa e puede consultar en el manual de usuario del microcontrolador (NXP en este caso), Capítulo 4.

En la parte inferior de la figura 3, se observan las denominadas Non-Core Vectors. Estas interrupciones son específicas de NXP y son generadas por el módulo NVIC.

<aside>
🔥

**Nota**:  observa el vector de interrupción **System tick timer (SysTick)**. ¿Recuerdas que ya lo usaste en algunos ejemplos en clase? Bueno, como puedes observar, este vector se genera por el core y no por el módulo de interrupciones, es por eso que no encuentras información específica del SysTick en el manual del microcontrolador. 

</aside>

## Interrupt Request (IRQ)

El número de IRQ (Interrupt Request) del módulo NVIC (Nested Vectored Interrupt Controller) es un identificador único asignado a cada tipo de interrupción en un microcontrolador. Este número identifica de manera exclusiva a una interrupción específica, permitiendo al NVIC direccionar adecuadamente el flujo de ejecución del programa hacia el controlador de interrupciones correspondiente cuando se produce una interrupción. Por ejemplo, si una interrupción de temporizador tiene asignado el número de IRQ 15, el NVIC utilizará este número para dirigir la ejecución del programa hacia el controlador de interrupción correspondiente cuando se active la interrupción del temporizador. Por lo tanto, el número de IRQ del módulo NVIC es esencial para el manejo eficiente de las interrupciones en un microcontrolador, ya que permite identificar y gestionar cada tipo de interrupción de manera única. En la tabla mostrada anteriormente, puede observar este número en la columna 3. 

## **Registros para la Gestión de Interrupciones:**

El módulo NVIC (Nested Vectored Interrupt Controller) contiene un conjunto de registros utilizados para gestionar las interrupciones en un microcontrolador. Estos registros permiten habilitar, generar, revisar o deshabilitar las interrupciones de manera eficiente. En particular, destacan los registros ISER (Interrupt SET ENABLE) y ICER (Interrupt CLEAR ENABLE), que se utilizan para habilitar o deshabilitar las interrupciones respectivamente, funcionando de manera similar a los comandos PSOR y PCOR. Además, se encuentran los registros ISPR (Interrupt SET PENDING) e ICPR (Interrupt CLEAR PENDING), que permiten establecer o borrar el estado pendiente de una interrupción, similar a los comandos PSOR y PCOR. Otro registro importante es IABR (Interrupt Active BIT), que indica qué interrupciones están actualmente activas, siendo similar a PDIR. Por último, el registro IPR (Interrupt Priority) se utiliza para establecer la prioridad de las interrupciones, siendo de 4 bits, a diferencia de los otros registros que son de un solo bit. Estos registros son esenciales para el correcto funcionamiento y gestión de las interrupciones en el microcontrolador.

### Resumen

Los registros del módulo NVIC son un conjunto. Observa la descripción a continuación.

NVIC_ISERx        (Interrupt SET ENABLE)
NVIC_ICERx        (Interrupt CLEAR ENABLE)
NVIC_ISPRx        (Interrupt SET PENDING)
NVIC_ICPRx        (Interrupt CLEAR PENDING)
NVIC_IABRx        (Interrupt Active BIT)
NVIC_IPRx           (Interrupt Priority)

La X indica que son varios registros bajo la misma denominación. ARM declara X desde cero hasta 14, tal que: `NVIC_ISER[0]—>NVIC_ISER[14]`

## **Niveles de Prioridad de Interrupciones:**

En los microcontroladores ARM Cortex-M4, las interrupciones se pueden clasificar en niveles de prioridad. Un valor más bajo de prioridad indica una mayor prioridad. Por ejemplo, una interrupción con prioridad 0 tiene la máxima prioridad, mientras que un 15 es una prioridad muy baja.

**Ejemplo:**

Supongamos que deseamos habilitar la interrupción del temporizador SysTick con una prioridad baja.

```nasm
    .section .text
    .global SysTick_Handler
    .global main

SysTick_Handler:
    // Implementación del manejador de interrupción
    // Aquí irían las instrucciones para manejar la interrupción
    BX LR                 // Retornar de la interrupción

main:
    // Cargar la frecuencia del reloj del sistema (SystemCoreClock)
    LDR R0, =0xE000E010   // Dirección del registro SysTick Control and Status
    LDR R1, =16000        // Cargar valor para contar 1 ms (asumiendo SystemCoreClock de 16 MHz)
    STR R1, [R0, #0x04]   // Escribir valor en el registro SysTick Reload Value

    // Configurar el SysTick
    LDR R1, [R0, #0x04]   // Leer el valor de recarga del SysTick
    MOV R2, #0x07         // Habilitar el temporizador, la interrupción y seleccionar el reloj del procesador
    STR R2, [R0]          // Escribir en el registro de control de SysTick

    // Configurar la prioridad del SysTick en el NVIC
    LDR R0, =0xE000ED20   // Dirección del registro de prioridad del NVIC para SysTick
    LDR R1, =0xF0000000   // Cargar la prioridad más baja (15 en binario)
    STR R1, [R0]          // Escribir la prioridad

    // Habilitar la interrupción en el NVIC
    LDR R0, =0xE000E100   // Dirección del registro NVIC ISER (Interruption Set Enable Register)
    LDR R1, =0x01000000   // Set bit para habilitar SysTick_IRQn (IRQ número 15)
    STR R1, [R0]          // Escribir en el registro NVIC ISER

main_loop:
    B main_loop           // Bucle infinito

    .end
```

<aside>
🔥

**Nota**: A contiuación te dejo también la función en C, para que puedas realizar una revisión rápida y más intuitiva en un nivel de abstracción superior. 

</aside>

```c
#include <stdint.h>

void SysTick_Handler(void) {
    // Manejar la interrupción del temporizador SysTick
}

int main(void) {
    // Configurar el temporizador SysTick para que interrumpa cada 1 ms
    SysTick_Config(SystemCoreClock / 1000);

    // Habilitar la interrupción del temporizador SysTick con una prioridad baja
    NVIC_SetPriority(SysTick_IRQn, 15);
    NVIC_EnableIRQ(SysTick_IRQn);

    // Resto del código

    while (1) {
        // Bucle principal
    }

    return 0;
}
```

En este ejemplo, se configura el temporizador SysTick para que interrumpa cada 1 ms. Luego, se establece la prioridad de la interrupción del temporizador SysTick en el nivel más bajo y se habilita dicha interrupción.

## Determinar la configuración de una interrupción

1. Se localiza el número de la IRQ. Por ejemplo, al Puerto A le corresponde la IRQ 59:

![Figura 4. Fragemento de la tabla de vectores de interrupción](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/aa54fef2-f3e2-4af8-8463-47962bc591aa/Untitled.png)

Figura 4. Fragemento de la tabla de vectores de interrupción

1. Se calcula a que registro X corresponde ese bit, para ello debemos determinar la posición del bit y el valor de X (`cada registro —> 32 bits`). Para ello se realiza una división entera entre el número de IRQ y 32. 

$IRQ/32 -> 59/32=1$

El regisro correspondiente es el `ISER1`

| **Registro ISERx** | **Bits** |
| --- | --- |
| ISER0 | bit31 a bit0 |
| ISER1 | bit63 a bit32 |
| ISER3 | bit95 a bit64 |
1. Se calcula la posición del bit con la expresión: $IRQ~módulo~32~$ (operación del residuo).

$59~mod~32=27$

1. Se calcula el registro de la prioridad. Para X de la prioridad se divide IRQ por 4. 

$IRQ/4 -> 59/4=14$

1. Se calcula la posición de los bits (offset) de la prioridad. Esto se realiza con el módulo de la IRQ por 4 multiplicado por 8 y el resultado más cuatro.
$Offset = (IRQ ~mod~ 4) * 8 + 4 = 28$

### Resumen

NVIC_ISER1 bit 27
NVIC_ICER1 bit 27
NVIC_ISPR1 bit 27
NVIC_ICPR1 bit 27
NVIC_IABR1 bit 27
NVIC_IPR14 offset 28

## Funciones para configurar los registros

<aside>
🔥

**Nota**: Las siguientes funciones están en C, para una mejor comprensión del proceso; sin embargo, cabe anotar, que el proceso para la configuración en Assembly puede deducirse a partir de estas funciones. 

</aside>

Para activar la interrupción

```c
NVIC->ISER[(NUM_IRQ/32)]=(1<<(NUM_IRQ%32));
//Otra opción más eficiente que reemplaza la anterior sería:
NVIC->ISER[(NUM_IRQ>>5)]=(1<<(NUM_IRQ & 0x1F));
//Cuando se trabaja con el SDK se pueden utilizar las funciones
NVIC_EnableIRQ(PORTA_IRQn);
```

Para desactivar la interrupción

```c
NVIC->ICER[(NUM_IRQ/32)]=(1<<(NUM_IRQ%32)); //27 para este ejemplo específico
//Otra opción más eficiente que reemplaza la anterior sería:
NVIC->ICER[(NUM_IRQ>>5)]=(1<<(NUM_IRQ & 0x1F));
NVIC_DisableIRQ(PORTA_IRQn);
```

Para forzar la interrupción

```c
NVIC->ISPR[(1)]=(1<<(NUM_IRQ/32));
NVIC_SetPendingIRQ(PORTA_IRQn);
```

Para evitar la interrupción

```c
NVIC->ISPR[(1)]=(1<<(27));  //27 para este ejemplo específico
NVIC_ClearPendingIRQ(PORTA_IRQn);
```

Para la prioridad la interrupción (prioridad 3)

```c
NVIC->IP[PORTA_IRQn]=(3<<4);
NVIC_SetPriority(PORTA_IRQn,3);
```

## Ejemplos de rutinas de atención a la interrución

```c
__attribute__((interrupt)) void SysTick_Handler(void) {
	contador_systick++;
}
__attribute__((interrupt)) void LPIT0_Ch0_IRQHandler(void) {
	LPIT0->MSR |= LPIT_MSR_TIF0_MASK; //limpiar la badera
	contador_pit++;
}
__attribute__((interrupt)) void PORTC_IRQHandler(void) {
	if ((PORTC->ISFR & MASK_PULSADOR)!=0) {
		PORTC->ISFR = (MASK_PULSADOR);  //limpiar la badera
	}
	if ((PORTC->ISFR & MASK_PULSADOR2)!=0) {
		PORTC->ISFR = (MASK_PULSADOR2);  //limpiar la badera
	}
}
```

# Ejemplo (Pulsador)

Para atender un pulsador mediante una interrupción de un puerto GPIO en un microcontrolador, se puede configurar el pin asociado al pulsador como una entrada GPIO y habilitar la generación de una interrupción en ese pin cuando se detecte un cambio de estado. Primero, se configura el pin como una entrada y se activan las resistencias de pull-up o pull-down, según sea necesario. Luego, se configura el microcontrolador para que genere una interrupción en el flanco de subida o bajada del pulsador. Cuando el pulsador se presiona o se suelta, se producirá un cambio de estado en el pin GPIO, lo que activará la interrupción correspondiente. En el manejador de interrupciones asociado, se puede realizar el procesamiento necesario, como actualizar una bandera o realizar alguna acción específica en respuesta al pulsador. Este enfoque permite manejar la entrada del pulsador de manera eficiente, ya que el microcontrolador solo se despierta cuando se produce una acción en el pulsador, en lugar de tener que estar continuamente verificando su estado en un bucle principal.

En la figura 5 se observa la configuración del regisrto PCR para que funcione como puerto de entrada para atender un pulsador. 

![Figura 5. Configuración del registro PCR como entrada para gestionar un pulsador.](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/eabbf780-9a78-4bc6-80c4-f18ce1d88f26/Untitled.png)

Figura 5. Configuración del registro PCR como entrada para gestionar un pulsador.

## Eventos que producen la interrupción

Las interrupciones pueden ser generadas por una variedad de eventos, incluyendo cambios en los niveles de tensión en los pines de entrada/salida de un microcontrolador. Estos cambios pueden ser capturados por el microcontrolador como transiciones, que se denominan flancos de subida o flancos de bajada, dependiendo de si la señal cambia de bajo a alto (flanco de subida) o de alto a bajo (flanco de bajada). Por ejemplo, al presionar un botón, se produce un cambio en el nivel de tensión en el pin correspondiente del microcontrolador, generando un flanco de subida. Del mismo modo, al soltar el botón, se produce un cambio en el nivel de tensión, generando un flanco de bajada. Estos flancos pueden ser utilizados para activar interrupciones y desencadenar la ejecución de rutinas de manejo de interrupciones específicas, permitiendo al microcontrolador responder de manera eficiente a eventos externos en tiempo real. Observa la figura 6 los diferentes eventos que pueden generar una interrupción.

![Figura 6. Eventos de la interrupción](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/7a3bf544-16fb-472c-847c-138cde32150f/Untitled.png)

Figura 6. Eventos de la interrupción

# Material complementario

Para los curiosos, acá les voy a dejar material adicional para que sigan profundizando en el tema:

Los siguientes videos, son una serie del canal: [**Embedded Systems and Deep Learning**](https://www.youtube.com/@embeddedsystemswitharmcort9051)