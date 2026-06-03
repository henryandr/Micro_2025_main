# Unidad 2: Máquinas de Estados Finitos y Programación en C Bare Metal

## Mapa de Navegación

### 📚 Documentos Principales

- **[README.md](README.md)** - Este documento (mapa de navegación y visión general)
- **[supuestos.md](supuestos.md)** - Supuestos técnicos y conocimientos previos
- **[depuracion_contenidos.md](depuracion_contenidos.md)** - Contenidos eliminados/modificados de Unidad2 original
- **[ampliaciones.md](ampliaciones.md)** - Contenidos añadidos y expandidos
- **[proyeccion_unidad3.md](proyeccion_unidad3.md)** - Recomendaciones para Unidad 3

### 🎯 Sesiones de Clase

La unidad está organizada en **8 sesiones** de 2 horas cada una, distribuidas en 4 semanas:

| Semana | Sesión | Tema Principal | Archivo |
|--------|--------|----------------|---------|
| 1 | 1 | Transición Assembly-C y Lenguaje C para Embebidos | [sesion1.md](sesiones/sesion1.md) |
| 1 | 2 | Máquinas de Estados Finitos - Teoría y Diseño | [sesion2.md](sesiones/sesion2.md) |
| 2 | 3 | Implementación de MEF en C | [sesion3.md](sesiones/sesion3.md) |
| 2 | 4 | Driver GPIO Completo en C | [sesion4.md](sesiones/sesion4.md) |
| 3 | 5 | Configuración de Timer (TIM2) en C | [sesion5.md](sesiones/sesion5.md) |
| 3 | 6 | Multiplexación y Aplicaciones | [sesion6.md](sesiones/sesion6.md) |
| 4 | 7 | Interrupciones (EXTI y NVIC) en C | [sesion7.md](sesiones/sesion7.md) |
| 4 | 8 | Proyecto Integrador y Evaluación Sumativa | [sesion8.md](sesiones/sesion8.md) |

### 💻 Código y Ejemplos

#### Ejemplos en C
- **[ejemplos_c/](ejemplos_c/)** - Código C bare metal comentado y funcional
  - `main_blink.c` - Primer programa en C (blink LED)
  - `mef_simple.c` - Ejemplo simple de MEF
  - `mef_debouncing.c` - MEF para antirrebote de botón
  - `mef_semaforo.c` - Semáforo con MEF
  - `timer_config.c` - Configuración de Timer
  - `multiplexacion_display.c` - Multiplexación de displays
  - `interrupciones_exti.c` - Ejemplo de interrupciones externas
  - Y más...

#### Drivers en C
- **[drivers_moduloxx/](drivers_moduloxx/)** - Drivers bare metal modulares
  - `gpio_driver.c/h` - Driver GPIO completo
  - `timer_driver.c/h` - Driver Timer (TIM2)
  - `uart_driver.c/h` - Driver UART básico (opcional)
  - `irq_handler.c/h` - Manejo de interrupciones

#### Infraestructura del Proyecto
- **[startup_stm32f407.s](ejemplos_c/startup_stm32f407.s)** - Código de inicio
- **[linker_script.ld](ejemplos_c/linker_script.ld)** - Script de enlazado
- **[Makefile](ejemplos_c/Makefile)** - Compilación automatizada
- **[stm32f407xx.h](ejemplos_c/stm32f407xx.h)** - Definiciones de registros

### 📝 Evaluaciones

- **[evaluaciones/](evaluaciones/)** - Instrumentos de evaluación completos
  - `diagnostica.md` - Evaluación diagnóstica (Sesión 1)
  - `quizzes_formativos.md` - Quizzes por sesión (Sesiones 1-8)
  - `sumativa.md` - Evaluación final (Sesión 8)
  - `rubricas.md` - Rúbricas de evaluación detalladas

### 📖 Recursos de Apoyo

- **[recursos/](recursos/)** - Material complementario
  - `bibliografia.md` - Bibliografía anotada y enlaces
  - `glosario.md` - Glosario de términos técnicos
  - `comparacion_assembly_c.md` - Comparación de conceptos
  - `guia_debugging.md` - Guía de debugging en C

## Visión General de la Unidad

### Objetivo General

Desarrollar competencias en programación de microcontroladores ARM Cortex M-4 en lenguaje C bare metal, aplicando Máquinas de Estados Finitos para diseño de sistemas reactivos, y configurando periféricos (GPIO, Timer, interrupciones) mediante acceso directo a registros.

### Objetivos Específicos

Al completar esta unidad, el estudiante será capaz de:

1. **Programar** en C bare metal para microcontroladores ARM Cortex M-4
2. **Diseñar** Máquinas de Estados Finitos (MEF) para sistemas reactivos
3. **Implementar** MEF en C de forma estructurada y mantenible
4. **Desarrollar** drivers completos en C para periféricos (GPIO, Timer)
5. **Configurar** interrupciones externas (EXTI) y NVIC en C
6. **Aplicar** técnicas de multiplexación para optimizar uso de pines
7. **Integrar** múltiples periféricos en proyectos funcionales
8. **Depurar** código C a nivel bare metal

### Distribución Temporal

- **Duración Total**: 4 semanas
- **Sesiones**: 8 sesiones de 2 horas (16 horas totales)
- **Trabajo Autónomo**: 24 horas estimadas (lectura, ejercicios, proyecto)
- **Evaluaciones**: 4 horas (diagnóstica + formativas + sumativa)

### Metodología

- **30% Teoría**: Explicación de conceptos (MEF, C bare metal, periféricos)
- **60% Práctica**: Ejercicios guiados, laboratorios, proyecto
- **10% Evaluación**: Diagnóstica, formativas y sumativa

### Competencias Desarrolladas

1. **Transición Assembly-C**: Aplicar conocimientos de Assembly en programación C
2. **Diseño de Sistemas**: Usar MEF para diseñar sistemas reactivos robustos
3. **Programación Modular**: Crear drivers y código reutilizable en C
4. **Configuración de Hardware**: Periféricos mediante registros en C
5. **Manejo de Eventos**: Interrupciones y sincronización

## Requisitos Previos

### Conocimientos Requeridos (de Unidad 1)

- ✅ Arquitectura ARM Cortex M-4
- ✅ Programación en Assembly ARM
- ✅ Manipulación de registros y bits
- ✅ Configuración básica de GPIO en Assembly
- ✅ Conceptos de interrupciones y NVIC
- ✅ Lectura de datasheets

### Conocimientos Requeridos (generales)

- ✅ Programación básica en C (variables, funciones, estructuras de control)
- ✅ Sistemas numéricos (binario, hexadecimal)
- ✅ Conceptos de punteros en C

### Conocimientos NO Requeridos (se enseñan en el curso)

- ❌ C bare metal (se enseña desde cero)
- ❌ Máquinas de Estados Finitos
- ❌ Desarrollo de drivers en C
- ❌ Multiplexación

### Hardware Necesario

- Tarjeta de desarrollo STM32F407VET6 Black Board
- Cable USB para programación
- LEDs externos (8 unidades recomendadas)
- Resistencias (220Ω - 1kΩ)
- Botones pulsadores (2-3 unidades)
- Protoboard y cables jumper
- Opcional: Display 7 segmentos, teclado matricial

### Software Necesario

- arm-none-eabi-gcc (toolchain)
- Editor de texto o IDE (recomendado: VS Code con extensiones C/C++)
- OpenOCD o ST-Link Utility
- Opcional: arm-none-eabi-gdb, ST-Link debugger

## Estructura de Cada Sesión

Cada sesión sigue el siguiente formato:

1. **Introducción y Repaso** (10 min)
2. **Objetivos Específicos** (2-3 objetivos medibles)
3. **Contenidos Temáticos** (teoría y ejemplos)
4. **Actividades Prácticas** (mínimo 2 por sesión)
5. **Evaluación Formativa** (quiz o revisión de código)
6. **Material para Casa** (lectura previa a siguiente sesión)
7. **Evidencias de Aprendizaje** (qué debe entregar el estudiante)

## Sistema de Evaluación

### Distribución de Calificaciones

| Componente | Porcentaje | Descripción |
|-----------|------------|-------------|
| Diagnóstica | 5% | Evaluación inicial (repaso Unidad 1) |
| Formativas | 30% | Quizzes y ejercicios (8 sesiones) |
| Proyecto Incremental | 35% | Evidencias por sesión + avance proyecto |
| Sumativa Final | 30% | Proyecto integrador + sustentación |

### Criterios de Aprobación

- **Mínimo**: 60% de calificación global
- **Asistencia**: Mínimo 80% (6 de 8 sesiones)
- **Proyecto Final**: Obligatorio entregar (mínimo 50% en evaluación sumativa)

## Ruta de Aprendizaje

```
Semana 1: TRANSICIÓN Y FUNDAMENTOS
├─ Sesión 1: C bare metal - estructura de proyectos
└─ Sesión 2: MEF - teoría y diseño

Semana 2: IMPLEMENTACIÓN
├─ Sesión 3: MEF en C - switch-case y tablas
└─ Sesión 4: Driver GPIO completo en C

Semana 3: TIMING Y OPTIMIZACIÓN
├─ Sesión 5: Timer para MEF - timing preciso
└─ Sesión 6: Multiplexación - optimizar pines

Semana 4: INTERRUPCIONES Y PROYECTO
├─ Sesión 7: EXTI y NVIC en C
└─ Sesión 8: Proyecto integrador - semáforo inteligente
```

## Conexión con Unidad 1

| Concepto Unidad 1 (Assembly) | Expansión en Unidad 2 (C) |
|-------------------------------|---------------------------|
| Manipulación de registros | Punteros y estructuras en C |
| Configuración GPIO básica | Driver GPIO completo y modular |
| Control de flujo | MEF para sistemas reactivos |
| Funciones en Assembly | Funciones y módulos en C |
| Conceptos de interrupciones | Implementación completa en C |
| Delays en Assembly | Timer configurado para timing preciso |

## Cómo Usar Este Material

### Para Estudiantes

1. **Antes de cada sesión**: Lee el archivo `sesionN.md` y material previo indicado
2. **Durante la sesión**: Participa activamente, programa los ejemplos, pregunta dudas
3. **Después de la sesión**: Completa las evidencias y ejercicios
4. **Proyecto**: Trabaja incrementalmente, no dejes todo para el final
5. **Consulta**: Usa el glosario, comparaciones Assembly-C, y guías de debugging

### Para Profesores

1. **Preparación**: Revisa la sesión completa y prepara hardware/software
2. **Flexibilidad**: Los tiempos son estimados, ajusta según el grupo
3. **Evaluación**: Usa las rúbricas proporcionadas para feedback consistente
4. **Adaptación**: Ejercicios opcionales para ajustar dificultad
5. **Recursos**: Material complementario en carpeta `recursos/`

## Soporte y Contacto

- **Documentación Oficial**:
  - [STM32F407 Reference Manual (RM0090)](https://www.st.com/resource/en/reference_manual/dm00031020.pdf)
  - [ARM Cortex-M4 Technical Reference Manual](https://developer.arm.com/documentation/100166/latest/)
  - [CMSIS Documentation](https://arm-software.github.io/CMSIS_5/General/html/index.html)

## Proyectos Sugeridos

### Proyecto 1: Semáforo Inteligente (Obligatorio - Sesión 8)
- MEF con 4-5 estados
- Botón peatonal con interrupción
- Timer para tiempos de estados
- LEDs para semáforo

### Proyecto 2: Control de Acceso (Opcional)
- MEF para estados (cerrado, esperando código, abierto, alarma)
- Teclado matricial con multiplexación
- Display 7 segmentos para feedback
- Buzzer para alarma

### Proyecto 3: Controlador de Display Multiplexado (Opcional)
- MEF para animaciones
- Multiplexación de 4 displays 7 segmentos
- Botones para control
- Contador o reloj simple

## Contribuciones y Mejoras

Este material está en constante mejora. Si encuentras errores o tienes sugerencias:
1. Documenta el problema claramente
2. Proporciona contexto (sesión, ejemplo, ejercicio)
3. Comparte con el instructor

---

## Checklist de Uso del Material

### Antes de Iniciar la Unidad

- [ ] Verificar que estudiantes completaron Unidad 1 con éxito
- [ ] Confirmar disponibilidad de hardware (tarjetas, LEDs, botones, etc.)
- [ ] Instalar toolchain C en laboratorio
- [ ] Probar compilación de un ejemplo
- [ ] Preparar datasheets y documentos de referencia
- [ ] Revisar todas las 8 sesiones

### Por Sesión

- [ ] Lectura previa del material
- [ ] Preparar hardware/demos necesarios
- [ ] Código de ejemplo probado y funcional
- [ ] Quiz o evaluación formativa lista
- [ ] Material de próxima sesión anunciado

### Al Finalizar la Unidad

- [ ] Evaluar sumativa con rúbrica
- [ ] Recopilar feedback de estudiantes
- [ ] Documentar ajustes necesarios
- [ ] Preparar introducción a Unidad 3 (si existe)

---

**Última actualización**: Noviembre 2025  
**Versión**: 1.0  
**Autor**: Material del curso de Microprocesadores

¡Bienvenido a la Unidad 2! 🚀
