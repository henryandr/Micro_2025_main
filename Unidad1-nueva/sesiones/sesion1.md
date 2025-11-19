# Sesión 1: Introducción y Diagnóstico - Arquitecturas de Procesadores

**Duración**: 2 horas  
**Modalidad**: Presencial con componentes teóricos y prácticos

---

## Objetivos Específicos

Al finalizar esta sesión, el estudiante será capaz de:

1. **Identificar** los componentes básicos de un microprocesador y su función dentro de un sistema computacional
2. **Distinguir** entre microprocesador, microcontrolador y computadora de propósito general
3. **Explicar** el ciclo básico de ejecución de instrucciones (fetch-decode-execute)

---

## Contenidos Temáticos

### 1. ¿Qué es un Procesador? (20 minutos)

Un **procesador** (o microprocesador) es un circuito integrado que ejecuta instrucciones de programas almacenados en memoria. Es el "cerebro" de cualquier sistema computacional.

#### Componentes Básicos de un Procesador

1. **Unidad de Control (Control Unit - CU)**
   - Coordina todas las operaciones del procesador
   - Decodifica instrucciones
   - Genera señales de control para otros componentes

2. **Unidad Aritmético-Lógica (Arithmetic Logic Unit - ALU)**
   - Realiza operaciones aritméticas (suma, resta, multiplicación, división)
   - Realiza operaciones lógicas (AND, OR, NOT, XOR)
   - Operaciones de comparación

3. **Registros**
   - Memoria interna muy rápida del procesador
   - Almacenan datos temporales y resultados intermedios
   - Ejemplos: registros de propósito general, contador de programa (PC), registro de estado

4. **Buses Internos**
   - Bus de datos: Transporta datos entre componentes
   - Bus de direcciones: Especifica ubicaciones de memoria
   - Bus de control: Transporta señales de control

#### Ciclo de Ejecución de Instrucciones

Todo procesador sigue un ciclo básico:

```
1. FETCH (Buscar): El procesador obtiene la instrucción desde memoria
   └─> PC (Program Counter) indica la dirección de la próxima instrucción

2. DECODE (Decodificar): La unidad de control interpreta la instrucción
   └─> Identifica qué operación realizar y qué datos necesita

3. EXECUTE (Ejecutar): Se realiza la operación
   └─> La ALU ejecuta operaciones, o se accede a memoria, etc.

4. WRITEBACK (Escribir resultado): El resultado se guarda
   └─> En un registro o en memoria

5. Incrementar PC y repetir
```

### 2. Microprocesador vs Microcontrolador (15 minutos)

| Característica | Microprocesador | Microcontrolador |
|----------------|----------------|------------------|
| **Definición** | CPU + buses externos | CPU + memoria + periféricos en un chip |
| **Memoria** | Externa (RAM, ROM separadas) | Interna (Flash, SRAM integrados) |
| **Periféricos** | Requiere chips externos | Integrados (GPIO, Timer, UART, ADC, etc.) |
| **Uso típico** | Computadoras, servidores | Sistemas embebidos, IoT, control |
| **Costo** | Mayor (requiere componentes adicionales) | Menor (todo integrado) |
| **Consumo** | Alto | Optimizado para bajo consumo |
| **Ejemplos** | Intel Core i7, AMD Ryzen | STM32F407, Arduino (ATmega328), ESP32 |

#### ¿Cuándo usar cada uno?

- **Microprocesador**: Aplicaciones que requieren alto poder de cómputo, múltiples tareas, sistemas operativos complejos
- **Microcontrolador**: Aplicaciones dedicadas, control en tiempo real, bajo consumo, costo reducido

### 3. Historia Breve de los Procesadores (10 minutos)

- **1971**: Intel 4004 - Primer microprocesador comercial (4 bits, 740 KHz)
- **1978**: Intel 8086 - Base de la arquitectura x86 (16 bits)
- **1985**: Intel 80386 - Primer procesador x86 de 32 bits
- **1985**: ARM1 - Primer procesador ARM (Acorn RISC Machine)
- **2004**: ARM Cortex-M - Familia optimizada para microcontroladores
- **2006**: ARM Cortex-M3 - Primera implementación comercial amplia
- **2010**: ARM Cortex-M4 - Con unidad de punto flotante y DSP

#### ¿Por qué ARM en este curso?

- Dominante en mercado de microcontroladores (>50% del mercado)
- Eficiencia energética superior
- Ecosistema robusto de herramientas y fabricantes
- Arquitectura RISC: más fácil de aprender
- Amplio uso industrial: desde relojes inteligentes hasta drones

---

## Actividades

### Actividad 1: Evaluación Diagnóstica (30 minutos)

**Objetivo**: Identificar conocimientos previos de los estudiantes.

**Instrucciones**:
1. Los estudiantes responden individualmente la evaluación diagnóstica
2. No es calificación punitiva, sino para ajustar el ritmo del curso
3. Pueden consultar materiales si lo necesitan (es diagnóstico, no examen)

**Archivo**: Ver `evaluaciones/diagnostica.md`

**Nota para el instructor**: Revisar rápidamente las respuestas para identificar:
- Nivel de programación en C
- Familiaridad con sistemas numéricos
- Conocimientos previos de hardware
- Expectativas del curso

### Actividad 2: Rompehielo Conceptual - "¿Qué hay dentro?" (20 minutos)

**Objetivo**: Activar conocimientos previos y generar curiosidad sobre arquitectura interna.

**Materiales**: Imágenes de procesadores (puede ser en proyector)

**Procedimiento**:
1. Mostrar imagen de die (troquel) de un procesador
2. Preguntas para discusión en grupos pequeños (3-4 estudiantes):
   - ¿Qué creen que hace cada sección visible?
   - ¿Por qué algunos procesadores son más grandes que otros?
   - ¿Qué diferencia creen que hay entre un procesador de celular y uno de laptop?

3. Cada grupo comparte 1 idea
4. Profesor complementa con información técnica

**Ejemplo de procesadores para mostrar**:
- Intel Core i7 (complejo, muchos núcleos)
- ARM Cortex-M4 (simple, optimizado)
- Comparación de tamaño y complejidad

### Actividad 3: Análisis de Bloques de un Procesador (15 minutos)

**Objetivo**: Identificar componentes básicos en un diagrama de bloques.

**Materiales**: Diagrama de bloques simple de un procesador

**Procedimiento**:
1. Mostrar diagrama de bloques de ARM Cortex-M4
2. En parejas, identificar:
   - ¿Dónde está la ALU?
   - ¿Dónde están los registros?
   - ¿Cómo fluyen los datos?
   - ¿Qué es la "unidad de control"?

3. Discusión breve con explicación del instructor

**Diagrama sugerido** (descripción textual):
```
+------------------------------------------------------------------+
|                      ARM Cortex-M4 Core                          |
|                                                                  |
|  +-------------+    +-------------+    +-----------------+       |
|  |   Fetch     |    |   Decode    |    |     Execute     |       |
|  |   Unit      |--->|   Unit      |--->|      (ALU)      |       |
|  +-------------+    +-------------+    +-----------------+       |
|                                               ^                  |
|                                               |                  |
|  +-----------------------------------------------------------+   |
|  |                    Register Bank (R0-R15)                 |   |
|  +-----------------------------------------------------------+   |
|                                                                  |
|  +---------------------------+    +-------------------------+    |
|  |     NVIC (Interrupts)     |    |   Debug Interface       |    |
|  +---------------------------+    +-------------------------+    |
+------------------------------------------------------------------+
         ^                    ^                    ^
         |                    |                    |
    Bus Matriz          Sistema Bus          Periféricos
```

---

## Material Complementario

### Para la Próxima Sesión (Lectura Previa)

**Lectura obligatoria** (30 minutos):
- Sección sobre arquitecturas Harvard y von Neumann (se proporcionará material)
- Diferencias RISC vs CISC (lectura corta)

**Video sugerido** (15 minutos):
- "How does a CPU work?" - Explicación visual del ciclo fetch-decode-execute
- Cualquier video educativo que explique el funcionamiento básico

### Recursos Adicionales (Opcionales)

- Historia de ARM: [ARM Architecture - Wikipedia](https://en.wikipedia.org/wiki/ARM_architecture)
- Comparación microprocesadores vs microcontroladores (artículos técnicos)

---

## Evaluación Formativa

### Preguntas de Cierre (10 minutos)

Al final de la sesión, los estudiantes responden brevemente (pueden ser orales):

1. ¿Cuáles son los tres componentes principales de un procesador?
2. ¿Cuál es la diferencia principal entre un microprocesador y un microcontrolador?
3. ¿Qué significa "ejecutar una instrucción"?

**Método**: Seleccionar 3-4 estudiantes al azar para responder. Complementar respuestas si es necesario.

---

## Evidencias de Aprendizaje

### Entregable de la Sesión

**Documento breve** (máximo 1 página) que incluya:

1. **Diagrama simple** de los componentes de un procesador (puede ser dibujado a mano y escaneado)
2. **Tres diferencias** entre microprocesador y microcontrolador
3. **Descripción breve** (3-4 líneas) del ciclo de ejecución de instrucciones
4. **Reflexión personal**: ¿Por qué creen que es importante aprender arquitectura de procesadores para programar microcontroladores?

**Formato**: PDF o Markdown  
**Entrega**: Próxima sesión (inicio de clase)  
**Evaluación**: Completitud (entrega = 100%, no entrega = 0%)

---

## Indicadores de Logro

| Indicador | Criterio de Evaluación |
|-----------|----------------------|
| **Comprensión conceptual** | Identifica correctamente ALU, registros, unidad de control |
| **Distinción de conceptos** | Explica diferencia microprocesador vs microcontrolador con al menos 2 características |
| **Secuencia de procesos** | Describe el ciclo fetch-decode-execute en orden correcto |
| **Participación** | Participa activamente en discusiones y actividades grupales |

---

## Notas para el Instructor

### Tiempo Estimado por Sección

- Introducción y presentación del curso: 10 min
- Contenido teórico (sección 1-3): 45 min
- Actividad 1 (diagnóstica): 30 min
- Actividades 2 y 3: 35 min
- Cierre y asignación de tarea: 10 min
- **Total**: ~120 min (2 horas)

### Adaptaciones Según Nivel del Grupo

**Si el grupo tiene experiencia previa**:
- Reducir tiempo en conceptos básicos
- Añadir comparación con otras arquitecturas (x86, MIPS)
- Profundizar en historia y evolución

**Si el grupo es principiante**:
- Más tiempo en conceptos fundamentales
- Usar más analogías (ej: procesador como "chef" siguiendo una receta)
- Enfatizar visualizaciones y diagramas

### Materiales Necesarios

- Proyector para presentación
- Imágenes de die de procesadores (buscar en internet antes de clase)
- Diagrama de bloques de ARM Cortex-M4 (puede dibujar en pizarra si no hay digital)
- Copias de evaluación diagnóstica o acceso digital

### Resolución de Problemas Comunes

**Problema**: Estudiantes intimidan por complejidad aparente  
**Solución**: Enfatizar que irán paso a paso, y que al final de la unidad todo tendrá sentido

**Problema**: Variedad de niveles en el mismo grupo  
**Solución**: Trabajo en parejas mixtas (principiante + avanzado), ejercicios opcionales más difíciles

**Problema**: Poco tiempo para todo el contenido  
**Solución**: Priorizar actividades prácticas sobre teoría exhaustiva, la teoría se refuerza en sesiones siguientes

---

## Conexión con Sesiones Siguientes

Esta sesión establece la base conceptual. En las próximas sesiones:

- **Sesión 2**: Profundizaremos en tipos de arquitecturas (Harvard/von Neumann, RISC/CISC)
- **Sesión 3**: Nos enfocaremos específicamente en ARM Cortex-M4
- **Sesión 4**: Comenzaremos a programar en Assembly

El objetivo de esta primera sesión es que todos tengan el mismo nivel base de comprensión antes de avanzar a detalles técnicos.

---

## Checklist Pre-Sesión

- [ ] Revisar evaluación diagnóstica y tener copias/acceso digital
- [ ] Preparar presentación con diagramas e imágenes
- [ ] Probar proyector y equipo
- [ ] Revisar lista de estudiantes
- [ ] Preparar material de lectura para próxima sesión
- [ ] Tener plan B si tecnología falla (imprimir diagramas clave)

---

**Próxima sesión**: Arquitecturas Harvard vs von Neumann, RISC vs CISC  
**Preparación para estudiantes**: Lectura sobre arquitecturas (se proporciona material)
