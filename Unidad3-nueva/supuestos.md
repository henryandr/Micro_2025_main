# Supuestos Técnicos - Unidad 3

## Conocimientos Previos Requeridos

### De Unidad 1 (Assembly)
- ✅ Comprensión de arquitectura ARM Cortex M-4
- ✅ Conocimiento de registros y manipulación a nivel de bits
- ✅ Experiencia con GPIO en Assembly
- ✅ Conceptos básicos de interrupciones y NVIC

### De Unidad 2 (C Bare Metal)
- ✅ Programación en C para sistemas embebidos
- ✅ Uso de `volatile` para registros de hardware
- ✅ Estructuras y punteros a registros
- ✅ Desarrollo de drivers modulares (GPIO, Timer)
- ✅ Máquinas de Estados Finitos (MEF)
- ✅ Manejo de interrupciones en C
- ✅ Uso de Timer para bases de tiempo

### Conceptos Generales
- ✅ Comunicación digital básica (niveles lógicos, flancos)
- ✅ Conceptos de sincronismo y asincronismo
- ✅ Interpretación de diagramas de tiempo (timing diagrams)
- ✅ Lectura de datasheets y manuales de referencia

---

## Microcontrolador de Referencia

### Hardware Base
- **MCU**: STM32F407VGT6 (ARM Cortex M-4F a 168 MHz)
- **Placa**: STM32F407 Discovery o equivalente
- **Memoria**: 1 MB Flash, 192 KB RAM

### Periféricos Utilizados

#### USART (Universal Synchronous/Asynchronous Receiver Transmitter)
- **Periféricos disponibles**: USART1, USART2, USART3, UART4, UART5, USART6
- **Uso principal**: USART2 (conectado a USB-Serial en Discovery)
- **Pines típicos**:
  - USART2_TX: PA2 (AF7)
  - USART2_RX: PA3 (AF7)

#### I2C (Inter-Integrated Circuit)
- **Periféricos disponibles**: I2C1, I2C2, I2C3
- **Uso principal**: I2C1
- **Pines típicos**:
  - I2C1_SCL: PB6 (AF4) o PB8 (AF4)
  - I2C1_SDA: PB7 (AF4) o PB9 (AF4)
- **Velocidades soportadas**: 
  - Standard mode: 100 kHz
  - Fast mode: 400 kHz
  - Fast mode plus: 1 MHz (con configuración especial)

#### SPI (Serial Peripheral Interface)
- **Periféricos disponibles**: SPI1, SPI2, SPI3
- **Uso principal**: SPI1
- **Pines típicos**:
  - SPI1_SCK: PA5 (AF5)
  - SPI1_MISO: PA6 (AF5)
  - SPI1_MOSI: PA7 (AF5)
  - SPI1_NSS: PA4 (AF5) - opcional, se puede usar GPIO
- **Velocidades**: Hasta 42 MHz (APB2/2)

#### CAN (Controller Area Network)
- **Periféricos disponibles**: CAN1, CAN2
- **Uso principal**: CAN1
- **Pines típicos**:
  - CAN1_TX: PA12 (AF9) o PB9 (AF9)
  - CAN1_RX: PA11 (AF9) o PB8 (AF9)
- **Baudrates típicos**: 125 kbps, 250 kbps, 500 kbps, 1 Mbps
- **Nota**: Requiere transceiver externo (MCP2551 o similar)

---

## Periféricos Externos Asumidos

### Para I2C

#### Memoria EEPROM
- **Modelo de referencia**: AT24C32, AT24C64, AT24C256
- **Dirección típica**: 0x50 (7-bit)
- **Capacidades**: 4KB, 8KB, 32KB

#### Sensor de Temperatura/Humedad
- **Modelo de referencia**: BME280, SHT31, Si7021
- **Dirección típica**: 0x76 o 0x77 (BME280)

#### Pantalla OLED I2C
- **Modelo de referencia**: Display 0.96" con SSD1306
- **Resolución**: 128x64 píxeles
- **Dirección típica**: 0x3C o 0x3D
- **Interfaz**: I2C (también disponible en SPI)

### Para SPI

#### Pantalla OLED SPI
- **Modelo de referencia**: Display 0.96" con SSD1306/SH1106
- **Resolución**: 128x64 píxeles
- **Interfaz**: SPI de 4 hilos (SCK, MOSI, DC, CS, RST)

#### Sensor de Temperatura
- **Modelo de referencia**: MAX31855, MAX6675 (termopar)
- **Interfaz**: SPI solo lectura

#### SD Card (opcional)
- **Interfaz**: SPI mode
- **Uso**: Almacenamiento de datos

### Para CAN

#### Transceiver CAN
- **Modelo de referencia**: MCP2551, TJA1050, SN65HVD230
- **Función**: Adaptación de niveles lógicos a señales diferenciales CAN
- **Obligatorio**: El STM32 no incluye transceiver interno

#### Segundo Nodo (opcional)
- Otro STM32 o placa con CAN para pruebas de red

---

## Toolchain y Herramientas

### Compilación y Enlazado
- **Compilador**: `arm-none-eabi-gcc` (versión 10.3 o superior)
- **Ensamblador**: `arm-none-eabi-as`
- **Linker**: `arm-none-eabi-ld`
- **Utilidades**: `arm-none-eabi-objcopy`, `arm-none-eabi-size`
- **Debugger**: `arm-none-eabi-gdb`

### Programación y Debugging
- **Programador**: ST-Link V2 (integrado en Discovery)
- **Software**: OpenOCD o ST-Link Utilities
- **GDB Server**: OpenOCD

### Terminal Serial
- **Linux/Mac**: `minicom`, `screen`, `picocom`
- **Windows**: PuTTY, TeraTerm, CoolTerm
- **Multiplataforma**: Arduino IDE Serial Monitor

### Analizador Lógico (Recomendado)
- **Hardware**: Saleae Logic, USBee, clones chinos
- **Software**: Logic 2 (Saleae), PulseView (Sigrok)
- **Uso**: Depuración de protocolos I2C, SPI, USART

### CAN Bus Tools (opcional)
- **Hardware**: CANable, PCAN-USB, PEAK CAN
- **Software**: can-utils (Linux), BUSMASTER (Windows)

---

## Direcciones de Memoria Base

### Periféricos de Comunicación

```c
// USART/UART
#define USART1_BASE    0x40011000
#define USART2_BASE    0x40004400
#define USART3_BASE    0x40004800
#define UART4_BASE     0x40004C00
#define UART5_BASE     0x40005000
#define USART6_BASE    0x40011400

// I2C
#define I2C1_BASE      0x40005400
#define I2C2_BASE      0x40005800
#define I2C3_BASE      0x40005C00

// SPI
#define SPI1_BASE      0x40013000
#define SPI2_BASE      0x40003800
#define SPI3_BASE      0x40003C00

// CAN
#define CAN1_BASE      0x40006400
#define CAN2_BASE      0x40006800
```

### Registros de Control de Reloj (RCC)

```c
#define RCC_BASE       0x40023800

// Offsets relevantes
#define RCC_APB1ENR    0x40  // Periféricos APB1 (I2C, SPI2/3, UART)
#define RCC_APB2ENR    0x44  // Periféricos APB2 (USART1/6, SPI1)
#define RCC_AHB1ENR    0x30  // GPIO clocks
```

### Bits de Habilitación de Reloj

```c
// RCC_APB1ENR
#define RCC_APB1ENR_USART2EN  (1 << 17)
#define RCC_APB1ENR_USART3EN  (1 << 18)
#define RCC_APB1ENR_UART4EN   (1 << 19)
#define RCC_APB1ENR_UART5EN   (1 << 20)
#define RCC_APB1ENR_I2C1EN    (1 << 21)
#define RCC_APB1ENR_I2C2EN    (1 << 22)
#define RCC_APB1ENR_I2C3EN    (1 << 23)
#define RCC_APB1ENR_CAN1EN    (1 << 25)
#define RCC_APB1ENR_CAN2EN    (1 << 26)
#define RCC_APB1ENR_SPI2EN    (1 << 14)
#define RCC_APB1ENR_SPI3EN    (1 << 15)

// RCC_APB2ENR
#define RCC_APB2ENR_USART1EN  (1 << 4)
#define RCC_APB2ENR_USART6EN  (1 << 5)
#define RCC_APB2ENR_SPI1EN    (1 << 12)
```

---

## Configuración de Clocks

### Frecuencias del Sistema

```
System Clock (SYSCLK): 168 MHz
AHB Clock (HCLK):      168 MHz (prescaler 1)
APB1 Clock (PCLK1):    42 MHz  (prescaler 4)
APB2 Clock (PCLK2):    84 MHz  (prescaler 2)
```

### Implicaciones para Baudrate

**USART**: 
- USART1, USART6 usan PCLK2 (84 MHz)
- USART2, USART3, UART4, UART5 usan PCLK1 (42 MHz)

**I2C**: 
- Usa PCLK1 (42 MHz)

**SPI**:
- SPI1 usa PCLK2 (84 MHz)
- SPI2, SPI3 usan PCLK1 (42 MHz)

**CAN**:
- Usa PCLK1 (42 MHz)

### Cálculo de Baudrate

#### USART
```c
// Baudrate = fPCLK / (8 × (2 - OVER8) × USARTDIV)
// Para OVER8=0 (oversampling x16):
// USARTDIV = fPCLK / (16 × Baudrate)

// Ejemplo: 115200 bps en USART2 (PCLK1 = 42 MHz)
// USARTDIV = 42000000 / (16 × 115200) = 22.786
// BRR = 0x16C (mantissa=22, fraction=12.56 ≈ 13)
```

#### I2C
```c
// Para 100 kHz (Standard mode) con PCLK1 = 42 MHz
// CCR = PCLK1 / (2 × I2C_CLK) = 42000000 / (2 × 100000) = 210

// Para 400 kHz (Fast mode)
// CCR = PCLK1 / (3 × I2C_CLK) = 42000000 / (3 × 400000) = 35
// (en modo Duty 16/9: CCR = PCLK1 / (25 × I2C_CLK))
```

#### SPI
```c
// Prescaler disponibles: 2, 4, 8, 16, 32, 64, 128, 256
// Para SPI1 (PCLK2 = 84 MHz):
// - BR[2:0] = 0: 84 MHz / 2 = 42 MHz
// - BR[2:0] = 1: 84 MHz / 4 = 21 MHz
// - BR[2:0] = 2: 84 MHz / 8 = 10.5 MHz
// - etc.
```

#### CAN
```c
// Baudrate configurado mediante BRP, TS1, TS2, SJW
// Ejemplo para 500 kbps con PCLK1 = 42 MHz:
// Time quanta = 1 / (42 MHz / (BRP+1))
// Bit time = (1 + TS1 + TS2) × time quanta
// Configuración típica: BRP=5, TS1=13, TS2=2
// Baudrate = 42000000 / ((5+1) × (1+13+2)) = 437.5 kbps ≈ 500 kbps
```

---

## Mapa de Memoria

```
0x0800 0000 - 0x080F FFFF : Flash (1 MB)
0x2000 0000 - 0x2001 FFFF : SRAM (128 KB)
0x2001 C000 - 0x2001 FFFF : SRAM adicional (64 KB core coupled)
0x4000 0000 - 0x5FFF FFFF : Periféricos
0xE000 0000 - 0xE00F FFFF : Cortex M-4 internos (NVIC, SysTick, etc.)
```

---

## Configuración de Pines (Alternate Functions)

### USART2 (más común para debug)
```c
// PA2: USART2_TX (AF7)
// PA3: USART2_RX (AF7)
GPIOA->MODER &= ~((3 << (2*2)) | (3 << (3*2)));
GPIOA->MODER |= (2 << (2*2)) | (2 << (3*2));  // Alternate function
GPIOA->AFR[0] &= ~((0xF << (2*4)) | (0xF << (3*4)));
GPIOA->AFR[0] |= (7 << (2*4)) | (7 << (3*4));  // AF7
```

### I2C1
```c
// PB6: I2C1_SCL (AF4)
// PB7: I2C1_SDA (AF4)
GPIOB->MODER &= ~((3 << (6*2)) | (3 << (7*2)));
GPIOB->MODER |= (2 << (6*2)) | (2 << (7*2));  // Alternate function
GPIOB->OTYPER |= (1 << 6) | (1 << 7);  // Open-drain
GPIOB->PUPDR |= (1 << (6*2)) | (1 << (7*2));  // Pull-up
GPIOB->AFR[0] &= ~((0xF << (6*4)) | (0xF << (7*4)));
GPIOB->AFR[0] |= (4 << (6*4)) | (4 << (7*4));  // AF4
```

### SPI1
```c
// PA5: SPI1_SCK (AF5)
// PA6: SPI1_MISO (AF5)
// PA7: SPI1_MOSI (AF5)
GPIOA->MODER &= ~((3 << (5*2)) | (3 << (6*2)) | (3 << (7*2)));
GPIOA->MODER |= (2 << (5*2)) | (2 << (6*2)) | (2 << (7*2));
GPIOA->AFR[0] &= ~((0xF << (5*4)) | (0xF << (6*4)) | (0xF << (7*4)));
GPIOA->AFR[0] |= (5 << (5*4)) | (5 << (6*4)) | (5 << (7*4));  // AF5
```

### CAN1
```c
// PA11: CAN1_RX (AF9)
// PA12: CAN1_TX (AF9)
GPIOA->MODER &= ~((3 << (11*2)) | (3 << (12*2)));
GPIOA->MODER |= (2 << (11*2)) | (2 << (12*2));
GPIOA->AFR[1] &= ~((0xF << ((11-8)*4)) | (0xF << ((12-8)*4)));
GPIOA->AFR[1] |= (9 << ((11-8)*4)) | (9 << ((12-8)*4));  // AF9
```

---

## Interrupciones

### NVIC IRQ Numbers

```c
// USART
#define USART1_IRQn    37
#define USART2_IRQn    38
#define USART3_IRQn    39
#define UART4_IRQn     52
#define UART5_IRQn     53
#define USART6_IRQn    71

// I2C
#define I2C1_EV_IRQn   31  // Event interrupt
#define I2C1_ER_IRQn   32  // Error interrupt
#define I2C2_EV_IRQn   33
#define I2C2_ER_IRQn   34
#define I2C3_EV_IRQn   72
#define I2C3_ER_IRQn   73

// SPI
#define SPI1_IRQn      35
#define SPI2_IRQn      36
#define SPI3_IRQn      51

// CAN
#define CAN1_TX_IRQn   19
#define CAN1_RX0_IRQn  20
#define CAN1_RX1_IRQn  21
#define CAN1_SCE_IRQn  22
```

### Prioridades de Interrupción

En esta unidad se sugieren las siguientes prioridades (menor número = mayor prioridad):

```c
// Prioridad 0: Eventos críticos de tiempo real
// CAN (comunicación crítica)

// Prioridad 1: Comunicaciones de alta prioridad  
// SPI (puede ser muy rápido)

// Prioridad 2: Comunicaciones medias
// I2C, USART (datos)

// Prioridad 3: Logging y debugging
// USART (solo para logs)
```

---

## Limitaciones y Consideraciones

### Hardware

1. **Transceiver CAN obligatorio**: El STM32 no incluye el transceiver
2. **Pull-ups I2C**: Requeridos externamente (típicamente 4.7kΩ)
3. **Niveles lógicos**: 3.3V, no 5V-tolerant en todos los pines
4. **Velocidad SPI**: Limitada por cables y dispositivos externos

### Software

1. **Sin HAL**: Todo el código es bare metal, acceso directo a registros
2. **Sin malloc**: Se evita asignación dinámica para sistemas embebidos
3. **Buffers estáticos**: Se usan arrays estáticos o circulares
4. **Timeouts obligatorios**: Para evitar bloqueos infinitos

### Debugging

1. **USART preferido**: Para debugging en tiempo real
2. **Logic analyzer**: Muy recomendado para protocolos
3. **Osciloscopio**: Útil para verificar señales físicas
4. **Printf**: Funcional pero agrega latencia

---

## Referencias de Documentación

### Documentos Principales

1. **RM0090**: STM32F4 Reference Manual (1700+ páginas)
   - Capítulo 25: USART
   - Capítulo 27: I2C
   - Capítulo 28: SPI
   - Capítulo 30: CAN

2. **DS8626**: STM32F407xx Datasheet
   - Pinout y alternate functions
   - Características eléctricas

3. **PM0214**: STM32F4 Programming Manual
   - Instruction set
   - Cortex M-4 specifics

### Application Notes

- **AN4031**: Using the STM32 hardware I2C
- **AN2606**: System memory boot mode
- **AN3281**: CAN protocol in STM32 bootloader

### Datasheets de Periféricos

- **SSD1306**: OLED display controller
- **AT24Cxx**: I2C EEPROM family
- **MCP2551**: CAN transceiver
- **BME280**: Environmental sensor (I2C/SPI)

---

## Estructura de Archivos Asumida

```
proyecto/
├── src/
│   ├── main.c
│   ├── drivers/
│   │   ├── usart_driver.c
│   │   ├── i2c_driver.c
│   │   ├── spi_driver.c
│   │   └── can_driver.c
│   ├── devices/
│   │   ├── oled_i2c.c
│   │   ├── oled_spi.c
│   │   └── eeprom_i2c.c
│   └── startup_stm32f407.s
├── inc/
│   ├── stm32f407xx.h
│   ├── usart_driver.h
│   ├── i2c_driver.h
│   ├── spi_driver.h
│   ├── can_driver.h
│   └── devices/
│       ├── oled_i2c.h
│       ├── oled_spi.h
│       └── eeprom_i2c.h
├── linker_script.ld
└── Makefile
```

---

## Supuestos de Implementación

### Manejo de Errores

Todos los drivers deben retornar códigos de error:

```c
typedef enum {
    STATUS_OK = 0,
    STATUS_ERROR,
    STATUS_TIMEOUT,
    STATUS_BUSY,
    STATUS_NACK,     // I2C specific
    STATUS_COLLISION // CAN specific
} Status_t;
```

### Timeouts

Todos los bucles de espera deben tener timeout:

```c
#define TIMEOUT_MS  1000

uint32_t timeout = TIMEOUT_MS;
while (!(periférico->SR & FLAG) && timeout > 0) {
    timeout--;
    delay_ms(1);
}
if (timeout == 0) return STATUS_TIMEOUT;
```

### Buffering

Se utilizan buffers circulares para USART:

```c
#define BUFFER_SIZE 256

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
} CircularBuffer_t;
```

---

**Nota Final**: Estos supuestos son la base para toda la unidad. Cualquier desviación debe ser documentada explícitamente en el código.
