# Unidad 1: Fundamentos de Microprocesadores y ARM Cortex M-4

## Mapa de Navegación

### 📚 Documentos Principales

- **[Unidad1.md](Unidad1.md)** - Documento maestro completo de la unidad
- **[supuestos.md](supuestos.md)** - Supuestos técnicos y de conocimientos previos
- **[depuracion_contenidos.md](depuracion_contenidos.md)** - Contenidos eliminados y su justificación
- **[ampliaciones.md](ampliaciones.md)** - Contenidos añadidos y expandidos
- **[proyeccion_unidad2.md](proyeccion_unidad2.md)** - Recomendaciones para la siguiente unidad

### 🎯 Sesiones de Clase

La unidad está organizada en **8 sesiones** de 2 horas cada una, distribuidas en 4 semanas:

| Semana | Sesión | Tema Principal | Archivo |
|--------|--------|----------------|---------|
| 1 | 1 | Introducción y Diagnóstico - Arquitecturas de Procesadores | [sesion1.md](sesiones/sesion1.md) |
| 1 | 2 | Harvard vs von Neumann, RISC vs CISC | [sesion2.md](sesiones/sesion2.md) |
| 2 | 3 | Arquitectura ARM Cortex M-4 - Componentes Internos | [sesion3.md](sesiones/sesion3.md) |
| 2 | 4 | Conjunto de Registros y Primeras Instrucciones Assembly | [sesion4.md](sesiones/sesion4.md) |
| 3 | 5 | Instrucciones ARM - Operaciones Aritméticas y Lógicas | [sesion5.md](sesiones/sesion5.md) |
| 3 | 6 | Control de Flujo en Assembly y C Bare Metal | [sesion6.md](sesiones/sesion6.md) |
| 4 | 7 | Desarrollo de Drivers GPIO y Timer | [sesion7.md](sesiones/sesion7.md) |
| 4 | 8 | Interrupciones y Evaluación Sumativa | [sesion8.md](sesiones/sesion8.md) |

### 💻 Código y Ejemplos

#### Ejemplos en Assembly
- **[ejemplos_assembly/](ejemplos_assembly/)** - Código Assembly comentado por sesión
  - `ejemplo01_mov_basico.s` - Instrucciones básicas de movimiento
  - `ejemplo02_aritmetica.s` - Operaciones aritméticas
  - `ejemplo03_logica_bits.s` - Operaciones lógicas y de bits
  - `ejemplo04_saltos.s` - Control de flujo y saltos condicionales
  - `ejemplo05_funciones.s` - Llamadas a función y stack
  - Y más...

#### Ejemplos en C
- **[ejemplos_c/](ejemplos_c/)** - Código C bare metal con headers
  - `startup_stm32f407.s` - Archivo de inicio (startup code)
  - `linker_script.ld` - Script de enlazado
  - `main.c` - Plantilla de main básico
  - `gpio_config.c` - Configuración de GPIO en C
  - `timer_config.c` - Configuración de Timer en C

#### Drivers Iniciales
- **[drivers_iniciales/](drivers_iniciales/)** - Drivers bare metal funcionales
  - `gpio_driver.c` / `gpio_driver.h` - Driver de GPIO
  - `timer_driver.c` / `timer_driver.h` - Driver de Timer básico
  - `irq_example.c` - Ejemplo de manejo de interrupciones

### 📝 Evaluaciones

- **[evaluaciones/](evaluaciones/)** - Instrumentos de evaluación completos
  - `diagnostica.md` - Evaluación diagnóstica (Sesión 1)
  - `quizzes_formativos.md` - Quizzes por sesión (Sesiones 1-8)
  - `sumativa.md` - Evaluación final (Sesión 8)
  - `rubricas.md` - Rúbricas de evaluación

### 📖 Recursos de Apoyo

- **[recursos/](recursos/)** - Material complementario
  - `bibliografia.md` - Bibliografía anotada y enlaces
  - `glosario.md` - Glosario de términos técnicos
  - `faq.md` - Preguntas frecuentes
  - `guia_herramientas.md` - Instalación de toolchain

## Visión General de la Unidad

### Objetivo General

Desarrollar competencias fundamentales en programación de microprocesadores ARM Cortex M-4, incluyendo comprensión de arquitecturas, programación en Assembly y C a nivel bare metal, y configuración básica de periféricos.

### Objetivos Específicos

Al completar esta unidad, el estudiante será capaz de:

1. **Comprender** las diferencias entre arquitecturas de microprocesadores (Harvard, von Neumann, RISC, CISC)
2. **Identificar** los componentes internos de un microcontrolador ARM Cortex M-4
3. **Programar** en Assembly ARM utilizando el conjunto de instrucciones Thumb-2
4. **Desarrollar** código C bare metal con acceso directo a registros de periféricos
5. **Configurar** periféricos básicos (GPIO, Timer) sin uso de HAL
6. **Implementar** drivers simples con manejo de interrupciones
7. **Depurar** código a bajo nivel utilizando conocimientos de arquitectura

### Distribución Temporal

- **Duración Total**: 4 semanas
- **Sesiones**: 8 sesiones de 2 horas (16 horas totales)
- **Trabajo Autónomo**: 24 horas estimadas (lectura, ejercicios, proyecto)
- **Evaluaciones**: 4 horas (diagnóstica + formativas + sumativa)

### Metodología

- **40% Teoría**: Explicación de conceptos fundamentales
- **50% Práctica**: Ejercicios guiados y laboratorios
- **10% Evaluación**: Diagnóstica, formativas y sumativa

### Competencias Desarrolladas

1. **Comprensión conceptual**: Arquitecturas de procesadores y su impacto en programación
2. **Programación de bajo nivel**: Assembly y C bare metal
3. **Configuración de hardware**: Periféricos y registros
4. **Depuración**: Análisis de problemas a nivel de hardware y software
5. **Documentación técnica**: Lectura e interpretación de datasheets

## Requisitos Previos

### Conocimientos Requeridos

- ✅ Programación básica en C (variables, funciones, estructuras de control)
- ✅ Sistemas numéricos (binario, hexadecimal)
- ✅ Conceptos básicos de electricidad digital

### Conocimientos NO Requeridos (se enseñan en el curso)

- ❌ Assembly
- ❌ Arquitectura de procesadores
- ❌ Programación de microcontroladores
- ❌ Configuración de periféricos

### Hardware Necesario

- Tarjeta de desarrollo STM32F407VET6 Black Board (o equivalente con Cortex M-4)
- Cable USB para programación
- LEDs y resistencias (220Ω - 1kΩ)
- Protoboard y cables jumper

### Software Necesario

- arm-none-eabi-gcc (toolchain)
- Editor de texto o IDE (recomendado: VS Code con extensiones ARM)
- OpenOCD o ST-Link Utility (para programación)
- Opcional: arm-none-eabi-gdb (para debugging)

Ver [guia_herramientas.md](recursos/guia_herramientas.md) para instalación detallada.

## Estructura de Cada Sesión

Cada sesión sigue el siguiente formato:

1. **Objetivos Específicos** (2-3 objetivos medibles)
2. **Contenidos Temáticos** (teoría y conceptos)
3. **Actividades en Clase** (mínimo 2 por sesión)
   - Actividades guiadas
   - Discusión técnica
   - Análisis de código
4. **Evaluación Formativa** (quiz o revisión de código)
5. **Material para Casa** (lectura previa a siguiente sesión)
6. **Evidencias** (qué debe entregar el estudiante)

## Sistema de Evaluación

### Distribución de Calificaciones

| Componente | Porcentaje | Descripción |
|-----------|------------|-------------|
| Diagnóstica | 5% | Evaluación inicial (no punitiva) |
| Formativas | 30% | Quizzes y ejercicios (8 sesiones) |
| Proyecto Incremental | 35% | Mini-labs y evidencias por sesión |
| Sumativa Final | 30% | Evaluación integral (Sesión 8) |

### Criterios de Aprobación

- **Mínimo**: 60% de calificación global
- **Asistencia**: Mínimo 80% (6 de 8 sesiones)
- **Proyecto Final**: Obligatorio entregar (mínimo 50% en evaluación sumativa)

## Ruta de Aprendizaje

```
Semana 1: FUNDAMENTOS
├─ Sesión 1: ¿Qué es un procesador? Arquitecturas generales
└─ Sesión 2: Diferencias fundamentales (Harvard/von Neumann, RISC/CISC)

Semana 2: ARM CORTEX M-4
├─ Sesión 3: Componentes internos, mapa de memoria, buses
└─ Sesión 4: Registros y primeras instrucciones Assembly

Semana 3: PROGRAMACIÓN
├─ Sesión 5: Operaciones aritméticas, lógicas, y de bits
└─ Sesión 6: Control de flujo, funciones, interoperabilidad C-Assembly

Semana 4: APLICACIÓN
├─ Sesión 7: Drivers GPIO y Timer bare metal
└─ Sesión 8: Interrupciones y proyecto final
```

## Cómo Usar Este Material

### Para Estudiantes

1. **Antes de cada sesión**: Lee el archivo `sesionN.md` correspondiente
2. **Durante la sesión**: Participa en actividades, toma notas de explicaciones del profesor
3. **Después de la sesión**: Completa las evidencias y ejercicios
4. **Consulta**: Usa el glosario y FAQ cuando tengas dudas
5. **Practica**: Ejecuta los ejemplos de código y modifícalos para experimentar

### Para Profesores

1. **Preparación**: Revisa la sesión completa y los recursos necesarios
2. **Flexibilidad**: Los tiempos son estimados, ajusta según el grupo
3. **Evaluación**: Usa las rúbricas proporcionadas para feedback consistente
4. **Adaptación**: Los ejercicios opcionales permiten ajustar dificultad
5. **Recursos**: Material complementario en carpeta `recursos/`

## Soporte y Contacto

- **Documentación Oficial**:
  - [STM32F407 Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020.pdf)
  - [ARM Cortex-M4 Technical Reference Manual](https://developer.arm.com/documentation/100166/latest/)
  
- **Comunidad**: 
  - Foro del curso (si existe)
  - Grupo de estudio
  - Horas de oficina del profesor

## Contribuciones

Este material está en constante mejora. Si encuentras errores, contenido poco claro, o tienes sugerencias de mejora:

1. Documenta el problema o sugerencia
2. Proporciona contexto (qué sesión, qué concepto)
3. Si es un error de código, proporciona el mensaje de error
4. Comparte con el instructor

## Licencia y Uso

Este material educativo está diseñado para el curso de Microprocesadores. Puede ser usado libremente con fines educativos citando la fuente.

---

## Checklist de Uso del Material

### Antes de Iniciar el Curso

- [ ] Verificar disponibilidad de hardware (tarjetas STM32)
- [ ] Instalar toolchain en laboratorio
- [ ] Probar un ejemplo de compilación
- [ ] Preparar datasheets y documentos de referencia
- [ ] Revisar todas las 8 sesiones

### Por Sesión

- [ ] Lectura previa del material
- [ ] Preparar hardware/demos si es necesario
- [ ] Imprimir o compartir digitalmente ejercicios
- [ ] Quiz o evaluación formativa lista
- [ ] Material de próxima sesión anunciado

### Al Finalizar la Unidad

- [ ] Evaluar sumativa con rúbrica
- [ ] Recopilar feedback de estudiantes
- [ ] Documentar ajustes necesarios para próximo semestre
- [ ] Preparar introducción a Unidad 2

---

**Última actualización**: Noviembre 2025  
**Versión**: 1.0  
**Autor**: Material del curso de Microprocesadores

¡Bienvenido a la Unidad 1! 🚀
