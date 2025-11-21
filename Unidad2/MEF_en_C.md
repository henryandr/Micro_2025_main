# Actividad 3: Programando una MEF en lenguaje C

---

## 1. Tutorial paso a paso

En esta actividad aprenderás a diseñar e implementar una Máquina de Estados Finitos (MEF) para el antirrebote de un pulsador utilizando la STM32F407VET6 y el lenguaje C. Te guiaré paso a paso en la traducción del diagrama de estados al código, exploraremos conceptos clave de las MEF (Moore vs Mealy) y desarrollarás las habilidades necesarias para gestionar el tiempo de debounce, estructurar tu programa y aplicar buenas prácticas de programación.

### 1.1 Análisis del diagrama de estados

**Diagrama de estados :**

![Figura 1. Diagrama de Estados.](attachment:a1566d3e-c279-4d2b-af04-87b3f53c65c0:image.png)

Figura 1. Diagrama de Estados.

**Tabla de Transiciones:**

| Estado Actual | Evento / Condición | Estado Siguiente |
| --- | --- | --- |
| Espera | Pulsador ON | Debounce_Press |
| Debounce_Press | 30 ms y pulsador ON | Presionado |
| Debounce_Press | 30 ms y pulsador OFF | Espera |
| Presionado | 30 ms y pulsador OFF | Debounce_Release |
| Presionado | 30 ms y pulsador ON | Presionado |
| Debounce_Release | 30 ms y pulsador OFF | Espera |
| Debounce_Release | 30 ms y pulsador ON | Presionado |

El diagrama de estados describe cuatro estados principales:

1. **Espera (ESPERA)**
2. **Debounce_Press (DEBOUNCE_PRESS)**
3. **Presionado (PRESIONADO)**
4. **Debounce_Release (DEBOUNCE_RELEASE)**

La lógica general es:

- Desde **ESPERA**, si detectamos el pulsador en ON, pasamos a **DEBOUNCE_PRESS** y esperamos 30 ms para confirmar que se mantiene ON.
- Si tras esos 30 ms el pulsador sigue ON, cambiamos a **PRESIONADO**; si no, volvemos a **ESPERA**.
- En **PRESIONADO**, nos quedamos ahí mientras cada 30 ms verifiquemos que el pulsador sigue ON.
- Si durante **PRESIONADO** detectamos que el pulsador se liberó, pasamos a **DEBOUNCE_RELEASE** y esperamos 30 ms para confirmar.
- Si tras 30 ms el pulsador sigue OFF, volvemos a **ESPERA**; si no, regresamos a **PRESIONADO**.

### 1.2 Definición del tipo de MEF: Moore o Mealy

Para el antirrebote de un botón, una **Máquina de Estados de tipo Moore** suele ser la más utilizada, porque las salidas dependen únicamente del estado actual, no de las entradas en tiempo real (salvo para determinar las transiciones).

### 1.3 Desarrollo del código en C

# Versión Bare-Metal

---

## 1. Configuración básica del proyecto

Para trabajar de forma “bare-metal” en un STM32F407, debemos incluir los archivos de cabecera que definen los registros y estructuras del microcontrolador. Normalmente, esto se hace incluyendo el header genérico de la familia:

```c
#include "stm32f407xx.h" // o "stm32f4xx.h" dependiendo de tu entorno

```

<aside>
🔥

> Nota: Si estás usando un IDE como Keil, IAR o STM32CubeIDE, asegúrate de que tu proyecto incluya los “device headers” correctos para tu micro.
> 
</aside>

---

## 2. Configuración del GPIO para el pulsador

Supondremos que el pulsador está conectado al **pin PA0**. Para usarlo como entrada, debemos:

1. **Activar el reloj** de GPIOA.
2. **Configurar el pin** como entrada (modo digital).

### 2.1. Habilitar el reloj de GPIOA

En el registro `RCC->AHB1ENR` (RCC AHB1 peripheral clock enable register) se habilita el bit correspondiente al puerto A:

```c
// Activar el clock para GPIOA
RCC->AHB1ENR |= (1 << 0);  // El bit 0 corresponde a GPIOA
```

### 2.2. Configurar PA0 como entrada

En el registro `GPIOA->MODER` (GPIO port mode register), cada pin usa 2 bits para definir el modo (00: entrada, 01: salida, 10: modo alterno, 11: modo analógico). Para el pin 0:

```c
// PA0 en modo entrada (00)
GPIOA->MODER &= ~(3 << (0 * 2));
```

> Opción de pull-up/pull-down: Si necesitas una resistencia de pull-up o pull-down interna, configúrala en GPIOA->PUPDR. Por ejemplo, para pull-down en PA0:
> 
> 
> ```c
> GPIOA->PUPDR &= ~(3 << (0 * 2)); // Limpiar
> GPIOA->PUPDR |=  (2 << (0 * 2)); // 10 = pull-down
> ```
> 

---

## 3. Configuración del SysTick

El **SysTick** es un temporizador de 24 bits integrado que podemos usar para generar interrupciones periódicas. Lo configuraremos para que genere una interrupción cada **1 ms** y así podamos contar tiempo en milisegundos.

### 3.1. Cálculo de la carga (LOAD)

Supongamos que el **clock del sistema** (HCLK) está a 16 MHz (por simplicidad). El SysTick se puede configurar para que su fuente de reloj sea el HCLK (o HCLK/8, según el bit `SysTick->CTRL.CLKSOURCE`).

- Para un **tick de 1 ms**, necesitamos que SysTick dispare a 16,000 cuentas si está usando HCLK directamente (16 MHz * 1 ms).

```c
// 1 ms => 16000 cuentas si el clock es 16 MHz
SysTick->LOAD  = 16000 - 1;  // El -1 es porque la cuenta va de 0 a LOAD
SysTick->VAL   = 0;          // Reiniciar el valor actual
```

### 3.2. Configurar y habilitar SysTick

En el registro `SysTick->CTRL`, debemos habilitar la interrupción, seleccionar la fuente de reloj y ponerlo en marcha.

```c
// Configurar SysTick
SysTick->CTRL = 0
    | (1 << 2)  // CLKSOURCE = 1 => usa HCLK directamente
    | (1 << 1)  // TICKINT   = 1 => habilita la interrupción
    | (1 << 0); // ENABLE    = 1 => inicia el contador
```

### 3.3. Implementar la rutina de interrupción

Cada vez que SysTick desborde, llamará a la función `SysTick_Handler()`. Podemos usar una variable global para llevar la cuenta de milisegundos:

```c
volatile uint32_t msTicks = 0;

void SysTick_Handler(void) {
    msTicks++;  // Incrementa cada 1 ms
}
```

> Nota: Asegúrate de que tu vector de interrupciones tenga referenciada la función SysTick_Handler. En la mayoría de los CMSIS, esto viene por defecto.
> 

### 3.4. Función para obtener el tiempo

Creamos una función de conveniencia para leer `msTicks`:

```c
uint32_t GetTick(void) {
    return msTicks;
}
```

---

## 4. Definición de la Máquina de Estados Finitos (MEF)

### 4.1. Estados de la MEF

Basados en tu diagrama, definimos cuatro estados:

```c
typedef enum {
    ESPERA,
    DEBOUNCE_PRESS,
    PRESIONADO,
    DEBOUNCE_RELEASE
} ButtonState_t;
```

### 4.2. Estructura para la MEF

Para encapsular los datos de la MEF (estado actual y tiempo de la última transición):

```c
typedef struct {
    ButtonState_t currentState;
    uint32_t lastChangeTime;
} ButtonFSM_t;
```

### 4.3. Inicializar la MEF

Al inicio, el estado es `ESPERA` y guardamos el tiempo actual:

```c
void ButtonFSM_Init(ButtonFSM_t *fsm) {
    fsm->currentState = ESPERA;
    fsm->lastChangeTime = GetTick(); // ms actual
}
```

### 4.4. Función para leer el pulsador

Sin HAL, leemos directamente el **Input Data Register (IDR)** del puerto A. Si el bit 0 está en alto, el pulsador está presionado:

```c
uint8_t ReadButton(void) {
    // Retorna 1 si el botón (PA0) está presionado, 0 si no
    return ((GPIOA->IDR & (1 << 0)) != 0) ? 1 : 0;
}
```

---

## 5. Lógica de la MEF: `ButtonFSM_Update()`

Esta función se llama de forma periódica (por ejemplo, en el `while(1)` principal). Utiliza el **SysTick** para medir el tiempo transcurrido y decidir los cambios de estado.

```c
void ButtonFSM_Update(ButtonFSM_t *fsm) {
    uint8_t buttonState = ReadButton();
    uint32_t currentTime = GetTick(); // Tiempo en ms
//Agrega el código necesario
    switch(fsm->currentState) {
        case ESPERA:
            // Si se detecta el botón presionado
            break;

        case DEBOUNCE_PRESS:
            // Esperamos 30 ms para confirmar
                // Si sigue presionado, pasamos a PRESIONADO
                // Si no, regresamos a ESPERA
            break;

        case PRESIONADO:
            // Mientras siga presionado, nos mantenemos aquí
            // Si se libera, pasamos a DEBOUNCE_RELEASE
            break;

        case DEBOUNCE_RELEASE:
            // Esperamos 30 ms para confirmar la liberación
            break;

        default:
            // Manejo de error o estado no esperado
            fsm->currentState = ESPERA;
            break;
    }
}
```

---

## 6. Estructura del `main.c`

Finalmente, en el archivo principal inicializamos todo:

1. **Configurar reloj** (dependiendo de tu setup).
2. **Configurar SysTick**.
3. **Configurar GPIO**.
4. **Inicializar la MEF**.
5. **Loop infinito** llamando a la función de actualización.

```c
#include "stm32f407xx.h"

volatile uint32_t msTicks = 0;

// Definiciones de la MEF (ButtonState_t, ButtonFSM_t, etc.)
// ...

// SysTick_Handler
void SysTick_Handler(void) {
    msTicks++;
}

// GetTick
uint32_t GetTick(void) {
    return msTicks;
}

// Configurar SysTick para 1 ms (ejemplo con 16 MHz)
void SysTick_Init(void) {
    SysTick->LOAD  = 16000 - 1;                   // Cuenta para 1 ms
    SysTick->VAL   = 0;                           // Reinicia el valor
    SysTick->CTRL  = (1 << 2) | (1 << 1) | (1 << 0);
    // Bits: CLKSOURCE=1, TICKINT=1, ENABLE=1
}

int main(void) {
    // 1. Activar el reloj de GPIOA
    RCC->AHB1ENR |= (1 << 0);

    // 2. Configurar PA0 como entrada
    GPIOA->MODER &= ~(3 << (0 * 2));  // Modo entrada
    // Opcional: Pull-down
    // GPIOA->PUPDR &= ~(3 << (0 * 2));
    // GPIOA->PUPDR |=  (2 << (0 * 2)); // 10 => pull-down

    // 3. Iniciar SysTick
    SysTick_Init();

    // 4. Inicializar la MEF
    ButtonFSM_t myButtonFSM;
    ButtonFSM_Init(&myButtonFSM);

    while(1) {
        // 5. Actualizar la MEF en cada iteración
        ButtonFSM_Update(&myButtonFSM);

        // Aquí puedes realizar otras tareas de tu aplicación
        // ...
    }
}
```

---

## 7. Notas finales y buenas prácticas

1. **Revisar el reloj del sistema:**
    - Asegúrate de que tu `SystemCoreClock` sea de 16 MHz o configura SysTick_Load adecuadamente según tu frecuencia real.
2. **Evitar retardos bloqueantes:**
    - El SysTick se usa de forma no bloqueante, lo cual es muy útil para sistemas con múltiples tareas.
3. **Posibles extensiones:**
    - Podrías controlar un LED en el estado `PRESIONADO`.
    - Manejar interrupciones de EXTI en lugar de polling si así lo requieres, aunque para el antirrebote generalmente se hace polling.
4. **Optimizar la frecuencia de actualización:**
    - Si no necesitas leer el pulsador en cada ciclo de CPU, podrías usar un timer o un RTOS. Pero esta aproximación es suficiente para muchos casos.

---

# Versión utilizando la HAL de ST

### Estructura básica

```c
#include "stm32f4xx_hal.h"

// Duración del debounce en milisegundos
#define DEBOUNCE_TIME 30

typedef enum {
    ESPERA,
    DEBOUNCE_PRESS,
    PRESIONADO,
    DEBOUNCE_RELEASE
} ButtonState_t;

typedef struct {
    ButtonState_t currentState;
    uint32_t lastChangeTime; // Para controlar el tiempo de debounce
} ButtonFSM_t;

// Prototipos
void ButtonFSM_Init(ButtonFSM_t *fsm);
void ButtonFSM_Update(ButtonFSM_t *fsm, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t ReadButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

```

### Inicialización de la MEF

```c
void ButtonFSM_Init(ButtonFSM_t *fsm) {
    fsm->currentState = ESPERA;
    fsm->lastChangeTime = HAL_GetTick(); // Lee el tick actual del sistema
}

```

### Función de lectura de botón

```c
uint8_t ReadButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    // Retorna 1 si el botón está presionado, 0 si no
    return (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET) ? 1 : 0;
}

```

### Actualización de la MEF

```c
void ButtonFSM_Update(ButtonFSM_t *fsm, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    uint8_t buttonState = ReadButton(GPIOx, GPIO_Pin);
    uint32_t currentTime = HAL_GetTick();

    switch(fsm->currentState) {
        case ESPERA:
            if (buttonState == 1) {
                fsm->currentState = DEBOUNCE_PRESS;
                fsm->lastChangeTime = currentTime;
            }
            break;

        case DEBOUNCE_PRESS:
            /*Completa aquí el código faltante de este estado*/
            break;

        case PRESIONADO:
            if (buttonState == 0) {
                fsm->currentState = DEBOUNCE_RELEASE;
                fsm->lastChangeTime = currentTime;
            }
            // Aquí podemos colocar código adicional para cuando el botón se mantiene presionado
            break;

        case DEBOUNCE_RELEASE:
            /*Completa aquí el código faltante de este estado*/

            break;

        default:
            // Estado inválido, podemos volver a ESPERA o manejar error
            fsm->currentState = ESPERA;
            break;
    }
}

```

### Llamada a la MEF en el `main.c`

```c
int main(void)
{
    HAL_Init();
    // Configurar reloj, GPIO, etc...

    ButtonFSM_t myButtonFSM;
    ButtonFSM_Init(&myButtonFSM);

    while(1)
    {
        ButtonFSM_Update(&myButtonFSM, GPIOA, GPIO_PIN_0);
        // Otras tareas...
    }
}

```

> 💡 Consejo: Es buena práctica tener un `archivo .c` dedicado (por ejemplo, `button_fsm.c`) y un .h (`button_fsm.h`) para declarar funciones y estructuras, facilitando la mantenibilidad.
> 

---

## 2. Preguntas de control

1. **¿Por qué usamos un temporizador (30 ms) para validar el estado del pulsador?**
    - Justifica cómo el retardo previene lecturas erróneas por rebotes.
2. **En qué se diferencia una MEF de tipo Moore de una Mealy y por qué en antirrebote es más común Moore?**
    - Explica la dependencia de las salidas frente a los estados o las entradas.
3. **¿Cómo podrías escalar este código para manejar múltiples pulsadores en simultáneo?**
    - Describe cómo adaptar las estructuras y la función de actualización.
4. **¿Por qué es importante actualizar el `lastChangeTime` cuando se pasa a un estado de debounce?**
    - Reflexiona sobre la función de esta variable en la lógica de tiempo.

---

## 3. Ejercicios prácticos

1. **Interpretación de diagramas de estados:**
    - Dibuja tu propio diagrama para otro periférico (por ejemplo, LED con encendido/apagado) y describe los estados y transiciones.
2. **Complementación de código incompleto:**
    - Imagina que faltan las partes donde se inicializan las GPIO. Complétalas agregando las llamadas de configuración (`HAL_GPIO_Init`).
3. **Desarrollo de funciones faltantes:**
    - Crea una función `ButtonFSM_IsPressed()` que retorne `true` si el botón se encuentra en el estado `PRESIONADO`.
4. **Corrección de errores intencionados:**
    - Cambia la condición del debounce a `if ((currentTime - fsm->lastChangeTime) < DEBOUNCE_TIME)` y explica por qué no funcionaría bien.

---

## 4. Callouts y notas clave

> ⚠️ Memoria y eficiencia:
> 
> - Evita variables globales innecesarias. Usa estructuras (como `ButtonFSM_t`) para encapsular la lógica de cada pulsador.
> - Asegúrate de que los contadores de tiempo no se desborden (considera el uso de variables `uint32_t`).

> 💡 Estructuras de control adecuadas:
> 
> - `switch-case` es más legible que múltiples `if-else`.
> - Usa `enum` para tus estados, evitando números mágicos en el código.

> 🚀 Utiliza las características de la STM32F407VET6:
> 
> - El **SysTick** (1 ms por defecto) te ayuda a medir el tiempo sin bloquear la ejecución.
> - Puedes implementar interrupciones GPIO si lo deseas, pero en la práctica para antirrebote, el método polling con la MEF es muy sencillo y efectivo.

---