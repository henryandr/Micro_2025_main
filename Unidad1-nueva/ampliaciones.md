# Ampliaciones de Contenido - Unidad 1

Este documento detalla los contenidos que se han añadido o expandido significativamente respecto a los materiales previos, con sus respectivas justificaciones.

## Contenidos Completamente Nuevos

### 1. Evaluación Diagnóstica Estructurada

**Contenido**: Evaluación de 10 preguntas de opción múltiple + 2 preguntas abiertas.

**Razón de añadir**:
- Los materiales previos no incluían evaluación inicial
- Es esencial para identificar conocimientos previos
- Permite ajustar el ritmo de enseñanza según el grupo
- Identifica estudiantes que requieren apoyo adicional

**Ubicación**: `evaluaciones/diagnostica.md`

### 2. Rúbricas de Evaluación Detalladas

**Contenido**: Rúbricas específicas para cada tipo de evaluación (código, documentación, comprensión conceptual).

**Razón de añadir**:
- Proporciona criterios objetivos de evaluación
- Da transparencia al estudiante sobre expectativas
- Facilita feedback constructivo
- Asegura consistencia en la calificación

**Ubicación**: `evaluaciones/rubricas.md`

### 3. Startup Code y Linker Script Explicados

**Contenido**: Explicación detallada de archivos de inicio y script de enlazado para ARM.

**Razón de añadir**:
- Los materiales previos asumían estos archivos sin explicarlos
- Son fundamentales para comprender el flujo de inicialización
- Necesarios para desarrollo bare metal efectivo
- Explican el proceso desde reset hasta main()

**Ubicación**: Sesión 6 y `ejemplos_c/startup_stm32f407.s`

### 4. Convenciones de Llamada ARM (ABI)

**Contenido**: Explicación de AAPCS (ARM Architecture Procedure Call Standard).

**Razón de añadir**:
- Esencial para interoperabilidad entre C y Assembly
- Explica uso de registros R0-R3 para parámetros
- Necesario para escribir funciones que respeten convenciones
- Ausente en materiales previos

**Ubicación**: Sesión 5

### 5. Introducción a Interrupciones y NVIC

**Contenido**: Controlador de interrupciones, prioridades, y manejo de excepciones.

**Razón de añadir**:
- Fundamental para sistemas embebidos
- Los materiales previos solo mencionaban SysTick superficialmente
- Necesario para programación reactiva
- Base para unidades posteriores (UART, ADC con interrupciones)

**Ubicación**: Sesión 8 y `drivers_iniciales/irq_example.c`

### 6. Tabla de Vectores de Interrupción

**Contenido**: Explicación de la vector table y su ubicación en memoria.

**Razón de añadir**:
- Crítico para comprender el arranque del sistema
- Explica manejo de reset, fallas, y excepciones
- Los materiales previos no lo cubrían
- Necesario para debugging efectivo

**Ubicación**: Sesión 6

### 7. Configuración de Reloj del Sistema (RCC)

**Contenido**: Explicación del Reset and Clock Control, PLL, y prescalers.

**Razón de añadir**:
- Los ejemplos previos usaban RCC sin explicar su propósito
- Es el primer periférico que debe configurarse
- Afecta todos los demás periféricos
- Necesario para cálculos de timing precisos

**Ubicación**: Sesión 7

### 8. Buses AHB, APB1, APB2

**Contenido**: Arquitectura de buses en ARM Cortex-M y STM32.

**Razón de añadir**:
- Explica por qué periféricos tienen diferentes direcciones base
- Importante para comprender limitaciones de velocidad
- Necesario para configuración de clock correcta
- Ausente en materiales previos

**Ubicación**: Sesión 3

## Contenidos Significativamente Expandidos

### 1. Registros de Propósito Especial

**Antes**: Mención breve de registros especiales.

**Ahora**: Explicación detallada de:
- **APSR** (Application Program Status Register): Flags N, Z, C, V
- **IPSR** (Interrupt Program Status Register): Número de excepción activa
- **EPSR** (Execution Program Status Register): Estado Thumb, IT block
- **PRIMASK, FAULTMASK, BASEPRI**: Máscaras de interrupción
- **CONTROL**: Selección de stack pointer y nivel de privilegio

**Justificación**: Críticos para manejo de excepciones y comprensión del estado del procesador.

**Ubicación**: Sesión 4

### 2. Modos de Direccionamiento

**Antes**: Ejemplos básicos de LDR/STR.

**Ahora**: Cobertura completa de:
- Direccionamiento inmediato
- Direccionamiento a registro
- Direccionamiento indirecto
- Pre-indexado vs post-indexado
- Escalado con desplazamiento (LSL)

**Justificación**: Necesario para acceso eficiente a arrays y estructuras.

**Ubicación**: Sesión 5

### 3. Instrucciones de Manipulación de Bits

**Antes**: Ejemplo con AND, OR, XOR.

**Ahora**: Ampliado con:
- Operaciones de desplazamiento (LSL, LSR, ASR, ROR)
- Bit field operations (BFI, BFC, UBFX, SBFX)
- Tabla de decisión para selección de operación
- Ejemplos de uso en configuración de registros

**Justificación**: Absolutamente esencial para configuración de periféricos.

**Ubicación**: Sesión 5

### 4. Stack y Uso de PUSH/POP

**Antes**: No mencionado explícitamente.

**Ahora**: Explicación completa de:
- Stack pointer (MSP vs PSP)
- Operaciones PUSH/POP múltiples registros
- Convención de stack frame
- Uso en llamadas a función
- Preservación de contexto

**Justificación**: Fundamental para funciones y manejo de interrupciones.

**Ubicación**: Sesión 5 y 8

### 5. Control de Flujo Avanzado

**Antes**: IF simple y WHILE básico.

**Ahora**: Ampliado con:
- Tablas de salto (branch tables)
- Implementación de switch/case
- Bucles optimizados (loop unrolling conceptual)
- Comparaciones sin signo vs con signo
- Banderas de condición y su uso

**Justificación**: Necesario para lógica de aplicación compleja.

**Ubicación**: Sesión 6

### 6. Acceso a Estructuras en C

**Antes**: Solo acceso directo a direcciones.

**Ahora**: Explicación de:
- Definición de estructuras para periféricos
- Punteros a estructuras
- Uso de offsetof()
- Volatile y su importancia
- Ejemplo completo con GPIO como estructura

**Justificación**: Práctica estándar en desarrollo bare metal profesional.

**Ubicación**: Sesión 7

### 7. Timer Básico

**Antes**: Solo mencionado en ejercicio de SysTick.

**Ahora**: Cobertura de:
- Configuración de Timer básico (TIM2/TIM3)
- Prescaler y ARR (Auto-Reload Register)
- Generación de delays precisos
- Interrupciones de timer
- Comparación: busy-wait vs timer con interrupción

**Justificación**: Periférico esencial para control de tiempo.

**Ubicación**: Sesión 7 y `drivers_iniciales/timer_driver.c`

## Contenidos de Soporte Añadidos

### 1. Glosario de Términos

**Contenido**: Glosario con 40+ términos técnicos en español con su equivalente en inglés.

**Justificación**: 
- Unifica terminología en el curso
- Facilita búsqueda de información en inglés
- Referencia rápida para estudiantes

**Ubicación**: `recursos/glosario.md`

### 2. Bibliografía Anotada

**Contenido**: Lista de referencias con descripción de cada recurso.

**Justificación**:
- Los materiales previos solo tenían enlaces sin contexto
- Guía al estudiante sobre qué buscar en cada documento
- Prioriza recursos oficiales y confiables

**Ubicación**: `recursos/bibliografia.md`

### 3. Preguntas Frecuentes (FAQ)

**Contenido**: 20+ preguntas comunes con respuestas detalladas.

**Justificación**:
- Anticipa dudas comunes
- Reduce tiempo de consulta en clase
- Referencia para autoaprendizaje

**Ubicación**: `recursos/faq.md`

### 4. Guía de Herramientas

**Contenido**: Instalación y uso de toolchain ARM.

**Justificación**:
- Los materiales previos asumían herramientas instaladas
- Muchos estudiantes enfrentan problemas de configuración
- Debe ser primer paso antes de cualquier código

**Ubicación**: `recursos/guia_herramientas.md`

### 5. Plantillas de Código

**Contenido**: Templates para proyectos bare metal.

**Justificación**:
- Acelera inicio de proyectos
- Muestra estructura profesional
- Evita errores comunes de principiantes

**Ubicación**: `ejemplos_c/template/`

## Actividades Prácticas Añadidas

### 1. Análisis de Código con Error Intencional

**Contenido**: Ejercicios de debugging con errores comunes (stack mal inicializado, clock no habilitado, etc.).

**Justificación**:
- Desarrolla habilidades de debugging
- Previene errores comunes
- Los materiales previos solo tenían código correcto

**Ubicación**: Sesiones 6, 7, 8

### 2. Laboratorios Incrementales

**Contenido**: Serie de mini-labs que construyen un proyecto completo.

**Justificación**:
- Progresión de simple a complejo
- Cada lab añade funcionalidad
- Culmina en mini-proyecto final
- Los materiales previos tenían ejercicios aislados

**Ubicación**: Cada sesión tiene 1-2 mini-labs

### 3. Comparación Assembly vs C Optimizado

**Contenido**: Ejercicios que comparan código Assembly escrito a mano vs código C compilado con -O1, -O2, -O3.

**Justificación**:
- Muestra cuándo usar Assembly y cuándo C
- Desmitifica la optimización de compilador
- Desarrolla criterio de decisión
- No presente en materiales previos

**Ubicación**: Sesión 6 (actividad opcional)

### 4. Lectura de Datasheet

**Contenido**: Ejercicio guiado de extracción de información de datasheet.

**Justificación**:
- Habilidad esencial para desarrollo embebido
- No cubierta en materiales previos
- Estudiantes suelen evitar datasheets

**Ubicación**: Sesión 7

### 5. Reflexión Escrita

**Contenido**: Actividades de reflexión sobre ventajas/desventajas de diferentes enfoques.

**Justificación**:
- Desarrolla pensamiento crítico
- No solo memorizar, sino comprender
- Formato ausente en materiales previos

**Ubicación**: Sesiones 3, 6, 8

## Contenidos de Evaluación Ampliados

### 1. Quizzes Formativos Sesión por Sesión

**Antes**: Sin evaluaciones formativas estructuradas.

**Ahora**: Quiz de 5-7 preguntas al final de cada sesión.

**Justificación**: Feedback inmediato, identificación temprana de dificultades.

### 2. Evaluación Sumativa Completa

**Antes**: Solo ejercicio de SysTick con múltiples LEDs.

**Ahora**: Evaluación integral con:
- Parte teórica (conceptos de arquitectura)
- Parte práctica (mini-driver funcional)
- Explicación oral/escrita
- Justificación de decisiones técnicas

**Justificación**: Evalúa comprensión profunda, no solo habilidad de copiar código.

### 3. Criterios de Evaluación Claros

**Antes**: Sin criterios explícitos.

**Ahora**: Rúbricas con niveles: Excelente, Bueno, Suficiente, Insuficiente para cada aspecto.

**Justificación**: Transparencia, feedback constructivo, autoevaluación.

## Soporte Visual Añadido

### 1. Diagramas de Flujo

**Contenido**: Diagramas para:
- Flujo de inicialización del MCU
- Manejo de interrupciones
- Proceso de compilación y enlazado

**Justificación**: Facilita comprensión de procesos complejos.

### 2. Mapas Conceptuales

**Contenido**: Mapas que conectan conceptos relacionados.

**Justificación**: Ayuda a ver el "big picture", no solo detalles aislados.

### 3. Tablas de Referencia Rápida

**Contenido**: 
- Tabla de registros
- Tabla de instrucciones frecuentes
- Tabla de periféricos y sus direcciones

**Justificación**: Referencia rápida durante prácticas.

## Conexión con Unidades Posteriores

### 1. Proyección a Unidad 2

**Contenido**: Documento que anticipa temas de Unidad 2 y cómo se conectan con Unidad 1.

**Justificación**:
- Da continuidad al curso
- Motiva al estudiante mostrando aplicaciones futuras
- Identifica prerequisitos que deben reforzarse

**Ubicación**: `proyeccion_unidad2.md`

## Resumen de Ampliaciones

| Categoría | Añadido | Justificación Principal |
|-----------|---------|------------------------|
| Evaluaciones | Diagnóstica + Formativas + Sumativa estructurada | Feedback y medición de progreso |
| Interrupciones | Explicación completa de NVIC y vector table | Fundamental para sistemas embebidos |
| Registros especiales | APSR, IPSR, EPSR, PRIMASK, etc. | Comprensión profunda del procesador |
| Modos de direccionamiento | Cobertura completa | Acceso eficiente a memoria |
| Startup code | Explicación detallada | Comprensión de inicialización |
| ABI | Convenciones de llamada | Interoperabilidad C-Assembly |
| Buses | AHB/APB | Arquitectura del sistema |
| Timer | Configuración y uso | Periférico esencial |
| Recursos de soporte | Glosario, FAQ, guías | Autoaprendizaje y referencia |
| Actividades prácticas | Debugging, lectura datasheet, reflexión | Habilidades prácticas |

## Principio Guía de Ampliaciones

Todos los contenidos añadidos siguen el principio:

> **"Proporcionar las herramientas y conocimientos mínimos necesarios para que el estudiante pueda desarrollar aplicaciones bare metal funcionales en ARM Cortex M-4, con comprensión suficiente para depurar problemas y aprender de forma autónoma."**

No se añadió contenido por completitud académica, sino por necesidad práctica demostrable en el desarrollo real de firmware.

---

**Fecha de creación**: Noviembre 2025  
**Versión**: 1.0
