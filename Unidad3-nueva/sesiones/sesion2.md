# Sesión 2: USART con Interrupciones y Printf Redirect

**Unidad 3**: Protocolos de Comunicación Serial  
**Semana**: 1  
**Duración**: 2 horas  
**Modalidad**: Presencial con práctica en laboratorio

---

## Objetivos de Aprendizaje

Al finalizar esta sesión, el estudiante será capaz de:

1. Configurar el módulo USART para operar con interrupciones (TX y RX)
2. Implementar buffers circulares para gestión eficiente de datos seriales
3. Redirigir la función `printf()` estándar de C hacia el puerto USART
4. Gestionar correctamente las interrupciones de USART con prioridades NVIC
5. Depurar aplicaciones usando mensajes de log por puerto serial

**Tiempo estimado de actividades**: 90 minutos (teoría: 45 min, práctica: 45 min)

---

## 1. Introducción

En la sesión anterior se implementó comunicación USART básica usando polling, lo cual es ineficiente para aplicaciones complejas donde el MCU debe realizar múltiples tareas. Las interrupciones permiten que la comunicación serial ocurra en segundo plano mientras el programa principal ejecuta otras funciones.

### ¿Por qué usar interrupciones con USART?

- **Eficiencia**: El CPU no espera activamente la transmisión/recepción
- **Multi-tarea**: Permite realizar otras operaciones mientras se comunica
- **No se pierden datos**: RX interrupt captura datos apenas llegan
- **Escalabilidad**: Facilita implementar protocolos complejos

---

## 2. Interrupciones de USART en STM32F407

### 2.1 Tipos de Interrupciones USART

El módulo USART del STM32F407 puede generar diversas interrupciones:

| Interrupción | Flag | Descripción |
|--------------|------|-------------|
| RXNE | USART_SR_RXNE | RX buffer no vacío (dato recibido) |
| TXE | USART_SR_TXE | TX buffer vacío (listo para enviar) |
| TC | USART_SR_TC | Transmisión completa |
| IDLE | USART_SR_IDLE | Línea RX inactiva (frame completo) |
| ORE | USART_SR_ORE | Overrun error (dato perdido) |
| PE | USART_SR_PE | Parity error |

### 2.2 Configuración de Interrupciones

```c
/**
 * @brief Habilita interrupciones RXNE y TXE en USART2
 */
void USART2_EnableInterrupts(void) {
    // Habilitar interrupción RXNE (recepción)
    USART2->CR1 |= USART_CR1_RXNEIE;
    
    // NO habilitar TXE aquí (se habilitará cuando haya datos a enviar)
    // USART2->CR1 |= USART_CR1_TXEIE;  // Solo cuando buffer TX tenga datos
    
    // Configurar prioridad en NVIC (IRQ 38 para USART2)
    NVIC_SetPriority(USART2_IRQn, 5);  // Prioridad media
    NVIC_EnableIRQ(USART2_IRQn);
}
```

**Nota importante**: La interrupción TXE debe habilitarse **solo cuando hay datos a transmitir**, de lo contrario se disparará continuamente (buffer siempre vacío).

---

## 3. Buffer Circular (Ring Buffer)

Un buffer circular es fundamental para gestión eficiente de datos seriales. Permite almacenar temporalmente datos entrantes/salientes sin bloquear el programa.

### 3.1 Estructura del Buffer Circular

```c
/**
 * @brief Estructura de buffer circular
 */
typedef struct {
    uint8_t buffer[256];    // Array de datos (tamaño potencia de 2 para optimización)
    volatile uint16_t head; // Índice de escritura (volátil porque se modifica en ISR)
    volatile uint16_t tail; // Índice de lectura
    uint16_t size;          // Tamaño del buffer (256 en este caso)
} RingBuffer_t;

// Buffers globales para TX y RX
RingBuffer_t usart2_tx_buffer;
RingBuffer_t usart2_rx_buffer;
```

### 3.2 Operaciones Básicas del Buffer

```c
/**
 * @brief Inicializa un buffer circular
 */
void RingBuffer_Init(RingBuffer_t *rb, uint16_t size) {
    rb->head = 0;
    rb->tail = 0;
    rb->size = size;
}

/**
 * @brief Escribe un byte en el buffer circular
 * @return 0 si éxito, -1 si buffer lleno
 */
int RingBuffer_Write(RingBuffer_t *rb, uint8_t data) {
    uint16_t next_head = (rb->head + 1) % rb->size;
    
    if (next_head == rb->tail) {
        return -1;  // Buffer lleno
    }
    
    rb->buffer[rb->head] = data;
    rb->head = next_head;
    return 0;
}

/**
 * @brief Lee un byte del buffer circular
 * @return Byte leído, o -1 si buffer vacío
 */
int RingBuffer_Read(RingBuffer_t *rb) {
    if (rb->head == rb->tail) {
        return -1;  // Buffer vacío
    }
    
    uint8_t data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->size;
    return data;
}

/**
 * @brief Verifica si el buffer está vacío
 */
int RingBuffer_IsEmpty(RingBuffer_t *rb) {
    return (rb->head == rb->tail);
}
```

---

## 4. Manejador de Interrupción USART

### 4.1 ISR Completa

```c
/**
 * @brief Handler de interrupción USART2
 * 
 * Se ejecuta cuando:
 * - Se recibe un byte (RXNE flag)
 * - Buffer TX está vacío y listo para enviar (TXE flag)
 */
void USART2_IRQHandler(void) {
    uint32_t sr = USART2->SR;  // Leer status register
    
    // Interrupción de recepción (RXNE)
    if (sr & USART_SR_RXNE) {
        uint8_t data = USART2->DR;  // Leer dato (limpia flag RXNE automáticamente)
        RingBuffer_Write(&usart2_rx_buffer, data);  // Guardar en buffer
    }
    
    // Interrupción de transmisión (TXE)
    if ((sr & USART_SR_TXE) && (USART2->CR1 & USART_CR1_TXEIE)) {
        if (!RingBuffer_IsEmpty(&usart2_tx_buffer)) {
            int data = RingBuffer_Read(&usart2_tx_buffer);
            if (data >= 0) {
                USART2->DR = (uint8_t)data;  // Enviar dato
            }
        } else {
            // No hay más datos, deshabilitar interrupción TXE
            USART2->CR1 &= ~USART_CR1_TXEIE;
        }
    }
    
    // Manejar errores (opcional pero recomendado)
    if (sr & USART_SR_ORE) {
        // Overrun error: leer DR para limpiar flag
        (void)USART2->DR;
    }
}
```

### 4.2 Funciones de Usuario

```c
/**
 * @brief Envía un byte por USART2 usando interrupciones
 */
void USART2_SendByte_IT(uint8_t data) {
    while (RingBuffer_Write(&usart2_tx_buffer, data) != 0) {
        // Esperar si buffer está lleno
    }
    
    // Habilitar interrupción TXE para iniciar transmisión
    USART2->CR1 |= USART_CR1_TXEIE;
}

/**
 * @brief Lee un byte del buffer RX (no bloqueante)
 * @return Byte leído o -1 si no hay datos
 */
int USART2_ReceiveByte_IT(void) {
    return RingBuffer_Read(&usart2_rx_buffer);
}

/**
 * @brief Envía un string por USART2 usando interrupciones
 */
void USART2_SendString_IT(const char *str) {
    while (*str) {
        USART2_SendByte_IT(*str++);
    }
}
```

---

## 5. Printf Redirect a USART

Una técnica muy útil para debugging es redirigir `printf()` al puerto serial. Esto permite usar la función estándar de C para enviar mensajes de depuración.

### 5.1 Implementación con Newlib (arm-none-eabi-gcc)

```c
#include <stdio.h>

/**
 * @brief Redefinición de _write para newlib
 * 
 * Esta función es llamada por printf(), puts(), etc.
 * Redirige la salida estándar al USART2
 */
int _write(int file, char *ptr, int len) {
    int i;
    
    // Solo redirigir stdout y stderr
    if (file == 1 || file == 2) {  // 1=stdout, 2=stderr
        for (i = 0; i < len; i++) {
            // Opción 1: Usar interrupciones (recomendado)
            USART2_SendByte_IT(ptr[i]);
            
            // Opción 2: Usar polling (más simple pero bloqueante)
            // USART2_SendByte(ptr[i]);
        }
        return len;
    }
    
    return -1;
}
```

### 5.2 Uso de Printf

```c
#include <stdio.h>

int main(void) {
    // Configuración del sistema
    SystemInit();
    USART2_Init(115200);
    USART2_EnableInterrupts();
    
    RingBuffer_Init(&usart2_tx_buffer, 256);
    RingBuffer_Init(&usart2_rx_buffer, 256);
    
    // ¡Ahora se puede usar printf normalmente!
    printf("Sistema inicializado\r\n");
    printf("Frecuencia CPU: %lu Hz\r\n", SystemCoreClock);
    
    int counter = 0;
    while (1) {
        printf("Counter: %d\r\n", counter++);
        delay_ms(1000);
    }
}
```

### 5.3 Consideraciones de Printf

**Ventajas**:
- Sintaxis familiar y poderosa
- Soporte de formato (%d, %x, %f, etc.)
- Facilita debugging

**Desventajas**:
- Incrementa tamaño de código (~10-15KB por sprintf)
- Usa más RAM (buffer interno de formateo)
- Puede ser lento si transmite mucho texto

**Alternativas ligeras**:
```c
// Función simple sin formateo
void debug_puts(const char *str) {
    USART2_SendString_IT(str);
    USART2_SendString_IT("\r\n");
}

// Función para números (implementación propia)
void debug_print_uint(uint32_t num) {
    char buffer[12];
    sprintf(buffer, "%lu", num);  // O implementar conversión manual
    USART2_SendString_IT(buffer);
}
```

---

## 6. Ejemplo Completo: Echo con Printf

```c
/**
 * @file usart_interrupt_echo.c
 * @brief Ejemplo de echo por USART con interrupciones y printf
 */

#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"

// Buffers circulares (declarados globalmente)
RingBuffer_t usart2_tx_buffer;
RingBuffer_t usart2_rx_buffer;

int main(void) {
    // 1. Configuración del sistema
    SystemInit();
    
    // 2. Inicializar USART2
    USART2_Init(115200);
    USART2_EnableInterrupts();
    
    // 3. Inicializar buffers
    RingBuffer_Init(&usart2_tx_buffer, 256);
    RingBuffer_Init(&usart2_rx_buffer, 256);
    
    // 4. Mensaje de bienvenida
    printf("\r\n");
    printf("===================================\r\n");
    printf("  USART2 Echo con Interrupciones  \r\n");
    printf("===================================\r\n");
    printf("Escriba algo y presione Enter\r\n\r\n");
    
    // 5. Loop principal
    uint32_t counter = 0;
    while (1) {
        // Leer datos del buffer RX
        int data = USART2_ReceiveByte_IT();
        
        if (data >= 0) {
            // Echo del carácter recibido
            printf("Recibido: '%c' (0x%02X)\r\n", (char)data, (uint8_t)data);
            
            // Si es Enter, imprimir contador
            if (data == '\r' || data == '\n') {
                printf("Línea #%lu recibida\r\n\r\n", counter++);
            }
        }
        
        // El CPU está libre para hacer otras tareas aquí
        // Por ejemplo: leer sensores, actualizar displays, etc.
    }
}
```

---

## Actividades Prácticas

### Actividad 1: Implementar Buffer Circular (30 min)

**Objetivo**: Crear funciones de buffer circular y verificar su funcionamiento.

**Tareas**:
1. Implementar las funciones `RingBuffer_Init()`, `RingBuffer_Write()`, `RingBuffer_Read()`
2. Agregar función `RingBuffer_Available()` que retorne cantidad de bytes disponibles
3. Crear test unitario que verifique:
   - Escritura y lectura secuencial de 10 bytes
   - Comportamiento cuando buffer está lleno
   - Comportamiento circular (wrap-around)

**Evidencia**: Código fuente comentado + captura de terminal mostrando test exitoso

---

### Actividad 2: USART con Interrupciones (30 min)

**Objetivo**: Configurar USART2 para operar con interrupciones RX.

**Tareas**:
1. Configurar USART2 a 115200 baud
2. Habilitar interrupción RXNE y configurar NVIC
3. Implementar `USART2_IRQHandler()` que:
   - Lea el byte recibido
   - Lo guarde en buffer circular RX
   - Encienda LED cada vez que recibe datos
4. En `main()`, leer buffer RX y hacer echo por polling TX

**Evidencia**: Demostración de echo funcional + código fuente

---

### Actividad 3: Printf Redirect (30 min)

**Objetivo**: Redirigir printf() a USART2 para debugging.

**Tareas**:
1. Implementar función `_write()` para newlib
2. Modificar programa anterior para usar `printf()` en lugar de funciones propias
3. Imprimir mensajes formateados:
   - Contador en decimal y hexadecimal
   - Timestamp en milisegundos
   - Estado de pines GPIO
4. Medir diferencia de tamaño de código (con y sin printf)

**Evidencia**: Captura de terminal mostrando mensajes formateados + análisis de memoria

---

## Quiz Formativo

**Pregunta 1**: ¿Por qué la interrupción TXE no debe habilitarse permanentemente?

a) Porque consume mucha corriente  
b) Porque se dispara continuamente cuando el buffer está vacío  
c) Porque interfiere con la recepción  
d) Porque reduce la velocidad de baudrate

**Respuesta correcta**: b) Se dispara continuamente cuando buffer vacío, saturando el CPU

---

**Pregunta 2**: En un buffer circular de 256 bytes, ¿cuál es la condición de buffer lleno?

a) `head == tail`  
b) `head == size - 1`  
c) `(head + 1) % size == tail`  
d) `tail == 0`

**Respuesta correcta**: c) El siguiente índice de head alcanza a tail

---

**Pregunta 3**: ¿Qué hace la lectura del registro DR cuando hay un error de overrun (ORE)?

a) Reinicia el USART  
b) Limpia automáticamente el flag ORE  
c) Envía un byte de error  
d) No tiene efecto

**Respuesta correcta**: b) Limpiar el flag ORE requiere leer SR y luego DR

---

**Pregunta 4**: ¿Cuál es la principal desventaja de usar printf() en sistemas embebidos?

a) No funciona con interrupciones  
b) Solo soporta números enteros  
c) Incrementa significativamente el tamaño del código  
d) Es más lento que assembly

**Respuesta correcta**: c) Agrega ~10-15KB de código por las funciones de formateo

---

**Pregunta 5**: ¿Por qué las variables `head` y `tail` del buffer deben ser `volatile`?

a) Para que sean más rápidas  
b) Para que ocupen menos memoria  
c) Porque se modifican en ISR y programa principal  
d) Para que soporten valores mayores a 255

**Respuesta correcta**: c) Evita optimizaciones incorrectas del compilador en variables compartidas

---

## Evidencias de Aprendizaje

Al finalizar la sesión, el estudiante debe entregar:

1. **Código funcional** de USART con interrupciones
   - Buffer circular implementado correctamente
   - ISR manejando RX y TX
   - Echo funcional

2. **Printf redirect** funcionando
   - Mensajes formateados visibles en terminal
   - Análisis de impacto en memoria

3. **Reporte breve** (1 página) que incluya:
   - Ventajas de interrupciones vs polling
   - Casos de uso de printf vs funciones propias
   - Problemas encontrados y soluciones

---

## Material para Casa

### Lectura Preparatoria (próxima sesión: I2C)

1. **Application Note AN4235**: "I2C timing configuration tool for STM32 microcontrollers"
2. **Datasheet AT24C64**: EEPROM I2C (páginas 1-5)
3. **Video**: "Understanding I2C Protocol" - NXP Semiconductors (10 min)

### Ejercicio Opcional

Implementar un **logger de eventos** que:
- Almacene timestamps de eventos en RAM
- Envíe log completo por USART cuando se presione un botón
- Use printf para formatear la salida

**Desafío adicional**: Agregar niveles de log (DEBUG, INFO, WARNING, ERROR) con prefijos de colores ANSI en terminal.

---

## Notas para el Instructor

### Errores Comunes

1. **No deshabilitar TXE**: Estudiantes habilitan TXE permanentemente y CPU se satura
   - **Solución**: Explicar que TXE se habilita solo cuando hay datos a enviar

2. **Olvidar volatile**: Variables del buffer no marcadas como volatile
   - **Solución**: Demostrar con optimizaciones (-O2) cómo falla el código

3. **Race conditions**: Acceso concurrente a buffer sin protección
   - **Solución**: Explicar que operaciones de escritura/lectura deben ser atómicas
   - Para buffers más complejos, considerar deshabilitar interrupciones temporalmente

4. **Printf bloqueante**: Usar printf con buffer TX pequeño
   - **Solución**: Aumentar tamaño de buffer o advertir sobre posibles bloqueos

### Extensiones Avanzadas

- **DMA con USART**: Para aplicaciones de alto throughput
- **Múltiples USART**: Gestión de varios puertos seriales
- **Protocolos seriales**: Implementar MODBUS RTU sobre USART
- **Flow control**: RTS/CTS para evitar pérdida de datos

### Tiempo de Sesión

- Introducción y teoría: 20 min
- Demostración del instructor: 15 min
- Actividad 1 (Buffer circular): 30 min
- Actividad 2 (USART interrupciones): 30 min
- Actividad 3 (Printf): 20 min
- Cierre y Q&A: 5 min

**Total**: 120 minutos

---

## Referencias

1. **STM32F407 Reference Manual** (RM0090), Sección 30: USART
2. **Application Note AN3109**: "Migrating between STM32 USART modes"
3. **GNU ARM Embedded Toolchain** - Newlib documentation
4. **Jack Ganssle**: "The Art of Designing Embedded Systems" - Capítulo sobre interrupciones
5. **Steve Oualline**: "Practical C Programming" - Capítulo sobre I/O

---

**Fin de Sesión 2**
