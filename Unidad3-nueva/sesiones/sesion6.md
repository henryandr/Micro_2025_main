# Sesión 6: Comunicación CAN Bus

**Unidad 3**: Protocolos de Comunicación Serial  
**Semana**: 3  
**Duración**: 2 horas

---

## Objetivos

1. Comprender el protocolo CAN (Controller Area Network)
2. Configurar CAN1 del STM32F407
3. Implementar TX/RX de mensajes CAN
4. Configurar filtros de recepción

---

## 1. Introducción a CAN

### 1.1 ¿Qué es CAN?

CAN es un protocolo robusto de comunicación serial desarrollado por Bosch para la industria automotriz.

**Características**:
- Bus diferencial de 2 cables (CAN_H, CAN_L)
- Multi-master: todos los nodos pueden transmitir
- Velocidades: hasta 1 Mbps
- Detección y corrección de errores
- Priorización por arbitraje

### 1.2 Topología

```
Terminación      Nodo 1       Nodo 2       Nodo 3      Terminación
120Ω            ┌──────┐     ┌──────┐     ┌──────┐        120Ω
  │─────────────┤CAN_H │─────┤CAN_H │─────┤CAN_H │─────────│
  │             │CAN_L │     │CAN_L │     │CAN_L │         │
  │─────────────┤      │─────┤      │─────┤      │─────────│
               └──────┘     └──────┘     └──────┘
```

---

## 2. CAN en STM32F407

### 2.1 Periféricos Disponibles

- **CAN1**: PB8 (RX), PB9 (TX) o PD0 (RX), PD1 (TX)
- **CAN2**: PB5 (RX), PB6 (TX) o PB12 (RX), PB13 (TX)

Usaremos **CAN1** con **PB8/PB9** + transceiver externo (TJA1050 o similar).

### 2.2 Configuración de CAN1

```c
/**
 * @brief Inicializa CAN1 a 500 kbps
 */
void CAN1_Init(void) {
    // 1. Habilitar clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    
    // 2. Configurar pines (PB8=RX, PB9=TX)
    GPIOB->MODER &= ~(0xF << (8*2));
    GPIOB->MODER |= (0xA << (8*2));  // AF mode
    GPIOB->AFR[1] &= ~(0xFF << 0);
    GPIOB->AFR[1] |= (0x99 << 0);    // AF9 (CAN1)
    
    // 3. Solicitar modo de inicialización
    CAN1->MCR |= CAN_MCR_INRQ;
    while (!(CAN1->MSR & CAN_MSR_INAK));
    
    // 4. Configurar timing para 500 kbps
    // APB1 = 42 MHz, Prescaler=6, BS1=12, BS2=2
    // Baudrate = 42MHz / (6 * (1+12+2)) = 466.67 kHz ≈ 500 kHz
    CAN1->BTR = 0;
    CAN1->BTR |= ((6-1) << 0);   // BRP (prescaler)
    CAN1->BTR |= (12 << 16);     // TS1
    CAN1->BTR |= (2 << 20);      // TS2
    CAN1->BTR |= (0 << 24);      // SJW
    
    // 5. Configurar modo normal
    CAN1->MCR &= ~CAN_MCR_SLEEP;  // Exit sleep
    CAN1->MCR &= ~CAN_MCR_INRQ;   // Exit init
    while (CAN1->MSR & CAN_MSR_INAK);
}
```

---

## 3. Mensajes CAN

### 3.1 Formato de Mensaje

```
┌────┬──────┬────┬───┬────────┬─────┬────┐
│SOF │ ID  │RTR│IDE│  DLC   │DATA │CRC │
└────┴──────┴────┴───┴────────┴─────┴────┘
```

- **ID**: Identificador (11 bits estándar, 29 bits extendido)
- **RTR**: Remote Transmission Request
- **IDE**: Identifier Extension bit
- **DLC**: Data Length Code (0-8 bytes)
- **DATA**: Hasta 8 bytes de datos

### 3.2 Transmisión de Mensaje

```c
/**
 * @brief Estructura de mensaje CAN
 */
typedef struct {
    uint32_t id;        // Identificador
    uint8_t  data[8];   // Datos (hasta 8 bytes)
    uint8_t  length;    // Longitud (0-8)
} CAN_Message_t;

/**
 * @brief Transmite un mensaje CAN
 */
int CAN1_Transmit(CAN_Message_t *msg) {
    // 1. Buscar mailbox vacío
    uint32_t tsr = CAN1->TSR;
    uint8_t mailbox;
    
    if (tsr & CAN_TSR_TME0) {
        mailbox = 0;
    } else if (tsr & CAN_TSR_TME1) {
        mailbox = 1;
    } else if (tsr & CAN_TSR_TME2) {
        mailbox = 2;
    } else {
        return -1;  // No hay mailbox disponible
    }
    
    // 2. Configurar identificador (estándar 11 bits)
    CAN1->sTxMailBox[mailbox].TIR = (msg->id << 21);
    
    // 3. Configurar DLC
    CAN1->sTxMailBox[mailbox].TDTR = msg->length & 0x0F;
    
    // 4. Cargar datos
    CAN1->sTxMailBox[mailbox].TDLR = 
        ((uint32_t)msg->data[0] << 0) |
        ((uint32_t)msg->data[1] << 8) |
        ((uint32_t)msg->data[2] << 16) |
        ((uint32_t)msg->data[3] << 24);
    
    CAN1->sTxMailBox[mailbox].TDHR = 
        ((uint32_t)msg->data[4] << 0) |
        ((uint32_t)msg->data[5] << 8) |
        ((uint32_t)msg->data[6] << 16) |
        ((uint32_t)msg->data[7] << 24);
    
    // 5. Solicitar transmisión
    CAN1->sTxMailBox[mailbox].TIR |= CAN_TI0R_TXRQ;
    
    // 6. Esperar transmisión (opcional)
    uint32_t timeout = 10000;
    while (!(CAN1->TSR & (CAN_TSR_RQCP0 << mailbox)) && timeout--);
    
    return (timeout == 0) ? -1 : 0;
}
```

### 3.3 Recepción de Mensaje

```c
/**
 * @brief Recibe un mensaje CAN del FIFO 0
 */
int CAN1_Receive(CAN_Message_t *msg) {
    // Verificar si hay mensajes pendientes
    if ((CAN1->RF0R & CAN_RF0R_FMP0) == 0) {
        return -1;  // FIFO vacío
    }
    
    // Leer identificador
    msg->id = (CAN1->sFIFOMailBox[0].RIR >> 21) & 0x7FF;
    
    // Leer DLC
    msg->length = CAN1->sFIFOMailBox[0].RDTR & 0x0F;
    
    // Leer datos
    uint32_t rdlr = CAN1->sFIFOMailBox[0].RDLR;
    uint32_t rdhr = CAN1->sFIFOMailBox[0].RDHR;
    
    msg->data[0] = (rdlr >> 0) & 0xFF;
    msg->data[1] = (rdlr >> 8) & 0xFF;
    msg->data[2] = (rdlr >> 16) & 0xFF;
    msg->data[3] = (rdlr >> 24) & 0xFF;
    msg->data[4] = (rdhr >> 0) & 0xFF;
    msg->data[5] = (rdhr >> 8) & 0xFF;
    msg->data[6] = (rdhr >> 16) & 0xFF;
    msg->data[7] = (rdhr >> 24) & 0xFF;
    
    // Liberar FIFO
    CAN1->RF0R |= CAN_RF0R_RFOM0;
    
    return 0;
}
```

---

## 4. Filtros CAN

Los filtros permiten recibir solo mensajes específicos:

```c
/**
 * @brief Configura filtro para aceptar todos los mensajes
 */
void CAN1_ConfigureFilterAll(void) {
    // Modo de inicialización de filtros
    CAN1->FMR |= CAN_FMR_FINIT;
    
    // Filtro 0: modo máscara, 32-bit, acepta todos
    CAN1->FM1R &= ~CAN_FM1R_FBM0;     // Mask mode
    CAN1->FS1R |= CAN_FS1R_FSC0;      // 32-bit scale
    CAN1->sFilterRegister[0].FR1 = 0; // ID = 0
    CAN1->sFilterRegister[0].FR2 = 0; // Mask = 0 (acepta todos)
    
    // Asignar a FIFO 0
    CAN1->FFA1R &= ~CAN_FFA1R_FFA0;
    
    // Activar filtro 0
    CAN1->FA1R |= CAN_FA1R_FACT0;
    
    // Salir de modo inicialización
    CAN1->FMR &= ~CAN_FMR_FINIT;
}

/**
 * @brief Configura filtro para ID específico
 */
void CAN1_ConfigureFilterID(uint32_t id) {
    CAN1->FMR |= CAN_FMR_FINIT;
    
    // Filtro 1: modo lista, 32-bit
    CAN1->FM1R |= CAN_FM1R_FBM1;      // List mode
    CAN1->FS1R |= CAN_FS1R_FSC1;      // 32-bit
    CAN1->sFilterRegister[1].FR1 = (id << 21);
    
    CAN1->FA1R |= CAN_FA1R_FACT1;
    CAN1->FMR &= ~CAN_FMR_FINIT;
}
```

---

## 5. Ejemplo: Red CAN Multi-Nodo

```c
/**
 * @brief Aplicación CAN: nodo sensor
 */
int main(void) {
    SystemInit();
    USART2_Init(115200);
    CAN1_Init();
    CAN1_ConfigureFilterAll();
    
    printf("Nodo CAN iniciado\r\n");
    
    CAN_Message_t tx_msg, rx_msg;
    uint32_t counter = 0;
    
    while (1) {
        // Transmitir datos periódicamente
        tx_msg.id = 0x100;  // ID del sensor
        tx_msg.length = 4;
        tx_msg.data[0] = (counter >> 0) & 0xFF;
        tx_msg.data[1] = (counter >> 8) & 0xFF;
        tx_msg.data[2] = (counter >> 16) & 0xFF;
        tx_msg.data[3] = (counter >> 24) & 0xFF;
        
        if (CAN1_Transmit(&tx_msg) == 0) {
            printf("TX: ID=0x%03lX, Datos=%lu\r\n", 
                   tx_msg.id, counter);
        }
        
        counter++;
        
        // Recibir mensajes
        if (CAN1_Receive(&rx_msg) == 0) {
            printf("RX: ID=0x%03lX, DLC=%d, Datos=", 
                   rx_msg.id, rx_msg.length);
            for (int i = 0; i < rx_msg.length; i++) {
                printf("%02X ", rx_msg.data[i]);
            }
            printf("\r\n");
        }
        
        delay_ms(1000);
    }
}
```

---

## Actividades Prácticas

### Actividad 1: Configuración CAN (25 min)

**Tareas**:
1. Configurar CAN1 a 500 kbps
2. Implementar TX/RX de mensajes
3. Verificar con analizador CAN o segundo nodo
4. Medir baudrate con osciloscopio

**Evidencia**: Código + captura de comunicación

---

### Actividad 2: Red CAN Multi-Nodo (30 min)

**Tareas**:
1. Configurar 2-3 nodos STM32
2. Implementar protocolo simple (comandos/respuestas)
3. Enviar datos de sensores entre nodos
4. Implementar heartbeat (mensaje periódico de vida)

**Evidencia**: Demostración en vivo + logs

---

### Actividad 3: Filtros CAN (20 min)

**Tareas**:
1. Configurar filtro para ID específico
2. Transmitir múltiples IDs y verificar filtrado
3. Implementar filtro por rango de IDs
4. Documentar comportamiento

**Evidencia**: Reporte con pruebas de filtrado

---

## Quiz Formativo

**P1**: ¿Por qué CAN usa señales diferenciales?
**R**: b) Mayor inmunidad al ruido

**P2**: ¿Qué determina la prioridad en el arbitraje CAN?
**R**: a) El ID más bajo tiene mayor prioridad

**P3**: ¿Cuántos bytes de datos puede llevar un mensaje CAN estándar?
**R**: d) 0 a 8 bytes

**P4**: ¿Para qué sirven los filtros CAN?
**R**: b) Reducir carga de CPU recibiendo solo mensajes relevantes

**P5**: ¿Qué es RTR en CAN?
**R**: c) Remote Transmission Request (solicitud de datos)

---

## Evidencias

1. Driver CAN1 funcional
2. Red multi-nodo operativa
3. Análisis de tráfico CAN

---

## Material para Casa

- **CAN Specification 2.0B** - Bosch
- **ISO 11898** - Standard CAN
- Comparar CAN vs CAN-FD

---

## Referencias

1. **STM32F407 Reference Manual**, Sección 33: bxCAN
2. **CAN Specification 2.0** - Bosch
3. **Application Note AN4981**: Using CAN in STM32

---

**Fin de Sesión 6**
