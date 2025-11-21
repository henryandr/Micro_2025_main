# Multiplexación

## Problemática

Veamos en la figura 1, un ejemplo del uso tradicional de los puertos de un microcontrolador. Cada uno de los elementos controlados, se gestiona con un puerto independiente.

![*Figura 1. Solución tradicional para el uso de los puertos de un microcontrolador.*](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/d7e08695-71e8-4a47-a01a-9b674f9c7128/Untitled.png)

*Figura 1. Solución tradicional para el uso de los puertos de un microcontrolador.*

Sin embargo, en la mayoría de casos, es posible encontrar un elemento común que puede ser utilizado para ahorrar el número de puertos necesarios para la gestión del periférico. Esto implica sacrificar el tiempo que se le dedica a cada elemento, pero dada la velocidad del procesador, en la mayoría de los casos esas transiciones son imperceptibles para el ser humano.

## Multiplexación

La multiplexación juega un papel crucial en los microcontroladores al permitir la gestión eficiente de múltiples dispositivos utilizando un número limitado de pines de entrada/salida. Tomemos como ejemplo un teclado matricial de 8 teclas y una matriz de LEDs de 8x8. En el caso del teclado, mediante la multiplexación, podemos reducir la cantidad de pines necesarios para leer las 8 teclas a solo 8, en lugar de requerir 16 pines individuales. De manera similar, en una matriz de LEDs, podemos controlar 64 LEDs con solo 16 pines, utilizando técnicas de multiplexación que permiten activar solo una fila o columna a la vez y alternar rápidamente entre ellas para dar la impresión de que todos los LEDs están encendidos simultáneamente. Esta optimización en el uso de pines es esencial en aplicaciones donde los recursos son limitados, como en sistemas embebidos y dispositivos IoT, permitiendo una implementación más eficiente y económica. Observa la figura 2, se muestra la cantidad de puertos del microcontrolador que se pueden ahorrar con una configuración matricial.

![*Figura 2. Técnica de multiplexación matricial*](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/237609a9-8e6f-4576-8223-ac8c3bb243b4/Untitled.png)

*Figura 2. Técnica de multiplexación matricial*

## Configuración del microcontrolador (Salida - Salida)

En la configuración Salida - Salida, todos los pines del microcontrolador se configuran como salida. 

Tomemos como ejemplo una matriz de leds 8x8 de cátodo común. Este suele ser un caso habitual, donde necesitamos realizar el proceso de multiplexación. Para configurar un microcontrolador y realizar una multiplexación en una matriz de este tipo, se pueden seguir los siguientes pasos:

1. **Configurar los pines del microcontrolador**: Asignar pines para controlar las filas y las columnas de la matriz de LEDs. Por ejemplo, si tenemos una matriz de 8x8, necesitaríamos al menos 8 pines para controlar las filas y 8 pines para controlar las columnas.
2. Establecer los pines de salida: Configurar los pines asignados como salidas en el microcontrolador.
3. **Selección de columnas**: Se activa una columna a la vez, configurando un nivel lógico 1 en la fila inicial, dado que estamos asumiendo cátodo común, vamos a proporcionar la tensión suficiente al ánodo. Observa la figura 3, para que te hagas una idea del proceso.
4. **Selección de filas:**  los pines de las filas se configuran como drenadores (estado lógico 0), lo cual proporciona la diferencia de potencial para hacer circular la corriente a través del LED en esa columna y filas, encendiendo los LEDs.
5. Ciclo de multiplexación: Se repiten los pasos 3 y 4 en un ciclo rápido, alternando entre las filas y las columnas para dar la impresión de que todos los LEDs están encendidos simultáneamente.

Este proceso de multiplexación se lleva a cabo a una velocidad lo suficientemente rápida como para que el ojo humano perciba todos los LEDs encendidos continuamente, a pesar de que en realidad solo se encienden una fila o columna a la vez. Esta técnica optimiza el uso de pines del microcontrolador y permite controlar una matriz de LEDs con un número mínimo de conexiones.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/609980e9-00fa-4c8a-8321-0d9a15d21567/Untitled.png)

*Figura 3. Matriz de leds 8x8 en configuración Cátodo común*

## Ejemplo 1

Observa el siguiente video, en él se muestra una secuencia para plasmar en la matriz una imagen. La persistencia de la luz en nuestros ojos, nos hace pensar que los leds están siempre encendidos, cuando realmente, esto no ocurre. 

https://youtu.be/HTjjUZRSjv0?si=ydRaDJDLmuDnpedw

Fuente: **Multiplexing on 8x8 dot Matrix with Arduino -** [Mario's Ideas](https://www.youtube.com/@marios_ideas)

## 👁️ Ejercicio 1

Consulta cuál es el tiempo que se necesita para lograr la persistencia en el ojo humano. Este dato será útil para lograr realizar 

## Configuración del microcontrolador (Salida - Entrada)

La técnica de multiplexación utilizando la configuración del microcontrolador en salida-entrada implica asignar los pines del microcontrolador, de manera que las filas del dispositivo a controlar se configuren como salidas, mientras que las columnas se establecen como entradas. Por ejemplo, consideremos un teclado matricial 4x4. En este caso, se asignarían 4 pines del microcontrolador como salidas para controlar las filas del teclado y se asignarían otros 4 pines como entradas para leer las columnas. Al presionar una tecla en el teclado, se activa una columna específica y se detecta en qué columna se encuentra la tecla presionada, ya que las filas están configuradas como salidas. Esto permitirá configurar un nivel de voltaje bajo (0 V) en una fila y leer todas las columnas, posteriormente, se rotará el cero a la siguiente fila y así hasta completar el ciclo. Esto permite identificar la posición de la tecla activada y tomar la acción correspondiente en el microcontrolador. Este método de multiplexación permite reducir el número de pines requeridos para leer un teclado matricial, optimizando así el uso de recursos del microcontrolador.

## Resistencias de Pull Up

Es necesario ubicar resistencias de pull-up en un teclado matricial para garantizar un funcionamiento adecuado del sistema y evitar problemas como el rebote de las teclas y el consumo excesivo de energía. Las resistencias de pull-up se colocan en las líneas de entrada (filas en el caso de un teclado matricial) y se conectan a un voltaje alto (generalmente Vcc, el voltaje de alimentación del microcontrolador). Cuando ninguna tecla está presionada, estas resistencias aseguran que las líneas de entrada se mantengan en un estado lógico alto (1 lógico). Cuando se presiona una tecla, se conecta la línea de entrada correspondiente a tierra (0 lógico), y el microcontrolador puede detectar este cambio en el estado de la línea.

Sin las resistencias de pull-up, las líneas de entrada quedarían flotantes cuando ninguna tecla está presionada, lo que podría provocar lecturas erróneas o inestabilidad en el sistema. Además, al conectar las líneas de entrada directamente a tierra cuando se presiona una tecla, se puede generar una corriente excesiva y consumo de energía, lo que no es eficiente en términos de consumo de energía y puede causar problemas en el funcionamiento del sistema a largo plazo. En resumen, las resistencias de pull-up son fundamentales para proporcionar una referencia estable cuando no se presionan teclas y para evitar problemas relacionados con el consumo de energía y la estabilidad del sistema.

<aside>
💡 **Importante**: No olvides ubicar las resistencias de Pull Up en tu circuito.

</aside>

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/1fbd22e9-5df4-4d01-8c51-2051e21e3cb0/Untitled.png)

*Figura 4. Teclado matricial de 16 pulsadores*

Observe la animación en el siguiente video. Esta es la estrategia usada para monitorear constantemente un teclado, con el fin de detectar una tecla presionada.

https://youtu.be/5beUZDBY8N0?si=tirimV5Je9AzxfAz

Fuente:  **How To Scan Keypad -** [Arduino Tutorials for Beginners](https://www.youtube.com/@arduino-tutorial)

## 👇🏻 Ejercicio 2

Consulte cuál es la máxima frecuencia con la que un ser humano puede presionar un interruptor.

# Implementación de Lectura de un Teclado Matricial 4x4 usando Multiplexación

### Introducción

Un teclado matricial 4x4 es una matriz de botones organizados en 4 filas y 4 columnas, lo que resulta en 16 botones. Cada botón está situado en la intersección de una fila y una columna. Este tipo de configuración es útil porque, en lugar de usar 16 pines GPIO para cada botón, se pueden usar 8 (4 para filas y 4 para columnas) utilizando una técnica de multiplexación. Observa la siguiente figura y verás la configuración por filas (L1 a L4) y columnas (C1 a C4).

![Figura 5. Estructura de un texlado matricial 4x4](https://prod-files-secure.s3.us-west-2.amazonaws.com/86ff57e3-0a96-46be-a958-59ebefbb5e94/203f0f89-f421-4b92-a5be-2de94e06fa30/image.png)

Figura 5. Estructura de un texlado matricial 4x4

### ¿Cómo funciona un Teclado Matricial?

Un teclado matricial funciona al conectar botones entre las filas y columnas de la matriz. Cuando se presiona un botón, conecta una fila y una columna. Para detectar qué botón se ha presionado, el microcontrolador activa una fila (poniéndola en alto o bajo) y luego verifica qué columna ha cambiado de estado.

### Multiplexación y Ahorro de Pines GPIO

La técnica de multiplexación implica escanear secuencialmente cada fila y luego leer las columnas correspondientes. Esto permite detectar qué tecla se presionó sin tener que asignar un pin GPIO a cada botón, ahorrando recursos valiosos.

### Pseudocódigo

El proceso básico para escanear el teclado es:

1. **Activar una fila**: Colocar una fila en un estado activo (por ejemplo, bajo).
2. **Leer las columnas**: Comprobar qué columna ha cambiado su estado para detectar la tecla presionada.
3. **Repetir para las otras filas**: Escanear cada fila de la matriz.

Pseudocódigo:

```nasm
Para cada fila en el teclado:
    Activar la fila
    Para cada columna:
        Leer el estado de la columna
        Si la columna está activa:
            Identificar la tecla presionada
    Desactivar la fila

```

### Descripción del Código

A continuación, explicamos cada parte del código que implementa la multiplexación para leer un teclado matricial 4x4:

### 1. Configuración de los relojes de los puertos B y D

La función `config_clock` habilita el reloj en los puertos GPIO que se utilizan para conectar el teclado.

```nasm
config_clock:
    LDR R0, =PCC_BASE             // Dirección base del registro de control del reloj
    LDR R1, [R0, #PCC_PORTB_OFFSET]// Leer valor de control para el puerto B
    ORR R1, R1, #(1<<30)          // Habilitar reloj para el puerto B
    STR R1, [R0, #PCC_PORTB_OFFSET]// Guardar el valor modificado

    LDR R1, [R0, #PCC_PORTD_OFFSET]// Leer valor de control para el puerto D
    ORR R1, R1, #(1<<30)          // Habilitar reloj para el puerto D
    STR R1, [R0, #PCC_PORTD_OFFSET]// Guardar el valor modificado
    BX LR                         // Regresar

```

### 2. Configuración de pines como GPIO

Los pines PTB12 a PTB15 se configuran como salidas para controlar las filas del teclado, y los pines PTD13 a PTD16 se configuran como entradas para leer las columnas.

```nasm
config_port:
    LDR R0, =PORTB_BASE           // Cargar la dirección base de PORTB
    LDR R1, =(0x100)              // Configurar como GPIO
    STR R1, [R0, #(12 * 4)]       // Configurar PTB12
    STR R1, [R0, #(13 * 4)]       // Configurar PTB13
    STR R1, [R0, #(14 * 4)]       // Configurar PTB14
    STR R1, [R0, #(15 * 4)]       // Configurar PTB15

    LDR R0, =PORTD_BASE           // Cargar la dirección base de PORTD
    LDR R1, =(0x103)              // Configurar PTD13-PTD16 con resistencias Pull-Up
    STR R1, [R0, #(13 * 4)]       // Configurar PTD13
    STR R1, [R0, #(14 * 4)]       // Configurar PTD14
    STR R1, [R0, #(15 * 4)]       // Configurar PTD15
    STR R1, [R0, #(16 * 4)]       // Configurar PTD16
    BX LR                         // Regresar

```

### 3. Escaneo del Teclado

En este código, se escanea una fila a la vez y luego se leen las columnas para detectar si alguna está en estado bajo (lo que indica que se ha presionado una tecla). Por ejemplo, el siguiente fragmento escanea la fila correspondiente a PTB12:

```nasm
scan_loop:
    LDR R0, =GPIOB_PDOR           // Cargar la dirección del registro de salida de GPIOB
    LDR R1, [R0]                  // Leer el valor actual
    ORR R1, R1, #(0b1101 << 12)   // Activar la primera fila PTB12 (0B1110)
    STR R1, [R0]                  // Escribir el valor en el registro

    LDR R0, =GPIOD_PDIR           // Dirección del registro de entrada de GPIOD
    LDR R2, [R0]                  // Leer el valor actual de las entradas

    TST R2, #(1 << 13)            // Verificar si PTD13 está baja (tecla presionada)
    BEQ tecla1_presionada
    B scan_loop                   // Repetir el escaneo para las siguientes filas

```

### 4. Manejo de Teclas Presionadas

Cada vez que se detecta una tecla presionada, se ejecuta una rutina específica. Por ejemplo, la siguiente rutina enciende un LED cuando se presiona la primera tecla:

```nasm
tecla1_presionada:
    LDR R0, =GPIOB_PCOR           // Enciende el LED
    LDR R1, =(1<<16)              // Configurar el LED en PTB16
    STR R1, [R0]                  // Escribir el valor para apagar el LED
    B scan_loop                   // Regresar al bucle de escaneo

```

### 5. Debounce

Para evitar que una sola pulsación de tecla sea registrada múltiples veces debido a rebotes eléctricos, se implementa una rutina de debounce que añade un pequeño retraso y verifica nuevamente el estado de la tecla antes de confirmar que fue presionada:

```nasm
debounce_tecla1:
    BL delay_ms                   // Llamar a la rutina de delay
    LDR R0, =GPIOD_PDIR           // Leer nuevamente el registro de entrada
    LDR R2, [R0]                  // Leer el valor
    TST R2, #(1 << 13)            // Verificar si PTD13 sigue baja
    BEQ tecla1_presionada          // Si sigue en bajo, procesar como tecla presionada
    B scan_loop                   // Si no, continuar escaneando

```

Este tutorial ha cubierto cómo se utiliza la multiplexación para escanear un teclado matricial 4x4, con un enfoque en cómo se ahorran pines GPIO y cómo se procesan las teclas presionadas. El uso de la técnica de debounce es esencial para un comportamiento fiable del teclado.