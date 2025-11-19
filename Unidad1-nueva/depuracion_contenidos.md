# Depuración de Contenidos - Unidad 1

Este documento detalla los contenidos que se han eliminado, reducido o reorganizado de los materiales previos, con sus respectivas justificaciones.

## Contenidos Eliminados Completamente

### 1. Procesador Hack (Nand2Tetris) - Ejercicios 6-9

**Archivo Original**: `1. Procesador.md`

**Razón de eliminación**:
- Aunque el procesador Hack es educativo, su arquitectura no es representativa de ARM Cortex M-4
- El conjunto de instrucciones Hack es específico del proyecto Nand2Tetris y no se utiliza en la industria
- El tiempo invertido en este contenido es mejor utilizado aprendiendo directamente Assembly ARM
- La arquitectura von Neumann del Hack contrasta con la arquitectura Harvard Modificada de ARM

**Contenido conservado**: El concepto de tipos de instrucción (A, C) se mantendrá como ejemplo histórico breve, pero sin ejercicios prácticos extensos.

### 2. Detalles Excesivos del Procesador 8086

**Archivo Original**: `2. Mapa_memoria.md`

**Contenidos reducidos**:
- Diagrama de bloques y pinout detallado del 8086
- Explicación de segmentación de memoria del 8086
- División en bloques de 64KB

**Razón**:
- El 8086 es útil como ejemplo de arquitectura CISC, pero el nivel de detalle era excesivo
- Los estudiantes no necesitan conocer el pinout específico del 8086 para comprender conceptos generales
- El enfoque debe estar en ARM, no en x86 histórico

**Conservado**: Comparación conceptual CISC vs RISC con el 8086 y x86 como ejemplos de CISC.

### 3. Análisis Estadístico Detallado de Instrucciones x86

**Archivo Original**: `2. Mapa_memoria.md`

**Contenido eliminado**: Estadísticas específicas de uso de instrucciones x86 de Peter Kankowski.

**Razón**:
- Aunque interesante, es demasiado específico para x86
- No aporta directamente al aprendizaje de ARM
- Puede incluirse como lectura opcional en recursos

**Conservado**: El concepto general de que algunas instrucciones son más frecuentes que otras.

### 4. Preguntas Orientadoras sin Respuestas

**Archivo Original**: `3. Arquitectura_ARM.md`

**Problema**: Las 10 preguntas orientadoras al inicio no tenían respuestas explícitas en el texto.

**Solución**: 
- Las preguntas se integran como objetivos de aprendizaje en las sesiones correspondientes
- Las respuestas se proporcionan explícitamente en el material
- Se eliminan como formato de "preguntas sin responder"

## Contenidos Reducidos o Simplificados

### 1. Arquitectura PIC16F877

**Archivo Original**: `2. Mapa_memoria.md`

**Cambio**: 
- De: Diagrama de bloques completo y descripción detallada
- A: Mención breve como ejemplo de arquitectura Harvard pura

**Justificación**:
- El curso se enfoca en ARM, no en PIC
- Solo se necesita como punto de comparación para Harvard vs von Neumann
- Los estudiantes no programarán en PIC

### 2. Comunidad ARM

**Archivo Original**: `3. Arquitectura_ARM.md`

**Cambio**:
- De: Párrafo extenso sobre empresas que usan ARM
- A: Mención breve del ecosistema ARM

**Justificación**:
- La información es contextual, no técnica
- Los estudiantes necesitan más tiempo en contenido técnico
- La lista de empresas puede quedar obsoleta rápidamente

### 3. Comparaciones de Desempeño entre Cortex-M

**Archivo Original**: `3. Arquitectura_ARM.md`

**Cambio**:
- De: Gráficos detallados de área vs desempeño
- A: Tabla simple de características principales

**Justificación**:
- Los estudiantes no diseñarán procesadores (no necesitan optimizar área de silicio)
- El enfoque debe ser en programación, no en diseño de hardware
- Una tabla de características es más útil para selección de MCU

## Contenidos Reorganizados

### 1. Instrucciones de Assembly Dispersas

**Archivos Originales**: Distribuidos en múltiples archivos sin secuencia clara

**Nueva Organización**:
- Sesión 4: Registros e instrucciones básicas de movimiento
- Sesión 5: Operaciones aritméticas y lógicas
- Sesión 6: Control de flujo y saltos condicionales
- Sesión 7: Instrucciones de carga/almacenamiento avanzadas

**Justificación**: Progresión pedagógica de simple a complejo.

### 2. Configuración de GPIO

**Archivo Original**: `6. Configurar_GPIO.md` - Todo en un solo ejercicio extenso

**Nueva Organización**:
- Sesión 7: Introducción a GPIO y configuración básica
- Sesión 8: Control completo y optimización

**Justificación**: 
- Demasiado contenido para una sola sesión
- Permite práctica incremental
- Los estudiantes necesitan tiempo para asimilar cada registro

### 3. Secciones de Memoria

**Archivo Original**: `5. Secciones_Memoria_Micro_ARM.md`

**Reorganización**:
- Sesión 3: Introducción al mapa de memoria
- Sesión 4: Uso de secciones .text, .data, .bss
- Sesión 5-7: Aplicación práctica en ejercicios

**Justificación**: El concepto es fundamental pero requiere refuerzo constante.

## Contenidos Corregidos

### 1. Errores de Nomenclatura

**Problema**: `Istruction_Set_Architecture.md` (debería ser "Instruction")

**Solución**: Título corregido en nueva estructura.

### 2. Fragmentos de Código Incompletos

**Archivo Original**: `1. Procesador.md` 

**Problema**: El archivo comienza con un fragmento de texto cortado ("ipo es la instrucción...")

**Solución**: 
- Se descarta el fragmento corrupto
- Se reconstruye el contenido coherente desde el contexto disponible

### 3. Referencias a Figuras sin Contexto

**Problema**: Múltiples archivos referencian figuras con URLs de Notion que pueden no ser accesibles

**Solución**:
- Se describen las figuras textualmente cuando la imagen no está disponible
- Se crean diagramas ASCII simples cuando sea posible
- Se incluyen referencias a documentos oficiales (datasheets, reference manuals)

## Redundancias Eliminadas

### 1. Repetición de Conceptos de Arquitectura Harvard

**Problema**: Explicado en múltiples archivos con ligeras variaciones

**Solución**: Una explicación unificada y clara en Sesión 2, con referencias posteriores cuando sea necesario.

### 2. Múltiples Explicaciones de Registros

**Problema**: Registros ARM explicados en diferentes archivos con diferente nivel de detalle

**Solución**: Explicación completa y estructurada en Sesión 4, con referencia rápida en apéndice.

## Contenidos Marcados como Lectura Opcional

Los siguientes contenidos se mantienen pero se marcan como **lectura opcional** o **ampliación**:

1. **Otras arquitecturas ARM** (Cortex-A, Cortex-R): Solo se cubren Cortex-M en detalle
2. **GPU y DSP**: Mención muy breve para contexto, sin profundizar
3. **MIPS**: Referencia histórica mínima
4. **Detalles avanzados de pipeline**: Se explica el concepto, no la implementación
5. **Microprogramación en CISC**: Concepto mencionado, no desarrollado

## Resumen de Decisiones

| Categoría | Antes | Después | Razón |
|-----------|-------|---------|-------|
| Nand2Tetris | 4 ejercicios extensos | Eliminado | No relevante para ARM |
| x86/8086 | Detalles técnicos | Comparación conceptual | Reducir distracción de ARM |
| PIC | Análisis detallado | Mención breve | Enfoque en ARM |
| Assembly ARM | Disperso | 4 sesiones estructuradas | Progresión pedagógica |
| GPIO | 1 ejercicio masivo | 2 sesiones incrementales | Mejor asimilación |
| Preguntas orientadoras | Sin respuestas | Integradas en objetivos | Claridad pedagógica |

## Contenidos que se Mantienen Intactos

- Comparación RISC vs CISC (es fundamental)
- Arquitectura Harvard vs von Neumann (es fundamental)
- Conjunto de registros ARM Cortex-M (es esencial)
- Instrucciones básicas de Assembly (núcleo del curso)
- Configuración de periféricos (objetivo del curso)
- Ejercicios prácticos de Assembly (necesarios para práctica)

---

**Criterio General de Depuración**: Se eliminó o redujo todo contenido que:
1. No contribuya directamente a programar ARM Cortex M-4
2. Sea demasiado específico de otras arquitecturas
3. Requiera tiempo excesivo sin beneficio pedagógico proporcional
4. Sea redundante con otro contenido mejor estructurado
5. Esté incompleto o corrupto sin posibilidad de reconstrucción

El objetivo es maximizar el tiempo en contenido relevante, práctico y directamente aplicable al desarrollo en ARM Cortex M-4.

---

**Fecha de creación**: Noviembre 2025  
**Versión**: 1.0
