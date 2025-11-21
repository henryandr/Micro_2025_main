# Proyección a Unidad 2 - Recomendaciones

Este documento identifica las brechas y oportunidades detectadas durante la Unidad 1, y proporciona recomendaciones para el diseño de la Unidad 2.

## Competencias Consolidadas en Unidad 1

Al finalizar la Unidad 1, los estudiantes deberían tener:

### ✅ Conocimientos Sólidos

1. **Arquitectura de Procesadores**: Comprensión clara de Harvard, von Neumann, RISC, CISC
2. **ARM Cortex M-4**: Conocimiento de componentes internos, registros, mapa de memoria
3. **Assembly Básico**: Capacidad de escribir programas simples con operaciones aritméticas, lógicas, y control de flujo
4. **GPIO**: Configuración completa de pines digitales (entrada/salida, pull-up/down, velocidad)
5. **Timer Básico**: Generación de delays y eventos periódicos
6. **Interrupciones**: Conceptos fundamentales, NVIC, y manejo básico

### ✅ Habilidades Desarrolladas

1. Leer y extraer información de datasheets
2. Configurar registros de periféricos mediante manipulación de bits
3. Escribir drivers bare metal simples
4. Depurar código a bajo nivel
5. Interoperar código C y Assembly

## Brechas Identificadas

### 📌 Conocimientos Limitados que Requieren Expansión

1. **Comunicación Serial**: Solo se mencionó UART conceptualmente
   - **Brecha**: No se implementó comunicación UART funcional
   - **Impacto**: Los estudiantes no pueden comunicarse con PC para debugging avanzado

2. **ADC (Analog-to-Digital Converter)**: No cubierto
   - **Brecha**: No pueden leer sensores analógicos
   - **Impacto**: Limitación para proyectos con sensores comunes

3. **PWM (Pulse Width Modulation)**: No cubierto
   - **Brecha**: No pueden controlar motores o LEDs con intensidad variable
   - **Impacto**: Limitación para proyectos de control

4. **DMA (Direct Memory Access)**: No mencionado
   - **Brecha**: Transferencias de datos sin intervención de CPU
   - **Impacto**: Ineficiencia en transferencias de grandes volúmenes

5. **Watchdog Timer**: No cubierto
   - **Brecha**: No conocen mecanismos de recuperación automática
   - **Impacto**: Sistemas menos robustos

6. **Low Power Modes**: Solo mencionado superficialmente
   - **Brecha**: No saben optimizar consumo energético
   - **Impacto**: Limitación para dispositivos con batería

7. **RTC (Real-Time Clock)**: No cubierto
   - **Brecha**: No pueden mantener tiempo real
   - **Impacto**: Limitación para aplicaciones con registro temporal

### 📌 Habilidades que Necesitan Profundización

1. **Optimización de Código**: Solo ejemplos básicos
   - **Necesidad**: Técnicas avanzadas de optimización para recursos limitados

2. **Debugging Avanzado**: Solo conceptos básicos
   - **Necesidad**: Uso de SWD, breakpoints, watchpoints, tracepoints

3. **Gestión de Memoria**: Solo conceptos básicos de stack/heap
   - **Necesidad**: Técnicas avanzadas, fragmentación, memory pools

4. **Protocolos de Comunicación**: Solo conceptos
   - **Necesidad**: Implementación completa de I2C, SPI

5. **Manejo de Errores**: Limitado
   - **Necesidad**: Estrategias robustas de detección y recuperación

## Recomendaciones para Unidad 2

### Tema Central Sugerido

**"Periféricos Avanzados y Comunicación en Sistemas Embebidos"**

### Estructura Sugerida (4 semanas, 8 sesiones)

#### Semana 1: Comunicación Serial

**Sesión 1**: UART - Teoría y Configuración
- Protocolo UART: baud rate, start/stop bits, paridad
- Configuración de registros USART en STM32
- Transmisión básica (polling)

**Sesión 2**: UART - Recepción y Uso Práctico
- Recepción con polling y con interrupciones
- Buffers circulares
- Printf redirection para debugging

**Entregable**: Driver UART funcional con printf() redirigido

#### Semana 2: Conversión Analógica y PWM

**Sesión 3**: ADC - Conversión Analógica Digital
- Principios de ADC: resolución, tiempo de muestreo, canales
- Configuración de ADC en STM32
- Lectura de potenciómetro y sensores analógicos

**Sesión 4**: PWM - Modulación por Ancho de Pulso
- Teoría de PWM: duty cycle, frecuencia
- Configuración de Timer en modo PWM
- Control de intensidad de LED y motores DC

**Entregable**: Sistema de control con lectura de sensor (ADC) y actuación (PWM)

#### Semana 3: Protocolos de Comunicación

**Sesión 5**: I2C - Inter-Integrated Circuit
- Protocolo I2C: master/slave, addressing, ACK/NACK
- Configuración de I2C en STM32
- Lectura de sensor I2C (ejemplo: acelerómetro, EEPROM)

**Sesión 6**: SPI - Serial Peripheral Interface
- Protocolo SPI: MOSI, MISO, SCK, SS
- Configuración de SPI en STM32
- Comunicación con sensor SPI o display

**Entregable**: Sistema que integra sensores I2C y SPI

#### Semana 4: Optimización y Robustez

**Sesión 7**: DMA y Optimización
- Direct Memory Access: configuración y uso
- Transferencias ADC-DMA, UART-DMA
- Técnicas de optimización de código
- Low power modes

**Sesión 8**: Proyecto Integrador
- Sistema completo que integre:
  - Múltiples periféricos (UART, ADC, PWM, I2C/SPI)
  - Manejo de interrupciones
  - Comunicación con PC
  - Optimización de recursos

**Entregable**: Proyecto final integrador con documentación completa

### Objetivos de Aprendizaje Unidad 2

Al completar la Unidad 2, el estudiante será capaz de:

1. **Implementar** comunicación serial UART para debugging e interfaz con PC
2. **Configurar** ADC para lectura de señales analógicas de sensores
3. **Generar** señales PWM para control de actuadores
4. **Utilizar** protocolos I2C y SPI para comunicación con periféricos externos
5. **Aplicar** DMA para transferencias eficientes sin carga de CPU
6. **Optimizar** código para consumo energético reducido
7. **Integrar** múltiples periféricos en un sistema funcional completo
8. **Depurar** sistemas complejos utilizando herramientas avanzadas

### Continuidad Conceptual

| Concepto Unidad 1 | Expansión en Unidad 2 |
|-------------------|----------------------|
| GPIO básico | GPIO con funciones alternativas (AF) para periféricos |
| Timer para delay | Timer en modo PWM, input capture |
| Interrupciones básicas | Interrupciones múltiples, prioridades, NVIC avanzado |
| Registros de periféricos | Configuración compleja de múltiples periféricos |
| Manejo de bits | Protocolos que requieren control preciso de timing |
| Stack y funciones | Gestión de buffers y estructuras de datos complejas |

### Prerequisitos Reforzar antes de Unidad 2

Basado en la experiencia de Unidad 1, reforzar:

1. **Manipulación de bits**: Crítico para configuración de periféricos complejos
2. **Punteros en C**: Necesario para buffers y estructuras de datos
3. **Interrupciones**: Base para comunicación asíncrona
4. **Lectura de datasheet**: Habilidad esencial que mejora con práctica

### Actividades de Transición (Entre Unidad 1 y 2)

**Tarea Puente**: Implementar un sistema simple de "echo" que:
1. Configure UART básico (puede seguir guía)
2. Reciba un carácter por UART
3. Reenvíe el mismo carácter (echo)
4. Incluya manejo de errores básico

**Objetivo**: Introducir suavemente UART antes de Unidad 2.

## Proyectos Integradores Sugeridos

### Proyecto 1: Estación Meteorológica Simple
- **Sensores**: Temperatura (I2C/SPI), Luz (ADC)
- **Actuadores**: LED de alerta (PWM para intensidad)
- **Comunicación**: UART para envío de datos a PC
- **Complejidad**: Media

### Proyecto 2: Control de Motor DC
- **Entrada**: Potenciómetro (ADC) para velocidad deseada
- **Control**: PWM para motor
- **Feedback**: Encoder (interrupciones externas) para velocidad real
- **Comunicación**: UART para monitoreo
- **Complejidad**: Media-Alta

### Proyecto 3: Data Logger
- **Sensores**: Múltiples entradas analógicas (ADC)
- **Almacenamiento**: EEPROM externa (I2C)
- **Tiempo**: RTC para timestamp
- **Comunicación**: UART para descarga de datos
- **Complejidad**: Alta

## Recursos Adicionales Necesarios para Unidad 2

### Hardware

- **Sensores**: 
  - Sensor I2C (ejemplo: MPU6050 acelerómetro/giroscopio)
  - Sensor de temperatura analógico (LM35 o similar)
  - Potenciómetro para ADC
  
- **Actuadores**:
  - Motor DC pequeño con driver (L293D o similar)
  - LED RGB para PWM
  
- **Comunicación**:
  - Cable USB-Serial (si no integrado en tarjeta)
  - EEPROM I2C (24C02 o similar) para prácticas
  
- **Display** (opcional):
  - LCD 16x2 con I2C adapter
  - O display OLED SPI

### Software

- **Terminal Serial**: PuTTY, RealTerm, o similar
- **Analizador Lógico** (opcional): Para debugging de protocolos
- **Herramienta de Gráficas**: Para visualización de datos de sensores

### Documentación

- Reference Manual STM32F407 (capítulos específicos):
  - Capítulo USART
  - Capítulo ADC
  - Capítulo I2C
  - Capítulo SPI
  - Capítulo DMA
  
- Datasheets de sensores seleccionados
- Application Notes de ST:
  - AN4621: I2C timing configuration
  - AN3216: SPI communication examples

## Advertencias y Consideraciones

### Complejidad Creciente

La Unidad 2 será significativamente más compleja que la Unidad 1:
- Más periféricos simultáneamente
- Sincronización y timing crítico
- Debugging más difícil (problemas intermitentes)
- Mayor cantidad de código

**Recomendación**: Enfatizar metodología incremental y testing progresivo.

### Gestión del Tiempo

Los protocolos de comunicación (I2C, SPI) pueden consumir más tiempo del planeado debido a:
- Problemas de timing
- Configuración incorrecta de pines (funciones alternativas)
- Problemas de hardware (conexiones, alimentación)

**Recomendación**: Incluir tiempo de buffer y tener ejemplos pre-probados.

### Hardware Failures

Con más hardware externo, aumenta probabilidad de fallas:
- Sensores dañados
- Conexiones flojas
- Incompatibilidades de voltaje

**Recomendación**: Tener componentes de repuesto y guías de troubleshooting.

## Métricas de Éxito

Para evaluar si Unidad 1 preparó adecuadamente para Unidad 2:

1. **Tasa de Aprobación Unidad 2**: Debería ser similar a Unidad 1 (±10%)
2. **Tiempo de Configuración de Periféricos**: Estudiantes deberían configurar UART en <1 hora
3. **Comprensión de Datasheet**: Deberían poder encontrar información de periféricos sin ayuda
4. **Debugging**: Deberían identificar errores de configuración de registros
5. **Integración**: Deberían poder combinar periféricos sin conflictos de recursos

## Ajustes a Unidad 1 Basados en Proyección

Si los estudiantes tendrán dificultad con Unidad 2, considerar ajustar Unidad 1:

1. **Incluir más práctica de lectura de datasheet** (Sesión 7)
2. **Enfatizar funciones alternativas de GPIO** (preparar para UART/I2C/SPI)
3. **Más ejercicios de buffers circulares** (necesario para UART)
4. **Introducir concepto de DMA** al menos teóricamente (Sesión 8)
5. **Mini-tarea de UART básico** como preparación

## Escalabilidad a Unidades Futuras

### Unidad 3 (Sugerencia): "Sistemas Operativos en Tiempo Real"
- Basado en competencias de Unidad 1 y 2
- Introducción a FreeRTOS o similar
- Tareas, semáforos, colas

### Unidad 4 (Sugerencia): "Proyecto Final Integrador"
- Aplicación completa de todas las unidades
- Diseño de sistema embebido completo
- Documentación profesional

## Conclusión

La Unidad 1 proporciona una base sólida, pero limitada. Los estudiantes han desarrollado competencias fundamentales, pero necesitan:

1. **Más periféricos**: Especialmente comunicación
2. **Más complejidad**: Sistemas multi-periférico
3. **Más robustez**: Manejo de errores y optimización
4. **Más autonomía**: Menor guía, más diseño independiente

La Unidad 2 debe construir sobre esta base, incrementando gradualmente la complejidad mientras se refuerzan los conceptos fundamentales de Unidad 1.

---

## Checklist de Preparación para Unidad 2

### Antes de Iniciar Unidad 2

- [ ] Verificar que estudiantes completaron proyecto final Unidad 1
- [ ] Evaluar nivel de comprensión de manipulación de bits (crítico)
- [ ] Confirmar disponibilidad de hardware adicional (sensores, actuadores)
- [ ] Probar ejemplos de UART, I2C, SPI en hardware
- [ ] Preparar troubleshooting guides para protocolos de comunicación
- [ ] Tener componentes de repuesto disponibles

### Primera Sesión Unidad 2

- [ ] Repaso breve de conceptos clave Unidad 1 (15 min)
- [ ] Demostración de sistema completo que se construirá (motivación)
- [ ] Explicar por qué la comunicación es crítica en sistemas embebidos
- [ ] Configurar entorno para debugging serial (instalar terminal)

---

**Fecha de creación**: Noviembre 2025  
**Versión**: 1.0  
**Revisión recomendada**: Al finalizar impartición de Unidad 1
