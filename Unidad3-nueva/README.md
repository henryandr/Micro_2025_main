# Unidad 3: Protocolos de Comunicación Serial en Microcontroladores ARM Cortex M-4

## Mapa de Navegación

### 📚 Documentos Principales

- **[README.md](README.md)** - Este documento (mapa de navegación y visión general)
- **[supuestos.md](supuestos.md)** - Supuestos técnicos y conocimientos previos
- **[depuracion_contenidos.md](depuracion_contenidos.md)** - Análisis de contenidos previos
- **[ampliaciones.md](ampliaciones.md)** - Contenidos añadidos y expandidos
- **[proyeccion_unidad4.md](proyeccion_unidad4.md)** - Recomendaciones para siguientes unidades

### 🎯 Sesiones de Clase

La unidad está organizada en **8 sesiones** de 2 horas cada una, distribuidas en 4 semanas:

| Semana | Sesión | Tema Principal | Protocolos | Archivo |
|--------|--------|----------------|------------|---------|
| 1 | 1 | Introducción a Comunicaciones Seriales y USART | USART/UART | [sesion1.md](sesiones/sesion1.md) |
| 1 | 2 | USART Avanzado: Interrupciones y Debugging | USART/UART | [sesion2.md](sesiones/sesion2.md) |
| 2 | 3 | Protocolo I2C: Fundamentos y Configuración | I2C | [sesion3.md](sesiones/sesion3.md) |
| 2 | 4 | I2C Avanzado: Sensores y Pantallas OLED | I2C | [sesion4.md](sesiones/sesion4.md) |
| 3 | 5 | Protocolo SPI: Master/Slave y Aplicaciones | SPI | [sesion5.md](sesiones/sesion5.md) |
| 3 | 6 | Protocolo CAN: Introducción y Configuración | CAN | [sesion6.md](sesiones/sesion6.md) |
| 4 | 7 | Integración de Protocolos y Proyecto | Todos | [sesion7.md](sesiones/sesion7.md) |
| 4 | 8 | Proyecto Final y Evaluación Sumativa | Todos | [sesion8.md](sesiones/sesion8.md) |

### 💻 Código y Ejemplos

#### Ejemplos en C
- **[ejemplos_c/](ejemplos_c/)** - Código C bare metal comentado y funcional
  - `usart_basic.c` - Transmisión/recepción básica por polling
  - `usart_interrupciones.c` - USART con interrupciones
  - `printf_redirect.c` - Redirección de printf a USART
  - `i2c_eeprom.c` - Lectura/escritura en EEPROM I2C
  - `i2c_oled.c` - Control de pantalla OLED por I2C
  - `spi_sensor.c` - Lectura de sensor SPI
  - `spi_oled.c` - Control de pantalla OLED por SPI
  - `can_basic.c` - Envío/recepción CAN
  - `proyecto_integrador.c` - Proyecto final completo

#### Drivers en C
- **[drivers_moduloxx/](drivers_moduloxx/)** - Drivers bare metal modulares
  - `usart_driver.c/h` - Driver USART completo
  - `i2c_driver.c/h` - Driver I2C master
  - `spi_driver.c/h` - Driver SPI master
  - `can_driver.c/h` - Driver CAN básico
  - `oled_i2c_driver.c/h` - Driver pantalla OLED I2C (SSD1306)
  - `oled_spi_driver.c/h` - Driver pantalla OLED SPI

### 📝 Evaluaciones

- **[evaluaciones/](evaluaciones/)** - Instrumentos de evaluación completos
  - `diagnostica.md` - Evaluación diagnóstica (Sesión 1)
  - `quizzes_formativos.md` - Quizzes por sesión (Sesiones 1-8)
  - `sumativa.md` - Evaluación final (Sesión 8)
  - `rubricas.md` - Rúbricas de evaluación detalladas
  - `entregables_proyecto.md` - Especificación de entregables incrementales

### 📖 Recursos de Apoyo

- **[recursos/](recursos/)** - Material complementario
  - `bibliografia.md` - Bibliografía anotada y enlaces
  - `glosario.md` - Glosario de términos de comunicaciones
  - `comparacion_protocolos.md` - Comparación USART/I2C/SPI/CAN
  - `guia_debugging_serial.md` - Guía de debugging de comunicaciones
  - `herramientas_analisis.md` - Logic analyzers, osciloscopios, software

---

## Visión General de la Unidad

### Objetivo General

Desarrollar competencias prácticas en la creación y uso de drivers para protocolos de comunicación serial (USART/UART, I2C, SPI, CAN) en microcontroladores ARM Cortex M-4, utilizando programación en C bare metal sin dependencias de HAL, para permitir la integración de periféricos externos y comunicación entre sistemas embebidos.

### Competencias a Desarrollar

Al finalizar esta unidad, el estudiante será capaz de:

1. **Configurar y utilizar USART/UART** para comunicación serial asíncrona
   - Configurar baudrate, paridad, bits de parada
   - Implementar transmisión y recepción por polling e interrupciones
   - Utilizar USART para debugging (printf redirect, logs)

2. **Implementar comunicación I2C** en modo master
   - Configurar el periférico I2C (velocidad, timing)
   - Realizar operaciones de lectura/escritura con dispositivos I2C
   - Integrar memorias EEPROM y pantallas OLED I2C

3. **Configurar y utilizar SPI** en modo master
   - Configurar parámetros SPI (CPOL, CPHA, velocidad)
   - Comunicarse con sensores y periféricos SPI
   - Controlar pantallas OLED y otros dispositivos SPI

4. **Utilizar el protocolo CAN** para comunicación en red
   - Configurar el controlador CAN (baudrate, filtros)
   - Enviar y recibir mensajes CAN
   - Implementar comunicación entre nodos

5. **Diseñar drivers modulares** siguiendo buenas prácticas
   - Separación de interfaz (header) e implementación
   - Documentación clara de funciones y parámetros
   - Manejo apropiado de errores y timeouts

6. **Integrar múltiples protocolos** en un sistema completo
   - Combinar USART, I2C, SPI y CAN en un mismo proyecto
   - Gestionar recursos y prioridades
   - Debugging avanzado de sistemas multi-protocolo

### Estructura Pedagógica

La unidad sigue una progresión lógica:

**Semana 1: USART/UART** - El protocolo más simple, ideal para debugging
- Sesión 1: Fundamentos y configuración básica
- Sesión 2: Interrupciones y aplicaciones avanzadas

**Semana 2: I2C** - Protocolo síncrono de 2 hilos, muy común en sensores
- Sesión 3: Fundamentos y configuración del bus I2C
- Sesión 4: Aplicaciones prácticas (EEPROM, OLED)

**Semana 3: SPI y CAN** - Protocolos de mayor velocidad y aplicaciones específicas
- Sesión 5: SPI master/slave, aplicaciones de alta velocidad
- Sesión 6: CAN para redes de microcontroladores

**Semana 4: Integración** - Proyecto completo combinando todos los protocolos
- Sesión 7: Integración y debugging multi-protocolo
- Sesión 8: Presentación de proyecto final

### Proyecto Integrador

El proyecto final combina los 4 protocolos en un sistema completo:

**Opción 1: Sistema de Monitoreo y Control IoT**
- **USART**: Comunicación con PC, debugging, logs
- **I2C**: Sensores (temperatura, humedad, presión)
- **SPI**: Pantalla OLED para visualización local
- **CAN**: Comunicación con otros nodos/módulos

**Opción 2: Robot Autónomo con Comunicación**
- **USART**: Comandos desde PC, telemetría
- **I2C**: Sensores de proximidad, acelerómetro
- **SPI**: Display para estado del sistema
- **CAN**: Comunicación entre módulos del robot

**Opción 3: Sistema de Adquisición de Datos Distribuido**
- **USART**: Envío de datos al servidor
- **I2C**: Múltiples sensores en el bus
- **SPI**: Almacenamiento en SD card
- **CAN**: Red de nodos de sensores

### Entregables Incrementales

El proyecto se desarrolla en fases:

1. **Semana 1**: Driver USART + printf redirect funcionando
2. **Semana 2**: Integración I2C (al menos un sensor o EEPROM)
3. **Semana 3**: Integración SPI y/o CAN
4. **Semana 4**: Sistema completo integrado + documentación

### Metodología

Cada sesión incluye:

- **Objetivos específicos** medibles y alcanzables
- **Contenido teórico** con fundamentos del protocolo
- **Ejemplos de código** completos y comentados
- **Actividades prácticas** (3 por sesión mínimo):
  - Configuración de registros
  - Implementación de funciones del driver
  - Integración con hardware real o simulado
  - Debugging de problemas comunes
- **Quiz formativo** (5 preguntas) para verificar comprensión
- **Evidencia de aprendizaje** tangible por sesión
- **Material para casa** (lecturas, ejercicios preparatorios)

### Evaluación

La evaluación es continua y sumativa:

- **Diagnóstica** (Sesión 1): 10% - Conocimientos previos
- **Formativa continua**: 30% - Quizzes, actividades, entregables incrementales
- **Proyecto final**: 60% - Sistema integrador completo
  - Funcionalidad: 40%
  - Código y estructura: 30%
  - Documentación: 15%
  - Presentación: 15%

### Herramientas Necesarias

- **Hardware**:
  - STM32F407 Discovery o similar
  - Dispositivos I2C (sensor, EEPROM, OLED)
  - Dispositivos SPI (sensor, OLED)
  - Conversor USB-Serial
  - (Opcional) Segundo MCU para CAN

- **Software**:
  - arm-none-eabi-gcc toolchain
  - OpenOCD o ST-Link utilities
  - Terminal serial (PuTTY, minicom, screen)
  - (Opcional) Logic analyzer software (PulseView)

- **Documentación**:
  - STM32F407 Reference Manual
  - STM32F407 Datasheet
  - Datasheets de dispositivos periféricos

### Diferenciadores de Esta Unidad

Comparada con Unidades 1 y 2:

- **Unidad 1**: Fundamentos de Assembly y arquitectura
- **Unidad 2**: Transición a C, MEF, GPIO, Timer, interrupciones básicas
- **Unidad 3**: 
  - Enfoque en **comunicación externa**
  - Integración de **periféricos reales**
  - **Protocolos estándar** de la industria
  - **Debugging avanzado** con herramientas
  - **Proyectos más complejos** multi-protocolo

### Buenas Prácticas Enfatizadas

1. **Modularidad**: Cada protocolo en su propio driver
2. **Abstracción**: API clara independiente del hardware
3. **Manejo de errores**: Timeouts, NACK, colisiones
4. **Documentación**: Comentarios Doxygen en todos los drivers
5. **Testing**: Verificación incremental de funcionalidad
6. **Debugging sistemático**: Uso de logs, analizadores lógicos

### Progresión Hacia Siguientes Unidades

Esta unidad sienta las bases para:

- **Unidad 4**: Sistemas operativos en tiempo real (RTOS)
- **Unidad 5**: Protocolos de red (Ethernet, TCP/IP, MQTT)
- **Unidad 6**: Optimización y proyectos avanzados

---

## Tabla de Planificación Semanal

| Semana | Sesión | Tema Central | Protocolos | Objetivos Específicos | Actividades Clave | Evidencia |
|--------|--------|--------------|------------|----------------------|-------------------|-----------|
| **1** | **1** | Introducción a Comunicación Serial y USART | USART | - Comprender comunicación serial asíncrona<br>- Configurar USART básico<br>- Transmitir/recibir por polling | - Config registros USART<br>- Echo test<br>- Envío de mensajes | Driver USART básico |
| **1** | **2** | USART con Interrupciones y Debugging | USART | - Implementar TX/RX con IRQ<br>- Redirigir printf a USART<br>- Crear sistema de logs | - Buffer circular<br>- Printf redirect<br>- Log levels | Printf funcional + logs |
| **2** | **3** | Protocolo I2C: Fundamentos | I2C | - Entender protocolo I2C<br>- Configurar I2C master<br>- Detectar dispositivos | - Config I2C<br>- Bus scanning<br>- EEPROM básico | Driver I2C + scan |
| **2** | **4** | I2C Avanzado: Sensores y OLED | I2C | - Leer sensores I2C<br>- Controlar pantalla OLED<br>- Integrar múltiples devices | - Sensor temp/hum<br>- OLED init/display<br>- Multi-device | OLED mostrando datos |
| **3** | **5** | Protocolo SPI | SPI | - Configurar SPI master<br>- Comunicarse con SPI devices<br>- Optimizar velocidad | - Config SPI<br>- Sensor SPI<br>- OLED SPI | Driver SPI funcional |
| **3** | **6** | Protocolo CAN | CAN | - Entender CAN<br>- Configurar CAN bus<br>- Enviar/recibir mensajes | - Config CAN<br>- TX/RX messages<br>- Filtros CAN | Comunicación CAN |
| **4** | **7** | Integración Multi-Protocolo | Todos | - Integrar 4 protocolos<br>- Debugging avanzado<br>- Gestión de recursos | - Sistema integrado<br>- Logic analyzer<br>- Troubleshooting | Prototipo integrado |
| **4** | **8** | Proyecto Final | Todos | - Completar proyecto<br>- Documentar sistema<br>- Presentar resultados | - Finalizar proyecto<br>- Documentación<br>- Demo y defensa | Proyecto completo |

---

## Recursos Recomendados

### Lectura Obligatoria
- STM32F4xx Reference Manual (RM0090) - Capítulos USART, I2C, SPI, CAN
- Application Notes de ST:
  - AN4031: Using the STM32F2, STM32F4 and STM32F7 hardware I2C
  - AN2606: STM32 microcontroller system memory boot mode
  - AN3281: CAN protocol used in the STM32 bootloader

### Tutoriales y Videos
- [Implementing UART in STM32 (ControllersTech)](https://controllerstech.com/)
- [I2C Protocol Tutorial (SparkFun)](https://learn.sparkfun.com/tutorials/i2c)
- [SPI Communication (Analog Devices)](https://www.analog.com/en/analog-dialogue/articles/introduction-to-spi-interface.html)

### Herramientas
- Logic Analyzer: Saleae Logic, Sigrok/PulseView
- Terminal Serial: PuTTY, minicom, screen, CoolTerm
- CAN Bus Analyzer: CANable, PCAN-USB

### Datasheets Importantes
- SSD1306: OLED display controller (I2C/SPI)
- AT24Cxx: EEPROM I2C
- MCP2515: Stand-alone CAN controller
- Sensors: BME280, MPU6050, etc.

---

## Criterios de Calidad del Código

Todo el código de la unidad sigue estos estándares:

1. **Compilabilidad**: Compatible con `arm-none-eabi-gcc`
2. **Comentarios**: Explicaciones en español de secciones críticas
3. **Documentación**: Headers estilo Doxygen
4. **Modularidad**: Separación .h/.c clara
5. **Manejo de errores**: Códigos de error, timeouts apropiados
6. **Portabilidad**: Configuración centralizada, fácil adaptación
7. **Ejemplos**: Código de ejemplo para cada función del driver

---

## Notas para el Instructor

### Desafíos Comunes

1. **Hardware**: No todos los estudiantes tienen todos los periféricos
   - Solución: Enfocarse en simulación y datasheet reading
   - Alternativa: Trabajo en equipos, compartir hardware

2. **Debugging**: Protocolos invisibles, difíciles de debuggear
   - Solución: Énfasis en logging por USART
   - Herramienta: Logic analyzer (software libre disponible)

3. **Complejidad**: 4 protocolos en 4 semanas es intenso
   - Solución: Entregables incrementales, no todo perfecto
   - Enfoque: Priorizar USART e I2C, SPI/CAN más básicos

### Adaptaciones Posibles

- **Sin hardware**: Usar simuladores (Proteus, Renode)
- **Menos tiempo**: Eliminar CAN, enfocarse en USART/I2C/SPI
- **Más tiempo**: Profundizar en cada protocolo, más proyectos

### Material Adicional Sugerido

- Videos de analizadores lógicos mostrando señales reales
- Demos de protocolos en osciloscopio
- Comparación de velocidades (benchmarks)
- Casos de uso industriales reales

---

## Checklist de Validación ✓

Verificación de criterios de calidad:

- [✓] Estructura clara y jerárquica (H1, H2, H3)
- [✓] Terminología consistente en español técnico
- [✓] 8 sesiones bien definidas (4 semanas, 2/semana)
- [✓] Objetivos específicos por sesión
- [✓] Mínimo 3 actividades prácticas por sesión
- [✓] Evaluación diagnóstica, formativa y sumativa
- [✓] Proyecto integrador multi-protocolo
- [✓] Entregables incrementales definidos
- [✓] Ejemplos de código (USART, I2C, SPI, CAN)
- [✓] Drivers modulares para cada protocolo
- [✓] Material complementario (bibliografía, glosario)
- [✓] Rúbricas de evaluación
- [✓] Referencias a datasheets y documentación oficial
- [✓] Énfasis en bare metal (sin HAL)
- [✓] Buenas prácticas de diseño de drivers
- [✓] Debugging avanzado incluido

---

**Versión**: 1.0  
**Fecha**: Noviembre 2025  
**Autor**: Generado para curso Microprocesadores ARM Cortex M-4  
**Microcontrolador de referencia**: STM32F407VGT6
