# Sesión 8: Proyecto Integrador y Evaluación Sumativa

**Duración**: 2 horas  
**Modalidad**: Presencial con evaluación práctica y teórica

---

## Objetivos Específicos

1. **Integrar** todos los conceptos de la Unidad 2 en un proyecto funcional
2. **Demostrar** competencias en MEF, drivers, timers e interrupciones
3. **Evaluar** conocimientos teóricos y prácticos adquiridos

---

## Proyecto Final: Semáforo Inteligente con Botón Peatonal

### Especificaciones del Proyecto

#### Requisitos Funcionales

**Estados del semáforo**:
1. **VERDE_VEHICULAR**: 
   - LED verde vehicular ON
   - Duración: 30 segundos
   - Peatones: LED rojo

2. **AMARILLO_VEHICULAR**:
   - LED amarillo vehicular ON
   - Duración: 3 segundos
   - Peatones: LED rojo

3. **ROJO_VEHICULAR**:
   - LED rojo vehicular ON
   - Duración: 25 segundos
   - Peatones: LED rojo

4. **VERDE_PEATONAL** (activado por botón):
   - LED rojo vehicular ON
   - LED verde peatonal ON
   - Duración: 15 segundos
   - Sonido intermitente (buzzer opcional)

5. **AMARILLO_PEATONAL**:
   - LED rojo vehicular ON
   - LED verde peatonal parpadea (1Hz)
   - Duración: 5 segundos
   - Advertencia de finalización

**Botón peatonal**:
- Presionar durante VERDE_VEHICULAR: activa modo peatonal después del ciclo actual
- Presionar en otros estados: sin efecto (o indicar con LED que solicitud fue registrada)
- Debounce: 30ms
- Usar interrupción EXTI

**Display**:
- Mostrar tiempo restante en estado actual
- 2 dígitos 7 segmentos
- Multiplexación sin flicker

#### Requisitos Técnicos

**Hardware necesario**:
- LEDs: Verde vehicular (PA5), Amarillo vehicular (PA6), Rojo vehicular (PA7)
- LEDs: Verde peatonal (PB0), Rojo peatonal (PB1)
- Botón peatonal (PC13) con pull-up
- Display 7 segmentos: 2 dígitos (PA0-PA7 segmentos, PB2-PB3 selección)
- Opcional: Buzzer (PB4)

**Software requerido**:
- MEF principal para estados del semáforo
- MEF secundaria para multiplexación de display
- Driver GPIO (de Sesión 4)
- Driver Timer (de Sesión 5)
- Configuración EXTI para botón (de Sesión 7)
- Base de tiempo precisa (1ms tick)

### Diagrama de Estados Sugerido

```
                    timeout_30s
    ┌────────────────────────────────────┐
    │                                    ↓
┌──────────────┐  timeout_3s    ┌─────────────┐
│VERDE_VEHIC   │───────────────>│AMARILLO_VEH │
└──────────────┘                └─────────────┘
    ↑                                   │
    │                                   │ timeout_3s
    │                                   ↓
    │                            ┌─────────────┐
    │                            │ROJO_VEHIC   │
    │                            └─────────────┘
    │                                   │
    │    solicitud_peatonal             │ timeout_25s
    │    (si hubo solicitud)            │ (sin solicitud)
    │    ↓                               │
    │   ┌──────────────┐                │
    └───│VERDE_PEAT    │<───────────────┘
        └──────────────┘
             │ timeout_15s
             ↓
        ┌──────────────┐
        │AMARILLO_PEAT │
        └──────────────┘
             │ timeout_5s
             └──> VERDE_VEHIC
```

### Estructura de Código Sugerida

```c
/**
 * @file proyecto_semaforo.c
 * @brief Semáforo inteligente con botón peatonal
 */

#include <stdint.h>
#include "gpio_driver.h"
#include "timer_driver.h"

// Estados del semáforo
typedef enum {
    ESTADO_VERDE_VEHICULAR,
    ESTADO_AMARILLO_VEHICULAR,
    ESTADO_ROJO_VEHICULAR,
    ESTADO_VERDE_PEATONAL,
    ESTADO_AMARILLO_PEATONAL
} EstadoSemaforo_t;

// Variables globales
static EstadoSemaforo_t estado_actual = ESTADO_VERDE_VEHICULAR;
static uint32_t tiempo_en_estado = 0;
static volatile uint8_t solicitud_peatonal = 0;
static uint8_t digitos[2] = {0, 0};

// Tiempos en ms
#define TIEMPO_VERDE_VEHICULAR     30000
#define TIEMPO_AMARILLO_VEHICULAR   3000
#define TIEMPO_ROJO_VEHICULAR      25000
#define TIEMPO_VERDE_PEATONAL      15000
#define TIEMPO_AMARILLO_PEATONAL    5000

/**
 * @brief Handler de interrupción del botón peatonal
 */
void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & (1 << 13)) {
        // Registrar solicitud
        solicitud_peatonal = 1;
        
        // Opcional: LED indicador de solicitud registrada
        GPIO_Write(GPIOB, 5, 1);
        
        EXTI->PR = (1 << 13);
    }
}

/**
 * @brief Actualizar LEDs según estado
 */
void actualizar_leds(void) {
    // Apagar todos primero
    GPIO_Write(GPIOA, 5, 0);  // Verde vehicular
    GPIO_Write(GPIOA, 6, 0);  // Amarillo vehicular
    GPIO_Write(GPIOA, 7, 0);  // Rojo vehicular
    GPIO_Write(GPIOB, 0, 0);  // Verde peatonal
    GPIO_Write(GPIOB, 1, 0);  // Rojo peatonal
    
    switch (estado_actual) {
        case ESTADO_VERDE_VEHICULAR:
            GPIO_Write(GPIOA, 5, 1);  // Verde vehicular
            GPIO_Write(GPIOB, 1, 1);  // Rojo peatonal
            break;
            
        case ESTADO_AMARILLO_VEHICULAR:
            GPIO_Write(GPIOA, 6, 1);  // Amarillo vehicular
            GPIO_Write(GPIOB, 1, 1);  // Rojo peatonal
            break;
            
        case ESTADO_ROJO_VEHICULAR:
        case ESTADO_VERDE_PEATONAL:
            GPIO_Write(GPIOA, 7, 1);  // Rojo vehicular
            if (estado_actual == ESTADO_VERDE_PEATONAL) {
                GPIO_Write(GPIOB, 0, 1);  // Verde peatonal
            } else {
                GPIO_Write(GPIOB, 1, 1);  // Rojo peatonal
            }
            break;
            
        case ESTADO_AMARILLO_PEATONAL:
            GPIO_Write(GPIOA, 7, 1);  // Rojo vehicular
            // Verde peatonal parpadea (manejado por tiempo)
            if ((tiempo_en_estado / 500) % 2) {
                GPIO_Write(GPIOB, 0, 1);
            }
            break;
    }
}

/**
 * @brief Actualizar display con tiempo restante
 */
void actualizar_display(void) {
    uint32_t tiempo_restante_s;
    
    switch (estado_actual) {
        case ESTADO_VERDE_VEHICULAR:
            tiempo_restante_s = (TIEMPO_VERDE_VEHICULAR - tiempo_en_estado) / 1000;
            break;
        case ESTADO_AMARILLO_VEHICULAR:
            tiempo_restante_s = (TIEMPO_AMARILLO_VEHICULAR - tiempo_en_estado) / 1000;
            break;
        case ESTADO_ROJO_VEHICULAR:
            tiempo_restante_s = (TIEMPO_ROJO_VEHICULAR - tiempo_en_estado) / 1000;
            break;
        case ESTADO_VERDE_PEATONAL:
            tiempo_restante_s = (TIEMPO_VERDE_PEATONAL - tiempo_en_estado) / 1000;
            break;
        case ESTADO_AMARILLO_PEATONAL:
            tiempo_restante_s = (TIEMPO_AMARILLO_PEATONAL - tiempo_en_estado) / 1000;
            break;
        default:
            tiempo_restante_s = 0;
    }
    
    digitos[0] = tiempo_restante_s / 10;
    digitos[1] = tiempo_restante_s % 10;
}

/**
 * @brief MEF principal del semáforo (llamar cada 1ms)
 */
void mef_semaforo(void) {
    actualizar_leds();
    actualizar_display();
    
    switch (estado_actual) {
        case ESTADO_VERDE_VEHICULAR:
            if (tiempo_en_estado >= TIEMPO_VERDE_VEHICULAR) {
                estado_actual = ESTADO_AMARILLO_VEHICULAR;
                tiempo_en_estado = 0;
            }
            break;
            
        case ESTADO_AMARILLO_VEHICULAR:
            if (tiempo_en_estado >= TIEMPO_AMARILLO_VEHICULAR) {
                estado_actual = ESTADO_ROJO_VEHICULAR;
                tiempo_en_estado = 0;
            }
            break;
            
        case ESTADO_ROJO_VEHICULAR:
            if (tiempo_en_estado >= TIEMPO_ROJO_VEHICULAR) {
                // Verificar si hay solicitud peatonal
                if (solicitud_peatonal) {
                    estado_actual = ESTADO_VERDE_PEATONAL;
                    solicitud_peatonal = 0;
                    GPIO_Write(GPIOB, 5, 0);  // Apagar LED solicitud
                } else {
                    estado_actual = ESTADO_VERDE_VEHICULAR;
                }
                tiempo_en_estado = 0;
            }
            break;
            
        case ESTADO_VERDE_PEATONAL:
            if (tiempo_en_estado >= TIEMPO_VERDE_PEATONAL) {
                estado_actual = ESTADO_AMARILLO_PEATONAL;
                tiempo_en_estado = 0;
            }
            break;
            
        case ESTADO_AMARILLO_PEATONAL:
            if (tiempo_en_estado >= TIEMPO_AMARILLO_PEATONAL) {
                estado_actual = ESTADO_VERDE_VEHICULAR;
                tiempo_en_estado = 0;
            }
            break;
            
        default:
            estado_actual = ESTADO_VERDE_VEHICULAR;
            tiempo_en_estado = 0;
            break;
    }
    
    tiempo_en_estado++;
}

// ... código de multiplexación e inicialización ...
```

---

## Evaluación Sumativa

### Parte 1: Evaluación Teórica (30 minutos, 30%)

**Sección A: Opción Múltiple (10 preguntas, 10 puntos)**

1. En una MEF de Moore, las salidas dependen de:
   a) Solo el estado actual ✓
   b) El estado y las entradas
   c) Solo las entradas
   d) El estado anterior

2. ¿Qué hace el registro BSRR de GPIO?
   a) Lee el estado del pin
   b) Set/Reset atómico ✓
   c) Configura el modo
   d) Selecciona velocidad

3. Con PSC=799 y CLK=16MHz, la frecuencia del timer es:
   a) 16 kHz
   b) 20 kHz ✓
   c) 800 Hz
   d) 16 MHz

4. El refresh rate mínimo para 4 displays sin flicker es:
   a) 50 Hz
   b) 100 Hz
   c) 200 Hz ✓
   d) 400 Hz

5. ¿Qué hace EXTI->PR en el handler de interrupción?
   a) Deshabilita la interrupción
   b) Limpia el flag de pending ✓
   c) Configura la prioridad
   d) Habilita el pin

6-10. (Más preguntas similares)

**Sección B: Preguntas Cortas (5 preguntas, 20 puntos)**

1. Explica la diferencia entre MEF de Moore y Mealy (4 puntos)
2. ¿Por qué es importante usar `volatile` para registros de hardware? (4 puntos)
3. Calcula PSC y ARR para generar tick de 10ms con CLK=16MHz (4 puntos)
4. Describe el proceso de debounce de botón con MEF (4 puntos)
5. ¿Cómo se comunica un evento de ISR a main loop? (4 puntos)

### Parte 2: Evaluación Práctica (90 minutos, 70%)

**Implementar el proyecto del semáforo según especificaciones**

**Rúbrica de evaluación**:

| Criterio | Excelente (100%) | Bueno (80%) | Aceptable (60%) | Insuficiente (0-40%) |
|----------|------------------|-------------|-----------------|----------------------|
| **Funcionalidad** (35%) | Todos los requisitos funcionan correctamente | Falta 1 requisito menor | Faltan 2-3 requisitos | No funciona o faltan >3 requisitos |
| **MEF bien diseñada** (15%) | Estados claros, transiciones correctas, código limpio | Pequeños problemas de diseño | Diseño confuso pero funcional | MEF mal implementada |
| **Drivers utilizados** (10%) | Usa drivers modulares correctamente | Usa drivers con pequeños errores | Mezcla drivers con código directo | No usa drivers |
| **Timer configurado** (10%) | Timing preciso, cálculos correctos | Timing con pequeños errores | Timing impreciso | Sin timer o mal configurado |
| **Interrupciones** (10%) | EXTI correcta, debounce, limpieza de flags | Funciona pero falta algo | Problemas intermitentes | No usa interrupciones |
| **Multiplexación** (10%) | Sin flicker, frecuencia adecuada | Flicker leve | Flicker notable | No multiplexado o no funciona |
| **Código limpio** (10%) | Comentarios, modular, nombres claros | Falta documentación | Código desorganizado | Código muy difícil de leer |

**Entregables**:
1. Código fuente completo (.c y .h)
2. README con instrucciones
3. Demostración en vivo (5 minutos)
4. Respuestas a preguntas del instructor sobre el código

---

## Cierre de Unidad 2

### Competencias Alcanzadas

Al completar esta unidad, los estudiantes han logrado:

✅ **Transición Assembly → C**: Comprender relación entre ambos lenguajes
✅ **MEF**: Diseñar e implementar máquinas de estados finitos
✅ **Drivers**: Crear código modular y reutilizable
✅ **Timers**: Configurar timing preciso
✅ **Multiplexación**: Optimizar uso de pines GPIO
✅ **Interrupciones**: Sistemas reactivos eficientes
✅ **Integración**: Combinar múltiples periféricos en proyecto completo

### Proyección a Unidad 3 (Sugerencia)

**Temas recomendados para siguiente unidad**:
- Comunicación serial (UART) para debugging e interfaz con PC
- Conversión analógica-digital (ADC) para sensores
- PWM para control de actuadores
- Protocolos I2C y SPI
- DMA para transferencias eficientes
- Introducción a RTOS (FreeRTOS)

---

## Retroalimentación y Mejora Continua

**Para estudiantes**:
- Completar encuesta de satisfacción del curso
- Sugerencias de mejora para próxima versión
- Temas que les gustaría profundizar

**Para instructor**:
- Documentar ajustes necesarios
- Recopilar problemas comunes encontrados
- Actualizar material según feedback

---

**¡Felicitaciones por completar la Unidad 2!** 🎉

El conocimiento adquirido es base sólida para desarrollo profesional en sistemas embebidos.
