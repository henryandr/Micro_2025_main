# Entregables del Proyecto - Unidad 3

## Semana 1: Fundamentos USART

**Entregable 1**: Driver USART básico
- Archivo: `usart_driver.c/h`
- Funciones mínimas:
  - `USART_Init()`
  - `USART_SendByte()`
  - `USART_ReceiveByte()`
- Test: Echo funcional a 115200 bps
- **Fecha límite**: Fin de Sesión 2

---

## Semana 2: I2C y Sensores

**Entregable 2**: Driver I2C + Sensor
- Archivos: `i2c_driver.c/h`, `sensor_app.c`
- Funciones mínimas:
  - `I2C_Init()`
  - `I2C_WriteByte()`
  - `I2C_ReadByte()`
  - Lectura de sensor específico
- Test: Leer y mostrar datos del sensor
- **Fecha límite**: Fin de Sesión 4

---

## Semana 3: SPI y CAN

**Entregable 3**: Drivers SPI y CAN básicos
- Archivos: `spi_driver.c/h`, `can_driver.c/h`
- SPI: Comunicación con dispositivo (OLED o sensor)
- CAN: TX/RX de mensajes entre nodos
- Test: Demostración de ambos protocolos
- **Fecha límite**: Fin de Sesión 6

---

## Semana 4: Proyecto Integrador

**Entregable Final**: Sistema completo
- Todos los drivers integrados
- Aplicación funcional según opción elegida
- Documentación completa:
  - README.md
  - Diagramas (bloques, conexiones)
  - Comentarios en código
- Video demo (2-3 min)
- **Fecha límite**: Sesión 8

---

## Formato de Entrega

**Estructura de carpetas**:
```
proyecto_unidad3/
├── README.md
├── src/
│   ├── main.c
│   ├── drivers/
│   │   ├── usart_driver.c/h
│   │   ├── i2c_driver.c/h
│   │   ├── spi_driver.c/h
│   │   └── can_driver.c/h
│   └── app/
│       └── application.c
├── docs/
│   ├── diagrama_bloques.png
│   └── diagrama_conexiones.png
└── video/
    └── demo.mp4
```

**Método de entrega**: Repositorio Git (GitHub/GitLab) + presentación en clase
