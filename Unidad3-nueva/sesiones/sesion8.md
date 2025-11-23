# Sesión 8: Proyecto Final Integrador

**Unidad 3**: Protocolos de Comunicación Serial  
**Semana**: 4  
**Duración**: 2 horas + trabajo autónomo

---

## Objetivos

1. Aplicar todos los conocimientos de protocolos seriales en un proyecto completo
2. Integrar USART, I2C, SPI y CAN en una solución funcional
3. Presentar y documentar el proyecto final
4. Evaluar competencias adquiridas

---

## 1. Especificaciones del Proyecto Final

### 1.1 Opciones de Proyecto

Los estudiantes deben elegir **una de las tres opciones**:

#### Opción 1: Sistema IoT de Monitoreo Ambiental

**Descripción**: Estación meteorológica que recopila datos y los distribuye.

**Requisitos**:
- **USART**: Debug, logs y comandos desde PC
- **I2C**: Sensor BME280 (temperatura, humedad, presión)
- **SPI**: Pantalla OLED para visualización local
- **CAN**: Comunicación con estación central (otro nodo)

**Funcionalidades**:
1. Lectura de sensores cada 1 segundo
2. Visualización en OLED (valores actuales + gráfico simple)
3. Envío de datos por CAN a estación central
4. Comandos por USART: `READ`, `CALIB`, `STATUS`
5. Almacenamiento de últimos 100 registros en RAM
6. LED indicador de estado (verde=OK, rojo=error)

---

#### Opción 2: Robot Autónomo Comunicado

**Descripción**: Robot móvil con sensores y comunicación distribuida.

**Requisitos**:
- **USART**: Comandos de navegación desde PC
- **I2C**: Sensor de distancia VL53L0X o similar
- **SPI**: Acelerómetro/giroscopio MPU6050
- **CAN**: Comunicación entre módulos del robot

**Funcionalidades**:
1. Control por comandos: `FORWARD`, `BACK`, `LEFT`, `RIGHT`, `STOP`
2. Lectura de sensores de distancia (I2C)
3. Lectura de orientación (SPI)
4. Envío de telemetría por CAN (posición, velocidad, sensores)
5. Modo autónomo: evitar obstáculos usando sensores
6. Display SPI opcional para mostrar estado

---

#### Opción 3: Sistema de Adquisición de Datos Distribuido

**Descripción**: Red de nodos sensores con almacenamiento y visualización.

**Requisitos**:
- **USART**: Interfaz con PC para descarga de datos
- **I2C**: Múltiples sensores (temp, luz, acelerómetro)
- **SPI**: Tarjeta microSD para almacenamiento
- **CAN**: Red de nodos intercambiando información

**Funcionalidades**:
1. Lectura de 3+ sensores I2C
2. Almacenamiento en microSD (SPI) en formato CSV
3. Sincronización de timestamp entre nodos (CAN)
4. Descarga de datos por USART con protocolo simple
5. Configuración de intervalos de muestreo
6. Indicadores LED de actividad

---

### 1.2 Requisitos Comunes (Todas las Opciones)

**Técnicos**:
- ✅ Código en C bare metal (sin HAL)
- ✅ Drivers modulares (.h/.c separados)
- ✅ Uso obligatorio de los 4 protocolos
- ✅ Gestión de errores y timeouts
- ✅ Al menos 2 interrupciones activas

**Documentación**:
- README.md con instrucciones de uso
- Diagrama de bloques del sistema
- Diagrama de conexiones (hardware)
- Comentarios en código (estilo Doxygen)

**Testing**:
- Video demostrativo (2-3 minutos)
- Casos de prueba documentados
- Manejo de al menos 2 condiciones de error

---

## 2. Entregables Incrementales

### Semana 1 (Sesión 2)
- [x] Driver USART funcional con printf
- [x] Especificación preliminar del proyecto
- [x] Lista de componentes necesarios

### Semana 2 (Sesión 4)
- [x] Driver I2C + sensor funcionando
- [x] Almacenamiento en EEPROM (I2C) funcional
- [x] Primera versión del README

### Semana 3 (Sesión 6)
- [x] Driver SPI operativo
- [x] Driver CAN básico
- [x] Integración parcial de 3 protocolos

### Semana 4 (Sesión 8)
- [x] Sistema completamente integrado
- [x] Documentación completa
- [x] Video demostrativo
- [x] Presentación en clase

---

## 3. Evaluación Sumativa

### 3.1 Componente Teórico (30%)

**Examen escrito (90 minutos)**

**Sección A: Conceptos (10 puntos)**
1. Comparar USART, I2C, SPI y CAN (4 puntos)
2. Explicar arbitraje de CAN (3 puntos)
3. Modos de reloj SPI (CPOL/CPHA) (3 puntos)

**Sección B: Análisis de Código (10 puntos)**
- Identificar errores en driver I2C (5 puntos)
- Optimizar función de transmisión SPI (5 puntos)

**Sección C: Diseño (10 puntos)**
- Proponer arquitectura para sistema multi-protocolo (5 puntos)
- Calcular baudrates y timings (5 puntos)

---

### 3.2 Componente Práctico (70%)

**Proyecto Final + Presentación**

| Criterio | Puntos | Descripción |
|----------|--------|-------------|
| **Funcionalidad** | 25 | Sistema opera según especificaciones |
| **Código** | 20 | Calidad, modularidad, comentarios |
| **Integración** | 10 | Los 4 protocolos funcionan juntos |
| **Documentación** | 10 | README, diagramas, código documentado |
| **Presentación** | 5 | Claridad, dominio del tema |
| **TOTAL** | 70 | |

---

### 3.3 Rúbrica Detallada

#### Funcionalidad (25 puntos)

| Nivel | Puntos | Descripción |
|-------|--------|-------------|
| Excelente | 23-25 | Sistema completo, maneja errores, extra features |
| Bueno | 18-22 | Funciones principales operan correctamente |
| Aceptable | 13-17 | Funciona parcialmente, algunos errores |
| Insuficiente | 0-12 | No funciona o incompleto |

#### Código (20 puntos)

**Estructura (8 puntos)**:
- Drivers modulares (.h/.c) (3 pts)
- Funciones bien definidas (3 pts)
- Constantes y macros apropiadas (2 pts)

**Calidad (7 puntos)**:
- Sin código duplicado (2 pts)
- Variables con nombres descriptivos (2 pts)
- Manejo de errores (3 pts)

**Documentación (5 puntos)**:
- Comentarios de funciones (Doxygen) (3 pts)
- Explicación de secciones complejas (2 pts)

#### Integración (10 puntos)

- USART funcional (2 pts)
- I2C funcional (2 pts)
- SPI funcional (2 pts)
- CAN funcional (2 pts)
- Integración coherente (2 pts)

#### Documentación del Proyecto (10 puntos)

- README completo (3 pts)
- Diagrama de bloques (2 pts)
- Diagrama de conexiones (2 pts)
- Instrucciones de uso (2 pts)
- Referencias (1 pt)

#### Presentación (5 puntos)

- Claridad en explicación (2 pts)
- Demostración en vivo (2 pts)
- Respuestas a preguntas (1 pt)

---

## 4. Formato de Presentación

### 4.1 Estructura (10 minutos por equipo)

1. **Introducción** (1 min)
   - Opción elegida y motivación
   - Objetivos del proyecto

2. **Arquitectura** (2 min)
   - Diagrama de bloques
   - Explicación de componentes

3. **Implementación** (3 min)
   - Decisiones técnicas importantes
   - Desafíos y soluciones
   - Fragmentos de código clave

4. **Demostración** (3 min)
   - Funcionamiento en vivo
   - Casos de uso principales
   - Manejo de errores

5. **Conclusiones** (1 min)
   - Aprendizajes
   - Posibles mejoras

---

## 5. Criterios de Aprobación

**Para aprobar la Unidad 3**:
- Examen teórico ≥ 60% (18/30 puntos)
- Proyecto práctico ≥ 60% (42/70 puntos)
- **Nota mínima total**: 60/100 puntos

**Para excelencia**:
- ≥ 90 puntos + implementación de característica avanzada:
  - DMA en algún protocolo
  - Bootloader por USART
  - Protocolo de aplicación sobre CAN
  - Interfaz gráfica avanzada en OLED

---

## 6. Actividades de la Sesión 8

### Primera Hora: Integración Final

- Completar integración de todos los protocolos
- Realizar testing exhaustivo
- Preparar material de presentación

### Segunda Hora: Presentaciones

- Presentaciones de equipos (10 min c/u)
- Sesión de Q&A
- Feedback del instructor

---

## 7. Recursos de Apoyo

### Hardware Recomendado

**Opción 1 (IoT)**:
- STM32F407 Discovery/Nucleo
- BME280 breakout
- OLED SSD1306 (SPI)
- Transceiver CAN (TJA1050)
- Cables y protoboard

**Opción 2 (Robot)**:
- Chasis de robot + motores
- Drivers de motor (L298N o similar)
- Sensores VL53L0X (I2C)
- MPU6050 (SPI)
- Transceiver CAN

**Opción 3 (Adquisición)**:
- Módulo microSD (SPI)
- 3+ sensores I2C diversos
- RTC I2C (DS3231)
- Transceiver CAN

### Software

- arm-none-eabi-gcc
- OpenOCD / ST-Link utility
- Terminal serial (PuTTY, minicom)
- Analizador lógico (Saleae Logic)
- Git para control de versiones

---

## 8. Checklist Final

**Antes de la Presentación**:
- [ ] Código compila sin errores ni warnings
- [ ] Todos los drivers probados individualmente
- [ ] Sistema completo probado al menos 3 veces
- [ ] Video de demostración grabado (backup)
- [ ] README completo y revisado
- [ ] Diagramas claros y legibles
- [ ] Presentación preparada y ensayada
- [ ] Hardware armado y verificado

---

## 9. Extensiones Opcionales (Bonus)

**+5 puntos extras** por implementar:
- [ ] Interfaz web para visualización (ESP32 + WiFi)
- [ ] Aplicación móvil para control
- [ ] Logger de datos con timestamp RTC
- [ ] Protocolo de aplicación documentado
- [ ] Modo de bajo consumo (sleep entre lecturas)

---

## 10. Reflexión y Cierre

### Preguntas de Autoevaluación

1. ¿Qué protocolo fue más desafiante? ¿Por qué?
2. ¿Cómo mejorarías tu diseño?
3. ¿Qué aprendiste sobre debugging de hardware?
4. ¿Cómo aplicarías esto en un proyecto real?

### Proyección a Unidad 4 (opcional)

Si el curso continúa:
- **RTOS (FreeRTOS)**: Gestión de múltiples tareas
- **USB**: Comunicación con PC sin USART
- **Ethernet**: IoT y conectividad de red
- **Wireless**: Bluetooth, WiFi, LoRa

---

## Notas para el Instructor

### Preparación

- Reservar laboratorio con suficientes componentes
- Preparar lista de verificación de hardware
- Tener hardware de respaldo disponible
- Preparar examen teórico con anticipación

### Durante Presentaciones

- Limitar tiempo estrictamente (10 min)
- Tomar notas para feedback
- Permitir 2-3 preguntas por equipo
- Enfocarse en proceso, no solo resultado

### Evaluación

- Usar rúbrica consistentemente
- Considerar esfuerzo y progreso
- Dar feedback constructivo
- Documentar para referencia futura

---

## Referencias Finales

1. **STM32F407 Reference Manual** - Completo
2. **Datasheets** de todos los periféricos usados
3. **Application Notes** relevantes de ST
4. **"Making Embedded Systems"** - Elecia White
5. **"Embedded Systems Architecture"** - Daniele Lacamera

---

## Conclusión de la Unidad 3

¡Felicitaciones por completar la Unidad 3!

Has desarrollado competencias en:
✅ Comunicación USART/UART
✅ Protocolo I2C (master)
✅ Protocolo SPI (master)
✅ Bus CAN
✅ Integración multi-protocolo
✅ Debugging avanzado
✅ Desarrollo de drivers bare metal

**Próximo nivel**: Sistemas operativos en tiempo real, comunicaciones inalámbricas, y aplicaciones IoT avanzadas.

---

**¡Éxito en su proyecto final!**

**Fin de Sesión 8 y Unidad 3**
