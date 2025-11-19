# Sesión 3: Arquitectura ARM Cortex M-4 - Componentes Internos

**Duración**: 2 horas  
**Modalidad**: Presencial con componentes teóricos y prácticos

---

## Objetivos Específicos

Al finalizar esta sesión, el estudiante será capaz de:

1. **Identificar** los componentes internos del ARM Cortex-M4 y su función específica
2. **Explicar** el mapa de memoria de 4GB y las regiones principales
3. **Describir** el funcionamiento del pipeline de 3 etapas del Cortex-M4

---

## Contenidos Temáticos

### 1. Visión General del ARM Cortex-M4 (20 minutos)

#### ¿Qué es el Cortex-M4?

El ARM Cortex-M4 es un procesador de 32 bits optimizado para:
- Sistemas embebidos
- Control en tiempo real  
- Procesamiento digital de señales (DSP)
- Bajo consumo energético

**Características principales**:
- Arquitectura ARMv7-M
- Pipeline de 3 etapas
- Hasta 168 MHz (en STM32F4)
- Unidad de punto flotante (FPU) opcional
- Instrucciones DSP (SIMD)
- NVIC con hasta 240 interrupciones
- MPU (Memory Protection Unit) opcional

### 2. Componentes Internos (30 minutos)

#### 2.1 Core del Procesador

**Pipeline de 3 Etapas**:
```
Instrucción N:     [Fetch] [Decode] [Execute]
Instrucción N+1:           [Fetch]  [Decode] [Execute]
Instrucción N+2:                    [Fetch]  [Decode] [Execute]
```

- **Fetch**: Buscar instrucción desde memoria
- **Decode**: Decodificar y preparar operandos
- **Execute**: Ejecutar en ALU o unidad de load/store

**Ventaja del Pipeline**: Múltiples instrucciones en proceso simultáneamente, aumentando throughput.

#### 2.2 ALU y Registros

**ALU (Arithmetic Logic Unit)**:
- Operaciones aritméticas: ADD, SUB, MUL, DIV
- Operaciones lógicas: AND, OR, XOR, NOT
- Operaciones de desplazamiento: LSL, LSR, ASR, ROR
- Comparaciones: CMP, CMN, TST

**Registros** (se verá en detalle en Sesión 4):
- R0-R12: Registros de propósito general
- R13 (SP): Stack Pointer
- R14 (LR): Link Register
- R15 (PC): Program Counter
- Registros especiales: APSR, IPSR, EPSR, etc.

#### 2.3 FPU (Floating Point Unit)

Cuando presente (FPv4-SP):
- Precisión simple (32 bits)
- 32 registros S0-S31
- Instrucciones: VADD, VSUB, VMUL, VDIV, VSQRT
- Cumple con IEEE 754

#### 2.4 NVIC (Nested Vectored Interrupt Controller)

- Hasta 240 interrupciones externas
- 16 niveles de prioridad (configurable)
- Anidación automática de interrupciones
- Latencia baja (12 ciclos típico)
- Wake-up rápido de bajo consumo

### 3. Mapa de Memoria (30 minutos)

#### Espacio de Direcciones de 4 GB

El Cortex-M4 tiene un espacio de direcciones lineal de 4 GB (32 bits):

```
0xFFFFFFFF  +------------------+
            |     Vendedor     |
            |     Específico   |
0xE0100000  +------------------+
            |   Periféricos    |
            |   Privados       |
            |   (NVIC, SysTick)|
0xE0000000  +------------------+
            |   Dispositivo    |
            |   Externo        |
0xA0000000  +------------------+
            |    RAM           |
            |    Externa       |
0x60000000  +------------------+
            |   Periféricos    |
            |                  |
0x40000000  +------------------+
            |      SRAM        |
            |                  |
0x20000000  +------------------+
            |      Código      |
            |   (Flash/ROM)    |
0x00000000  +------------------+
```

#### Regiones Principales

**1. Código (0x00000000 - 0x1FFFFFFF)**
- Flash memory
- Ejecutable, lectura
- Región de código principal del programa

**2. SRAM (0x20000000 - 0x3FFFFFFF)**  
- RAM para datos
- Lectura/escritura rápida
- Variables, stack, heap

**3. Periféricos (0x40000000 - 0x5FFFFFFF)**
- GPIO, UART, I2C, SPI, ADC, Timers, etc.
- Acceso mediante registros mapeados en memoria
- Divididos en buses APB1, APB2, AHB1, AHB2

**4. Periféricos Privados (0xE0000000 - 0xE00FFFFF)**
- NVIC (0xE000E100)
- SysTick (0xE000E010)
- MPU, FPU, Debug
- Específicos del core ARM

### 4. Buses del Sistema (20 minutos)

#### Matriz de Buses

El STM32F4 implementa múltiples buses para acceso paralelo:

**AHB (Advanced High-performance Bus)**:
- Alta velocidad (hasta 168 MHz)
- Periféricos críticos: DMA, USB, Ethernet
- Memoria SRAM

**APB1 (Advanced Peripheral Bus 1)**:
- Hasta 42 MHz (típico)
- Periféricos de velocidad media: UART, I2C, SPI bajo, Timers básicos

**APB2 (Advanced Peripheral Bus 2)**:
- Hasta 84 MHz (típico)
- Periféricos rápidos: ADC, USART1, SPI1, Timers avanzados

**Ventaja**: Múltiples periféricos pueden operar simultáneamente sin bloquear el bus.

---

## Actividades

### Actividad 1: Exploración del Mapa de Memoria (25 minutos)

**Objetivo**: Familiarizarse con el mapa de memoria del STM32F407.

**Materiales**: Reference Manual RM0090 (capítulo 2 - Memory map)

**Procedimiento**:
1. En grupos de 2-3, abrir el Reference Manual
2. Completar tabla:

| Periférico | Dirección Base | Bus | Región de Memoria |
|-----------|----------------|-----|------------------|
| GPIOA | | | |
| USART1 | | | |
| TIM2 | | | |
| ADC1 | | | |
| NVIC | | | |
| SRAM (inicio) | | | |
| Flash (inicio) | | | |

3. Compartir resultados y discutir

**Respuestas esperadas**:
- GPIOA: 0x40020000, AHB1, Periféricos
- USART1: 0x40011000, APB2, Periféricos  
- TIM2: 0x40000000, APB1, Periféricos
- ADC1: 0x40012000, APB2, Periféricos
- NVIC: 0xE000E100, -, Privados
- SRAM: 0x20000000, -, SRAM
- Flash: 0x08000000, -, Código

### Actividad 2: Análisis de Pipeline (15 minutos)

**Objetivo**: Comprender el funcionamiento del pipeline.

**Escenario**: Ejecutar 5 instrucciones secuenciales simples.

```
ADD R0, R1, R2
SUB R3, R4, R5
MOV R6, R7
CMP R0, R3
B label
```

**Tarea**: Dibujar diagrama de tiempo mostrando qué etapa del pipeline ejecuta cada instrucción en cada ciclo de reloj.

**Discusión**: 
- ¿Cuántos ciclos para ejecutar 5 instrucciones sin pipeline?
- ¿Cuántos ciclos con pipeline de 3 etapas?
- ¿Qué pasa cuando hay un salto (branch)?

### Actividad 3: Identificación de Componentes en Diagrama (10 minutos)

**Objetivo**: Identificar componentes en diagrama de bloques del Cortex-M4.

**Procedimiento**:
1. Proyectar diagrama de bloques oficial de ARM
2. Estudiantes identifican:
   - Core (ALU, registros, pipeline)
   - NVIC
   - FPU
   - Buses I-Code, D-Code, System
   - Debug interface

3. Explicación breve de cada componente

---

## Material Complementario

### Para la Próxima Sesión (Lectura Previa)

**Lectura obligatoria** (30 minutos):
- Registros del ARM Cortex-M4: R0-R15, APSR
- Instrucciones básicas: MOV, LDR, STR

**Documento**: Sesión 4 o material preparado por instructor

### Recursos Adicionales

- ARMv7-M Architecture Reference Manual (capítulo Memory Model)
- STM32F407 Reference Manual RM0090 (capítulo 2 - Memory and bus architecture)

---

## Evaluación Formativa

### Quiz Formativo - Sesión 3 (10 minutos)

1. ¿En qué rango de direcciones está típicamente la memoria Flash? (2 puntos)
2. ¿Qué es el NVIC y para qué sirve? (2 puntos)
3. ¿Cuántas etapas tiene el pipeline del Cortex-M4? (1 punto)
4. ¿Qué bus (AHB/APB1/APB2) es el más rápido? (1 punto)
5. ¿Dónde se encuentra el SysTick en el mapa de memoria? (2 puntos)
6. ¿Qué significa que el Cortex-M4 tenga FPU? (2 puntos)

**Total**: 10 puntos

---

## Evidencias de Aprendizaje

### Entregable de la Sesión

**Diagrama anotado** del mapa de memoria que incluya:

1. Las 5 regiones principales (con rangos de direcciones)
2. Al menos 3 periféricos específicos con sus direcciones
3. Anotaciones explicando qué se almacena en cada región
4. Indicación de qué regiones son lectura/escritura vs solo lectura

Además, **párrafo breve** (5-7 líneas): ¿Por qué crees que el mapa de memoria está organizado en regiones específicas?

**Formato**: PDF, imagen escaneada, o dibujo digital  
**Entrega**: Antes de próxima sesión

---

## Indicadores de Logro

| Indicador | Criterio |
|-----------|----------|
| Identificación de componentes | Nombra ALU, registros, NVIC, pipeline correctamente |
| Comprensión del mapa de memoria | Identifica al menos 4 de 5 regiones principales |
| Relación con arquitectura | Conecta conceptos con Harvard modificada |

---

## Notas para el Instructor

### Materiales Necesarios
- Reference Manual RM0090 (digital o impreso)
- Diagrama de bloques oficial de ARM Cortex-M4
- Proyector para mostrar mapas de memoria

### Puntos Clave
- El mapa de memoria es **estándar** para todos los Cortex-M
- Los periféricos específicos varían según fabricante (ST, NXP, etc.)
- El pipeline es **transparente** al programador (pero afecta timing)

### Errores Comunes
- Confundir direcciones de periféricos entre diferentes STM32
- Pensar que pueden escribir directamente en Flash (es ROM)
- No considerar que periféricos necesitan clock habilitado

---

**Próxima sesión**: Registros y Primeras Instrucciones Assembly  
**Preparación**: Lectura sobre registros R0-R15
