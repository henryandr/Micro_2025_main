# Comparación de Protocolos de Comunicación Serial

## Tabla Comparativa General

| Característica | USART | I2C | SPI | CAN |
|----------------|-------|-----|-----|-----|
| **Cables** | 2-4 | 2 | 3-4+ | 2 |
| **Topología** | Punto a punto | Multi-master | Master-slave | Multi-master |
| **Velocidad típica** | 9600-115200 bps | 100-400 kHz | 1-20 MHz | 125 kbps-1 Mbps |
| **Velocidad máxima** | 10 Mbps | 3.4 MHz | 50+ MHz | 1 Mbps |
| **Distancia típica** | 15 m | 1-2 m | < 1 m | 40 m |
| **Número de dispositivos** | 2 | 127 | Limitado por CS | Ilimitado |
| **Direccionamiento** | No | Sí (7/10 bits) | CS físico | ID (11/29 bits) |
| **Full-duplex** | Sí | No | Sí | No |
| **Detección de errores** | Opcional (paridad) | Básica (ACK) | No | Avanzada (CRC) |
| **Costo (pines)** | Bajo | Muy bajo | Medio-Alto | Bajo |
| **Complejidad** | Baja | Media | Baja-Media | Alta |
| **Alimentación** | N/A | N/A | N/A | Diferencial |

## Comparación Detallada

### 1. Velocidad y Throughput

**USART**:
- Baudrates comunes: 9600, 19200, 38400, 57600, 115200 bps
- Throughput real: ~80-90% (start/stop bits)
- Ventaja: Configuración simple
- Desventaja: Limitado para aplicaciones de alta velocidad

**I2C**:
- Standard: 100 kHz, Fast: 400 kHz, Fast Plus: 1 MHz
- Overhead de protocolo: ~30-40% (address, ACK, START/STOP)
- Ventaja: Suficiente para la mayoría de sensores
- Desventaja: Relativamente lento para displays o almacenamiento

**SPI**:
- Velocidades típicas: 1-20 MHz en STM32
- Overhead mínimo: full-duplex puro
- Ventaja: Máxima velocidad para distancias cortas
- Desventaja: Requiere más pines, no estandarizado

**CAN**:
- 125 kbps, 250 kbps, 500 kbps, 1 Mbps
- Overhead significativo (stuffing bits, CRC, ACK)
- Ventaja: Robusto a interferencias
- Desventaja: Velocidad limitada por longitud de cable

### 2. Complejidad de Implementación

**Facilidad de uso** (de menor a mayor complejidad):
1. **USART**: Más simple, solo TX/RX
2. **SPI**: Simple pero requiere gestión de CS
3. **I2C**: Requiere entender START/STOP, ACK, addressing
4. **CAN**: Más complejo: filtros, arbitraje, error handling

### 3. Casos de Uso Típicos

#### USART
**Mejor para**:
- Comunicación con PC (debug, logs)
- GPS modules
- Bluetooth/WiFi modules
- Terminal de comandos

**Evitar para**:
- Múltiples dispositivos
- Distancias > 15m sin transceiver
- Aplicaciones que requieren alta velocidad

#### I2C
**Mejor para**:
- Sensores (temperatura, acelerómetro, etc.)
- Memorias EEPROM pequeñas
- RTCs (Real Time Clocks)
- Displays pequeños (OLED I2C)
- Múltiples dispositivos con pocos pines

**Evitar para**:
- Actualizaciones rápidas de displays grandes
- Transferencias de datos grandes
- Distancias > 2m
- Entornos con mucho ruido

#### SPI
**Mejor para**:
- Displays (OLED, TFT)
- Tarjetas SD/microSD
- Memorias flash
- ADC/DAC de alta velocidad
- Sensores de alta frecuencia de muestreo

**Evitar para**:
- Muchos dispositivos (requiere un CS por cada uno)
- Distancias > 1m
- Aplicaciones con pines limitados

#### CAN
**Mejor para**:
- Automotriz (comunicación entre ECUs)
- Robótica (comunicación entre módulos)
- Control industrial
- Entornos con ruido electromagnético
- Aplicaciones que requieren confiabilidad

**Evitar para**:
- Transferencias de datos grandes
- Aplicaciones de bajo costo (requiere transceiver)
- Comunicación con PC (requiere adaptador USB-CAN)

### 4. Consumo de Pines

**Ejemplo: Sistema con 5 periféricos**

| Protocolo | Pines MCU | Resistencias | Transceivers |
|-----------|-----------|--------------|--------------|
| USART × 5 | 10 (2 por cada) | No | Opcional |
| I2C | 2 (compartidos) | 2 pull-up | No |
| SPI | 8 (3 + 5 CS) | No | No |
| CAN | 2 (todos en bus) | 2×120Ω | Sí (1) |

**Conclusión**: I2C y CAN son más eficientes en uso de pines para múltiples dispositivos.

### 5. Costo Total del Sistema

**Consideraciones de costo** (ordenadas de menor a mayor):

1. **I2C**: Solo pull-ups, sin hardware adicional
2. **USART**: Sin hardware adicional para distancias cortas
3. **SPI**: Sin hardware adicional
4. **CAN**: Requiere transceiver (TJA1050, MCP2551) + resistencias de terminación

### 6. Robustez y Confiabilidad

**Orden de robustez** (menor a mayor):

1. **USART/SPI**: Sin detección de errores integrada
2. **I2C**: ACK/NACK básico
3. **CAN**: Detección avanzada (CRC, stuffing, error frames)

**Recomendaciones**:
- **Entornos industriales ruidosos**: CAN
- **Aplicaciones críticas**: CAN + redundancia
- **Prototipado rápido**: USART o I2C
- **Alta velocidad local**: SPI

### 7. Escalabilidad

**Agregar dispositivos**:

- **USART**: Difícil (solo punto a punto)
- **I2C**: Fácil (hasta 127 dispositivos)
- **SPI**: Posible pero requiere más pines CS
- **CAN**: Muy fácil (solo conectar al bus)

## Recomendaciones por Aplicación

### IoT Sensor Node
- Sensores: **I2C**
- Display local: **SPI**
- Comunicación WiFi: **USART**
- Red de nodos: **CAN** (si industrial)

### Robot Móvil
- Motores/encoders: **USART** o **CAN**
- Sensores: **I2C**
- IMU de alta frecuencia: **SPI**
- Display: **SPI**
- Comunicación entre módulos: **CAN**

### Adquisición de Datos
- Sensores lentos: **I2C**
- ADC rápido: **SPI**
- Almacenamiento (microSD): **SPI**
- PC: **USART**
- Sincronización: **CAN**

### Sistema Automotriz
- Comunicación ECUs: **CAN**
- Sensores: **I2C** o **CAN**
- Actuadores: **CAN**
- Diagnóstico: **CAN + USART**

## Conclusión

**No hay un protocolo "mejor" universalmente**. La elección depende de:
- Requisitos de velocidad
- Número de dispositivos
- Distancia
- Pines disponibles
- Costo
- Confiabilidad requerida
- Experiencia del desarrollador

**Estrategia recomendada**: Usar **múltiples protocolos** en el mismo sistema, cada uno para lo que hace mejor.

---

**Referencia**: Basado en especificaciones oficiales y experiencia práctica.
