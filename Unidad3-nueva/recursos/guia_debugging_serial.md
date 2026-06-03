# Guía de Debugging de Comunicaciones Seriales

## 1. Problemas Comunes y Soluciones

### USART

#### Problema: No hay comunicación

**Síntomas**:
- No se recibe nada
- No se transmite nada

**Causas posibles y soluciones**:

1. **Baudrate incorrecto**
   - Verificar cálculo de BRR
   - Medir frecuencia con osciloscopio
   - Probar baudrates estándar (9600, 115200)
   
2. **Pines no configurados**
   - Verificar MODER (Alternate Function)
   - Verificar AFR (AF correcta, típicamente AF7)
   - Verificar conexión física (TX↔RX cruzado)

3. **Clock no habilitado**
   - Verificar RCC->APB1ENR o APB2ENR
   - Verificar clock de GPIO

**Debug steps**:
```c
// 1. Verificar configuración
printf("BRR = %lu\r\n", USART2->BRR);
printf("CR1 = 0x%lX\r\n", USART2->CR1);

// 2. Test de loopback (cortocircuitar TX-RX)
USART2_SendByte('A');
uint8_t received = USART2_ReceiveByte();
// Debe recibir 'A'
```

#### Problema: Caracteres corruptos

**Síntomas**:
- Recibe basura
- Caracteres aleatorios

**Soluciones**:
- Ajustar baudrate (fino)
- Verificar paridad
- Verificar stop bits
- Medir con osciloscopio

---

### I2C

#### Problema: No hay ACK del slave

**Síntomas**:
- Flag ADDR no se pone
- Timeout en espera de ACK

**Causas y soluciones**:

1. **Dirección incorrecta**
   - Verificar dirección del dispositivo (7 bits)
   - Algunos dispositivos tienen A0-A2 configurables
   - Hacer bus scan para detectar dispositivos

2. **Pull-ups ausentes o incorrectas**
   - Verificar resistencias (típicamente 4.7kΩ)
   - Medir voltaje en SDA/SCL (debe ser ~3.3V en reposo)

3. **Dispositivo no alimentado o en reset**
   - Verificar alimentación del slave
   - Verificar pin de reset si existe

**Bus Scanner**:
```c
for (uint8_t addr = 0; addr < 128; addr++) {
    I2C1_Start();
    if (I2C1_SendAddress(addr, 0) == 0) {
        printf("Dispositivo en 0x%02X\r\n", addr);
    }
    I2C1_Stop();
    delay_ms(10);
}
```

#### Problema: Bus stuck (bus ocupado permanentemente)

**Síntomas**:
- Flag BUSY siempre activo
- No se puede generar START

**Solución**:
```c
void I2C1_Reset(void) {
    // Software reset
    I2C1->CR1 |= I2C_CR1_SWRST;
    delay_us(10);
    I2C1->CR1 &= ~I2C_CR1_SWRST;
    
    // Re-inicializar
    I2C1_Init();
}
```

---

### SPI

#### Problema: Datos incorrectos

**Causas**:

1. **Modo de reloj incorrecto**
   - Verificar CPOL/CPHA del datasheet del slave
   - Probar los 4 modos (0, 1, 2, 3)

2. **CS timing**
   - Agregar delay después de CS_Low()
   - Verificar que CS va a bajo antes de transmitir

3. **Velocidad muy alta**
   - Reducir baudrate (aumentar prescaler)
   - Verificar límites del slave

**Test de loopback**:
```c
// Cortocircuitar MOSI-MISO
uint8_t sent = 0xAA;
uint8_t received = SPI1_TransferByte(sent);
// received debe ser igual a sent
```

---

### CAN

#### Problema: No transmite

**Síntomas**:
- Mailbox siempre lleno
- Timeout en transmisión

**Causas y soluciones**:

1. **Terminaciones ausentes**
   - Verificar resistencias de 120Ω en ambos extremos
   - Sin terminaciones, el bus no funciona

2. **Bus-off por errores**
   - Leer registro ESR (Error Status Register)
   - Reset del periférico CAN

3. **Configuración incorrecta**
   - Verificar baudrate (todos los nodos deben coincidir)
   - Verificar BTR (prescaler, BS1, BS2)

**Verificar estado**:
```c
uint32_t esr = CAN1->ESR;
uint8_t rec = (esr >> 24) & 0xFF;  // RX error counter
uint8_t tec = (esr >> 16) & 0xFF;  // TX error counter

if (tec > 127) {
    // Bus-off: reset necesario
    CAN1->MCR |= CAN_MCR_RESET;
}
```

## 2. Herramientas de Debugging

### 2.1 Osciloscopio / Analizador Lógico

**Qué medir**:

**USART**:
- Baudrate (1/bit_time)
- Niveles lógicos (0V, 3.3V)
- Start bit, stop bit

**I2C**:
- START condition (SDA cae mientras SCL alto)
- Frecuencia SCL
- ACK bits
- STOP condition

**SPI**:
- Frecuencia SCK
- Relación temporal SCK-MOSI-MISO
- CS antes de primer clock

**CAN**:
- Niveles diferenciales (CAN_H - CAN_L)
- Baudrate
- Presencia de ACK bits

### 2.2 Software

**Analizador lógico** (Saleae Logic):
- Capturar múltiples canales
- Decodificadores automáticos
- Exportar datos para análisis

**Terminal serial**:
- PuTTY, minicom, screen
- Verificar configuración (baudrate, bits, paridad)

**CANalyzer** (para CAN):
- Monitorear tráfico CAN
- Enviar mensajes de prueba
- Análisis de timing

## 3. Técnicas de Debugging

### Printf Debugging

```c
#define DEBUG_USART 1
#define DEBUG_I2C 1
#define DEBUG_SPI 1
#define DEBUG_CAN 1

#if DEBUG_I2C
#define I2C_LOG(fmt, ...) printf("[I2C] " fmt "\r\n", ##__VA_ARGS__)
#else
#define I2C_LOG(fmt, ...)
#endif

// Uso
I2C_LOG("Sending address 0x%02X", addr);
```

### Timeouts

**Siempre implementar timeouts**:
```c
uint32_t timeout = 10000;
while (!(I2C1->SR1 & I2C_SR1_SB) && timeout--);
if (timeout == 0) {
    printf("ERROR: Timeout en START\r\n");
    return -1;
}
```

### Códigos de Error

```c
typedef enum {
    COM_OK = 0,
    COM_ERR_TIMEOUT = -1,
    COM_ERR_NACK = -2,
    COM_ERR_BUS_BUSY = -3,
    COM_ERR_OVERRUN = -4
} ComError_t;
```

### LED de Estado

```c
#define LED_GREEN_ON()  GPIOD->ODR |= (1 << 12)
#define LED_RED_ON()    GPIOD->ODR |= (1 << 14)

// En código
if (i2c_status == 0) {
    LED_GREEN_ON();  // Éxito
} else {
    LED_RED_ON();    // Error
}
```

## 4. Checklist de Debugging

### Antes de empezar

- [ ] Datasheet del dispositivo leído
- [ ] Configuración de pines verificada
- [ ] Alimentación correcta
- [ ] Conexiones físicas verificadas

### Durante debugging

- [ ] Clocks habilitados
- [ ] Configuración de periférico correcta
- [ ] Timeouts implementados
- [ ] Manejo de errores implementado
- [ ] Logs informativos agregados

### Con herramientas

- [ ] Señales medidas con osciloscopio
- [ ] Protocolo decodificado correctamente
- [ ] Timing verificado
- [ ] Niveles lógicos correctos

## 5. Errores Frecuentes por Protocolo

### USART
1. ❌ Olvidar configurar pines como AF
2. ❌ BRR calculado incorrectamente
3. ❌ TX-RX no cruzados

### I2C
1. ❌ No leer SR2 después de ADDR
2. ❌ Olvidar pull-ups
3. ❌ Dirección de 8 bits en vez de 7

### SPI
1. ❌ No controlar CS manualmente
2. ❌ CPOL/CPHA incorrectos
3. ❌ No leer DR después de transmitir

### CAN
1. ❌ Olvidar terminaciones
2. ❌ Filtros bloqueando mensajes
3. ❌ Baudrate diferente entre nodos

## 6. Recursos de Ayuda

**Documentación**:
- Reference Manual del STM32
- Datasheet del periférico
- Application Notes de ST

**Foros**:
- STM32 Community
- Stack Overflow
- EEVblog

**Herramientas online**:
- Calculadoras de baudrate
- Decodificadores de protocolos
- Simuladores de timing

---

**Recuerda**: El debugging sistemático es clave. No adivinar, medir y verificar.
