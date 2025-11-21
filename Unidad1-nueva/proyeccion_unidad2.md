# Proyección a Unidad 2 - Recomendaciones

Este documento identifica las brechas y oportunidades detectadas durante la Unidad 1, y proporciona recomendaciones para el diseño de la Unidad 2.

## Competencias Consolidadas en Unidad 1

Al finalizar la Unidad 1, los estudiantes deberían tener:

### ✅ Conocimientos Sólidos en Assembly

1. **Arquitectura de Procesadores**: Comprensión clara de Harvard, von Neumann, RISC, CISC
2. **ARM Cortex M-4**: Conocimiento profundo de componentes internos, registros, mapa de memoria, buses
3. **Assembly Avanzado**: Capacidad de escribir programas complejos con:
   - Operaciones aritméticas, lógicas, y de bits
   - Control de flujo (if-else, while, for)
   - Funciones con manejo de stack y convenciones AAPCS
   - Manipulación directa de registros de periféricos
4. **GPIO en Assembly**: Configuración completa de pines usando registros (MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR)
5. **Interrupciones**: Conceptos fundamentales, NVIC, Vector Table, estructura de handlers
6. **Lectura de Datasheets**: Habilidad para extraer información de registros y configuraciones

### ✅ Habilidades Desarrolladas

1. Leer y extraer información de datasheets y reference manuals
2. Configurar registros de periféricos mediante manipulación directa de bits en Assembly
3. Escribir código modular y reutilizable en Assembly
4. Depurar código a bajo nivel entendiendo cada instrucción
5. Implementar drivers básicos en Assembly puro
6. Comprender el flujo de ejecución con interrupciones

## Transición de Unidad 1 a Unidad 2

### Cambio de Paradigma: Assembly → C Bare Metal

**Unidad 1**: Programación exclusiva en Assembly
- Enfoque en comprensión profunda del hardware
- Manipulación directa de registros
- Control total sobre cada instrucción
- Fundamentos sólidos de arquitectura

**Unidad 2**: Programación en C Bare Metal
- Aplicación de conocimientos de Assembly en C
- Mayor productividad manteniendo control sobre hardware
- Desarrollo de drivers completos y reutilizables
- Proyectos más complejos e integradores

### ¿Por Qué Esta Transición?

1. **Assembly primero** da comprensión profunda de:
   - Cómo funciona el hardware a bajo nivel
   - Qué hace realmente cada operación
   - Cómo se organizan los datos en memoria
   - El costo real de cada operación

2. **C después** permite:
   - Desarrollo más rápido y mantenible
   - Abstracciones útiles sin perder control
   - Proyectos más complejos en menos tiempo
   - Mejor lectura y documentación del código
   - Facilita trabajo en equipo

### Conocimiento Previo Crítico para Unidad 2

Los estudiantes que completen Unidad 1 con éxito tendrán ventaja significativa en Unidad 2:
- Entenderán qué genera el compilador de C
- Sabrán optimizar código crítico
- Podrán depurar a nivel de Assembly cuando sea necesario
- Comprenderán el costo de abstracciones en C

## Brechas Identificadas (para Unidad 2)

### 📌 Conocimientos Nuevos Requeridos

1. **Lenguaje C para Embebidos**:
   - **Brecha**: No han programado en C para sistemas embebidos
   - **Impacto**: Necesitan aprender diferencias entre C estándar y C bare metal
   - **Solución**: Sesión dedicada a transición Assembly-C

2. **Máquinas de Estados Finitos (MEF)**:
   - **Brecha**: Concepto no cubierto en Unidad 1
   - **Impacto**: Necesario para diseño de sistemas reactivos
   - **Solución**: 2-3 sesiones sobre MEF (Moore, Mealy, implementación en C)

3. **Comunicación Serial (UART)**:
   - **Brecha**: No implementado en Unidad 1
   - **Impacto**: Necesario para debugging avanzado e interfaz con PC
   - **Solución**: 1-2 sesiones sobre UART en C

4. **Timer Avanzado**:
   - **Brecha**: Solo conceptos básicos de delays
   - **Impacto**: Necesitan timing preciso para MEF y control
   - **Solución**: Configuración completa de TIM2 en C

5. **Interrupciones en C**:
   - **Brecha**: Solo vieron conceptos en Assembly
   - **Impacto**: Necesitan implementar handlers en C
   - **Solución**: Integración de interrupciones con MEF

6. **Estructura de Proyectos C Bare Metal**:
   - **Brecha**: No han trabajado con startup code, linker scripts, Makefiles
   - **Impacto**: No pueden crear proyectos desde cero
   - **Solución**: Sesión sobre toolchain y estructura de proyectos

### 📌 Habilidades que Necesitan Desarrollarse

1. **Abstracción en C**: Crear estructuras de datos y funciones modulares
2. **Diseño de Drivers**: API clara, separación de capas
3. **Manejo de Estados**: Implementar MEF en código estructurado
4. **Debugging en C**: Técnicas diferentes a debugging en Assembly
5. **Gestión de Proyectos**: Múltiples archivos .c y .h, compilación modular

## Recomendaciones para Unidad 2

### Tema Central Propuesto

**"Máquinas de Estados Finitos y Programación en C Bare Metal para Microcontroladores ARM Cortex M-4"**

### Estructura Sugerida (4 semanas, 8 sesiones)

#### Semana 1: Transición a C y Fundamentos de MEF

**Sesión 1**: Lenguaje C para Sistemas Embebidos
- Diferencias entre C estándar y C bare metal
- Estructura de proyecto: startup code, linker script, Makefile
- Primer programa en C: blink LED
- Comparación con Assembly (mostrar código generado)
- Tipos de datos, punteros a periféricos
- Palabra clave `volatile` y su importancia

**Sesión 2**: Máquinas de Estados Finitos - Teoría
- Concepto de MEF y su aplicación en sistemas embebidos
- Diferencia entre Moore y Mealy
- Diagramas de estado
- Tablas de transición
- Ejemplos prácticos (semáforo, control de acceso, etc.)
- Ventajas del diseño basado en estados

**Entregable**: Programa en C que implemente blink LED + diagrama de MEF simple

#### Semana 2: Implementación de MEF y GPIO en C

**Sesión 3**: Implementación de MEF en C
- Estructura enum para estados
- Switch-case vs tabla de transiciones
- Variables de estado
- Eventos y transiciones
- Temporizadores para MEF
- Ejemplo: MEF para debouncing de botón

**Sesión 4**: Driver GPIO Completo en C
- Estructura de datos para GPIO
- Funciones de inicialización y control
- Header file vs implementation file
- Macros y constantes
- Ejemplo: Sistema de LEDs controlado por MEF

**Entregable**: Driver GPIO en C + programa con MEF para control de LEDs

#### Semana 3: Timers y Multiplexación

**Sesión 5**: Configuración de Timer (TIM2) en C
- Conceptos: prescaler, auto-reload, update event
- Configuración de registros en C
- Generación de delays precisos
- Timer para base de tiempo de MEF
- Ejemplo: MEF con timing preciso

**Sesión 6**: Multiplexación y Técnicas Avanzadas
- Concepto de multiplexación
- Multiplexación de displays 7 segmentos
- Multiplexación de teclados matriciales
- Implementación con MEF
- Optimización de uso de pines

**Entregable**: Sistema con Timer y multiplexación

#### Semana 4: Interrupciones y Proyecto Integrador

**Sesión 7**: Interrupciones y NVIC en C
- Configuración de EXTI (interrupciones externas)
- Implementación de handlers en C
- Comunicación entre ISR y main
- Flags y sincronización
- Debouncing con interrupciones
- Prioridades y anidación

**Sesión 8**: Proyecto Final - Sistema con MEF
- Integración de todo lo aprendido
- Proyecto: Semáforo inteligente con botón peatonal
  - MEF para estados del semáforo
  - Interrupción para botón peatonal
  - Timer para tiempos de estados
  - Multiplexación si se usa display
- Evaluación sumativa
- Presentación de proyectos

**Entregable**: Proyecto completo funcional con documentación

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
