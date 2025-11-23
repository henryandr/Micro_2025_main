# Herramientas de Análisis para Protocolos Seriales

## 1. Analizadores Lógicos

### 1.1 Saleae Logic Pro

**Características**:
- 8-16 canales digitales
- Frecuencia de muestreo: hasta 500 MS/s
- Decodificadores integrados: UART, I2C, SPI, CAN, etc.
- Software multiplataforma (Windows, Mac, Linux)

**Ventajas**:
- Interfaz intuitiva
- Decodificación automática
- Exportación de datos (CSV, VCD)
- Búsqueda de patrones

**Precio**: $399-$1,449 USD

**Uso típico**:
```
Conexiones:
- Canal 0-1: USART TX/RX
- Canal 2-3: I2C SDA/SCL
- Canal 4-6: SPI MOSI/MISO/SCK
- Canal 7: Trigger/Debug pin
```

### 1.2 Logic Analyzer de 8 Canales (USB)

**Características**:
- 8 canales
- 24 MHz muestreo
- Compatible con sigrok/PulseView
- Bajo costo

**Ventajas**:
- Económico ($10-30 USD)
- Open source software
- Suficiente para la mayoría de aplicaciones

**Limitaciones**:
- Frecuencia limitada
- Sin entrada analógica

### 1.3 DSLogic Plus

**Características**:
- 16 canales digitales
- 400 MS/s
- Buffer de 256M muestras
- Compatible con sigrok

**Precio**: ~$150 USD

## 2. Osciloscopios Digitales

### 2.1 Rigol DS1054Z

**Características**:
- 4 canales analógicos
- 50 MHz bandwidth
- Decodificadores de protocolo (I2C, SPI, UART, CAN)
- 24 Mpts memoria

**Ventajas**:
- Ver señales analógicas (niveles, timing)
- Decodificación de protocolos
- Mediciones automáticas
- Buen precio/rendimiento

**Precio**: ~$350 USD

**Hack**: Actualizable a 100 MHz vía software

### 2.2 Siglent SDS1104X-E

**Características**:
- 4 canales
- 100 MHz
- Decodificadores incluidos
- Generador de funciones integrado

**Precio**: ~$400 USD

## 3. Software de Análisis

### 3.1 PulseView (sigrok)

**Características**:
- Open source
- Decodificadores de 100+ protocolos
- Compatible con múltiples hardware
- Análisis offline

**Instalación**:
```bash
# Ubuntu/Debian
sudo apt-get install pulseview

# Windows
# Descargar de sigrok.org
```

**Uso**:
1. Conectar analizador lógico
2. Seleccionar canales
3. Agregar decodificadores
4. Capturar y analizar

### 3.2 Logic 2 (Saleae)

**Características**:
- Software oficial de Saleae
- Interfaz moderna
- Mediciones avanzadas
- Exportación flexible

**Descarga**: saleae.com

### 3.3 ScanaStudio

**Características**:
- Software avanzado de análisis
- Generación de señales
- Scripting con JavaScript
- Protocolo personalizado

**Compatibilidad**:
- Ikalogic devices
- Algunos clones genéricos

## 4. Herramientas Específicas de Protocolo

### 4.1 CAN Tools

#### PCAN-USB (Peak System)

**Características**:
- Adaptador USB-CAN
- Software PCAN-View incluido
- Alta confiabilidad
- Soporte de 11 y 29-bit IDs

**Precio**: ~$200 USD

**Software**: PCAN-View para Windows/Linux

#### CANalyzer (Vector)

**Características**:
- Software profesional de análisis CAN
- Simulación de nodos
- Scripting (CAPL)
- Generación de tráfico

**Precio**: $$$$ (licencia corporativa)

**Alternativa open source**: python-can + can-utils

### 4.2 I2C Tools

#### Total Phase Aardvark I2C/SPI

**Características**:
- Adaptador USB-I2C/SPI
- Master y slave mode
- Software Data Center incluido
- Python/C API

**Precio**: $250-350 USD

#### Bus Pirate

**Características**:
- Open source
- Múltiples protocolos (UART, I2C, SPI, 1-Wire)
- Terminal interactivo
- Bajo costo

**Precio**: ~$30 USD

**Comandos**:
```
# Entrar a modo I2C
m  (elegir I2C)
# Scan de bus
(1)
# Leer EEPROM
[0xa0 0x00 [0xa1 r:8]
```

### 4.3 SPI Tools

#### FT232H (FTDI)

**Características**:
- USB a SPI/I2C/GPIO
- Soporte de librerías (PyFTDI, libftdi)
- Velocidad hasta 30 Mbps
- Económico

**Precio**: ~$15 USD (breakout board)

**Uso con Python**:
```python
from pyftdi.spi import SpiController

spi = SpiController()
spi.configure('ftdi://ftdi:232h/1')
slave = spi.get_port(cs=0, freq=1E6)
data = slave.exchange([0x9F], 3)  # Read ID
```

## 5. Herramientas de Desarrollo

### 5.1 IDE con Debugging

#### STM32CubeIDE

**Características**:
- Based on Eclipse
- Debugger GDB integrado
- Breakpoints, watch, memoria
- Gratis

**Descarga**: st.com

#### Keil MDK-ARM

**Características**:
- Debugger profesional
- Event Recorder
- Logic Analyzer virtual
- Performance Analyzer

**Precio**: Gratis (limitado a 32KB), $$$ (full)

### 5.2 Debuggers de Hardware

#### ST-Link V3

**Características**:
- SWD debugging
- Virtual COM port
- Velocidad alta
- Standalone o integrado

**Precio**: ~$30 USD (clone), $90 USD (original)

#### J-Link EDU

**Características**:
- Debugger profesional
- RTT (Real-Time Transfer)
- Unlimited Flash Breakpoints
- Educacional

**Precio**: $60 USD (EDU), $$$ (comercial)

**Ventaja RTT**:
- Printf sin usar USART
- No afecta timing
- Alta velocidad

## 6. Herramientas de Simulación

### 6.1 Proteus

**Características**:
- Simulación de circuitos
- Soporte STM32
- Visualización de señales
- Virtual instruments

**Precio**: $$$

### 6.2 QEMU

**Características**:
- Emulador open source
- Soporte básico de ARM Cortex-M
- Testing sin hardware
- CI/CD integration

**Uso**:
```bash
qemu-system-arm -machine netduino2 \
  -kernel firmware.elf \
  -serial stdio
```

## 7. Herramientas Online

### 7.1 Calculadoras

**Baudrate Calculator**:
- stm32duino.com/baudrate
- Calcula BRR para USART

**I2C Timing Calculator**:
- st.com (AN4235)
- Calcula CCR y TRISE

### 7.2 Conversores

**Hex/Binary/Decimal Converter**:
- rapidtables.com/convert/number

**ASCII Table**:
- asciitable.com

## 8. Recomendaciones por Presupuesto

### Bajo Presupuesto (< $100)

- Analizador lógico USB 8ch: $20
- Bus Pirate: $30
- ST-Link clone: $10
- Software: sigrok (gratis)

**Total**: ~$60

### Presupuesto Medio ($100-500)

- Saleae Logic 8: $400
- FT232H breakout: $15
- ST-Link V3: $30
- PuTTY/minicom: gratis

**Total**: ~$445

### Presupuesto Alto (> $500)

- Osciloscopio Rigol DS1054Z: $350
- Saleae Logic Pro 8: $900
- PCAN-USB: $200
- J-Link EDU: $60

**Total**: ~$1,510

## 9. Setup Recomendado para Curso

**Mínimo necesario**:
- Analizador lógico USB (8 canales)
- Software sigrok/PulseView
- Terminal serial (PuTTY)
- Multímetro

**Ideal**:
- Osciloscopio básico (4 canales)
- Analizador lógico Saleae
- Bus Pirate o FT232H
- Adaptadores USB para cada protocolo

## 10. Tips de Uso

1. **Siempre usar GND común** entre dispositivos y herramientas
2. **Cuidado con niveles**: 3.3V vs 5V
3. **Capturar antes y después** del evento de interés
4. **Usar triggers** para eventos específicos
5. **Documentar configuraciones** (baudrate, canales, etc.)
6. **Exportar datos** para análisis posterior
7. **Verificar con múltiples herramientas** cuando hay dudas

---

**Última actualización**: 2025
**Fuente**: Experiencia práctica y especificaciones de fabricantes
