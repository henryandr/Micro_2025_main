# Sesión 1: Introducción a Comunicaciones Seriales y USART

**Unidad 3 - Semana 1, Sesión 1**  
**Duración**: 2 horas  
**Modalidad**: Teórico-práctica

---

## Objetivos Específicos

Al finalizar esta sesión, el estudiante será capaz de:

1. **Explicar** los fundamentos de la comunicación serial asíncrona (UART)
2. **Identificar** los parámetros de configuración de USART (baudrate, paridad, bits de parada)
3. **Configurar** el periférico USART del STM32F407 a nivel de registros
4. **Implementar** funciones básicas de transmisión y recepción por polling
5. **Utilizar** USART para comunicación con PC mediante terminal serial

---

## Contenido Teórico

### 1. Introducción a Comunicaciones Seriales

#### ¿Por qué Comunicación Serial?

En sistemas embebidos, necesitamos comunicación entre:
- Microcontrolador ↔ PC (debugging, configuración)
- Microcontrolador ↔ Sensores/actuadores
- Microcontrolador ↔ Otros microcontroladores

**Comunicación Paralela** vs **Comunicación Serial**:

| Aspecto | Paralela | Serial |
|---------|----------|--------|
| **Cables** | Muchos (8, 16, 32 líneas) | Pocos (1-4 líneas) |
| **Velocidad** | Alta por ciclo | Requiere más ciclos |
| **Costo** | Alto (más pines, cables) | Bajo |
| **Distancia** | Corta (interferencia) | Larga |
| **Uso actual** | Interno (buses de memoria) | Comunicación externa |

**Conclusión**: Para distancias >1m y comunicación externa, serial es preferido.

#### Tipos de Comunicación Serial

**Por Sincronización**:
1. **Asíncrona**: No hay clock compartido (UART)
   - Usa bits de start/stop para sincronización
   - Ejemplos: UART, RS-232

2. **Síncrona**: Clock compartido entre transmisor y receptor
   - Sincronización precisa por señal de clock
   - Ejemplos: SPI, I2C

**Por Dirección**:
1. **Simplex**: Solo una dirección
2. **Half-duplex**: Ambas direcciones, pero no simultáneas
3. **Full-duplex**: Ambas direcciones simultáneamente (UART típico)

### 2. UART/USART: Fundamentos

#### Diferencia UART vs USART

- **UART** (Universal Asynchronous Receiver Transmitter): Solo asíncrono
- **USART** (Universal Synchronous/Asynchronous...): Soporta ambos modos

El STM32F407 tiene **USART** que puede funcionar en modo asíncrono (UART) o síncrono.

#### Señales UART Básicas

```
Mínimo Full-Duplex:
MCU1                    MCU2
TX  ──────────────────> RX
RX  <────────────────── TX
GND ───────────────────GND
```

- **TX** (Transmit): Salida de datos
- **RX** (Receive): Entrada de datos
- **GND**: Tierra común (obligatoria)

#### Formato de Trama UART

```
    ┌─────────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬────────┬──────┐
    │  IDLE   │START│ D0  │ D1  │ D2  │ D3  │ D4  │ D5  │ D6  │ D7    │ STOP │
    │  (1)    │ (0) │     │     │     │     │     │     │     │       │ (1)  │
    └─────────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴────────┴──────┘
     Nivel alto  Flanco  LSB first                              MSB      1 o 2 bits
                 descendente                                            nivel alto
```

**Componentes**:
1. **Start bit**: 1 bit en 0 (marca inicio de transmisión)
2. **Data bits**: 7, 8 o 9 bits (típicamente 8)
3. **Parity bit** (opcional): Para detección de errores
4. **Stop bits**: 1, 1.5 o 2 bits en 1 (marca fin)

**Estado IDLE**: Línea en nivel alto cuando no hay transmisión.

#### Baudrate (Velocidad)

**Definición**: Número de símbolos (bits) por segundo.

Baudrates comunes:
- 9600 bps: Uso general, bajo consumo
- 38400 bps: Balance velocidad/confiabilidad
- 115200 bps: **Más común para debug**
- 230400, 460800, 921600 bps: Alta velocidad

**Importante**: Ambos lados (TX y RX) deben usar el **mismo baudrate**.

**Tolerancia**: Típicamente ±2% es aceptable. Más allá puede causar errores.

#### Paridad

Bit adicional para detección de errores:

- **None**: Sin paridad (más común)
- **Even**: Número par de 1's en data + parity
- **Odd**: Número impar de 1's en data + parity

Ejemplo con paridad par:
- Data: `1010 0011` → 4 unos (par) → Parity = 0
- Data: `1010 0111` → 5 unos (impar) → Parity = 1

**Nota**: Solo detecta 1 bit erróneo, no corrige. Para aplicaciones críticas, usar CRC.

### 3. USART en STM32F407

#### Periféricos Disponibles

El STM32F407 tiene:
- **3 USART**: USART1, USART2, USART3
- **2 UART**: UART4, UART5
- **1 USART adicional**: USART6

**Diferencias**:
- USART: Soporta modo síncrono, SmartCard, IrDA
- UART: Solo asíncrono

En esta unidad usamos **USART2** porque en la placa Discovery está conectado al USB (ST-Link).

#### Registros Principales de USART

**Estructura base**:
```c
typedef struct {
    volatile uint32_t SR;      // Status Register
    volatile uint32_t DR;      // Data Register
    volatile uint32_t BRR;     // Baud Rate Register
    volatile uint32_t CR1;     // Control Register 1
    volatile uint32_t CR2;     // Control Register 2
    volatile uint32_t CR3;     // Control Register 3
    volatile uint32_t GTPR;    // Guard Time and Prescaler
} USART_TypeDef;

#define USART2_BASE  0x40004400
#define USART2       ((USART_TypeDef*)USART2_BASE)
```

##### SR (Status Register)

```
Bits importantes:
[7] TXE   - Transmit Data Register Empty (1 = puede escribir a DR)
[6] TC    - Transmission Complete (1 = transmisión terminada)
[5] RXNE  - Read Data Register Not Empty (1 = dato disponible en DR)
[4] IDLE  - IDLE line detected
[3] ORE   - Overrun Error
[2] NE    - Noise Error
[1] FE    - Framing Error
[0] PE    - Parity Error
```

##### DR (Data Register)

```
[8:0] Data - Dato a transmitir o recibido (8 o 9 bits)
```

##### BRR (Baud Rate Register)

```
[15:4] DIV_Mantissa - Parte entera del divisor
[3:0]  DIV_Fraction - Parte fraccionaria del divisor

Cálculo:
USARTDIV = fCK / (16 × Baudrate)
Donde fCK es la frecuencia del clock del periférico
```

##### CR1 (Control Register 1)

```
[13] UE   - USART Enable
[12] M    - Word length (0 = 8 bits, 1 = 9 bits)
[10] PCE  - Parity Control Enable
[9]  PS   - Parity Selection (0 = even, 1 = odd)
[7]  TXEIE - TXE Interrupt Enable
[6]  TCIE  - TC Interrupt Enable
[5]  RXNEIE - RXNE Interrupt Enable
[3]  TE   - Transmitter Enable
[2]  RE   - Receiver Enable
```

##### CR2 (Control Register 2)

```
[13:12] STOP - Stop bits (00 = 1 bit, 10 = 2 bits)
```

#### Cálculo de Baudrate

Para USART2 en STM32F407 (asumiendo PCLK1 = 42 MHz):

```c
// Baudrate deseado: 115200 bps
// PCLK1 = 42,000,000 Hz
// Oversampling = 16

// USARTDIV = PCLK1 / (16 × Baudrate)
// USARTDIV = 42,000,000 / (16 × 115,200) = 22.786

// Separar en mantissa y fraction:
// Mantissa = 22 (parte entera)
// Fraction = 0.786 × 16 = 12.58 ≈ 13

// BRR = (Mantissa << 4) | Fraction
// BRR = (22 << 4) | 13 = 0x16D
```

### 4. Secuencia de Inicialización de USART

#### Paso 1: Habilitar Clocks

```c
// Habilitar clock de GPIOA (pines TX/RX)
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

// Habilitar clock de USART2
RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
```

#### Paso 2: Configurar Pines GPIO

USART2 usa PA2 (TX) y PA3 (RX) con Alternate Function 7:

```c
// PA2: USART2_TX (AF7)
// PA3: USART2_RX (AF7)

// Configurar como Alternate Function
GPIOA->MODER &= ~((3 << (2*2)) | (3 << (3*2)));
GPIOA->MODER |= (2 << (2*2)) | (2 << (3*2));

// Alta velocidad
GPIOA->OSPEEDR |= (3 << (2*2)) | (3 << (3*2));

// Push-pull (TX), no relevante para RX
GPIOA->OTYPER &= ~((1 << 2) | (1 << 3));

// Sin pull-up/pull-down
GPIOA->PUPDR &= ~((3 << (2*2)) | (3 << (3*2)));

// Seleccionar AF7 para PA2 y PA3
GPIOA->AFR[0] &= ~((0xF << (2*4)) | (0xF << (3*4)));
GPIOA->AFR[0] |= (7 << (2*4)) | (7 << (3*4));
```

#### Paso 3: Configurar USART

```c
// Asegurar que USART esté deshabilitado
USART2->CR1 &= ~USART_CR1_UE;

// Configurar baudrate (115200 bps con PCLK1=42MHz)
USART2->BRR = 0x16D;  // O calcular dinámicamente

// Configurar formato: 8 bits, sin paridad, 1 stop bit
USART2->CR1 &= ~USART_CR1_M;    // 8 bits de datos
USART2->CR1 &= ~USART_CR1_PCE;  // Sin paridad
USART2->CR2 &= ~USART_CR2_STOP; // 1 stop bit (00)

// Habilitar TX y RX
USART2->CR1 |= USART_CR1_TE;    // Transmitter enable
USART2->CR1 |= USART_CR1_RE;    // Receiver enable

// Habilitar USART
USART2->CR1 |= USART_CR1_UE;
```

### 5. Transmisión por Polling

Para enviar un byte:

```c
void USART2_SendChar(char c) {
    // Esperar a que el registro de transmisión esté vacío
    while (!(USART2->SR & USART_SR_TXE));
    
    // Escribir el dato
    USART2->DR = c;
    
    // (Opcional) Esperar a que la transmisión termine
    // while (!(USART2->SR & USART_SR_TC));
}
```

Para enviar una cadena:

```c
void USART2_SendString(const char* str) {
    while (*str) {
        USART2_SendChar(*str++);
    }
}
```

**Nota sobre TXE vs TC**:
- **TXE**: Data Register vacío (puede escribir siguiente byte)
- **TC**: Transmisión completa (último bit ya salió por TX)

Para envíos continuos, usar TXE. Para confirmar fin de transmisión, usar TC.

### 6. Recepción por Polling

Para recibir un byte:

```c
char USART2_ReceiveChar(void) {
    // Esperar a que haya un dato disponible
    while (!(USART2->SR & USART_SR_RXNE));
    
    // Leer el dato
    return (char)USART2->DR;
}
```

Para recibir con timeout:

```c
#define TIMEOUT_MS 1000

int USART2_ReceiveChar_Timeout(char* c) {
    uint32_t timeout = TIMEOUT_MS;
    
    while (!(USART2->SR & USART_SR_RXNE) && timeout > 0) {
        delay_ms(1);
        timeout--;
    }
    
    if (timeout == 0) {
        return -1;  // Timeout
    }
    
    *c = (char)USART2->DR;
    return 0;  // OK
}
```

### 7. Ejemplo Completo: Echo Test

```c
#include <stdint.h>

// Definiciones base
#define RCC_BASE        0x40023800
#define GPIOA_BASE      0x40020000
#define USART2_BASE     0x40004400

#define RCC             ((RCC_TypeDef*)RCC_BASE)
#define GPIOA           ((GPIO_TypeDef*)GPIOA_BASE)
#define USART2          ((USART_TypeDef*)USART2_BASE)

// Bits de habilitación
#define RCC_AHB1ENR_GPIOAEN  (1 << 0)
#define RCC_APB1ENR_USART2EN (1 << 17)

// USART SR bits
#define USART_SR_TXE   (1 << 7)
#define USART_SR_RXNE  (1 << 5)

// USART CR1 bits
#define USART_CR1_UE   (1 << 13)
#define USART_CR1_M    (1 << 12)
#define USART_CR1_PCE  (1 << 10)
#define USART_CR1_TE   (1 << 3)
#define USART_CR1_RE   (1 << 2)

void USART2_Init(void) {
    // 1. Habilitar clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    
    // 2. Configurar pines PA2 y PA3
    GPIOA->MODER &= ~((3 << 4) | (3 << 6));
    GPIOA->MODER |= (2 << 4) | (2 << 6);  // AF mode
    GPIOA->AFR[0] &= ~((0xF << 8) | (0xF << 12));
    GPIOA->AFR[0] |= (7 << 8) | (7 << 12);  // AF7
    
    // 3. Configurar USART2
    USART2->CR1 = 0;  // Reset
    USART2->BRR = 0x16D;  // 115200 @ 42MHz
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
    USART2->CR1 |= USART_CR1_UE;
}

void USART2_SendChar(char c) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

char USART2_ReceiveChar(void) {
    while (!(USART2->SR & USART_SR_RXNE));
    return (char)USART2->DR;
}

void USART2_SendString(const char* str) {
    while (*str) {
        USART2_SendChar(*str++);
    }
}

int main(void) {
    USART2_Init();
    
    USART2_SendString("Echo Test - Iniciado\r\n");
    USART2_SendString("Escribe algo y presiona Enter\r\n");
    
    while (1) {
        char c = USART2_ReceiveChar();
        USART2_SendChar(c);  // Echo back
        
        // Si es Enter, enviar nueva línea
        if (c == '\r') {
            USART2_SendChar('\n');
        }
    }
}
```

---

## Actividades Prácticas

### Actividad 1: Configuración Básica de USART (30 min)

**Objetivo**: Configurar USART2 y transmitir mensajes a PC.

**Pasos**:
1. Crear proyecto básico con estructura de la Unidad 2
2. Implementar función `USART2_Init()` con configuración:
   - Baudrate: 115200 bps
   - 8 bits de datos
   - Sin paridad
   - 1 stop bit
3. Implementar `USART2_SendChar()` y `USART2_SendString()`
4. En `main()`, enviar mensaje "Hola desde STM32!\r\n"
5. Compilar y programar
6. Abrir terminal serial (115200 8N1) y verificar mensaje

**Evidencia**: Captura de pantalla del terminal mostrando el mensaje.

### Actividad 2: Echo Test con Formato (40 min)

**Objetivo**: Recibir caracteres y reenviarlos con formato.

**Pasos**:
1. Implementar `USART2_ReceiveChar()`
2. Modificar `main()` para:
   - Enviar mensaje de bienvenida
   - Esperar caracteres del usuario
   - Hacer echo de cada carácter recibido
   - Si se recibe 'a'-'z', convertir a mayúscula antes de echo
   - Si se recibe '\r', enviar "\r\n"
3. Compilar y probar

**Evidencia**: Código fuente + demostración en vivo.

### Actividad 3: Contador con USART (40 min)

**Objetivo**: Enviar contador cada segundo por USART.

**Pasos**:
1. Integrar Timer de Unidad 2 (SysTick o TIM2)
2. Crear función `delay_ms(uint32_t ms)`
3. En `main()`:
   ```c
   uint32_t contador = 0;
   while (1) {
       char buffer[32];
       sprintf(buffer, "Contador: %lu\r\n", contador++);
       USART2_SendString(buffer);
       delay_ms(1000);
   }
   ```
4. Compilar y verificar en terminal

**Nota**: Para usar `sprintf`, incluir `<stdio.h>` y puede requerir implementación de `_write()` (o usar funciones propias).

**Evidencia**: Terminal mostrando contador incrementando cada segundo.

---

## Quiz Formativo (10 min)

**Instrucciones**: Responder las siguientes preguntas. Pueden ser de opción múltiple o breves.

1. **¿Cuál es la principal diferencia entre UART y USART?**
   - a) UART es más rápido
   - b) USART soporta modo síncrono
   - c) USART solo existe en ARM
   - d) No hay diferencia

2. **En una trama UART de 8N1, ¿cuántos bits se transmiten en total por cada byte de datos?**
   - a) 8
   - b) 9
   - c) 10
   - d) 11

3. **¿Qué flag del registro SR indica que se puede escribir un nuevo dato en DR?**
   - a) RXNE
   - b) TC
   - c) TXE
   - d) IDLE

4. **Para USART2 en STM32F407 con PCLK1=42MHz y baudrate 115200, el valor aproximado de BRR es:**
   - a) 0x16D
   - b) 0x45
   - c) 0x1A0
   - d) 0x2AF

5. **¿Qué debe hacerse ANTES de escribir a USART2->DR para enviar un byte?**
   - a) Limpiar el bit TC
   - b) Esperar a que TXE = 1
   - c) Deshabilitar USART
   - d) Leer el registro DR

**Respuestas**: 1-b, 2-c, 3-c, 4-a, 5-b

---

## Evidencia de Aprendizaje

Al finalizar la sesión, el estudiante debe entregar:

1. **Código fuente** de las 3 actividades (archivo .c con comentarios)
2. **Captura de pantalla** del terminal serial mostrando:
   - Mensaje de la Actividad 1
   - Echo test de la Actividad 2
   - Contador de la Actividad 3
3. **Respuestas** del quiz formativo

**Formato de entrega**: Carpeta comprimida (.zip) con nombre `Unidad3_Sesion1_ApellidoNombre.zip`

---

## Material para Casa

### Lectura Preparatoria (para Sesión 2)

1. Leer RM0090 (Reference Manual), Capítulo 25, secciones:
   - 25.3.4: USART interrupts
   - 25.6.1: USART_SR (profundizar en bits de error)

2. Investigar sobre **buffer circular** (circular buffer o ring buffer):
   - ¿Para qué sirve?
   - ¿Cómo se implementa?
   - Ventajas en comunicaciones seriales

### Ejercicio Preparatorio

Modificar el código de echo para que:
- Cuente el número de caracteres recibidos
- Después de 10 caracteres, envíe: "Recibidos 10 caracteres\r\n"
- Reinicie el contador

**Pista**: Usar una variable global `uint8_t char_count = 0;`

---

## Notas para el Instructor

### Puntos Clave a Enfatizar

1. **Baudrate matching**: Ambos lados deben usar exactamente el mismo baudrate
2. **Polling es bloqueante**: El procesador se queda esperando, no puede hacer otras cosas
3. **TXE vs TC**: Explicar bien la diferencia, es fuente de confusión
4. **Configuración de pines**: El orden importa (GPIO antes que USART)

### Errores Comunes

1. **Olvidar habilitar clocks**: De GPIO o USART → no funciona
2. **No configurar Alternate Function**: Los pines quedan como GPIO normal
3. **Baudrate incorrecto**: Calcular BRR basándose en frecuencia real del clock
4. **No esperar TXE**: Escribir a DR cuando no está listo → sobrescritura
5. **Terminal con configuración incorrecta**: Baudrate diferente → caracteres extraños

### Material de Apoyo Adicional

- Video recomendado: "STM32 UART Tutorial" de ControllersTech (YouTube)
- Simulador online: Tinkercad Circuits (solo conceptual, no STM32)
- Herramienta: Verificar baudrate con lógica analizador o osciloscopio

### Timing de la Sesión

- 0-15 min: Introducción teórica (comunicación serial, tipos)
- 15-40 min: UART/USART fundamentos (formato, baudrate, paridad)
- 40-65 min: USART en STM32 (registros, configuración)
- 65-75 min: Demo en vivo (ejemplo completo)
- 75-105 min: Actividad 1 y 2 (guiadas)
- 105-115 min: Actividad 3 (más independiente)
- 115-120 min: Quiz y cierre

---

## Recursos Adicionales

### Datasheets y Manuales

- **RM0090**: STM32F4 Reference Manual, Capítulo 25
- **DS8626**: STM32F407 Datasheet, sección de pinout

### Herramientas de Software

- **PuTTY** (Windows): https://www.putty.org/
- **minicom** (Linux): `sudo apt install minicom`
- **CoolTerm** (multiplataforma): https://freeware.the-meiers.org/

### Tutoriales Online

- ControllersTech: https://controllerstech.com/uart-tutorial/
- DeepBlue Embedded: https://deepbluembedded.com/stm32-usart-uart-tutorial/

---

**Próxima sesión**: USART Avanzado con Interrupciones y Debugging (printf redirect, buffer circular, logging)
