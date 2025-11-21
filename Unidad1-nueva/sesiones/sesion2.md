# Sesión 2: Harvard vs von Neumann, RISC vs CISC

**Duración**: 2 horas  
**Modalidad**: Presencial con componentes teóricos y prácticos

---

## Objetivos Específicos

Al finalizar esta sesión, el estudiante será capaz de:

1. **Comparar** las arquitecturas Harvard y von Neumann, identificando ventajas y desventajas de cada una
2. **Diferenciar** entre arquitecturas RISC y CISC, y explicar el impacto en programación
3. **Justificar** por qué ARM utiliza arquitectura Harvard modificada y filosofía RISC

---

## Contenidos Temáticos

### 1. Arquitectura von Neumann (25 minutos)

#### Historia y Concepto

Propuesta por John von Neumann en 1945, es la arquitectura más tradicional y utilizada en computadoras de propósito general.

#### Características Principales

**Memoria Unificada**: 
- Instrucciones y datos comparten el mismo espacio de memoria
- Un solo bus de dirección y un solo bus de datos
- Acceso secuencial: no se puede leer instrucción y dato simultáneamente

**Diagrama Conceptual**:
```
    +-------------------+
    |       CPU         |
    |  +-------------+  |
    |  | Control Unit|  |
    |  +-------------+  |
    |  +-------------+  |
    |  |     ALU     |  |
    |  +-------------+  |
    |  +-------------+  |
    |  |  Registros  |  |
    |  +-------------+  |
    +-------------------+
            |
            | Bus Único (Datos e Instrucciones)
            |
    +-------------------+
    |  Memoria          |
    |  Unificada        |
    |  +-------------+  |
    |  | Programa    |  |
    |  | (Código)    |  |
    |  +-------------+  |
    |  | Datos       |  |
    |  +-------------+  |
    +-------------------+
```

#### Ventajas de von Neumann

1. **Simplicidad**: Diseño más simple, menos buses
2. **Flexibilidad**: La memoria puede asignarse dinámicamente entre código y datos
3. **Costo**: Menor cantidad de componentes
4. **Universalidad**: Programa almacenado en memoria (concepto revolucionario en su época)

#### Desventajas de von Neumann

1. **Cuello de botella (von Neumann bottleneck)**: 
   - CPU debe esperar para acceder a memoria (no puede buscar instrucción y dato simultáneamente)
   - Límita velocidad de procesamiento

2. **Riesgo de sobrescritura**:
   - Un error de programación puede sobrescribir código (punteros mal manejados)

3. **Menor rendimiento**:
   - En aplicaciones intensivas en procesamiento

#### Ejemplos de Arquitectura von Neumann

- Intel x86 (8086, 80386, Pentium, Core i7)
- AMD64 (Ryzen, EPYC)
- Computadoras de escritorio y laptops tradicionales

### 2. Arquitectura Harvard (25 minutos)

#### Historia y Concepto

Propuesta en la Universidad de Harvard en los años 1930s para el Mark I. Diseñada originalmente para superar las limitaciones de velocidad.

#### Características Principales

**Memorias Separadas**:
- Memoria de instrucciones (típicamente Flash o ROM)
- Memoria de datos (típicamente RAM)
- Buses separados para cada memoria
- Acceso simultáneo a instrucción y dato

**Diagrama Conceptual**:
```
                +-------------------+
                |       CPU         |
                |  +-------------+  |
                |  | Control Unit|  |
                |  +-------------+  |
                |  +-------------+  |
                |  |     ALU     |  |
                |  +-------------+  |
                |  +-------------+  |
                |  |  Registros  |  |
                |  +-------------+  |
                +-------------------+
                    |            |
        Bus de      |            |    Bus de
     Instrucciones  |            |    Datos
                    |            |
        +-----------+            +-----------+
        |                                    |
+----------------+                  +----------------+
|  Memoria de    |                  |  Memoria de    |
|  Programa      |                  |  Datos         |
|  (Flash/ROM)   |                  |  (RAM)         |
+----------------+                  +----------------+
```

#### Ventajas de Harvard

1. **Mayor velocidad**: Acceso simultáneo a instrucciones y datos (paralelismo)
2. **Seguridad**: Código no puede ser modificado accidentalmente
3. **Optimización**: Memorias pueden tener anchos de bus diferentes (ej: 32 bits para código, 8 bits para datos)
4. **Determinismo**: Tiempo de ejecución más predecible

#### Desventajas de Harvard

1. **Complejidad**: Más buses, más conexiones
2. **Costo**: Mayor cantidad de componentes
3. **Inflexibilidad**: No se puede ejecutar código desde RAM fácilmente
4. **Diseño**: Más complejo de implementar

#### Ejemplos de Arquitectura Harvard Pura

- Microcontroladores PIC de Microchip (PIC16F, PIC18F)
- DSP (Digital Signal Processors) tempranos
- Procesadores especializados

### 3. Arquitectura Harvard Modificada (15 minutos)

ARM Cortex-M utiliza **arquitectura Harvard modificada**, que combina lo mejor de ambos mundos.

#### Características de Harvard Modificada

1. **Buses separados internamente**: El core tiene buses I-Code y D-Code
2. **Espacio de direcciones unificado**: Desde el punto de vista del programador, es un solo espacio de memoria
3. **Flexibilidad**: Puede ejecutar código desde RAM si es necesario (útil para bootloaders)
4. **Cachés separadas** (en procesadores más avanzados): Cache de instrucciones y cache de datos

#### ¿Por qué ARM usa Harvard Modificada?

- **Rendimiento**: Acceso paralelo a instrucciones y datos
- **Flexibilidad**: Ejecutar código desde RAM cuando sea necesario
- **Programación**: Más fácil para el programador (parece von Neumann)
- **Optimización**: El hardware puede optimizar accesos sin complicar el software

**Ejemplo Práctico en STM32F407**:
```
Espacio de Memoria (aparente von Neumann):
0x00000000 - 0x0007FFFF: Flash (512 KB) - Código
0x20000000 - 0x2001FFFF: SRAM (128 KB) - Datos
0x40000000 - 0x5FFFFFFF: Periféricos

Pero internamente:
- Bus I-Code accede a Flash
- Bus D-Code accede a SRAM y periféricos
- Acceso simultáneo posible
```

### 4. Arquitectura CISC (Complex Instruction Set Computer) (20 minutos)

#### Concepto y Filosofía

**CISC**: Conjunto de instrucciones complejo, donde cada instrucción puede realizar múltiples operaciones.

#### Características de CISC

1. **Instrucciones complejas**: Una instrucción puede hacer múltiples operaciones
   - Ejemplo x86: `ADD [mem1], [mem2]` - Lee, suma, escribe en una instrucción

2. **Tamaño variable de instrucciones**: De 1 a 15 bytes en x86

3. **Muchos modos de direccionamiento**: Formas complejas de acceder a memoria

4. **Microprogramación**: Instrucciones complejas se implementan como secuencias de micro-operaciones internas

5. **Enfoque**: Reducir número de instrucciones por programa (código más compacto)

#### Ventajas de CISC

1. **Código compacto**: Menos instrucciones por programa
2. **Compiladores más simples** (históricamente): Instrucciones complejas facilitan traducción desde lenguajes de alto nivel
3. **Compatibilidad**: x86 mantiene compatibilidad desde 1978

#### Desventajas de CISC

1. **Hardware complejo**: Decodificación lenta
2. **Consumo energético**: Mayor cantidad de transistores
3. **Pipeline complejo**: Difícil de optimizar
4. **Muchas instrucciones poco usadas**: ~20% de instrucciones son el 80% del uso

**Tabla de Estadísticas x86** (basada en análisis de código real):

| Tipo de Instrucción | Uso (%) |
|---------------------|---------|
| Movimiento de datos | 43% |
| Saltos/Control | 23% |
| Aritmética | 15% |
| Comparaciones | 13% |
| Lógica | 5% |
| Otras | 1% |

**Conclusión**: La mayoría del tiempo se usan instrucciones simples, no las complejas.

#### Ejemplos de CISC

- Intel x86 (8086, Pentium, Core)
- AMD x86-64
- Motorola 68000

### 5. Arquitectura RISC (Reduced Instruction Set Computer) (20 minutos)

#### Concepto y Filosofía

**RISC**: Conjunto de instrucciones reducido, donde cada instrucción es simple y se ejecuta rápido.

#### Principios de RISC

1. **Instrucciones simples**: Una instrucción = una operación básica
2. **Tamaño fijo**: Todas las instrucciones mismo tamaño (32 bits en ARM)
3. **Load/Store**: Solo LDR/STR acceden a memoria, otras operaciones solo en registros
4. **Muchos registros**: Reducir accesos a memoria
5. **Pipeline eficiente**: Instrucciones simples facilitan pipeline profundo
6. **Ejecución en un ciclo** (idealmente): Cada instrucción en 1 ciclo de reloj

#### Ventajas de RISC

1. **Velocidad**: Pipeline eficiente, alta frecuencia de reloj
2. **Consumo bajo**: Menos transistores, menor consumo
3. **Diseño simple**: Más fácil de optimizar
4. **Predecibilidad**: Tiempo de ejecución más consistente

#### Desventajas de RISC

1. **Más instrucciones por programa**: Cada operación requiere más pasos
2. **Compilador más complejo**: Debe optimizar agresivamente
3. **Código más grande** (potencialmente): Más instrucciones = más bytes

#### Ejemplos de RISC

- ARM (Cortex-M, Cortex-A, Cortex-R)
- MIPS
- RISC-V
- SPARC (menos común ahora)
- PowerPC

### 6. Comparación RISC vs CISC

| Aspecto | CISC (x86) | RISC (ARM) |
|---------|-----------|------------|
| **Instrucciones** | Complejas, tamaño variable | Simples, tamaño fijo (32 bits) |
| **Ejemplo ADD** | `ADD [mem], [mem]` | `LDR R0, [mem]` <br> `LDR R1, [mem2]` <br> `ADD R2, R0, R1` <br> `STR R2, [mem3]` |
| **Ciclos/Instrucción** | 1-varios ciclos | ~1 ciclo (ideal) |
| **Registros** | Pocos (8 en 8086) | Muchos (16 en ARM) |
| **Pipeline** | Complejo | Simple y eficiente |
| **Consumo** | Alto | Bajo |
| **Aplicaciones** | PC, servidores | Móviles, embebidos, IoT |

#### ¿RISC vs CISC hoy?

La distinción se ha difuminado:
- **x86 moderno**: Internamente traduce instrucciones CISC a micro-ops RISC
- **ARM moderno**: Ha añadido algunas instrucciones más complejas

**Pero la filosofía persiste**: ARM sigue optimizado para eficiencia energética con instrucciones simples.

---

## Actividades

### Actividad 1: Análisis Comparativo con Diagramas (25 minutos)

**Objetivo**: Visualizar diferencias entre Harvard y von Neumann.

**Materiales**: 
- Diagramas en pizarra o proyector
- Hoja de trabajo para estudiantes

**Procedimiento**:

1. **Dividir clase en grupos de 3-4 estudiantes**

2. **Cada grupo recibe un escenario**:
   - Escenario A: "CPU debe sumar dos números almacenados en memoria"
   - Escenario B: "CPU debe ejecutar un bucle que suma 100 números"

3. **Tarea**: Dibujar el flujo de accesos a memoria en:
   - Arquitectura von Neumann
   - Arquitectura Harvard

4. **Preguntas guía**:
   - ¿Cuántos accesos a memoria se necesitan?
   - ¿Hay conflictos de acceso?
   - ¿Qué arquitectura es más rápida?

5. **Presentación**: 2 grupos comparten sus diagramas

6. **Discusión**: Profesor destaca puntos clave y corrige malentendidos

**Tiempo**: 15 min trabajo + 10 min discusión

### Actividad 2: Comparación de Código RISC vs CISC (20 minutos)

**Objetivo**: Comprender impacto de filosofía RISC en programación.

**Materiales**: Ejemplos de código proyectados

**Ejemplo 1: Sumar dos variables en memoria**

**Estilo CISC (x86)**:
```asm
; x86 - CISC
MOV EAX, [var1]     ; Leer var1 a registro
ADD EAX, [var2]     ; Sumar var2 directo desde memoria
MOV [result], EAX   ; Guardar resultado
; 3 instrucciones, pero ADD accede a memoria
```

**Estilo RISC (ARM)**:
```asm
; ARM - RISC
LDR R0, =var1       ; Dirección de var1
LDR R1, [R0]        ; Leer var1
LDR R0, =var2       ; Dirección de var2
LDR R2, [R0]        ; Leer var2
ADD R3, R1, R2      ; Sumar (solo registros)
LDR R0, =result     ; Dirección resultado
STR R3, [R0]        ; Guardar resultado
; 7 instrucciones, pero cada una es simple
```

**Preguntas para discusión**:
1. ¿Cuál código es más corto? (CISC)
2. ¿Cuál es más fácil de implementar en hardware? (RISC)
3. ¿Cuál es más rápido considerando pipeline? (Depende, discutir)
4. ¿Cuál es más claro de leer? (Opiniones varían)

**Ejemplo 2: Multiplicar por 8**

**CISC**:
```asm
; Puede tener instrucción de multiplicación directa
MUL EAX, 8
```

**RISC**:
```asm
; Usa shift (más eficiente)
LSL R0, R0, #3    ; Desplazar izquierda 3 bits = multiplicar por 8
```

**Discusión**: RISC promueve uso de operaciones eficientes (shift vs multiplicación).

### Actividad 3: Quiz Rápido - Verdadero o Falso (10 minutos)

**Formato**: Individual, luego discusión

**Preguntas**:

1. **V/F**: En arquitectura von Neumann, código y datos están en memorias físicamente separadas.  
   **Respuesta**: Falso (misma memoria)

2. **V/F**: ARM Cortex-M4 usa arquitectura Harvard pura.  
   **Respuesta**: Falso (Harvard modificada)

3. **V/F**: En RISC, solo instrucciones de carga/almacenamiento acceden a memoria.  
   **Respuesta**: Verdadero

4. **V/F**: x86 tiene más registros de propósito general que ARM.  
   **Respuesta**: Falso (ARM tiene más)

5. **V/F**: Arquitectura CISC ejecuta cada instrucción en exactamente 1 ciclo.  
   **Respuesta**: Falso (puede ser varios ciclos)

6. **V/F**: El "cuello de botella de von Neumann" se refiere a que no se puede acceder simultáneamente a código y datos.  
   **Respuesta**: Verdadero

7. **V/F**: ARM usa arquitectura RISC principalmente por eficiencia energética.  
   **Respuesta**: Verdadero

**Dinámica**: 
- Estudiantes levantan mano para V o F
- Discutir respuestas inmediatamente
- Aclarar confusiones

---

## Material Complementario

### Para la Próxima Sesión (Lectura Previa)

**Lectura obligatoria** (30 minutos):
- Documento sobre ARM Cortex-M4: componentes internos
- Diagrama de bloques del STM32F407
- Concepto de buses AHB y APB

**Video sugerido** (20 minutos):
- "ARM Architecture Explained" (buscar en YouTube)

### Recursos Adicionales (Opcionales)

- [RISC vs CISC - Computerphile](https://www.youtube.com/watch?v=g16wZWKcao4) (video)
- Artículo: "The Case for RISC" - David Patterson (paper histórico)

---

## Evaluación Formativa

### Quiz Formativo (archivo: evaluaciones/quizzes_formativos.md - Sesión 2)

**Formato**: 5 preguntas, 10 minutos al final de clase

**Preguntas tipo**:

1. Explica con tus palabras la diferencia entre Harvard y von Neumann (2 puntos)
2. ¿Por qué ARM usa arquitectura RISC en vez de CISC? (2 puntos)
3. Menciona 2 ventajas de arquitectura Harvard (2 puntos)
4. En código ARM, ¿por qué se usan LDR y STR separados de operaciones aritméticas? (2 puntos)
5. ¿Qué significa "Harvard modificada"? (2 puntos)

**Total**: 10 puntos

---

## Evidencias de Aprendizaje

### Entregable de la Sesión

**Tabla comparativa** (puede ser en documento o imagen de tabla dibujada):

Crear tabla con 4 columnas y 6 filas:

| Característica | von Neumann | Harvard | Harvard Modificada |
|----------------|-------------|---------|-------------------|
| Memorias | | | |
| Buses | | | |
| Velocidad | | | |
| Flexibilidad | | | |
| Ejemplo | | | |

Además, **párrafo breve** (5-7 líneas): ¿Por qué crees que ARM eligió RISC sobre CISC para microcontroladores?

**Formato**: PDF o imagen clara  
**Entrega**: Antes de próxima sesión (subir a plataforma o email)  
**Evaluación**: Rúbrica simple (completo/incompleto/ausente)

---

## Indicadores de Logro

| Indicador | Excelente | Bueno | Suficiente | Insuficiente |
|-----------|-----------|-------|------------|--------------|
| **Distinción Harvard/von Neumann** | Explica diferencias con ejemplos claros | Identifica diferencias básicas | Menciona que son diferentes | No distingue |
| **Comprensión RISC/CISC** | Explica filosofías y su impacto | Enumera características | Reconoce que ARM es RISC | Confunde conceptos |
| **Análisis crítico** | Justifica por qué ARM usa Harvard+RISC | Identifica ventajas de cada arquitectura | Repite información dada | No analiza |

---

## Notas para el Instructor

### Tiempo Estimado por Sección

- Contenido Harvard/von Neumann: 40 min
- Contenido RISC/CISC: 40 min
- Actividades 1 y 2: 45 min
- Quiz y cierre: 15 min
- **Total**: ~120 min

### Puntos Clave a Enfatizar

1. **Harvard Modificada es lo mejor de ambos mundos**: Rendimiento + flexibilidad
2. **RISC no significa "peor"**: Significa "optimizado diferente"
3. **x86 moderno también usa ideas RISC internamente**: La distinción ya no es absoluta
4. **ARM domina microcontroladores**: Por buenas razones técnicas (eficiencia)

### Errores Comunes de Estudiantes

**Error**: "Harvard es mejor que von Neumann"  
**Corrección**: Depende de la aplicación. Von Neumann es excelente para computadoras de propósito general.

**Error**: "RISC tiene menos instrucciones totales en un programa"  
**Corrección**: Al contrario, más instrucciones, pero cada una es más simple.

**Error**: "Harvard significa código en ROM y datos en RAM"  
**Corrección**: Es una consecuencia común, pero la definición es "buses y memorias separados".

### Adaptaciones

**Si falta tiempo**:
- Reducir actividad 2 (ejemplo de código)
- Quiz como tarea para casa

**Si sobra tiempo**:
- Discutir RISC-V como arquitectura moderna
- Mostrar código Assembly real de STM32

---

## Conexión con Sesiones Siguientes

Esta sesión establece por qué ARM Cortex-M4 tiene su arquitectura particular. En las próximas:

- **Sesión 3**: Detalles internos de ARM Cortex-M4 (ahora sabemos por qué tiene esa arquitectura)
- **Sesión 4-6**: Programación en Assembly ARM (aplicaremos principios RISC)

---

**Próxima sesión**: Arquitectura ARM Cortex M-4 en Detalle  
**Preparación**: Lectura sobre componentes internos de Cortex-M4
