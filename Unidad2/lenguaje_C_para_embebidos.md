# Actividad 2: Lenguaje C para sistemas embebidos

---

# **Introducción a la Programación en C**

La programación en C es ampliamente utilizada en sistemas embebidos debido a su eficiencia y facilidad para manejar hardware a bajo nivel. C permite un control detallado sobre la memoria y el hardware, características esenciales para sistemas con recursos limitados.

### Ventajas de programar en C:

- Código más compacto y fácil de entender.
- Mantención y actualización más sencillas.
- Mejor gestión de proyectos grandes y equipos con varios programadores.
- Más portable en comparación con lenguajes de bajo nivel como el ensamblador.

### Desventajas de programar en C:

- El compilador puede ser costoso.
- Para proyectos pequeños, el ensamblador puede ser más rápido y eficiente.

## 📤 **Ejercicio 1 - para subir al repositorio**

Realizar una breve investigación sobre otros lenguajes que se utilicen en sistemas embebidos y compararlos con C. Responde las siguientes preguntas:

1. ¿Cuáles son los lenguajes en los que se puede programar sistemas embebidos? Haz un listado
2. ¿Qué ventajas y desventajas tienen dichos lenguajes comparados con C?
3. ¿Existe un ranking de lenguajes para sistemas embebidos? Si lo hay, comparte un link a la página y escribe tus percepciones de este ranking.

---

## **Estructura básica de un programa en C**

Cada programa en C tiene una estructura básica que comienza con directivas del preprocesador (por ejemplo, `#include`), seguido de la función `main()`, donde se define la lógica principal.

Observa a continuación el ejemplo:

```c
//*******************************************************************************
//Directivas de Preprocesador
//*******************************************************************************
#include <stdint.h>  // Librería para tipos de datos enteros con tamaño definido
#include <stdio.h>   // Librería estándar de entrada y salida

// Definiciones y Macros
#define GPIO_PORTD_BASE  0x400FF0C0  // Dirección base ficticia del puerto D
#define PCC_PORTD_BASE   0x40065000  // Dirección base del controlador de reloj para el puerto D
#define PCC_PORTD_OFFSET 0x130       // Offset del registro para habilitar reloj del puerto D
#define LED_PIN 5  // El LED está conectado al pin 5 del puerto D
//*******************************************************************************
// Prototipos de funciones
//*******************************************************************************
void config_clock_portD(void);
void config_gpio_portD(void);
void led_on(void);
void led_off(void);

//*******************************************************************************
//Función Principal
//*******************************************************************************
int main(void) {
    // Configurar el reloj para el puerto D
    config_clock_portD();
    
    // Configurar el puerto D como salida para el pin del LED
    config_gpio_portD();

    // Encender el LED
    led_on();
    
    // Pequeña espera simulada (no usaremos delays exactos)
    for (volatile int i = 0; i < 1000000; i++);

    // Apagar el LED
    led_off();

    while(1)
    {
    }
}
//*******************************************************************************
// Implementación de funciones
//*******************************************************************************
// Función para habilitar el reloj del puerto D
void config_clock_portD(void) {
    // Habilitar el bit 30 para habilitar el clock del puerto D
    PCC_PORTD |= (1 << 30);
}

// Función para configurar el pin del puerto D como salida
void config_gpio_portD(void) {
    // Configurar el pin LED_PIN como salida (escribiendo un 1 en el bit correspondiente del PDDR)
    GPIO_PDDR |= (1 << LED_PIN);
}

// Función para encender el LED
void led_on(void) {
    // Poner en bajo el bit correspondiente al pin LED_PIN en PDOR (asume lógica inversa)
    GPIO_PDOR &= ~(1 << LED_PIN);  // LED encendido
}

// Función para apagar el LED
void led_off(void) {
    // Poner en alto el bit correspondiente al pin LED_PIN en PDOR
    GPIO_PDOR |= (1 << LED_PIN);   // LED apagado
}
```

## 🧾 Ejercicio 2

Crea un script para escribir código en C en el IDE que estás utilizando para programar tu microcontrolador. Utilizaremos este scrpit en los siguientes ejercicios.

## **Macros y Preprocesador**

Las directivas de preprocesador permiten elegir qué parte del código se quiere pasar al compilador y cual se decide evitar, dependiendo de una directiva configurada en la sección de preprocesador. Observa el siguiente ejemplo:

```c
#include <stdio.h>

// Definición de constantes para seleccionar el puerto
//#define CONFIGURAR_PORT_A  // Descomenta esta línea para configurar el puerto A
#define CONFIGURAR_PORT_B  // Descomenta esta línea para configurar el puerto B

// Prototipos de las funciones
void configurarPortA(void);
void configurarPortB(void);

int main() {
    // Configurar el puerto adecuado según la constante definida
#ifdef CONFIGURAR_PORT_A
    configurarPortA();  // Configura el puerto A si está definida la constante
#elif defined(CONFIGURAR_PORT_B)
    configurarPortB();  // Configura el puerto B si está definida la constante
#else
    printf("Error: No se ha definido ningún puerto para configurar.\n");
#endif

    return 0;
}

// Función que configura el puerto A
void configurarPortA(void) {
    // Aquí irían los registros de hardware para configurar el puerto A en un sistema embebido
    printf("Puerto A configurado correctamente.\n");
}

// Función que configura el puerto B
void configurarPortB(void) {
    // Aquí irían los registros de hardware para configurar el puerto B en un sistema embebido
    printf("Puerto B configurado correctamente.\n");
}
```

El preprocesador de C permite definir macros, que son fragmentos de código que se reemplazan antes de la compilación. Son útiles para evitar sobrecarga de funciones y hacer el código más eficiente.

A continuación te muestro un programa en C que incluye tres ejemplos del uso de macros como instrucciones del preprocesador. Uno de los ejemplos es un clásico en sistemas embebidos.

```c
#include <stdio.h>

// Ejemplo 1: Macro para calcular el cuadrado de un número
#define SQUARE(x) ((x) * (x))

// Ejemplo 2: Macro para el valor de PI
#define PI 3.14159

// Ejemplo 3: En sistemas embebidos - Definir registros
// Definición de un puerto de salida en un microcontrolador (ejemplo)

#define PORTA_DATA_REG *(volatile unsigned int *)(0x400FF000)  

int main() {
    int num = 5;
    float area;

    // Ejemplo 1: Uso de la macro SQUARE
    printf("El cuadrado de %d es: %d\\n", num, SQUARE(num));

    // Ejemplo 2: Uso de la macro PI para calcular el área de un círculo
    float radio = 3.0;
    area = PI * SQUARE(radio);  // PI * radio^2
    printf("El área de un círculo con radio %.2f es: %.2f\\n", radio, area);

    return 0;
}

```

### Explicación de los ejemplos:

1. **Ejemplo 1: Macro para calcular el cuadrado de un número**
    
    Esta macro `SQUARE(x)` toma un valor `x` y devuelve su cuadrado. Es una macro simple que evita la sobrecarga de llamar a una función.
    
    ```c
    #define SQUARE(x) ((x) * (x))
    
    ```
    
2. **Ejemplo 2: Uso de la macro PI**
    
    La macro `PI` define el valor de pi como una constante. Se utiliza para calcular el área de un círculo usando la fórmula `PI * radio^2`. Las macros son útiles para definir constantes como estas, que se reutilizan en todo el código.
    
    ```c
    #define PI 3.14159
    ```
    

Este programa ilustra cómo usar macros en C, tanto para cálculos simples como para operaciones típicas en sistemas embebidos.

## #️⃣ Ejercicio 3

**Debes crear tres macros en C** enfocadas en el trabajo con sistemas embebidos. Las macros se utilizarán para manipular registros del microcontrolador u otras operaciones similares. 

1. **Aplicar una máscara para escribir en un registro del microcontrolador.** **Descripción**: En sistemas embebidos, es común escribir en registros específicos de hardware utilizando máscaras para modificar solo los bits deseados, sin afectar el resto del registro. **Crea una macro** que aplique una máscara a un registro. La macro debe permitirte especificar el registro y la máscara, y establecer los bits correspondientes en ese registro. Elige un registro del microcontrolador, selecciona la máscara que se debería aplicar para realizar una tarea particular. Elige un nombre adecuado para esta macro. 
2. **Determinar si un periférico está presente en el microprocesador**. **Descripción**: Los microcontroladores a menudo tienen registros de control de periféricos (como el registro PCC) que indican si ciertos periféricos, como los puertos, están habilitados. En esta tarea, deberás crear una macro para verificar si un periférico (ej. puerto E) está presente usando el registro de control. **Crea una macro** que verifique si el bit correspondiente a un periférico (ej. puerto E) está activado en el registro PCC.
3. **Alternar un bit de un registro**. **Descripción**: En algunos casos, es necesario alternar (cambiar) un bit específico de un registro, por ejemplo, para cambiar el estado de un LED o para activar/desactivar un periférico. **Crea una macro** que alterne el estado de un bit específico en un registro.
4. Piensa en otra macro que pudiera ser útil y que puedas usar en tus proyectos e impleméntala. 

---

## **Tipos de datos y operadores**

C soporta una variedad de tipos de datos como `char`, `int`, `float`, y `double`. Es importante también entender los operadores aritméticos y lógicos, como `+`, `-`, `*`, `/`, y `%`, así como los operadores de incremento y decremento (`++`, `--`). Lógicos: OR (|), AND (&), XOR (^), NOT (~)

## **Ejercicio 4**:

A continuación te muestro algunos ejemplos de errores que pueden ocurrir si no se utilizan los tipos de datos correctos o la sintaxis adecuada. 

```c
#include <stdio.h>
#include <string.h>

int main() {
    // Datos enteros
    int entero = 10.5;  
    printf("El valor del entero es: %d\n", entero);  

    // Datos float
    float decimal = 3 / 2;  
    printf("El valor del decimal es: %f\n", decimal);  
    float decimal_1 = 12.5 * 5;  
    printf("El valor del decimal es: %d\n", decimal_1);  

    // Caracteres
    char letra = "A";  
    printf("El valor del caracter es: %c\n", letra);

    // Cadenas de caracteres
    char nombre[5];  
    strcpy(nombre, "Henry");  
    printf("El nombre es: %s\n", nombre);

    return 0;
}
```

Analiza los ejemplos anteriores, identifica los posibles errores en cada caso y propón una solución. Prueba tu solución en un compilador Online. Puedes utilizar el del link: [**Compilador Online de C**](https://www.onlinegdb.com/online_c_compiler)

## ➡️  Operadores relacionales

Analiza el siguiente ejemplo donde se muestra el uso de los operadores relacionales. 

```c
#include <stdio.h>

int main() {
    int edad = 25;
    float salario = 3000.0;
    int experiencia = 5;
    // Verificar si la persona cumple con los requisitos para un trabajo
    // Requisitos: Edad entre 20 y 30 años, experiencia mayor o igual a 3 años
    // y salario mayor a 2500 o menor a 3500
    if ((edad >= 20 && edad <= 30) && (experiencia >= 3) && (salario > 2500 || salario < 3500)) {
        printf("La persona cumple con los requisitos para el trabajo.\n");
    } else {
        printf("La persona NO cumple con los requisitos para el trabajo.\n");
    }
    return 0;
}
```

## Operadores a nivel de bits

En sistemas embebidos, es muy importante el uso de operadores que funcionan a nivel de bits. A continuación te propongo una serie de ejemplos en los que podrás observar cómo se utilizan estos operadores en C.

```c
#include <stdio.h>

int main() {
    unsigned char a = 0x5A;  // 01011010 en binario
    unsigned char b = 0x3C;  // 00111100 en binario
    unsigned char resultado;

    // Operador AND (&)
    resultado = a & b;  // 01011010 & 00111100 = 00011000 (0x18)
    printf("Resultado de a & b: 0x%X\\n", resultado);

    // Operador OR (|)
    resultado = a | b;  // 01011010 | 00111100 = 01111110 (0x7E)
    printf("Resultado de a | b: 0x%X\\n", resultado);

    // Operador XOR (^)
    resultado = a ^ b;  // 01011010 ^ 00111100 = 01100110 (0x66)
    printf("Resultado de a ^ b: 0x%X\\n", resultado);

    // Desplazamiento a la izquierda (<<)
    resultado = a << 2;  // 01011010 << 2 = 10110100 (0xB4)
    printf("Resultado de a << 2: 0x%X\\n", resultado);

    // Desplazamiento a la derecha (>>)
    resultado = a >> 2;  // 01011010 >> 2 = 00010110 (0x16)
    printf("Resultado de a >> 2: 0x%X\\n", resultado);

    return 0;
}
```

### Explicación de los operadores:

1. **AND (&)**:
El operador AND compara bit a bit entre dos números. El resultado es 1 solo si ambos bits son 1, de lo contrario es 0.
    - `a = 0x5A` → `01011010`
    - `b = 0x3C` → `00111100`
    - `a & b` → `00011000` (0x18)
2. **OR (|)**:
El operador OR también compara bit a bit. El resultado es 1 si al menos uno de los bits es 1.
    - `a = 0x5A` → `01011010`
    - `b = 0x3C` → `00111100`
    - `a | b` → `01111110` (0x7E)
3. **XOR (^)**:
El operador XOR (OR exclusivo) compara bit a bit, y el resultado es 1 si los bits correspondientes son diferentes.
    - `a = 0x5A` → `01011010`
    - `b = 0x3C` → `00111100`
    - `a ^ b` → `01100110` (0x66)
4. **Desplazamiento a la izquierda (<<)**:
El operador de desplazamiento a la izquierda mueve todos los bits a la izquierda por el número de posiciones especificado. Los bits vacíos a la derecha se rellenan con 0. Esto es equivalente a multiplicar el número por 2 elevado al número de posiciones que se desplazó.
    - `a = 0x5A` → `01011010`
    - `a << 2` → `10110100` (0xB4)
5. **Desplazamiento a la derecha (>>)**:
El operador de desplazamiento a la derecha mueve todos los bits a la derecha por el número de posiciones especificado. Los bits vacíos a la izquierda se rellenan con 0. Esto es equivalente a dividir el número por 2 elevado al número de posiciones que se desplazó.
    - `a = 0x5A` → `01011010`
    - `a >> 2` → `00010110` (0x16)

### Resultado de la ejecución:

Ejecuta el código anterior y verifica que la salida sea la esperada:

```c
Resultado de a & b: 0x18
Resultado de a | b: 0x7E
Resultado de a ^ b: 0x66
Resultado de a << 2: 0xB4
Resultado de a >> 2: 0x16
```

---

## **Control de flujo**

C ofrece estructuras para controlar el flujo del programa como `if`, `else`, `while`, `for`, `switch`, entre otras. Estas estructuras permiten crear programas que tomen decisiones o repitan acciones.

## Ejericicio 4

Tu tarea es identificar los errores y proponer una solución para corregirlos. Los errores pueden ser bucles infinitos, accesos inválidos a memoria, o condiciones incorrectas. Explica cuál es el problema en cada uno y cómo solucionarlo.

```c
#include <stdio.h>

int main() {
    int i;
    int num = 10;
    int array[5] = {1, 2, 3, 4, 5};
    int contador = 0;

    for (i = 1; i < 10; i--) {
        printf("Valor de i: %d\n", i);
    }

    for (i = 0; i <= 5; i++) {
        printf("Elemento del array: %d\n", array[i]);
    }

    while (num != 0) {
        printf("Valor de num: %d\n", num);
        num = num + 1;  
    }

    while (contador < 5) {
        printf("Valor de contador: %d\n", contador);
    }

    return 0;
}
```

---

## Punteros en C y su Aplicación en Sistemas Embebidos

Los punteros en C son variables que almacenan direcciones de memoria, permitiendo un acceso directo y eficiente a ubicaciones de memoria. En sistemas embebidos, donde es necesario interactuar con hardware como registros de control, periféricos y puertos de entrada/salida (I/O), los punteros son esenciales para acceder a direcciones específicas que representan estos recursos. En este contexto, los punteros permiten leer y escribir directamente en registros de hardware mapeados en memoria.

Por ejemplo, un microcontrolador tiene diversos registros asociados a la configuración de periféricos (como puertos de entrada/salida o timers). Estos registros se encuentran en direcciones de memoria específicas, y mediante punteros en C, es posible manipular estos registros para controlar el hardware.

### Explicación del Código

```c
#define PCC_PORTB (*(volatile uint32_t *)(PCC_BASE + PCC_PORTB_OFFSET))
```

Este código es una definición de macro que crea un acceso directo al registro del puerto B (PORTB) del microcontrolador a través de un puntero. A continuación, explico cada parte:

1. **`PCC_BASE`**: Es una constante que define la dirección base del módulo **PCC** (Peripheral Clock Controller), que controla los relojes de los periféricos.
2. **`PCC_PORTB_OFFSET`**: Define el **desplazamiento (offset)** dentro de la dirección base del PCC donde se encuentra el registro que controla el puerto B (PORTB).
3. **`(PCC_BASE + PCC_PORTB_OFFSET)`**: Suma la dirección base del PCC con el desplazamiento del registro del puerto B, obteniendo así la **dirección completa** del registro PORTB.
4. **`(volatile uint32_t *)`**: Es un **puntero a un entero sin signo de 32 bits** (`uint32_t`) que apunta a la dirección de memoria calculada. La palabra clave **`volatile`** indica que este valor puede cambiar en cualquier momento, por lo que el compilador no debe optimizar el acceso a esa memoria (esto es importante para registros de hardware).
5. **`(...)`**: El operador de **desreferenciación**  permite acceder al valor contenido en la dirección de memoria apuntada por el puntero. Es decir, estamos accediendo directamente al contenido del registro del puerto B.

---

## **Funciones**

Las funciones modularizan el código, mejorando su legibilidad y reusabilidad. Además, se pueden pasar parámetros a las funciones por valor o por referencia, lo cual es útil para trabajar con punteros y arrays.

## **Ejercicio 5**:

Implementa una función que reciba un número y devuelva su factorial. Luego, llama a esta función desde el `main()`.

### Cuidado con las funciones recursivas

**Las funciones recursivas** son aquellas que se llaman a sí mismas para resolver un problema, dividiéndolo en subproblemas más pequeños y simples. Este tipo de función es útil cuando un problema puede expresarse en términos de sí mismo, como en el cálculo de factoriales o en la búsqueda en estructuras de datos como árboles. En una función recursiva, siempre debe existir una condición de terminación o caso base que detenga las llamadas recursivas, evitando que la función se ejecute indefinidamente. A continuación se muestra un ejemplo de una función recursiva que calcula el factorial de un número:

```c
int factorial(int n) {
    if (n == 0) {
        return 1;  // Caso base
    } else {
        return n * factorial(n - 1);  // Llamada recursiva
    }
}
```

En este caso, la función se llama a sí misma con el valor `n - 1` hasta que `n` sea igual a 0, momento en el cual se detiene. 

<aside>
💡

**Nota:** es importante ser muy cuidadoso al usar funciones recursivas, ya que si no se establece correctamente la condición de terminación, se puede generar un desbordamiento de pila ("***Stack Overflow***") rápidamente, causando que el programa falle debido al consumo excesivo de memoria.

</aside>

---

## **Manejo de Interrupciones y Variables Volátiles**

En sistemas embebidos, es común utilizar interrupciones para responder a eventos externos. Las variables `volatile` se emplean para decirle al compilador que la variable puede cambiar en cualquier momento, por ejemplo, en una rutina de servicio de interrupción (ISR).

---

## **Estructuras y Uniones**

Las estructuras (`struct`) y uniones (`union`) en C permiten agrupar diferentes tipos de datos bajo un mismo nombre. Las estructuras son útiles para representar datos complejos, mientras que las uniones comparten el mismo espacio de memoria para diferentes variables.

A continuación te muestro un ejemplo en C donde se combina el uso de una estructura y una unión. Este tipo de combinación es muy útil en sistemas embebidos cuando se necesita ahorrar memoria y representar datos en diferentes formas, pero compartiendo el mismo espacio de memoria.

### Ejemplo de Estructura y Unión:

```c
#include <stdio.h>

// Definición de la unión dentro de la estructura
typedef union {
    struct {
        unsigned char byte1;
        unsigned char byte2;
        unsigned char byte3;
        unsigned char byte4;
    } bytes;
    float valor_flotante;
} DataUnion;

typedef struct {
    char nombre[10];   // Nombre del sensor
    int id;            // ID del sensor
    DataUnion data;    // Unión para compartir memoria entre el float y los bytes
} Sensor;

int main() {
    Sensor sensor1;

    // Asignar valores al sensor
    sensor1.id = 101;
    snprintf(sensor1.nombre, 10, "Temp_Sens");

    // Asignar un valor flotante a través de la unión
    sensor1.data.valor_flotante = 23.75;

    // Mostrar los valores
    printf("Sensor: %s (ID: %d)\\n", sensor1.nombre, sensor1.id);
    printf("Valor flotante: %.2f\\n", sensor1.data.valor_flotante);

    // Acceder a los bytes individuales que componen el valor flotante
    printf("Bytes del valor flotante:\\n");
    printf("Byte 1: 0x%02X\\n", sensor1.data.bytes.byte1);
    printf("Byte 2: 0x%02X\\n", sensor1.data.bytes.byte2);
    printf("Byte 3: 0x%02X\\n", sensor1.data.bytes.byte3);
    printf("Byte 4: 0x%02X\\n", sensor1.data.bytes.byte4);

    return 0;
}
```

### Explicación:

### 1. **Estructura `Sensor`**:

- La estructura contiene:
    - Un array de caracteres (`nombre`) para almacenar el nombre del sensor.
    - Un entero (`id`) que identifica al sensor.
    - Una **unión** llamada `data`, que permite almacenar un valor flotante o acceder a los bytes individuales que componen ese flotante.

### 2. **Unión `DataUnion`**:

- La unión define dos maneras diferentes de interpretar los datos:
    - Como cuatro bytes (`byte1`, `byte2`, `byte3`, `byte4`).
    - Como un valor de tipo `float` (`valor_flotante`).
- Como es una **unión**, ambas variables (los bytes y el valor flotante) **comparten el mismo espacio de memoria**. Esto significa que cualquier cambio en una de ellas afecta a la otra. Al modificar el valor flotante, los cuatro bytes de la unión también se ven afectados, ya que representan físicamente el mismo dato.

### 3. **Cómo comparten memoria**:

- Cuando asignas un valor flotante a `sensor1.data.valor_flotante`, ese valor se almacena en los mismos 4 bytes que también están representados por `byte1`, `byte2`, `byte3` y `byte4`.
- Por ejemplo, si asignas el valor `23.75` al `float`, los bytes se organizan en memoria de acuerdo con el formato IEEE 754, y puedes acceder a ellos directamente usando los miembros `byte1`, `byte2`, `byte3`, y `byte4` de la unión.

### 4. **Uso correcto**:

- Este tipo de combinación se usa cuando necesitas representar un dato de diferentes formas (por ejemplo, leer un valor como flotante y acceder a sus bytes individuales para enviarlos a través de un puerto o realizar operaciones de bajo nivel).
- Es importante recordar que **solo puedes usar una de las representaciones a la vez**, ya que cambiar el valor de un miembro de la unión afecta a los otros.

### Salida esperada:

```c
Sensor: Temp_Sens (ID: 101)
Valor flotante: 23.75
Bytes del valor flotante:
Byte 1: 0x00
Byte 2: 0x40
Byte 3: 0xBE
Byte 4: 0x41
```

---