# Sesión 3: Implementación de MEF en C

**Duración**: 2 horas  
**Modalidad**: Presencial con práctica intensiva de código

---

## Objetivos Específicos

1. **Implementar** Máquinas de Estados Finitos en C usando `enum` y `switch-case`
2. **Aplicar** MEF para resolver el problema de antirrebote de botones
3. **Integrar** MEF con hardware real (GPIO y timing)

---

## Contenidos Temáticos

### 1. Estructuras de Datos para MEF (20 minutos)

#### Enumeración de Estados

**`enum` en C**: Define conjunto de constantes nombradas

```c
// Definir estados como enumeración
typedef enum {
    ESTADO_LIBERADO,
    ESTADO_DEBOUNCE_PRESS,
    ESTADO_PRESIONADO,
    ESTADO_DEBOUNCE_RELEASE
} Estado_t;

// Variable de estado actual
Estado_t estado_actual = ESTADO_LIBERADO;
```

**Ventajas de `enum`**:
- Código más legible que números mágicos
- El compilador puede detectar errores
- Autocompletado en IDE
- Fácil de mantener

**Comparación con alternativas**:

```c
// MAL: Números mágicos
int estado = 0;  // ¿Qué significa 0?
if (estado == 2) { ... }  // ¿Qué es estado 2?

// MAL: Macros
#define LIBERADO 0
#define PRESIONADO 1
int estado = LIBERADO;  // No hay type checking

// BIEN: Enum
Estado_t estado = ESTADO_LIBERADO;  // Claro y type-safe
```

#### Variables de Tiempo para MEF

```c
#include <stdint.h>

// Variable para tracking de tiempo
uint32_t tiempo_en_estado = 0;  // Milisegundos en estado actual

// Función que se llama cada 1ms (por timer interrupt)
void actualizar_tiempo(void) {
    tiempo_en_estado++;
}

// Función que resetea tiempo al cambiar de estado
void cambiar_estado(Estado_t nuevo_estado) {
    estado_actual = nuevo_estado;
    tiempo_en_estado = 0;  // Resetear contador
}
```

### 2. Patrón Switch-Case para MEF (30 minutos)

#### Estructura Básica

```c
void procesar_mef(void) {
    switch (estado_actual) {
        case ESTADO_1:
            // Acciones en estado 1
            // Verificar condiciones de transición
            if (condicion_para_ir_a_estado_2) {
                cambiar_estado(ESTADO_2);
            }
            break;
            
        case ESTADO_2:
            // Acciones en estado 2
            if (condicion_para_ir_a_estado_1) {
                cambiar_estado(ESTADO_1);
            }
            break;
            
        default:
            // Estado inválido - recuperación
            cambiar_estado(ESTADO_1);
            break;
    }
}
```

#### Ejemplo Completo: LED Parpadeante con MEF

```c
#include <stdint.h>
#include "stm32f407xx.h"

// Estados
typedef enum {
    LED_APAGADO,
    LED_ENCENDIDO
} EstadoLED_t;

// Variables globales
EstadoLED_t estado_led = LED_APAGADO;
uint32_t tiempo_en_estado = 0;

// Constantes de timing
#define TIEMPO_ON_MS  500
#define TIEMPO_OFF_MS 500

// Función principal de MEF (llamar cada 1ms)
void mef_led_parpadeo(void) {
    switch (estado_led) {
        case LED_APAGADO:
            // Acción: mantener LED apagado
            GPIOA->BSRR = (1 << 22);  // PA6 OFF
            
            // Transición: después de TIEMPO_OFF_MS
            if (tiempo_en_estado >= TIEMPO_OFF_MS) {
                estado_led = LED_ENCENDIDO;
                tiempo_en_estado = 0;
            }
            break;
            
        case LED_ENCENDIDO:
            // Acción: mantener LED encendido
            GPIOA->BSRR = (1 << 6);   // PA6 ON
            
            // Transición: después de TIEMPO_ON_MS
            if (tiempo_en_estado >= TIEMPO_ON_MS) {
                estado_led = LED_APAGADO;
                tiempo_en_estado = 0;
            }
            break;
            
        default:
            // Estado inválido - recuperación
            estado_led = LED_APAGADO;
            tiempo_en_estado = 0;
            break;
    }
    
    tiempo_en_estado++;
}
```

### 3. MEF para Antirrebote de Botón (40 minutos)

#### Diagrama de Estados (Repaso Sesión 2)

```
┌──────────┐  boton_down    ┌────────────────┐
│LIBERADO  │───────────────>│DEBOUNCE_PRESS  │
└──────────┘<───────────────└────────────────┘
    ↑         boton_up &&        │
    │         timeout              │ boton_down &&
    │                             │ timeout
    │                             ↓
┌──────────────────┐       ┌──────────────┐
│DEBOUNCE_RELEASE  │<──────│ PRESIONADO   │
└──────────────────┘       └──────────────┘
```

#### Implementación Completa

```c
/**
 * @file debounce_mef.c
 * @brief MEF para antirrebote de botón
 */

#include <stdint.h>
#include "stm32f407xx.h"

// Estados de la MEF
typedef enum {
    BTN_LIBERADO,
    BTN_DEBOUNCE_PRESS,
    BTN_PRESIONADO,
    BTN_DEBOUNCE_RELEASE
} EstadoBoton_t;

// Variables de la MEF
static EstadoBoton_t estado_boton = BTN_LIBERADO;
static uint32_t tiempo_en_estado = 0;
static uint8_t boton_estable = 0;  // Salida: 0=liberado, 1=presionado

// Constantes
#define TIEMPO_DEBOUNCE_MS 30
#define PIN_BOTON 13  // PC13 en muchas boards STM32

/**
 * @brief Leer estado físico del botón
 * @return 1 si presionado, 0 si liberado
 */
static uint8_t leer_boton_fisico(void) {
    // Asumir botón activo bajo con pull-up
    uint8_t estado = (GPIOC->IDR & (1 << PIN_BOTON)) ? 0 : 1;
    return estado;
}

/**
 * @brief Obtener estado estable del botón (salida de MEF)
 * @return 1 si presionado, 0 si liberado
 */
uint8_t obtener_estado_boton(void) {
    return boton_estable;
}

/**
 * @brief Procesar MEF de antirrebote
 * @note Llamar cada 1ms desde interrupción de timer
 */
void procesar_debounce_mef(void) {
    uint8_t boton_actual = leer_boton_fisico();
    
    switch (estado_boton) {
        case BTN_LIBERADO:
            // Salida del estado
            boton_estable = 0;
            
            // Transición: detectar pulsación
            if (boton_actual == 1) {
                estado_boton = BTN_DEBOUNCE_PRESS;
                tiempo_en_estado = 0;
            }
            break;
            
        case BTN_DEBOUNCE_PRESS:
            // Salida del estado (mantener anterior)
            boton_estable = 0;
            
            // Esperar tiempo de debounce
            if (tiempo_en_estado >= TIEMPO_DEBOUNCE_MS) {
                if (boton_actual == 1) {
                    // Confirmar pulsación
                    estado_boton = BTN_PRESIONADO;
                } else {
                    // Fue un glitch, volver a liberado
                    estado_boton = BTN_LIBERADO;
                }
                tiempo_en_estado = 0;
            }
            break;
            
        case BTN_PRESIONADO:
            // Salida del estado
            boton_estable = 1;
            
            // Transición: detectar liberación
            if (boton_actual == 0) {
                estado_boton = BTN_DEBOUNCE_RELEASE;
                tiempo_en_estado = 0;
            }
            break;
            
        case BTN_DEBOUNCE_RELEASE:
            // Salida del estado (mantener anterior)
            boton_estable = 1;
            
            // Esperar tiempo de debounce
            if (tiempo_en_estado >= TIEMPO_DEBOUNCE_MS) {
                if (boton_actual == 0) {
                    // Confirmar liberación
                    estado_boton = BTN_LIBERADO;
                } else {
                    // Fue un glitch, volver a presionado
                    estado_boton = BTN_PRESIONADO;
                }
                tiempo_en_estado = 0;
            }
            break;
            
        default:
            // Estado inválido - recuperación
            estado_boton = BTN_LIBERADO;
            tiempo_en_estado = 0;
            boton_estable = 0;
            break;
    }
    
    tiempo_en_estado++;
}

/**
 * @brief Inicializar hardware para botón
 */
void init_boton(void) {
    // Habilitar clock GPIOC
    RCC->AHB1ENR |= (1 << 2);
    
    // Configurar PC13 como entrada
    GPIOC->MODER &= ~(0x3 << (PIN_BOTON * 2));  // 00 = input
    
    // Configurar pull-up
    GPIOC->PUPDR &= ~(0x3 << (PIN_BOTON * 2));
    GPIOC->PUPDR |= (0x1 << (PIN_BOTON * 2));   // 01 = pull-up
}
```

### 4. Alternativa: Tabla de Transiciones (20 minutos)

#### Concepto

En lugar de `switch-case`, usar tabla que define transiciones.

**Ventajas**:
- Más escalable para MEF grandes
- Fácil de modificar sin tocar código
- Puede cargarse de memoria externa

**Desventajas**:
- Más compleja de implementar inicialmente
- Uso de memoria para tabla

#### Estructura de Tabla

```c
// Estructura para transición
typedef struct {
    Estado_t estado_origen;
    uint8_t (*condicion)(void);  // Puntero a función que evalúa condición
    Estado_t estado_destino;
    void (*accion)(void);        // Puntero a función de acción
} Transicion_t;

// Funciones de condición
uint8_t condicion_boton_down(void) {
    return leer_boton_fisico() == 1;
}

uint8_t condicion_boton_up_y_timeout(void) {
    return (leer_boton_fisico() == 0) && (tiempo_en_estado >= 30);
}

// Funciones de acción
void accion_nada(void) {
    // No hacer nada
}

// Tabla de transiciones
Transicion_t tabla_transiciones[] = {
    {BTN_LIBERADO, condicion_boton_down, BTN_DEBOUNCE_PRESS, accion_nada},
    {BTN_DEBOUNCE_PRESS, condicion_boton_up_y_timeout, BTN_LIBERADO, accion_nada},
    // ... más transiciones
};

// Procesar MEF usando tabla
void procesar_mef_tabla(void) {
    for (int i = 0; i < NUM_TRANSICIONES; i++) {
        if (tabla_transiciones[i].estado_origen == estado_actual) {
            if (tabla_transiciones[i].condicion()) {
                tabla_transiciones[i].accion();
                estado_actual = tabla_transiciones[i].estado_destino;
                tiempo_en_estado = 0;
                break;
            }
        }
    }
}
```

**Nota**: Para esta unidad, usaremos `switch-case` por simplicidad. Tabla de transiciones es tema avanzado opcional.

---

## Actividades

### Actividad 1: Implementar MEF Simple (30 minutos)

**Tarea**: Implementar MEF de LED parpadeante con frecuencias variables

**Requisitos**:
- 3 estados: LENTO (1Hz), MEDIO (2Hz), RAPIDO (5Hz)
- Cambiar entre estados con botón
- Usar MEF de debounce para el botón

**Plantilla proporcionada**:
```c
typedef enum {
    PARPADEO_LENTO,
    PARPADEO_MEDIO,
    PARPADEO_RAPIDO
} ModoParpadeo_t;

void mef_parpadeo_variable(void) {
    // Implementar aquí
}
```

### Actividad 2: MEF con Múltiples Salidas (30 minutos)

**Sistema**: Secuencia de LEDs

**Requisitos**:
- 4 LEDs en PA5, PA6, PA7, PA8
- Secuencia: LED1 → LED2 → LED3 → LED4 → LED1
- Cada LED permanece encendido 500ms
- Botón para pausar/reanudar secuencia

**Estados sugeridos**:
- ESTADO_LED1
- ESTADO_LED2
- ESTADO_LED3
- ESTADO_LED4
- ESTADO_PAUSADO

### Actividad 3: Debugging de MEF (20 minutos)

**Código con errores**:
```c
typedef enum {ESPERA, ACTIVO} Estado_t;
Estado_t estado = ESPERA;
uint32_t tiempo = 0;

void mef_buggy(void) {
    switch (estado) {
        case ESPERA:
            if (boton_presionado()) {
                estado = ACTIVO;
                // BUG 1: Falta resetear tiempo
            }
            break;
        case ACTIVO:
            LED_ON();
            if (tiempo > 1000) {
                estado = ESPERA;
                // BUG 2: Falta apagar LED
            }
            break;
        // BUG 3: Falta default case
    }
    tiempo++;  // BUG 4: Se incrementa siempre
}
```

**Tarea**: Identificar y corregir los 4 bugs

---

## Evaluación Formativa

### Quiz - Sesión 3

1. **¿Por qué usar `enum` para estados en lugar de enteros?** (2 puntos)
   - Respuesta: Código más legible, type-safe, mantenible, menos errores.

2. **¿Qué debe hacerse al cambiar de estado en una MEF?** (2 puntos)
   - Respuesta: Resetear contador de tiempo, ejecutar acción de salida del estado anterior, ejecutar acción de entrada del nuevo estado.

3. **¿Para qué sirve el tiempo de debounce de 30ms?** (2 puntos)
   - Respuesta: Filtrar rebotes mecánicos del botón, esperar a que la señal se estabilice.

4. **¿Qué hace el `default` case en el `switch` de una MEF?** (2 puntos)
   - Respuesta: Maneja estados inválidos, permite recuperación de errores.

5. **Completa el código para cambiar de estado** (2 puntos)
   ```c
   if (condicion) {
       estado_actual = NUEVO_ESTADO;
       tiempo_en_estado = ____;  // ¿Qué va aquí?
   }
   ```
   - Respuesta: `0` (resetear a cero)

---

## Evidencias de Aprendizaje

**Entregable**: Código funcional de MEF para control de semáforo simple

**Especificaciones**:
1. 3 estados: VERDE, AMARILLO, ROJO
2. Tiempos:
   - Verde: 5 segundos
   - Amarillo: 2 segundos
   - Rojo: 5 segundos
3. 3 LEDs (PA5=Verde, PA6=Amarillo, PA7=Rojo)
4. Ciclo continuo
5. Botón (PC13) para pausar/reanudar en cualquier momento
6. Usar MEF de debounce para el botón

**Estructura del código**:
```c
// Estados del semáforo
typedef enum {
    SEMAFORO_VERDE,
    SEMAFORO_AMARILLO,
    SEMAFORO_ROJO,
    SEMAFORO_PAUSADO
} EstadoSemaforo_t;

void mef_semaforo(void);
void init_semaforo(void);
```

**Archivos a entregar**:
1. `semaforo.c` - Implementación completa
2. `semaforo.h` - Header file
3. `main.c` - Programa principal que llama la MEF
4. `README.txt` - Instrucciones de compilación y uso

**Evaluación**:
- Funcionalidad correcta: 40%
- Código estructurado con enum y switch: 20%
- Manejo correcto de tiempo: 15%
- Debounce implementado: 15%
- Comentarios y claridad: 10%

---

## Material para Casa

### Lectura Previa a Sesión 4
- Repaso de funciones en C
- Concepto de API (Application Programming Interface)
- Archivos .h vs .c (header vs implementation)

### Ejercicio Opcional
Extender el semáforo para:
- Modo peatonal (botón adicional activa secuencia especial)
- Peatonal: ROJO → VERDE peatonal (10s) → AMARILLO peatonal (3s) → ROJO → secuencia normal

### Video Recomendado
- "State Machine Design Patterns in C"

---

## Notas para el Instructor

### Preparación
- Código de ejemplo compilado y probado en hardware
- Proyector para mostrar código en vivo
- Hardware: tarjeta STM32, LEDs, botones, cables

### Demos Importantes
1. Mostrar MEF de debounce en funcionamiento con LED indicador
2. Comparar comportamiento con y sin debounce (glitches visibles)
3. Live coding de MEF simple con explicación línea por línea

### Enfoque Pedagógico
- Comenzar con ejemplo simple (2 estados)
- Incrementar complejidad gradualmente
- Enfatizar importancia de resetear tiempo al cambiar estado
- Mostrar debugging con LEDs (cada estado enciende LED diferente)

### Errores Comunes
- Olvidar resetear `tiempo_en_estado`
- No incluir `default` case
- Incrementar tiempo fuera del switch
- Confundir estado con salida
- No usar `static` para variables de la MEF

### Debugging Tips
- Usar LEDs para indicar estado actual
- Printf via UART para tracing (si configurado)
- Agregar contador de transiciones para debugging

---

**Próxima sesión**: Driver GPIO Completo en C  
**Preparación**: Revisar código de semáforo, estará listo para modularizar
