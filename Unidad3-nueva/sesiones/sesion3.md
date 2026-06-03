# Sesión 3: Comunicación I2C - Fundamentos

**Unidad 3**: Protocolos de Comunicación Serial  
**Semana**: 2  
**Duración**: 2 horas  
**Modalidad**: Presencial con práctica en laboratorio

---

## Objetivos de Aprendizaje

Al finalizar esta sesión, el estudiante será capaz de:

1. Comprender el funcionamiento del protocolo I2C (Inter-Integrated Circuit)
2. Configurar el periférico I2C1 del STM32F407 en modo master
3. Realizar operaciones de lectura y escritura en dispositivos I2C
4. Implementar bus scanning para detectar dispositivos conectados
5. Comunicarse con una EEPROM I2C (AT24C64 o similar)

**Tiempo estimado de actividades**: 90 minutos (teoría: 40 min, práctica: 50 min)

---

## 1. Introducción al Protocolo I2C

### 1.1 ¿Qué es I2C?

I2C (Inter-Integrated Circuit) es un protocolo de comunicación serial síncrono desarrollado por Philips (ahora NXP) en 1982. Es ampliamente usado para comunicación entre microcontroladores y periféricos de baja velocidad.

**Características principales**:
- Bus de 2 cables: **SDA** (datos) y **SCL** (reloj)
- Multi-master, multi-slave (hasta 127 dispositivos)
- Velocidades: Standard (100 kHz), Fast (400 kHz), Fast Plus (1 MHz)
- Direccionamiento de 7 o 10 bits
- Acknowledgement (ACK/NACK) en cada byte

### 1.2 Topología del Bus I2C

```
        VDD
         |
        [Pull-up 4.7kΩ]    [Pull-up 4.7kΩ]
         |                   |
    SDA  |              SCL  |
    -----+-----------------+----- 
         |                 |
    +----+----+       +----+----+       +----+----+
    | Master  |       | Slave 1 |       | Slave 2 |
    | (MCU)   |       | (EEPROM)|       | (Sensor)|
    +---------+       +---------+       +---------+
      Addr: --          Addr: 0x50        Addr: 0x76
```

**Resistencias pull-up**: Obligatorias porque I2C usa salidas open-drain. Típicamente 4.7kΩ para 100kHz, 2.2kΩ para 400kHz.

### 1.3 Formato de Comunicación

**Secuencia típica de escritura**:
```
START | Addr+W | ACK | Data1 | ACK | Data2 | ACK | ... | STOP
  S   | 7 bits | A   | 8 bits| A   | 8 bits| A   |     | P
```

**Secuencia típica de lectura**:
```
START | Addr+R | ACK | Data1 | ACK | Data2 | NACK | STOP
  S   | 7 bits | A   | 8 bits| A   | 8 bits| N    | P
```

**Bits especiales**:
- **START (S)**: SCL alto, SDA cae de alto a bajo
- **STOP (P)**: SCL alto, SDA sube de bajo a alto
- **ACK/NACK**: Bit de reconocimiento (0=ACK, 1=NACK)
- **R/W**: Bit 0 del byte de dirección (0=Write, 1=Read)

---

## 2. I2C en STM32F407

### 2.1 Periféricos I2C Disponibles

El STM32F407 tiene **3 periféricos I2C** (I2C1, I2C2, I2C3):

| Periférico | Pines SCL | Pines SDA | Bus APB |
|------------|-----------|-----------|---------|
| I2C1 | PB6, PB8 | PB7, PB9 | APB1 (42 MHz) |
| I2C2 | PB10, PF1 | PB11, PF0 | APB1 (42 MHz) |
| I2C3 | PA8, PH7 | PC9, PH8 | APB1 (42 MHz) |

Usaremos **I2C1** con **PB6** (SCL) y **PB7** (SDA).

### 2.2 Registros Principales de I2C

| Registro | Descripción |
|----------|-------------|
| I2C_CR1 | Control register 1 (enable, ACK, START, STOP) |
| I2C_CR2 | Control register 2 (frecuencia APB, interrupciones) |
| I2C_OAR1 | Own address register (dirección del MCU como slave) |
| I2C_DR | Data register (transmisión/recepción) |
| I2C_SR1 | Status register 1 (flags de eventos) |
| I2C_SR2 | Status register 2 (flags de bus) |
| I2C_CCR | Clock control register (velocidad del bus) |
| I2C_TRISE | Rise time register (tiempo de subida) |

---

## 3. Configuración de I2C1

### 3.1 Paso 1: Habilitar Clocks

```c
/**
 * @brief Inicializa clocks para I2C1
 */
void I2C1_ClockInit(void) {
    // Habilitar clock de GPIOB (para PB6 y PB7)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    
    // Habilitar clock de I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
}
```

### 3.2 Paso 2: Configurar Pines GPIO

```c
/**
 * @brief Configura PB6 (SCL) y PB7 (SDA) para I2C1
 */
void I2C1_GPIO_Init(void) {
    // PB6 y PB7: Modo alternate function
    GPIOB->MODER &= ~(0xF << (6*2));  // Limpiar bits
    GPIOB->MODER |= (0xA << (6*2));   // AF mode (10b) para PB6 y PB7
    
    // Configurar como open-drain (requisito de I2C)
    GPIOB->OTYPER |= (1 << 6) | (1 << 7);
    
    // Velocidad alta
    GPIOB->OSPEEDR |= (0xF << (6*2));  // Very high speed
    
    // Pull-up (opcional si hay resistencias externas)
    GPIOB->PUPDR &= ~(0xF << (6*2));
    GPIOB->PUPDR |= (0x5 << (6*2));    // Pull-up
    
    // Seleccionar AF4 (I2C1) para PB6 y PB7
    GPIOB->AFR[0] &= ~(0xFF << (6*4));  // Limpiar AF de PB6 y PB7
    GPIOB->AFR[0] |= (0x44 << (6*4));   // AF4 para ambos
}
```

### 3.3 Paso 3: Configurar I2C1 (100 kHz)

```c
/**
 * @brief Configura I2C1 en modo master a 100 kHz
 * 
 * Asume APB1 = 42 MHz
 */
void I2C1_Init(void) {
    // 1. Deshabilitar I2C1 para configuración
    I2C1->CR1 &= ~I2C_CR1_PE;
    
    // 2. Configurar frecuencia del bus APB1 (42 MHz)
    I2C1->CR2 &= ~I2C_CR2_FREQ;
    I2C1->CR2 |= 42;  // 42 MHz
    
    // 3. Configurar CCR para 100 kHz (Standard Mode)
    // T_high = T_low = CCR * T_PCLK1
    // T_scl = 2 * CCR * T_PCLK1
    // CCR = F_PCLK1 / (2 * F_I2C) = 42000000 / (2 * 100000) = 210
    I2C1->CCR &= ~I2C_CCR_CCR;
    I2C1->CCR |= 210;
    
    // 4. Configurar tiempo de subida (TRISE)
    // TRISE = (T_rise_max / T_PCLK1) + 1
    // Para 100 kHz: T_rise_max = 1000 ns
    // TRISE = (1000ns / 23.8ns) + 1 = 43
    I2C1->TRISE = 43;
    
    // 5. Configurar dirección propia (no usado en master, pero obligatorio)
    I2C1->OAR1 |= (1 << 14);  // Bit 14 debe ser 1 por software
    I2C1->OAR1 |= (0x33 << 1); // Dirección arbitraria 0x33
    
    // 6. Habilitar I2C1
    I2C1->CR1 |= I2C_CR1_PE;
    
    // 7. Habilitar ACK
    I2C1->CR1 |= I2C_CR1_ACK;
}
```

**Cálculo de velocidad** (modo estándar):
- Frecuencia APB1: 42 MHz
- Frecuencia I2C deseada: 100 kHz
- CCR = F_APB1 / (2 × F_I2C) = 42,000,000 / 200,000 = 210

---

## 4. Operaciones Básicas I2C

### 4.1 Generar START Condition

```c
/**
 * @brief Genera condición START y espera a que el bus esté libre
 * @return 0 si éxito, -1 si timeout
 */
int I2C1_Start(void) {
    uint32_t timeout = 10000;
    
    // Generar START
    I2C1->CR1 |= I2C_CR1_START;
    
    // Esperar flag SB (START bit sent)
    while (!(I2C1->SR1 & I2C_SR1_SB) && timeout--);
    
    if (timeout == 0) {
        return -1;  // Timeout
    }
    
    return 0;
}
```

### 4.2 Enviar Dirección del Slave

```c
/**
 * @brief Envía dirección del slave + bit R/W
 * @param address Dirección de 7 bits
 * @param read 0=Write, 1=Read
 * @return 0 si ACK recibido, -1 si NACK o timeout
 */
int I2C1_SendAddress(uint8_t address, uint8_t read) {
    uint32_t timeout = 10000;
    
    // Enviar dirección (7 bits) + bit R/W
    I2C1->DR = (address << 1) | (read & 0x01);
    
    // Esperar flag ADDR (Address sent)
    while (!(I2C1->SR1 & I2C_SR1_ADDR) && timeout--);
    
    if (timeout == 0) {
        return -1;  // Timeout o NACK
    }
    
    // Limpiar flag ADDR leyendo SR1 y SR2
    (void)I2C1->SR1;
    (void)I2C1->SR2;
    
    return 0;
}
```

### 4.3 Escribir un Byte

```c
/**
 * @brief Escribe un byte por I2C
 * @param data Byte a transmitir
 * @return 0 si éxito, -1 si timeout
 */
int I2C1_WriteByte(uint8_t data) {
    uint32_t timeout = 10000;
    
    // Esperar que DR esté vacío (TXE flag)
    while (!(I2C1->SR1 & I2C_SR1_TXE) && timeout--);
    
    if (timeout == 0) {
        return -1;
    }
    
    // Escribir dato
    I2C1->DR = data;
    
    // Esperar que transmisión termine (BTF flag)
    timeout = 10000;
    while (!(I2C1->SR1 & I2C_SR1_BTF) && timeout--);
    
    return (timeout == 0) ? -1 : 0;
}
```

### 4.4 Leer un Byte

```c
/**
 * @brief Lee un byte por I2C
 * @param last_byte 1 si es el último byte (enviar NACK)
 * @return Byte leído o -1 si error
 */
int I2C1_ReadByte(uint8_t last_byte) {
    uint32_t timeout = 10000;
    
    if (last_byte) {
        // Si es el último byte, deshabilitar ACK
        I2C1->CR1 &= ~I2C_CR1_ACK;
    } else {
        // Habilitar ACK para los demás bytes
        I2C1->CR1 |= I2C_CR1_ACK;
    }
    
    // Esperar dato recibido (RXNE flag)
    while (!(I2C1->SR1 & I2C_SR1_RXNE) && timeout--);
    
    if (timeout == 0) {
        return -1;
    }
    
    // Leer dato
    return I2C1->DR;
}
```

### 4.5 Generar STOP Condition

```c
/**
 * @brief Genera condición STOP
 */
void I2C1_Stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
}
```

---

## 5. Bus Scanning

Una herramienta útil para debugging es el **bus scanner**: envía direcciones a todos los dispositivos posibles (0x00-0x7F) y detecta cuáles responden con ACK.

```c
/**
 * @brief Escanea el bus I2C buscando dispositivos
 * 
 * Imprime direcciones de dispositivos que responden con ACK
 */
void I2C1_ScanBus(void) {
    printf("Escaneando bus I2C...\r\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\r\n");
    
    for (uint8_t addr = 0; addr < 128; addr++) {
        if (addr % 16 == 0) {
            printf("%02X ", addr);
        }
        
        // Generar START
        if (I2C1_Start() != 0) {
            printf("-- ");
            continue;
        }
        
        // Enviar dirección en modo write
        if (I2C1_SendAddress(addr, 0) == 0) {
            printf("%02X ", addr);  // Dispositivo encontrado
        } else {
            printf("-- ");
        }
        
        // Generar STOP
        I2C1_Stop();
        
        // Pequeña espera entre intentos
        for (volatile int i = 0; i < 1000; i++);
        
        if ((addr + 1) % 16 == 0) {
            printf("\r\n");
        }
    }
    
    printf("\r\n");
}
```

**Salida esperada** (ejemplo):
```
Escaneando bus I2C...
   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
00 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
10 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30 -- -- -- -- -- -- -- -- -- -- -- -- 3C -- -- --
40 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50 50 -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70 -- -- -- -- -- -- -- 77 -- -- -- -- -- -- -- --
```
En este ejemplo: dispositivos en 0x3C (OLED), 0x50 (EEPROM), 0x77 (BME280)

---

## 6. Comunicación con EEPROM I2C (AT24C64)

### 6.1 Características del AT24C64

- Capacidad: 64 Kbits (8 KB)
- Organización: 256 páginas de 32 bytes
- Dirección I2C: 0x50 (típica, puede variar con pines A0-A2)
- Escritura por página: hasta 32 bytes
- Tiempo de escritura: ~5 ms por página

### 6.2 Escritura de un Byte

```c
/**
 * @brief Escribe un byte en la EEPROM
 * @param mem_addr Dirección de memoria (0-8191)
 * @param data Byte a escribir
 * @return 0 si éxito, -1 si error
 */
int EEPROM_WriteByte(uint16_t mem_addr, uint8_t data) {
    // 1. START condition
    if (I2C1_Start() != 0) return -1;
    
    // 2. Enviar dirección del slave (EEPROM) + Write
    if (I2C1_SendAddress(0x50, 0) != 0) {
        I2C1_Stop();
        return -1;
    }
    
    // 3. Enviar dirección de memoria (2 bytes para AT24C64)
    if (I2C1_WriteByte((mem_addr >> 8) & 0xFF) != 0) {  // MSB
        I2C1_Stop();
        return -1;
    }
    if (I2C1_WriteByte(mem_addr & 0xFF) != 0) {  // LSB
        I2C1_Stop();
        return -1;
    }
    
    // 4. Enviar dato
    if (I2C1_WriteByte(data) != 0) {
        I2C1_Stop();
        return -1;
    }
    
    // 5. STOP condition
    I2C1_Stop();
    
    // 6. Esperar a que la EEPROM complete la escritura (~5 ms)
    delay_ms(5);
    
    return 0;
}
```

### 6.3 Lectura de un Byte

```c
/**
 * @brief Lee un byte de la EEPROM
 * @param mem_addr Dirección de memoria (0-8191)
 * @return Byte leído o -1 si error
 */
int EEPROM_ReadByte(uint16_t mem_addr) {
    int data;
    
    // 1. START condition
    if (I2C1_Start() != 0) return -1;
    
    // 2. Enviar dirección del slave + Write (para establecer puntero)
    if (I2C1_SendAddress(0x50, 0) != 0) {
        I2C1_Stop();
        return -1;
    }
    
    // 3. Enviar dirección de memoria
    if (I2C1_WriteByte((mem_addr >> 8) & 0xFF) != 0) {
        I2C1_Stop();
        return -1;
    }
    if (I2C1_WriteByte(mem_addr & 0xFF) != 0) {
        I2C1_Stop();
        return -1;
    }
    
    // 4. Repeated START
    if (I2C1_Start() != 0) {
        I2C1_Stop();
        return -1;
    }
    
    // 5. Enviar dirección del slave + Read
    if (I2C1_SendAddress(0x50, 1) != 0) {
        I2C1_Stop();
        return -1;
    }
    
    // 6. Leer byte (último byte, enviar NACK)
    data = I2C1_ReadByte(1);
    
    // 7. STOP condition
    I2C1_Stop();
    
    return data;
}
```

### 6.4 Escritura de Página (32 bytes)

```c
/**
 * @brief Escribe hasta 32 bytes en una página de EEPROM
 * @param mem_addr Dirección inicial (debe estar alineada a página)
 * @param data Puntero al buffer de datos
 * @param length Cantidad de bytes (máximo 32)
 * @return 0 si éxito, -1 si error
 */
int EEPROM_WritePage(uint16_t mem_addr, uint8_t *data, uint8_t length) {
    if (length > 32) return -1;
    
    // START + Address + Write
    if (I2C1_Start() != 0) return -1;
    if (I2C1_SendAddress(0x50, 0) != 0) {
        I2C1_Stop();
        return -1;
    }
    
    // Dirección de memoria
    if (I2C1_WriteByte((mem_addr >> 8) & 0xFF) != 0) {
        I2C1_Stop();
        return -1;
    }
    if (I2C1_WriteByte(mem_addr & 0xFF) != 0) {
        I2C1_Stop();
        return -1;
    }
    
    // Escribir todos los bytes
    for (uint8_t i = 0; i < length; i++) {
        if (I2C1_WriteByte(data[i]) != 0) {
            I2C1_Stop();
            return -1;
        }
    }
    
    // STOP
    I2C1_Stop();
    
    // Esperar escritura de página
    delay_ms(5);
    
    return 0;
}
```

---

## Actividades Prácticas

### Actividad 1: Bus Scanning (20 min)

**Objetivo**: Detectar dispositivos I2C conectados al bus.

**Tareas**:
1. Configurar I2C1 según código de esta sesión
2. Implementar función `I2C1_ScanBus()`
3. Conectar al menos un dispositivo I2C (EEPROM, sensor, OLED)
4. Ejecutar scan y verificar que detecta el dispositivo
5. Documentar direcciones encontradas

**Evidencia**: Captura de terminal mostrando scan exitoso + foto del montaje

---

### Actividad 2: EEPROM Write/Read (30 min)

**Objetivo**: Escribir y leer datos en una EEPROM I2C.

**Tareas**:
1. Implementar `EEPROM_WriteByte()` y `EEPROM_ReadByte()`
2. Escribir secuencia de bytes en direcciones 0x0000-0x000F
3. Leer misma secuencia y verificar integridad
4. Implementar test que:
   - Escriba 16 bytes
   - Lea 16 bytes
   - Compare y reporte si coinciden
5. Probar con ciclo de apagado/encendido (verificar persistencia)

**Evidencia**: Código fuente + captura mostrando test exitoso

---

### Actividad 3: Análisis con Osciloscopio/Analizador Lógico (20 min)

**Objetivo**: Visualizar señales I2C en tiempo real.

**Tareas**:
1. Conectar osciloscopio o analizador lógico a SDA y SCL
2. Capturar comunicación durante escritura de un byte
3. Identificar en la captura:
   - Condición START
   - Dirección del slave + bit W
   - ACK del slave
   - Byte de datos
   - ACK del dato
   - Condición STOP
4. Medir frecuencia del reloj SCL (debe ser ~100 kHz)

**Evidencia**: Foto de la captura con anotaciones de las partes identificadas

---

## Quiz Formativo

**Pregunta 1**: ¿Por qué I2C requiere resistencias pull-up externas?

a) Para incrementar la velocidad  
b) Porque usa salidas open-drain  
c) Para reducir consumo de corriente  
d) Para soportar múltiples voltajes

**Respuesta correcta**: b) I2C usa salidas open-drain que requieren pull-up

---

**Pregunta 2**: En una dirección I2C de 7 bits, ¿cuántas direcciones son teóricamente posibles?

a) 128  
b) 256  
c) 127  
d) 254

**Respuesta correcta**: a) 128 (0x00-0x7F), aunque algunas están reservadas

---

**Pregunta 3**: ¿Qué indica un NACK del slave después de enviar su dirección?

a) El dispositivo no existe en esa dirección  
b) El bus está ocupado  
c) Error de paridad  
d) El slave está en modo sleep

**Respuesta correcta**: a) No hay dispositivo o no está listo para comunicar

---

**Pregunta 4**: ¿Cuál es el propósito del "repeated START"?

a) Reiniciar la comunicación por error  
b) Cambiar de write a read sin liberar el bus  
c) Aumentar la velocidad  
d) Enviar múltiples bytes

**Respuesta correcta**: b) Permite cambiar dirección sin generar STOP

---

**Pregunta 5**: En la AT24C64, ¿por qué hay que esperar ~5ms después de escribir?

a) Para que el bus se estabilice  
b) Para que la EEPROM complete el ciclo de escritura interno  
c) Por limitación del protocolo I2C  
d) Para evitar colisiones en el bus

**Respuesta correcta**: b) La EEPROM necesita tiempo para grabar en memoria no volátil

---

## Evidencias de Aprendizaje

Al finalizar la sesión, el estudiante debe entregar:

1. **Código funcional** de I2C1 configurado
   - Bus scanning operativo
   - Lectura/escritura de EEPROM

2. **Reporte técnico** (2 páginas) que incluya:
   - Explicación del protocolo I2C
   - Cálculo de CCR y TRISE para 100 kHz
   - Dispositivos detectados en el bus
   - Problemas encontrados y soluciones

3. **Capturas de osciloscopio/analizador** mostrando:
   - START, dirección, datos, STOP claramente identificados

---

## Material para Casa

### Lectura Preparatoria (próxima sesión: I2C Avanzado)

1. **Datasheet BME280**: Sensor de temperatura/humedad/presión (secciones 1-4)
2. **Datasheet SSD1306**: Controlador OLED (páginas de comandos I2C)
3. **Application Note AN2824**: "I2C bus protocol in embedded systems"

### Ejercicio Opcional

Implementar función que:
- Lea temperatura del sensor (si disponible)
- La almacene en EEPROM cada minuto
- Permita descargar historial completo vía USART

---

## Notas para el Instructor

### Errores Comunes

1. **Olvidar pull-ups**: Bus no funciona sin resistencias
2. **No leer SR2**: Flag ADDR no se limpia correctamente
3. **Direcciones incorrectas**: Confundir 7 bits vs 8 bits (con R/W)
4. **Timeouts insuficientes**: En buses lentos o con problemas

### Extensiones

- **Multi-master**: Implementar arbitración de bus
- **Clock stretching**: Manejar slaves que ralentizan el reloj
- **10-bit addressing**: Para sistemas con >127 dispositivos
- **SMBus**: Variante de I2C con timeouts obligatorios

### Tiempo de Sesión

- Teoría: 40 min
- Actividad 1: 20 min
- Actividad 2: 30 min
- Actividad 3: 20 min
- Cierre: 10 min

**Total**: 120 minutos

---

## Referencias

1. **I2C Specification** (UM10204) - NXP Semiconductors
2. **STM32F407 Reference Manual** (RM0090), Sección 27: I2C
3. **AT24C64 Datasheet** - Microchip Technology
4. **Application Note AN2824**: "STM32F10xxx I2C optimized examples"

---

**Fin de Sesión 3**
