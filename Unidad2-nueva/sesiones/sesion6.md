# Sesión 6: Multiplexación y Aplicaciones

**Duración**: 2 horas  
**Modalidad**: Presencial con práctica de multiplexación

---

## Objetivos Específicos

1. **Comprender** el concepto de multiplexación y su aplicación en sistemas embebidos
2. **Implementar** multiplexación de displays 7 segmentos usando MEF
3. **Calcular** frecuencias de refresco adecuadas para evitar flicker

---

## Contenidos Temáticos

### 1. Concepto de Multiplexación (20 minutos)

#### ¿Qué es Multiplexación?

**Definición**: Técnica que permite compartir un conjunto de recursos (pines GPIO) entre múltiples dispositivos, activándolos secuencialmente.

**Problema sin multiplexación**:
```
4 displays 7 segmentos = 4 × 8 pines = 32 pines GPIO ❌
```

**Solución con multiplexación**:
```
4 displays 7 segmentos = 8 (segmentos) + 4 (selección) = 12 pines ✓
Ahorro: 20 pines (62.5%)
```

**Principio**: 
- Mostrar datos en display 1 brevemente
- Apagar display 1, mostrar datos en display 2
- Repetir rápidamente
- El ojo humano percibe todos encendidos simultáneamente

#### Persistencia Visual

**Ojo humano**: ~50ms de persistencia
**Frecuencia mínima**: 20 Hz (cada display)
**Frecuencia recomendada**: 60-100 Hz
**Con 4 displays**: refrescar cada uno a 60Hz = ciclo completo a 240Hz (4.16ms)

### 2. Multiplexación de Displays 7 Segmentos (35 minutos)

#### Hardware

**Configuración típica**:
```
         PA0-PA7: Segmentos (a, b, c, d, e, f, g, dp)
         PB0-PB3: Selección de display (D1, D2, D3, D4)
```

**Displays cátodo común**:
- Segmentos: 1=ON, 0=OFF
- Selección: 0=activo (GND), 1=inactivo

#### Implementación con MEF

```c
#include <stdint.h>
#include "gpio_driver.h"

// Estados de multiplexación
typedef enum {
    DISPLAY_1,
    DISPLAY_2,
    DISPLAY_3,
    DISPLAY_4
} EstadoDisplay_t;

// Variables globales
EstadoDisplay_t display_actual = DISPLAY_1;
uint8_t digitos[4] = {1, 2, 3, 4};  // Valores a mostrar

// Tabla de 7 segmentos (0-9)
const uint8_t tabla_7seg[10] = {
    0x3F,  // 0
    0x06,  // 1
    0x5B,  // 2
    0x4F,  // 3
    0x66,  // 4
    0x6D,  // 5
    0x7D,  // 6
    0x07,  // 7
    0x7F,  // 8
    0x6F   // 9
};

/**
 * @brief Escribir patrón a segmentos
 */
void escribir_segmentos(uint8_t patron) {
    // PA0-PA7
    for (int i = 0; i < 8; i++) {
        GPIO_Write(GPIOA, i, (patron >> i) & 1);
    }
}

/**
 * @brief Seleccionar display activo
 */
void seleccionar_display(uint8_t display) {
    // Desactivar todos (PB0-PB3 = 1)
    for (int i = 0; i < 4; i++) {
        GPIO_Write(GPIOB, i, 1);
    }
    
    // Activar el seleccionado (0)
    GPIO_Write(GPIOB, display, 0);
}

/**
 * @brief MEF de multiplexación (llamar cada 1ms)
 */
void mef_multiplexacion(void) {
    uint8_t digito;
    
    switch (display_actual) {
        case DISPLAY_1:
            digito = digitos[0];
            seleccionar_display(0);
            escribir_segmentos(tabla_7seg[digito]);
            display_actual = DISPLAY_2;
            break;
            
        case DISPLAY_2:
            digito = digitos[1];
            seleccionar_display(1);
            escribir_segmentos(tabla_7seg[digito]);
            display_actual = DISPLAY_3;
            break;
            
        case DISPLAY_3:
            digito = digitos[2];
            seleccionar_display(2);
            escribir_segmentos(tabla_7seg[digito]);
            display_actual = DISPLAY_4;
            break;
            
        case DISPLAY_4:
            digito = digitos[3];
            seleccionar_display(3);
            escribir_segmentos(tabla_7seg[digito]);
            display_actual = DISPLAY_1;
            break;
    }
}

/**
 * @brief Actualizar valor mostrado
 */
void actualizar_displays(uint16_t valor) {
    digitos[0] = (valor / 1000) % 10;  // Miles
    digitos[1] = (valor / 100) % 10;   // Centenas
    digitos[2] = (valor / 10) % 10;    // Decenas
    digitos[3] = valor % 10;           // Unidades
}
```

### 3. Teclado Matricial (25 minutos)

#### Configuración 4×4

**Concepto**: 16 teclas con solo 8 pines (4 filas + 4 columnas)

**Escaneo**:
1. Activar fila 1 (LOW)
2. Leer columnas (si LOW = tecla presionada)
3. Desactivar fila 1
4. Repetir con filas 2, 3, 4

```c
typedef enum {
    ESCANEO_FILA_0,
    ESCANEO_FILA_1,
    ESCANEO_FILA_2,
    ESCANEO_FILA_3
} EstadoTeclado_t;

// Matriz de teclas
const char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

/**
 * @brief Escanear teclado matricial
 * @return Tecla presionada o 0 si ninguna
 */
char escanear_teclado(void) {
    static EstadoTeclado_t fila_actual = ESCANEO_FILA_0;
    char tecla = 0;
    
    // Activar fila actual (PA0-PA3)
    for (int i = 0; i < 4; i++) {
        GPIO_Write(GPIOA, i, (i == fila_actual) ? 0 : 1);
    }
    
    // Pequeña pausa para estabilización
    for (volatile int i = 0; i < 100; i++);
    
    // Leer columnas (PB0-PB3)
    for (int col = 0; col < 4; col++) {
        if (GPIO_Read(GPIOB, col) == 0) {
            // Tecla presionada
            tecla = teclas[fila_actual][col];
            break;
        }
    }
    
    // Siguiente fila
    fila_actual = (fila_actual + 1) % 4;
    
    return tecla;
}
```

### 4. Cálculos de Frecuencia (15 minutos)

#### Frecuencia de Refresco

**Para evitar flicker visible**:
```
N displays: cada uno debe refrescarse a mínimo 50 Hz
Ciclo completo: N × 50 Hz
Tiempo por display: 1 / (N × 50) segundos

Ejemplo 4 displays:
Tiempo por display = 1 / (4 × 50) = 5 ms
```

**En código**:
```c
// Timer interrumpiendo cada 5ms
Timer_Init(799, 99);  // 16MHz / 800 / 100 = 200Hz
// 200Hz / 4 displays = 50Hz por display
```

---

## Actividades

### Actividad 1: Display de 2 Dígitos (30 minutos)

**Implementar**: Contador 00-99 en 2 displays 7 segmentos

**Requisitos**:
- Incrementar cada segundo
- Multiplexar correctamente
- Sin flicker visible

### Actividad 2: Teclado + Display (40 minutos)

**Sistema**: Calculadora simple
- Teclado 4×4
- Display 4 dígitos
- Mostrar dígitos ingresados

### Actividad 3: Optimización (20 minutos)

**Tarea**: Medir consumo de corriente
- Comparar: todos displays encendidos permanentemente vs multiplexados
- Calcular duty cycle y consumo promedio

---

## Evaluación Formativa

### Quiz - Sesión 6

1. **¿Por qué se usa multiplexación?** (2 puntos)
2. **¿Qué es el refresh rate mínimo para evitar flicker?** (2 puntos)
3. **Con 8 displays, ¿qué frecuencia total se necesita para 60Hz cada uno?** (2 puntos)
4. **¿Cómo se detecta tecla en teclado matricial?** (2 puntos)
5. **¿Qué problemas puede causar refresh rate muy bajo?** (2 puntos)

---

## Evidencias de Aprendizaje

**Entregable**: Reloj digital 00:00 - 23:59

**Especificaciones**:
- 4 displays 7 segmentos (HH:MM)
- Incremento automático cada minuto
- Botones para ajustar hora y minuto
- Sin flicker visible
- Usar MEF para multiplexación

**Evaluación**:
- Funcionalidad: 40%
- Multiplexación correcta: 30%
- Código estructurado: 20%
- Documentación: 10%

---

## Material para Casa

### Lectura Sesión 7
- Interrupciones externas (EXTI)
- SYSCFG para mapeo de pines
- Prioridades de interrupciones

**Próxima sesión**: Interrupciones EXTI y NVIC
