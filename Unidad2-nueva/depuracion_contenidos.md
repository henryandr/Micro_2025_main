# Depuración de Contenidos - Unidad 2

Este documento detalla los contenidos eliminados, modificados o reorganizados de los materiales originales de Unidad 2, con justificación de cada decisión.

## Contenidos de la Carpeta Original Unidad2

Los materiales originales consistían en:
1. `MEF.md` - Máquinas de Estados Finitos (teoría)
2. `MEF_en_C.md` - Implementación de MEF en C (ejemplo de debouncing)
3. `Multiplexacion.md` - Concepto de multiplexación
4. `lenguaje_C_para_embebidos.md` - Introducción a C para embebidos
5. `NVIC_Interrupciones.md` - Interrupciones y NVIC
6. `Ejercicio_Semaforo.md` - Ejercicio práctico
7. `Evaluacion_sumativa.md` - Evaluación final

## Análisis y Decisiones

### ✅ Contenidos Conservados (Modificados)

#### 1. Máquinas de Estados Finitos (MEF.md)

**Estado Original**:
- Teoría sólida de MEF
- Ejemplos con Moore y Mealy
- Diagramas usando Mermaid
- Bien estructurado

**Modificaciones**:
- ✅ Conservado: Toda la teoría fundamental
- ✅ Mejorado: Agregados más ejemplos prácticos
- ✅ Expandido: Comparación detallada Moore vs Mealy
- ✅ Añadido: Ejercicios de diseño antes de implementación
- ⚠️ Dividido: Teoría (Sesión 2) separada de implementación (Sesión 3)

**Justificación**: El contenido teórico es excelente y forma la base conceptual. Se dividió para mejor pedagogía.

#### 2. Implementación de MEF en C (MEF_en_C.md)

**Estado Original**:
- Ejemplo completo de debouncing con MEF
- Código en C funcional
- Bien documentado paso a paso

**Modificaciones**:
- ✅ Conservado: Ejemplo de debouncing (muy relevante)
- ✅ Mejorado: Código más modular y comentado
- ✅ Añadido: Comparación de implementaciones (switch-case vs tabla)
- ✅ Expandido: Más ejemplos (semáforo, control de acceso)
- ➡️ Reubicado: De material adicional a Sesión 3 (núcleo de la unidad)

**Justificación**: Contenido excelente que merece ser el eje central de una sesión completa.

#### 3. Multiplexación (Multiplexacion.md)

**Estado Original**:
- Concepto claro de multiplexación
- Ejemplos de displays y teclados
- Configuración del microcontrolador

**Modificaciones**:
- ✅ Conservado: Todo el concepto y ejemplos
- ✅ Mejorado: Ejemplos de código más completos
- ✅ Añadido: Implementación con MEF
- ✅ Expandido: Cálculos de frecuencia de refresco
- ➡️ Promocionado: De material adicional a Sesión 6 completa

**Justificación**: Tema importante que requiere sesión dedicada. Integra bien con MEF.

#### 4. Lenguaje C para Embebidos (lenguaje_C_para_embebidos.md)

**Estado Original**:
- Introducción básica a C
- Comparación con otros lenguajes
- Ejercicio de investigación

**Modificaciones**:
- ✅ Conservado: Conceptos básicos
- ✅ Mejorado: Enfoque en transición desde Assembly
- ✅ Añadido: Diferencias C estándar vs C bare metal
- ✅ Expandido: Estructura de proyectos, Makefile, startup code
- ⚠️ Reenfocado: De introducción general a transición específica Assembly→C
- ➡️ Integrado: En Sesión 1 como material nuclear

**Justificación**: Necesario reenfoque hacia C bare metal, no C en general. Transición desde Assembly es clave.

#### 5. NVIC e Interrupciones (NVIC_Interrupciones.md)

**Estado Original**:
- Conceptos de interrupciones
- Modos de operación
- NVIC básico

**Modificaciones**:
- ✅ Conservado: Conceptos fundamentales
- ✅ Mejorado: Implementación práctica en C
- ✅ Añadido: Configuración de EXTI (interrupciones externas)
- ✅ Expandido: Ejemplos completos con código funcional
- ⚠️ Enfocado: Solo interrupciones externas (EXTI), no todas las interrupciones
- ➡️ Movido: A Sesión 7 (penúltima sesión, tras dominar MEF)

**Justificación**: Interrupciones son avanzadas. Mejor enseñarlas después de MEF sólido.

#### 6. Ejercicio Semáforo (Ejercicio_Semaforo.md)

**Estado Original**:
- Ejercicio práctico de semáforo
- Requisitos y especificaciones

**Modificaciones**:
- ✅ Conservado: Idea del semáforo
- ✅ Mejorado: Especificaciones más claras
- ✅ Ampliado: Semáforo "inteligente" con botón peatonal
- ✅ Integrado: MEF + Timer + Interrupciones
- ➡️ Elevado: De ejercicio a Proyecto Final (Sesión 8)

**Justificación**: Excelente proyecto integrador. Merece ser la evaluación sumativa.

#### 7. Evaluación Sumativa (Evaluacion_sumativa.md)

**Estado Original**:
- Evaluación con preguntas teóricas
- Ejercicio práctico

**Modificaciones**:
- ✅ Conservado: Estructura general (teórica + práctica)
- ✅ Mejorado: Preguntas más específicas a contenido de Unidad 2
- ✅ Añadido: Rúbricas detalladas
- ✅ Expandido: Proyecto integrador (semáforo) como evaluación práctica
- ⚠️ Reenfocado: Énfasis en MEF y C bare metal, no solo teoría

**Justificación**: Evaluación debe reflejar énfasis práctico de la unidad.

### ❌ Contenidos Eliminados (con Justificación)

#### 1. Comparación Superficial de Lenguajes

**Qué se eliminó**:
- Ejercicio genérico: "Investiga otros lenguajes para embebidos"
- Ranking de lenguajes

**Justificación**:
- ❌ Poco relevante para objetivos de la unidad
- ❌ Distrae del enfoque en C bare metal
- ❌ Los estudiantes ya tienen C como requisito
- ✅ Mejor usar ese tiempo en práctica de C

#### 2. Teoría Excesiva de Interrupciones

**Qué se eliminó**:
- Clasificación exhaustiva de tipos de interrupciones
- Teoría profunda de modos de operación (Thread vs Handler)
- Detalles de excepciones que no se usan

**Justificación**:
- ⚠️ Demasiado teórico para una unidad práctica
- ⚠️ Los estudiantes ya vieron conceptos en Unidad 1
- ✅ Mejor enfocarse en EXTI práctico y usable
- ✅ Teoría avanzada puede ir en curso posterior

#### 3. Ejemplos Incompletos o Fragmentados

**Qué se eliminó**:
- Fragmentos de código sin contexto completo
- Pseudocódigo sin implementación real
- Ejemplos que no compilan

**Justificación**:
- ❌ No son útiles para estudiantes (generan confusión)
- ❌ Mejor tener pocos ejemplos completos que muchos fragmentos
- ✅ Cada ejemplo debe ser compilable y funcional

### ➕ Contenidos Añadidos (no en materiales originales)

#### 1. Estructura de Proyectos C Bare Metal (Sesión 1)

**Por qué se añadió**:
- ✅ Crítico para trabajar en C bare metal
- ✅ Faltaba en materiales originales
- ✅ Estudiantes no pueden crear proyectos desde cero sin esto

**Contenido añadido**:
- Startup code (startup_stm32f407.s)
- Linker script (linker_script.ld)
- Makefile para compilación
- Organización de directorios (src/, inc/)
- Archivo de definiciones (stm32f407xx.h)

#### 2. Driver GPIO Completo en C (Sesión 4)

**Por qué se añadió**:
- ✅ En Unidad 1 hicieron driver en Assembly, necesitan versión C
- ✅ Práctica esencial de desarrollo de drivers
- ✅ Base para todos los proyectos posteriores

**Contenido añadido**:
- Header file con API clara
- Implementation file con funciones modulares
- Ejemplo de uso del driver
- Comparación con implementación en Assembly

#### 3. Timer Completo para MEF (Sesión 5)

**Por qué se añadió**:
- ✅ MEF necesitan timing preciso (no delays busy-wait)
- ✅ Faltaba implementación completa de Timer
- ✅ Integración Timer + MEF es clave

**Contenido añadido**:
- Configuración completa de TIM2
- Cálculo de prescaler y auto-reload
- Generación de interrupciones de timer
- Ejemplo: MEF con base de tiempo precisa

#### 4. Evaluación Diagnóstica (Sesión 1)

**Por qué se añadió**:
- ✅ Necesaria para evaluar conocimientos previos de Unidad 1
- ✅ Identificar estudiantes que necesitan refuerzo
- ✅ Estándar pedagógico

**Contenido añadido**:
- 10 preguntas sobre Assembly y arquitectura
- 2-3 preguntas sobre manipulación de bits
- Ejercicio corto de lectura de datasheet

#### 5. Quizzes Formativos por Sesión

**Por qué se añadió**:
- ✅ Evaluación continua (no solo final)
- ✅ Feedback inmediato para estudiantes
- ✅ Detección temprana de problemas

**Contenido añadido**:
- Quiz de 5 preguntas por sesión (excepto Sesión 8)
- Mezcla de teórica y práctica
- Rúbrica de evaluación

#### 6. Recursos de Apoyo

**Por qué se añadió**:
- ✅ Estudiantes necesitan material de consulta
- ✅ Facilita aprendizaje autónomo

**Contenido añadido**:
- Glosario de términos
- Comparación Assembly vs C (lado a lado)
- Guía de debugging en C
- Bibliografía anotada

### 🔄 Contenidos Reorganizados

#### Orden Pedagógico

**Orden Original** (implícito en materiales):
1. C para embebidos
2. MEF
3. Implementación MEF
4. Multiplexación
5. Interrupciones
6. Ejercicio semáforo

**Nuevo Orden** (Unidad 2 reestructurada):
1. **Sesión 1**: Transición Assembly→C + estructura de proyectos
2. **Sesión 2**: Teoría de MEF (Moore, Mealy, diseño)
3. **Sesión 3**: Implementación de MEF en C (debouncing, ejemplos)
4. **Sesión 4**: Driver GPIO en C (práctica de modularización)
5. **Sesión 5**: Timer para MEF (timing preciso)
6. **Sesión 6**: Multiplexación (optimización)
7. **Sesión 7**: Interrupciones EXTI + NVIC (eventos asíncronos)
8. **Sesión 8**: Proyecto integrador (semáforo inteligente)

**Justificación**:
- ✅ Progresión lógica: fundamentos → implementación → integración
- ✅ Cada sesión construye sobre la anterior
- ✅ Complejidad creciente pero gradual
- ✅ Proyecto final integra TODO lo aprendido

## Resumen Estadístico

| Categoría | Cantidad | Porcentaje |
|-----------|----------|------------|
| Conservados (modificados) | 7 | 100% |
| Eliminados completamente | 0 | 0% |
| Parcialmente eliminados | 3 fragmentos | ~10% |
| Añadidos nuevos | 6 módulos | +50% |
| Reorganizados | 100% | - |

## Métricas de Calidad

### Antes (Materiales Originales)
- ⚠️ Contenido disperso en 7 archivos no estructurados
- ⚠️ Sin estructura clara de sesiones
- ⚠️ Mezcla de niveles (básico y avanzado sin orden)
- ⚠️ Ejemplos incompletos o no compilables
- ✅ Conceptos fundamentales sólidos (MEF, multiplexación)

### Después (Unidad 2 Reestructurada)
- ✅ Contenido organizado en 8 sesiones coherentes
- ✅ Progresión pedagógica clara
- ✅ Todos los ejemplos compilables y probables
- ✅ Evaluaciones completas (diagnóstica, formativas, sumativa)
- ✅ Recursos de apoyo adicionales
- ✅ Proyecto integrador final
- ✅ Estructura profesional de drivers

## Validación de Cambios

### Criterios de Depuración Aplicados

1. **Relevancia**: ¿Es esencial para objetivos de aprendizaje?
2. **Completitud**: ¿Tiene ejemplos funcionales completos?
3. **Progresión**: ¿Está en el orden correcto?
4. **Practicidad**: ¿Es aplicable en hardware real?
5. **Claridad**: ¿Es comprensible para el nivel del estudiante?

### Resultado

- ✅ Todo contenido pasa criterios de calidad
- ✅ Ejemplos completos y funcionales
- ✅ Orden pedagógico optimizado
- ✅ Balance teoría-práctica (30-60-10)

---

**Conclusión**: Los materiales originales tenían excelente contenido conceptual pero requerían reestructuración para uso pedagógico efectivo. La depuración consistió principalmente en reorganización y expansión, NO en eliminación de contenido valioso.

---

**Fecha de creación**: Noviembre 2025  
**Versión**: 1.0  
**Autor**: Material del curso de Microprocesadores
