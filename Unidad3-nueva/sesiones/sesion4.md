# Sesión 4: I2C Avanzado - Sensores y Displays OLED

**Unidad 3**: Protocolos de Comunicación Serial  
**Semana**: 2  
**Duración**: 2 horas  
**Modalidad**: Presencial con práctica en laboratorio

---

## Objetivos de Aprendizaje

Al finalizar esta sesión, el estudiante será capaz de:

1. Comunicarse con sensores I2C complejos (BME280)
2. Implementar drivers de dispositivos I2C específicos
3. Controlar pantallas OLED (SSD1306) por I2C
4. Leer hojas de datos para extraer comandos y registros I2C
5. Integrar múltiples dispositivos I2C en un sistema

**Tiempo estimado de actividades**: 90 minutos (teoría: 35 min, práctica: 55 min)

---

## 1. Sensor BME280 (Bosch)

### 1.1 Características del BME280

- **Tipo**: Sensor ambiental digital
- **Mediciones**: Temperatura, humedad, presión atmosférica
- **Interfaces**: I2C (hasta 3.4 MHz) y SPI
- **Dirección I2C**: 0x76 (SDO a GND) o 0x77 (SDO a VDD)
- **Voltaje**: 1.8V - 3.6V
- **Precisión**:
  - Temperatura: ±1°C
  - Humedad: ±3% RH
  - Presión: ±1 hPa

### 1.2 Registros Principales

| Dirección | Nombre | Descripción |
|-----------|---------|-------------|
| 0xD0 | ID | Chip ID (siempre 0x60 para BME280) |
| 0xF2 | ctrl_hum | Control de medición de humedad |
| 0xF4 | ctrl_meas | Control de temp/presión + modo |
| 0xF5 | config | Tiempo de standby, filtro IIR |
| 0xF7-0xFE | press/temp/hum_data | Datos de mediciones (20 bits) |
| 0x88-0xE7 | calib | Coeficientes de calibración |

### 1.3 Inicialización del BME280

```c
#define BME280_ADDR 0x76  // o 0x77

/**
 * @brief Verifica presencia del BME280 leyendo chip ID
 * @return 0 si éxito (ID = 0x60), -1 si error
 */
int BME280_CheckID(void) {
    int chip_id;
    
    // Leer registro ID (0xD0)
    if (I2C1_Start() != 0) return -1;
    if (I2C1_SendAddress(BME280_ADDR, 0) != 0) {
        I2C1_Stop();
        return -1;
    }
    if (I2C1_WriteByte(0xD0) != 0) {  // Registro ID
        I2C1_Stop();
        return -1;
    }
    
    // Repeated START para lectura
    if (I2C1_Start() != 0) {
        I2C1_Stop();
        return -1;
    }
    if (I2C1_SendAddress(BME280_ADDR, 1) != 0) {
        I2C1_Stop();
        return -1;
    }
    
    chip_id = I2C1_ReadByte(1);  // Último byte
    I2C1_Stop();
    
    return (chip_id == 0x60) ? 0 : -1;
}

/**
 * @brief Inicializa BME280 en modo normal
 */
void BME280_Init(void) {
    // 1. Soft reset
    BME280_WriteRegister(0xE0, 0xB6);
    delay_ms(10);
    
    // 2. Configurar humedad (oversampling x1)
    BME280_WriteRegister(0xF2, 0x01);
    
    // 3. Configurar temp/presión + modo normal
    // [7:5] osrs_t = 001 (oversampling x1 temp)
    // [4:2] osrs_p = 001 (oversampling x1 presión)
    // [1:0] mode = 11 (normal mode)
    BME280_WriteRegister(0xF4, 0x27);
    
    // 4. Configurar standby time y filtro
    // [7:5] t_sb = 101 (1000 ms)
    // [4:2] filter = 000 (off)
    // [0] spi3w_en = 0
    BME280_WriteRegister(0xF5, 0xA0);
}

/**
 * @brief Escribe un registro del BME280
 */
void BME280_WriteRegister(uint8_t reg, uint8_t value) {
    I2C1_Start();
    I2C1_SendAddress(BME280_ADDR, 0);
    I2C1_WriteByte(reg);
    I2C1_WriteByte(value);
    I2C1_Stop();
}
```

### 1.4 Lectura de Datos del BME280

```c
/**
 * @brief Lee los datos crudos de temperatura
 * @return Valor crudo de 20 bits
 */
int32_t BME280_ReadRawTemperature(void) {
    uint8_t data[3];
    
    // Leer registros 0xFA, 0xFB, 0xFC
    BME280_ReadRegisters(0xFA, data, 3);
    
    // Combinar en valor de 20 bits
    int32_t adc_T = ((int32_t)data[0] << 12) | 
                    ((int32_t)data[1] << 4) | 
                    ((data[2] >> 4) & 0x0F);
    
    return adc_T;
}

/**
 * @brief Lee múltiples registros consecutivos
 */
void BME280_ReadRegisters(uint8_t reg, uint8_t *buffer, uint8_t length) {
    // Establecer registro inicial
    I2C1_Start();
    I2C1_SendAddress(BME280_ADDR, 0);
    I2C1_WriteByte(reg);
    
    // Repeated START para lectura
    I2C1_Start();
    I2C1_SendAddress(BME280_ADDR, 1);
    
    // Leer bytes
    for (uint8_t i = 0; i < length; i++) {
        uint8_t last = (i == length - 1) ? 1 : 0;
        buffer[i] = I2C1_ReadByte(last);
    }
    
    I2C1_Stop();
}
```

### 1.5 Compensación de Temperatura

El BME280 requiere aplicar algoritmos de compensación usando coeficientes de calibración almacenados en el chip.

```c
// Coeficientes de calibración (leer una vez al inicio)
uint16_t dig_T1;
int16_t dig_T2, dig_T3;

/**
 * @brief Lee coeficientes de calibración de temperatura
 */
void BME280_ReadCalibration(void) {
    uint8_t calib[6];
    
    // Leer registros 0x88-0x8D
    BME280_ReadRegisters(0x88, calib, 6);
    
    dig_T1 = (calib[1] << 8) | calib[0];
    dig_T2 = (calib[3] << 8) | calib[2];
    dig_T3 = (calib[5] << 8) | calib[4];
}

/**
 * @brief Compensa temperatura según datasheet del BME280
 * @param adc_T Valor crudo de temperatura
 * @return Temperatura en centésimas de grado (2506 = 25.06°C)
 */
int32_t BME280_CompensateTemperature(int32_t adc_T) {
    int32_t var1, var2, T;
    
    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * 
            ((int32_t)dig_T2)) >> 11;
    
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * 
              ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * 
            ((int32_t)dig_T3)) >> 14;
    
    T = var1 + var2;
    T = (T * 5 + 128) >> 8;
    
    return T;
}

/**
 * @brief Lee temperatura compensada
 * @return Temperatura en °C (float)
 */
float BME280_ReadTemperature(void) {
    int32_t adc_T = BME280_ReadRawTemperature();
    int32_t T = BME280_CompensateTemperature(adc_T);
    return T / 100.0f;
}
```

---

## 2. Display OLED SSD1306

### 2.1 Características del SSD1306

- **Resolución**: 128x64 píxeles (monocromático)
- **Interfaces**: I2C (hasta 400 kHz) o SPI
- **Dirección I2C**: 0x3C (SA0 a GND) o 0x3D (SA0 a VDD)
- **Memoria**: GDDRAM de 1024 bytes (128x64 bits)
- **Voltaje**: 3.3V - 5V

### 2.2 Comandos Básicos

El SSD1306 usa un byte de control antes de cada comando o dato:

| Byte Control | Descripción |
|--------------|-------------|
| 0x00 | Siguiente byte es comando |
| 0x40 | Siguiente byte es dato (píxel) |
| 0x80 | Siguiente bytes son comandos múltiples |

### 2.3 Inicialización del SSD1306

```c
#define SSD1306_ADDR 0x3C

/**
 * @brief Envía un comando al SSD1306
 */
void SSD1306_Command(uint8_t cmd) {
    I2C1_Start();
    I2C1_SendAddress(SSD1306_ADDR, 0);
    I2C1_WriteByte(0x00);  // Control byte: comando
    I2C1_WriteByte(cmd);
    I2C1_Stop();
}

/**
 * @brief Inicializa display SSD1306
 */
void SSD1306_Init(void) {
    delay_ms(100);  // Esperar que display esté listo
    
    // Secuencia de inicialización
    SSD1306_Command(0xAE);  // Display OFF
    SSD1306_Command(0xD5);  // Set display clock divide
    SSD1306_Command(0x80);  // Ratio sugerido
    SSD1306_Command(0xA8);  // Set multiplex ratio
    SSD1306_Command(0x3F);  // 64 líneas
    SSD1306_Command(0xD3);  // Set display offset
    SSD1306_Command(0x00);  // Sin offset
    SSD1306_Command(0x40);  // Set start line a 0
    SSD1306_Command(0x8D);  // Charge pump
    SSD1306_Command(0x14);  // Enable charge pump
    SSD1306_Command(0x20);  // Memory addressing mode
    SSD1306_Command(0x00);  // Horizontal addressing
    SSD1306_Command(0xA1);  // Segment remap (flip horizontal)
    SSD1306_Command(0xC8);  // COM scan direction (flip vertical)
    SSD1306_Command(0xDA);  // Set COM pins
    SSD1306_Command(0x12);  // Alternative COM config
    SSD1306_Command(0x81);  // Set contrast
    SSD1306_Command(0x7F);  // Contraste medio
    SSD1306_Command(0xD9);  // Set precharge period
    SSD1306_Command(0xF1);
    SSD1306_Command(0xDB);  // Set VCOMH deselect level
    SSD1306_Command(0x40);
    SSD1306_Command(0xA4);  // Display resume (normal)
    SSD1306_Command(0xA6);  // Normal display (no invertido)
    SSD1306_Command(0xAF);  // Display ON
}
```

### 2.4 Control de Píxeles

```c
// Buffer de pantalla (1024 bytes = 128x64/8)
uint8_t ssd1306_buffer[1024];

/**
 * @brief Limpia el buffer de pantalla
 */
void SSD1306_Clear(void) {
    for (uint16_t i = 0; i < 1024; i++) {
        ssd1306_buffer[i] = 0x00;
    }
}

/**
 * @brief Enciende un píxel en el buffer
 * @param x Coordenada X (0-127)
 * @param y Coordenada Y (0-63)
 */
void SSD1306_DrawPixel(uint8_t x, uint8_t y) {
    if (x >= 128 || y >= 64) return;
    
    // El display está organizado en páginas de 8 píxeles verticales
    uint8_t page = y / 8;
    uint8_t bit = y % 8;
    uint16_t index = page * 128 + x;
    
    ssd1306_buffer[index] |= (1 << bit);
}

/**
 * @brief Envía el buffer al display
 */
void SSD1306_Update(void) {
    // Establecer posición inicial
    SSD1306_Command(0x21);  // Set column address
    SSD1306_Command(0);     // Start column
    SSD1306_Command(127);   // End column
    SSD1306_Command(0x22);  // Set page address
    SSD1306_Command(0);     // Start page
    SSD1306_Command(7);     // End page
    
    // Enviar todo el buffer
    I2C1_Start();
    I2C1_SendAddress(SSD1306_ADDR, 0);
    I2C1_WriteByte(0x40);  // Control byte: datos
    
    for (uint16_t i = 0; i < 1024; i++) {
        I2C1_WriteByte(ssd1306_buffer[i]);
    }
    
    I2C1_Stop();
}
```

### 2.5 Dibujar Texto Simple

```c
// Font 5x8 (ASCII 32-127)
const uint8_t font5x8[96][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // Space
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    // ... (completar con tabla de fuente completa)
};

/**
 * @brief Dibuja un carácter en el buffer
 * @param x Posición X
 * @param y Posición Y (página, 0-7)
 * @param c Carácter ASCII
 */
void SSD1306_DrawChar(uint8_t x, uint8_t y, char c) {
    if (c < 32 || c > 127) c = '?';
    
    uint8_t char_index = c - 32;
    uint16_t buffer_index = y * 128 + x;
    
    for (uint8_t i = 0; i < 5; i++) {
        ssd1306_buffer[buffer_index + i] = font5x8[char_index][i];
    }
}

/**
 * @brief Dibuja un string en el buffer
 */
void SSD1306_DrawString(uint8_t x, uint8_t y, const char *str) {
    while (*str) {
        SSD1306_DrawChar(x, y, *str++);
        x += 6;  // 5 píxeles + 1 de espaciado
        if (x > 122) break;
    }
}
```

---

## 3. Integración: BME280 + SSD1306

```c
/**
 * @brief Aplicación completa: leer sensor y mostrar en OLED
 */
int main(void) {
    // Inicialización del sistema
    SystemInit();
    USART2_Init(115200);
    I2C1_ClockInit();
    I2C1_GPIO_Init();
    I2C1_Init();
    
    // Inicializar BME280
    if (BME280_CheckID() == 0) {
        printf("BME280 detectado\r\n");
        BME280_ReadCalibration();
        BME280_Init();
    } else {
        printf("ERROR: BME280 no detectado\r\n");
    }
    
    // Inicializar SSD1306
    SSD1306_Init();
    SSD1306_Clear();
    SSD1306_DrawString(0, 0, "Sistema de");
    SSD1306_DrawString(0, 1, "Monitoreo");
    SSD1306_Update();
    delay_ms(2000);
    
    // Loop principal
    char buffer[20];
    while (1) {
        // Leer temperatura
        float temp = BME280_ReadTemperature();
        
        // Mostrar en OLED
        SSD1306_Clear();
        SSD1306_DrawString(0, 0, "Temperatura:");
        sprintf(buffer, "%.2f C", temp);
        SSD1306_DrawString(0, 2, buffer);
        SSD1306_Update();
        
        // Mostrar por USART
        printf("Temp: %.2f C\r\n", temp);
        
        delay_ms(1000);
    }
}
```

---

## Actividades Prácticas

### Actividad 1: Driver BME280 (30 min)

**Objetivo**: Leer temperatura del sensor BME280.

**Tareas**:
1. Implementar funciones de inicialización del BME280
2. Leer coeficientes de calibración
3. Leer temperatura cruda y aplicar compensación
4. Imprimir temperatura por USART cada segundo
5. Verificar con termómetro real

**Evidencia**: Código + captura mostrando lecturas estables

---

### Actividad 2: Driver SSD1306 (30 min)

**Objetivo**: Mostrar texto en pantalla OLED.

**Tareas**:
1. Implementar inicialización del SSD1306
2. Crear buffer de pantalla y funciones de dibujo
3. Mostrar mensaje de bienvenida
4. Implementar función para mostrar números grandes
5. Crear animación simple (logo deslizándose)

**Evidencia**: Video corto mostrando OLED funcionando

---

### Actividad 3: Sistema Integrado (25 min)

**Objetivo**: Combinar BME280 + SSD1306 en aplicación completa.

**Tareas**:
1. Leer temperatura, humedad y presión del BME280
2. Mostrar las 3 mediciones en el OLED
3. Implementar interfaz gráfica básica (marcos, íconos)
4. Actualizar display cada 2 segundos
5. Agregar indicador visual si temperatura supera umbral

**Evidencia**: Demostración en vivo + código fuente

---

## Quiz Formativo

**Pregunta 1**: ¿Por qué el BME280 requiere compensación de datos?

a) Para corregir errores de transmisión I2C  
b) Los valores crudos tienen offset y ganancia únicos por chip  
c) Para reducir consumo de energía  
d) Es opcional, solo mejora ligeramente la precisión

**Respuesta correcta**: b) Cada chip tiene coeficientes únicos de fabricación

---

**Pregunta 2**: En el SSD1306, ¿qué representa cada byte del buffer?

a) Un píxel  
b) Una columna de 8 píxeles verticales  
c) Una fila completa  
d) Un carácter

**Respuesta correcta**: b) 8 píxeles verticales en modo página

---

**Pregunta 3**: ¿Cuál es la ventaja de usar buffer local antes de actualizar el OLED?

a) Usa menos memoria  
b) Es más rápido  
c) Permite dibujar elementos complejos sin parpadeo  
d) No hay ventaja, es innecesario

**Respuesta correcta**: c) Evita parpadeo al actualizar toda la pantalla de una vez

---

**Pregunta 4**: ¿Por qué los sensores I2C suelen requerir "soft reset" al iniciar?

a) Para limpiar registros internos y partir de estado conocido  
b) Para verificar que el sensor funciona  
c) Es obligatorio por el protocolo I2C  
d) Para activar el sensor que viene apagado

**Respuesta correcta**: a) Garantiza estado inicial conocido

---

**Pregunta 5**: ¿Qué sucede si se intenta actualizar el SSD1306 a más de 60 fps?

a) El display se daña  
b) Incrementa consumo innecesariamente sin beneficio visual  
c) Se corrompen los datos  
d) Funciona normalmente

**Respuesta correcta**: b) El ojo humano no percibe más de ~60 Hz

---

## Evidencias de Aprendizaje

Al finalizar la sesión, el estudiante debe entregar:

1. **Drivers funcionales** para BME280 y SSD1306
2. **Aplicación integradora** mostrando datos del sensor en OLED
3. **Documentación** del diseño (diagramas, explicación del código)

---

## Material para Casa

### Lectura para próxima sesión (SPI)

1. **SPI Protocol Overview** - Application Note
2. **Datasheet MAX31855** (termopar por SPI)
3. **Comparación I2C vs SPI** - Ventajas/desventajas

---

## Notas para el Instructor

### Errores Comunes

1. **Olvidar calibración**: Usar datos crudos sin compensar
2. **Timeout insuficiente**: SSD1306 necesita tiempo para iniciar
3. **Buffer overflow**: Escribir fuera de límites del buffer OLED

### Tiempo de Sesión

- Teoría: 35 min
- Actividades: 85 min
- **Total**: 120 minutos

---

## Referencias

1. **BME280 Datasheet** - Bosch Sensortec
2. **SSD1306 Datasheet** - Solomon Systech
3. **Adafruit GFX Library** - Referencia de funciones gráficas

---

**Fin de Sesión 4**
