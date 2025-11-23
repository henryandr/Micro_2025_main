# Sesión 5: Comunicación SPI - Master/Slave

**Unidad 3**: Protocolos de Comunicación Serial  
**Semana**: 3  
**Duración**: 2 horas  
**Modalidad**: Presencial con práctica en laboratorio

---

## Objetivos de Aprendizaje

Al finalizar esta sesión, el estudiante será capaz de:

1. Comprender el funcionamiento del protocolo SPI (Serial Peripheral Interface)
2. Configurar el periférico SPI1 del STM32F407 en modo master
3. Implementar comunicación full-duplex con dispositivos SPI
4. Configurar modos de reloj (CPOL/CPHA) según requisitos del dispositivo
5. Comunicarse con sensores y displays por SPI

**Tiempo estimado de actividades**: 90 minutos (teoría: 40 min, práctica: 50 min)

---

## 1. Introducción al Protocolo SPI

### 1.1 ¿Qué es SPI?

SPI (Serial Peripheral Interface) es un protocolo de comunicación serial síncrono desarrollado por Motorola. Es más rápido que I2C pero requiere más pines.

**Características principales**:
- Bus de 4 cables (o 3 en modo 3-wire)
- Comunicación full-duplex (TX y RX simultáneos)
- Velocidades: hasta decenas de MHz
- Un solo master, múltiples slaves
- Sin direccionamiento (usa CS/SS para seleccionar slave)

### 1.2 Señales SPI

| Señal | Nombre | Descripción |
|-------|---------|-------------|
| SCK | Serial Clock | Reloj generado por master |
| MOSI | Master Out Slave In | Datos master → slave |
| MISO | Master In Slave Out | Datos slave → master |
| CS/SS | Chip Select / Slave Select | Selección de slave (activo bajo) |

**Topología típica**:
```
    Master (MCU)
    ┌────────────┐
    │   SCK  ────┼────┬────────┬─────── SCK (todos los slaves)
    │   MOSI ────┼────┼────────┼─────── MOSI (todos los slaves)
    │   MISO ────┼────┼────────┼─────── MISO (todos los slaves)
    │   CS1  ────┼────┘        │
    │   CS2  ────┼─────────────┘
    └────────────┘
         │             │             │
    ┌────▼─────┐  ┌───▼──────┐ ┌────▼─────┐
    │ Slave 1  │  │ Slave 2  │ │ Slave 3  │
    └──────────┘  └──────────┘ └──────────┘
```

### 1.3 Modos de Reloj (CPOL/CPHA)

SPI tiene 4 modos según configuración de polaridad (CPOL) y fase (CPHA):

| Modo | CPOL | CPHA | SCK idle | Muestreo |
|------|------|------|----------|----------|
| 0 | 0 | 0 | Bajo | Flanco subida |
| 1 | 0 | 1 | Bajo | Flanco bajada |
| 2 | 1 | 0 | Alto | Flanco bajada |
| 3 | 1 | 1 | Alto | Flanco subida |

- **CPOL**: Polaridad del reloj en reposo (0=bajo, 1=alto)
- **CPHA**: Fase del reloj (0=muestrea en 1er flanco, 1=muestrea en 2do flanco)

**Ejemplo de Modo 0 (CPOL=0, CPHA=0)**:
```
CS   ────┐                             ┌────
         └─────────────────────────────┘
SCK  ────┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌────
         └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘
MOSI ────┤D7│D6│D5│D4│D3│D2│D1│D0│────────
         └───┴───┴───┴───┴───┴───┴───┴────
MISO ────┤D7│D6│D5│D4│D3│D2│D1│D0│────────
         └───┴───┴───┴───┴───┴───┴───┴────
         ↑ Muestreo en flanco subida
```

---

## 2. SPI en STM32F407

### 2.1 Periféricos SPI Disponibles

El STM32F407 tiene **3 periféricos SPI** (SPI1, SPI2, SPI3):

| Periférico | SCK | MOSI | MISO | Bus | Vel. max |
|------------|-----|------|------|-----|----------|
| SPI1 | PA5, PB3 | PA7, PB5 | PA6, PB4 | APB2 | 42 MHz |
| SPI2 | PB10, PB13 | PB15, PC3 | PB14, PC2 | APB1 | 21 MHz |
| SPI3 | PB3, PC10 | PB5, PC12 | PB4, PC11 | APB1 | 21 MHz |

Usaremos **SPI1** con:
- **PA5**: SCK
- **PA6**: MISO
- **PA7**: MOSI
- **PA4**: CS (GPIO manual)

### 2.2 Registros Principales de SPI

| Registro | Descripción |
|----------|-------------|
| SPI_CR1 | Control register 1 (enable, baudrate, modo) |
| SPI_CR2 | Control register 2 (interrupciones, DMA) |
| SPI_SR | Status register (flags TXE, RXNE, BSY) |
| SPI_DR | Data register (TX/RX) |

---

## 3. Configuración de SPI1

### 3.1 Paso 1: Habilitar Clocks

```c
/**
 * @brief Inicializa clocks para SPI1
 */
void SPI1_ClockInit(void) {
    // Habilitar clock de GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    
    // Habilitar clock de SPI1 (en APB2)
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
}
```

### 3.2 Paso 2: Configurar Pines GPIO

```c
/**
 * @brief Configura pines para SPI1
 */
void SPI1_GPIO_Init(void) {
    // PA5 (SCK), PA6 (MISO), PA7 (MOSI): Alternate Function
    GPIOA->MODER &= ~(0x3F << (5*2));  // Limpiar bits PA5-PA7
    GPIOA->MODER |= (0x2A << (5*2));   // AF mode (10b)
    
    // Velocidad muy alta
    GPIOA->OSPEEDR |= (0x3F << (5*2));
    
    // Sin pull-up/down
    GPIOA->PUPDR &= ~(0x3F << (5*2));
    
    // Seleccionar AF5 (SPI1) para PA5, PA6, PA7
    GPIOA->AFR[0] &= ~(0xFFF << (5*4));
    GPIOA->AFR[0] |= (0x555 << (5*4));  // AF5
    
    // PA4 como GPIO para CS (manual)
    GPIOA->MODER &= ~(0x3 << (4*2));
    GPIOA->MODER |= (0x1 << (4*2));  // Output
    GPIOA->ODR |= (1 << 4);  // CS alto (inactivo)
}
```

### 3.3 Paso 3: Configurar SPI1

```c
/**
 * @brief Configura SPI1 en modo master
 * 
 * Configuración:
 * - Modo 0 (CPOL=0, CPHA=0)
 * - 8 bits
 * - MSB first
 * - Baudrate: APB2/16 = 84MHz/16 = 5.25 MHz
 */
void SPI1_Init(void) {
    // 1. Deshabilitar SPI1
    SPI1->CR1 &= ~SPI_CR1_SPE;
    
    // 2. Configurar CR1
    SPI1->CR1 = 0;  // Limpiar
    
    // Master mode
    SPI1->CR1 |= SPI_CR1_MSTR;
    
    // Baudrate: fPCLK/16 (BR[2:0] = 011)
    SPI1->CR1 |= (0x3 << 3);  // BR = 011
    
    // CPOL = 0, CPHA = 0 (Modo 0)
    // Por defecto ya están en 0
    
    // 8-bit data frame
    // Por defecto ya está en 0 (DFF bit)
    
    // MSB first
    // Por defecto ya está en 0 (LSBFIRST bit)
    
    // Software slave management (SSM=1, SSI=1)
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    
    // 3. Habilitar SPI1
    SPI1->CR1 |= SPI_CR1_SPE;
}
```

**Cálculo de baudrate**:
- APB2 clock: 84 MHz
- Prescaler: 16 (BR=011)
- Baudrate SPI: 84/16 = 5.25 MHz

---

## 4. Operaciones Básicas SPI

### 4.1 Funciones de Control CS

```c
/**
 * @brief Activa CS (bajo = activo)
 */
void SPI1_CS_Low(void) {
    GPIOA->ODR &= ~(1 << 4);
}

/**
 * @brief Desactiva CS (alto = inactivo)
 */
void SPI1_CS_High(void) {
    GPIOA->ODR |= (1 << 4);
}
```

### 4.2 Transmitir/Recibir un Byte

```c
/**
 * @brief Transmite y recibe un byte por SPI (full-duplex)
 * @param data Byte a transmitir
 * @return Byte recibido
 */
uint8_t SPI1_TransferByte(uint8_t data) {
    // 1. Esperar que TX buffer esté vacío
    while (!(SPI1->SR & SPI_SR_TXE));
    
    // 2. Escribir dato a transmitir
    *(volatile uint8_t *)&SPI1->DR = data;
    
    // 3. Esperar que RX buffer tenga datos
    while (!(SPI1->SR & SPI_SR_RXNE));
    
    // 4. Leer dato recibido
    return *(volatile uint8_t *)&SPI1->DR;
}
```

### 4.3 Transmitir Múltiples Bytes

```c
/**
 * @brief Transmite un buffer por SPI
 * @param data Puntero al buffer
 * @param length Cantidad de bytes
 */
void SPI1_Transmit(uint8_t *data, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        SPI1_TransferByte(data[i]);
    }
}

/**
 * @brief Recibe un buffer por SPI
 * @param data Puntero al buffer destino
 * @param length Cantidad de bytes
 */
void SPI1_Receive(uint8_t *data, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        data[i] = SPI1_TransferByte(0xFF);  // Dummy byte
    }
}
```

---

## 5. Ejemplo: Sensor MAX31855 (Termopar K)

### 5.1 Características del MAX31855

- **Función**: Convertidor termopar K a digital
- **Interface**: SPI (modo 0 o 3)
- **Resolución**: 0.25°C
- **Rango**: -200°C a +1350°C
- **Datos**: 32 bits (14 bits de temperatura + info adicional)

### 5.2 Lectura del MAX31855

```c
/**
 * @brief Lee temperatura del MAX31855
 * @return Temperatura en °C (float)
 */
float MAX31855_ReadTemperature(void) {
    uint32_t raw_data = 0;
    
    // Activar CS
    SPI1_CS_Low();
    delay_us(1);  // Pequeña espera
    
    // Leer 4 bytes (32 bits)
    for (uint8_t i = 0; i < 4; i++) {
        raw_data <<= 8;
        raw_data |= SPI1_TransferByte(0xFF);
    }
    
    // Desactivar CS
    SPI1_CS_High();
    
    // Verificar bit de error (bit 16)
    if (raw_data & 0x00010000) {
        return -999.0f;  // Error en lectura
    }
    
    // Extraer temperatura (bits 31-18)
    int16_t temp_raw = (raw_data >> 18) & 0x3FFF;
    
    // Si es negativo (bit 13 = 1), extender signo
    if (temp_raw & 0x2000) {
        temp_raw |= 0xC000;
    }
    
    // Convertir a °C (resolución 0.25°C)
    float temperature = temp_raw * 0.25f;
    
    return temperature;
}
```

---

## 6. Ejemplo: Display OLED por SPI

El SSD1306 también puede usar SPI (más rápido que I2C):

```c
// Pines adicionales para SPI del SSD1306
#define SSD1306_DC_PIN  8   // PA8: Data/Command
#define SSD1306_RST_PIN 9   // PA9: Reset

/**
 * @brief Envía comando al SSD1306 por SPI
 */
void SSD1306_SPI_Command(uint8_t cmd) {
    GPIOA->ODR &= ~(1 << SSD1306_DC_PIN);  // DC = 0 (comando)
    SPI1_CS_Low();
    SPI1_TransferByte(cmd);
    SPI1_CS_High();
}

/**
 * @brief Envía dato al SSD1306 por SPI
 */
void SSD1306_SPI_Data(uint8_t data) {
    GPIOA->ODR |= (1 << SSD1306_DC_PIN);   // DC = 1 (dato)
    SPI1_CS_Low();
    SPI1_TransferByte(data);
    SPI1_CS_High();
}

/**
 * @brief Actualiza pantalla completa por SPI
 */
void SSD1306_SPI_Update(uint8_t *buffer) {
    // Establecer dirección inicial
    SSD1306_SPI_Command(0x21);  // Column address
    SSD1306_SPI_Command(0);
    SSD1306_SPI_Command(127);
    SSD1306_SPI_Command(0x22);  // Page address
    SSD1306_SPI_Command(0);
    SSD1306_SPI_Command(7);
    
    // Enviar todo el buffer (más rápido que I2C)
    GPIOA->ODR |= (1 << SSD1306_DC_PIN);  // DC = 1
    SPI1_CS_Low();
    
    for (uint16_t i = 0; i < 1024; i++) {
        SPI1_TransferByte(buffer[i]);
    }
    
    SPI1_CS_High();
}
```

**Ventaja de SPI vs I2C para displays**:
- I2C a 400 kHz: ~20 ms para actualizar pantalla
- SPI a 5 MHz: ~2 ms para actualizar pantalla (10x más rápido)

---

## Actividades Prácticas

### Actividad 1: Configuración de SPI1 (20 min)

**Objetivo**: Configurar SPI1 y verificar comunicación básica.

**Tareas**:
1. Implementar inicialización de SPI1 en modo 0
2. Crear función de loopback test (MOSI → MISO cortocircuitado)
3. Transmitir secuencia de bytes y verificar recepción
4. Cambiar baudrate y medir con osciloscopio
5. Probar modo 3 (CPOL=1, CPHA=1)

**Evidencia**: Capturas de osciloscopio mostrando SCK, MOSI, MISO

---

### Actividad 2: Sensor por SPI (30 min)

**Objetivo**: Leer temperatura del MAX31855 o sensor SPI disponible.

**Tareas**:
1. Conectar sensor SPI al STM32
2. Implementar función de lectura
3. Leer temperatura cada 500 ms
4. Mostrar por USART y/o OLED
5. Calcular tasa de actualización máxima

**Evidencia**: Código + demostración en vivo

---

### Actividad 3: Comparación I2C vs SPI (20 min)

**Objetivo**: Analizar diferencias prácticas entre protocolos.

**Tareas**:
1. Actualizar OLED por I2C y medir tiempo
2. Actualizar mismo OLED por SPI y medir tiempo
3. Comparar cantidad de pines usados
4. Analizar ventajas/desventajas de cada protocolo
5. Documentar casos de uso ideales para cada uno

**Evidencia**: Tabla comparativa con mediciones reales

---

## Quiz Formativo

**Pregunta 1**: ¿Cuál es la principal ventaja de SPI sobre I2C?

a) Usa menos pines  
b) Mayor velocidad de comunicación  
c) Soporta más dispositivos  
d) Menor consumo de energía

**Respuesta correcta**: b) SPI puede alcanzar decenas de MHz vs ~1 MHz de I2C

---

**Pregunta 2**: En modo full-duplex, ¿qué sucede al transmitir un byte?

a) Solo se envía, no se recibe  
b) Se envía y simultáneamente se recibe otro byte  
c) Se envía y luego se recibe  
d) Depende del dispositivo slave

**Respuesta correcta**: b) TX y RX son simultáneos en SPI

---

**Pregunta 3**: ¿Por qué CS debe controlarse manualmente desde GPIO?

a) Para ahorrar energía  
b) Para soportar múltiples slaves con líneas CS independientes  
c) Es más rápido  
d) Es obligatorio por el protocolo

**Respuesta correcta**: b) Cada slave necesita su propia línea CS

---

**Pregunta 4**: Si CPOL=1, ¿cuál es el nivel de SCK cuando está inactivo?

a) Alto  
b) Bajo  
c) Depende de CPHA  
d) No importa

**Respuesta correcta**: a) CPOL=1 significa reloj en reposo alto

---

**Pregunta 5**: ¿Qué se envía por MOSI al leer datos de un slave?

a) Nada, MOSI se desactiva  
b) Un comando de lectura  
c) Bytes dummy (generalmente 0xFF)  
d) Ceros

**Respuesta correcta**: c) Dummy bytes para generar pulsos de reloj

---

## Evidencias de Aprendizaje

Al finalizar la sesión, el estudiante debe entregar:

1. **Driver SPI1** completo y funcional
2. **Aplicación con sensor SPI** (MAX31855 u otro)
3. **Análisis comparativo** I2C vs SPI con mediciones

---

## Material para Casa

### Lectura para próxima sesión (CAN)

1. **Introduction to CAN** - Texas Instruments
2. **CAN Bus Explained** - CSS Electronics
3. **STM32 CAN peripheral** - Application Note

---

## Notas para el Instructor

### Errores Comunes

1. **Olvidar desactivar CS**: Slave queda activo permanentemente
2. **Modo de reloj incorrecto**: Verificar CPOL/CPHA del datasheet
3. **Baudrate muy alto**: Puede causar errores con cables largos
4. **No leer DR después de TX**: Causa overflow en buffer RX

### Extensiones

- **DMA con SPI**: Para transferencias grandes
- **SPI con interrupciones**: Para aplicaciones no bloqueantes
- **Quad-SPI**: Para memorias flash de alta velocidad

### Tiempo de Sesión

- Teoría: 40 min
- Actividades: 70 min
- Cierre: 10 min

**Total**: 120 minutos

---

## Referencias

1. **STM32F407 Reference Manual** (RM0090), Sección 28: SPI
2. **MAX31855 Datasheet** - Maxim Integrated
3. **SPI Block Guide** - Motorola/NXP

---

**Fin de Sesión 5**
