# Sesión 7: Integración Multi-Protocolo y Debugging Avanzado

**Unidad 3**: Protocolos de Comunicación Serial  
**Semana**: 4  
**Duración**: 2 horas

---

## Objetivos

1. Integrar múltiples protocolos en una aplicación compleja
2. Utilizar herramientas de análisis de protocolos seriales
3. Depurar problemas comunes en comunicaciones
4. Optimizar rendimiento y gestión de errores

---

## 1. Integración de Protocolos

### 1.1 Arquitectura Multi-Protocolo

```
┌─────────────────────────────────────────┐
│            Aplicación Principal          │
│    (Estado, Lógica, Coordinación)        │
└───────┬─────────┬────────┬────────┬──────┘
        │         │        │        │
    ┌───▼──┐  ┌──▼───┐ ┌──▼───┐ ┌──▼───┐
    │USART │  │ I2C  │ │ SPI  │ │ CAN  │
    │Driver│  │Driver│ │Driver│ │Driver│
    └───┬──┘  └──┬───┘ └──┬───┘ └──┬───┘
        │        │        │        │
     Printf   Sensores  Display   Red
```

### 1.2 Sistema de Ejemplo

**Estación Meteorológica Distribuida**:
- **USART**: Debug y logs
- **I2C**: Sensor BME280 (temp/humedad/presión)
- **SPI**: Display OLED local
- **CAN**: Comunicación con estación central

```c
/**
 * @brief Estructura de datos del sistema
 */
typedef struct {
    float temperature;
    float humidity;
    float pressure;
    uint32_t timestamp;
    uint8_t status;
} SensorData_t;

/**
 * @brief Loop principal integrando todos los protocolos
 */
void MainLoop(void) {
    SensorData_t data;
    CAN_Message_t can_msg;
    char oled_buffer[32];
    
    while (1) {
        // 1. Leer sensores por I2C
        data.temperature = BME280_ReadTemperature();
        data.humidity = BME280_ReadHumidity();
        data.pressure = BME280_ReadPressure();
        data.timestamp = millis();
        
        // 2. Log por USART
        printf("[%lu] T=%.1f°C, H=%.1f%%, P=%.1fhPa\r\n",
               data.timestamp, 
               data.temperature, 
               data.humidity, 
               data.pressure);
        
        // 3. Actualizar display por SPI
        SSD1306_Clear();
        sprintf(oled_buffer, "T: %.1fC", data.temperature);
        SSD1306_DrawString(0, 0, oled_buffer);
        sprintf(oled_buffer, "H: %.1f%%", data.humidity);
        SSD1306_DrawString(0, 2, oled_buffer);
        SSD1306_Update();
        
        // 4. Enviar por CAN a estación central
        can_msg.id = 0x200;  // ID de este nodo
        can_msg.length = 8;
        memcpy(can_msg.data, &data.temperature, 4);
        memcpy(can_msg.data + 4, &data.humidity, 4);
        CAN1_Transmit(&can_msg);
        
        delay_ms(1000);
    }
}
```

---

## 2. Herramientas de Debugging

### 2.1 Analizador Lógico

**Uso de Saleae Logic o similar**:
- Captura simultánea de múltiples señales
- Decodificación automática de protocolos
- Análisis de timing
- Detección de errores

**Configuración típica**:
```
Canal 0: USART TX
Canal 1: USART RX
Canal 2: I2C SDA
Canal 3: I2C SCL
Canal 4: SPI MOSI
Canal 5: SPI SCK
Canal 6: CAN TX
Canal 7: Trigger/Debug
```

### 2.2 Osciloscopio Digital

**Verificaciones importantes**:
- Niveles de voltaje (3.3V vs 5V)
- Tiempos de setup/hold
- Frecuencias de reloj
- Integridad de señal (rise time, ringing)

### 2.3 Printf Debugging

**Técnica de logs estructurados**:
```c
#define LOG_ERROR(fmt, ...) \
    printf("[ERROR] " fmt "\r\n", ##__VA_ARGS__)

#define LOG_WARN(fmt, ...) \
    printf("[WARN ] " fmt "\r\n", ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) \
    printf("[INFO ] " fmt "\r\n", ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) \
    printf("[DEBUG] " fmt "\r\n", ##__VA_ARGS__)

// Uso
LOG_INFO("Sistema iniciado");
LOG_ERROR("Sensor I2C no responde: addr=0x%02X", sensor_addr);
LOG_DEBUG("CAN TX: ID=0x%03X, len=%d", msg.id, msg.length);
```

---

## 3. Problemas Comunes y Soluciones

### 3.1 USART

**Problema**: Caracteres corruptos
- **Causa**: Baudrate incorrecto
- **Solución**: Verificar cálculo de BRR, medir con osciloscopio

**Problema**: Buffer overflow
- **Causa**: Interrupciones no atienden a tiempo
- **Solución**: Aumentar tamaño de buffer, reducir carga de ISR

### 3.2 I2C

**Problema**: ACK no recibido
- **Causa**: Dispositivo no conectado o dirección incorrecta
- **Solución**: Bus scanner, verificar pull-ups

**Problema**: Bus busy permanente
- **Causa**: No se generó STOP condition
- **Solución**: Reset del periférico I2C

**Código de reset**:
```c
void I2C1_Reset(void) {
    I2C1->CR1 |= I2C_CR1_SWRST;
    delay_us(10);
    I2C1->CR1 &= ~I2C_CR1_SWRST;
    I2C1_Init();  // Re-inicializar
}
```

### 3.3 SPI

**Problema**: Datos incorrectos
- **Causa**: Modo de reloj (CPOL/CPHA) incorrecto
- **Solución**: Verificar datasheet del slave, probar los 4 modos

**Problema**: CS no funciona
- **Causa**: Timing incorrecto (CS muy corto)
- **Solución**: Agregar delay_us() después de CS_Low()

### 3.4 CAN

**Problema**: No transmite
- **Causa**: Bus-off por errores repetidos
- **Solución**: Verificar terminaciones (120Ω), niveles lógicos

**Problema**: Transmite pero no recibe
- **Causa**: Filtros mal configurados
- **Solución**: Configurar filtro para aceptar todos (mask=0)

---

## 4. Optimización de Rendimiento

### 4.1 DMA para Transferencias Largas

```c
/**
 * @brief Configura DMA para SPI TX
 */
void SPI1_DMA_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    
    // DMA2 Stream 3, Canal 3: SPI1 TX
    DMA2_Stream3->CR = 0;
    DMA2_Stream3->CR |= (3 << 25);  // Canal 3
    DMA2_Stream3->CR |= (1 << 8);   // Incrementar memoria
    DMA2_Stream3->CR |= (1 << 4);   // TX complete interrupt
    
    // Habilitar DMA en SPI
    SPI1->CR2 |= SPI_CR2_TXDMAEN;
}

/**
 * @brief Transmite buffer por SPI usando DMA
 */
void SPI1_DMA_Transmit(uint8_t *data, uint16_t length) {
    DMA2_Stream3->M0AR = (uint32_t)data;
    DMA2_Stream3->PAR = (uint32_t)&SPI1->DR;
    DMA2_Stream3->NDTR = length;
    DMA2_Stream3->CR |= DMA_SxCR_EN;  // Start
}
```

**Ventaja**: CPU libre durante transferencia (ideal para displays).

### 4.2 Gestión de Prioridades

```c
/**
 * @brief Configura prioridades de interrupciones
 */
void Configure_IRQ_Priorities(void) {
    // Prioridad alta (0-1): Eventos críticos
    NVIC_SetPriority(CAN1_RX0_IRQn, 0);  // CAN urgente
    
    // Prioridad media (2-4): Comunicaciones
    NVIC_SetPriority(USART2_IRQn, 2);
    NVIC_SetPriority(I2C1_EV_IRQn, 3);
    NVIC_SetPriority(SPI1_IRQn, 3);
    
    // Prioridad baja (5-7): Tareas de fondo
    NVIC_SetPriority(TIM2_IRQn, 5);  // Timer genérico
}
```

---

## Actividades Prácticas

### Actividad 1: Sistema Integrado (35 min)

**Objetivo**: Crear aplicación usando USART+I2C+SPI+CAN.

**Tareas**:
1. Implementar estación meteorológica
2. Leer BME280 por I2C cada segundo
3. Mostrar en OLED por SPI
4. Enviar por CAN a central
5. Log por USART

**Evidencia**: Demostración funcionando + código

---

### Actividad 2: Análisis con Logic Analyzer (30 min)

**Objetivo**: Capturar y analizar tráfico de protocolos.

**Tareas**:
1. Conectar analizador lógico
2. Capturar transacción I2C completa
3. Capturar actualización de SPI
4. Identificar timings y comandos
5. Detectar errores si los hay

**Evidencia**: Capturas anotadas con análisis

---

### Actividad 3: Debugging de Problemas (25 min)

**Objetivo**: Diagnosticar y resolver problemas simulados.

**Escenarios**:
1. USART con baudrate incorrecto (propuesto)
2. I2C sin pull-ups (desconectar resistencias)
3. SPI con modo de reloj incorrecto
4. CAN sin terminaciones

**Evidencia**: Reporte de troubleshooting con soluciones

---

## Quiz Formativo

**P1**: ¿Cuál es la ventaja de usar DMA para SPI?
**R**: c) Libera CPU durante transferencia

**P2**: ¿Qué herramienta es ideal para analizar protocolos seriales?
**R**: b) Analizador lógico

**P3**: ¿Por qué usar logs con niveles (INFO, DEBUG, ERROR)?
**R**: c) Facilita filtrado de información relevante

**P4**: ¿Qué hacer si I2C retorna siempre NACK?
**R**: a) Verificar dirección y pull-ups

**P5**: ¿Cómo afecta la prioridad de interrupciones al sistema?
**R**: b) Determina qué eventos se atienden primero

---

## Evidencias

1. Sistema multi-protocolo funcional
2. Capturas de analizador lógico
3. Reporte de debugging con soluciones aplicadas

---

## Material para Casa

- Preparar propuesta de proyecto final
- Revisar datasheet de periféricos elegidos
- Diseñar diagrama de estados del proyecto

---

## Referencias

1. **Application Note AN4031**: Using DMA with STM32
2. **Saleae Logic Analyzer** - User Guide
3. **"Debugging Embedded Systems"** - Clay Montgomery

---

**Fin de Sesión 7**
