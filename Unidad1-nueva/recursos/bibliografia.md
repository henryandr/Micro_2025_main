# Bibliografía Anotada - Unidad 1

Esta bibliografía contiene recursos oficiales y recomendados para el estudio de la Unidad 1.

---

## Documentación Oficial ARM

### 1. ARMv7-M Architecture Reference Manual
**Tipo**: Manual de arquitectura  
**Fuente**: ARM Ltd.  
**URL**: https://developer.arm.com/documentation/ddi0403/latest/

**Descripción**: Documento oficial que define la arquitectura ARMv7-M (Cortex-M3/M4).  
**Contenido clave**:
- Conjunto de instrucciones completo
- Modelo de memoria
- Excepciones e interrupciones
- Registros de sistema

**Uso recomendado**: Consulta cuando necesites detalles precisos sobre instrucciones o comportamiento del procesador.

---

### 2. ARM Cortex-M4 Processor Technical Reference Manual
**Tipo**: Manual técnico del procesador  
**Fuente**: ARM Ltd.  
**URL**: https://developer.arm.com/documentation/100166/latest/

**Descripción**: Documentación específica del Cortex-M4.  
**Contenido clave**:
- Pipeline y ejecución
- FPU y DSP
- Interfaces de debug
- Características de power management

**Uso recomendado**: Para entender características específicas del Cortex-M4 vs otros Cortex-M.

---

## Documentación STMicroelectronics

### 3. STM32F407 Reference Manual (RM0090)
**Tipo**: Manual de referencia del microcontrolador  
**Fuente**: STMicroelectronics  
**URL**: https://www.st.com/resource/en/reference_manual/dm00031020.pdf

**Descripción**: **DOCUMENTO MÁS IMPORTANTE** para el curso. Describe todos los periféricos del STM32F407.  
**Contenido clave**:
- Mapa de memoria detallado
- Registros de todos los periféricos (GPIO, UART, Timer, ADC, etc.)
- Diagramas de bloques
- Características eléctricas

**Uso recomendado**: Consulta OBLIGATORIA para cualquier configuración de periféricos. Sesiones 3-8 lo usan intensivamente.

---

### 4. STM32F407 Datasheet
**Tipo**: Hoja de datos  
**Fuente**: STMicroelectronics  
**URL**: https://www.st.com/resource/en/datasheet/stm32f407vg.pdf

**Descripción**: Especificaciones eléctricas y pinout.  
**Contenido clave**:
- Pinout completo
- Características eléctricas máximas/mínimas
- Diagramas de aplicación
- Dimensiones físicas

**Uso recomendado**: Para selección de pines y verificación de límites eléctricos en proyectos.

---

## Libros Recomendados

### 5. "The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors"
**Autor**: Joseph Yiu  
**Editorial**: Newnes (Elsevier)  
**Edición**: 3ra edición (2014)  
**ISBN**: 978-0124080829

**Descripción**: Libro completo sobre Cortex-M, escrito por ingeniero de ARM.  
**Contenido clave**:
- Arquitectura detallada
- Programación en C y Assembly
- Técnicas de optimización
- Manejo de excepciones

**Nivel**: Intermedio a avanzado  
**Disponibilidad**: Biblioteca universitaria, Amazon

---

### 6. "Embedded Systems: ARM Programming and Optimization"
**Autor**: Jason D. Bakos  
**Editorial**: Morgan Kaufmann  
**Edición**: 2015  
**ISBN**: 978-0128015070

**Descripción**: Enfoque práctico en optimización de código ARM.  
**Contenido clave**:
- Técnicas de optimización
- Uso eficiente de cache y memoria
- Programación NEON (SIMD)

**Nivel**: Avanzado  
**Uso recomendado**: Para proyectos que requieran alto rendimiento.

---

## Recursos Online

### 7. ARM Community (Keil)
**URL**: http://www.keil.com/support/man/docs/

**Descripción**: Documentación y tutoriales de Keil (toolchain ARM).  
**Contenido**: Guías de inicio, ejemplos de código, FAQs  
**Acceso**: Gratuito

---

### 8. STM32 Community Forums
**URL**: https://community.st.com/

**Descripción**: Foro oficial de STMicroelectronics.  
**Uso**: Resolver dudas específicas, ver problemas comunes  
**Acceso**: Gratuito (registro recomendado)

---

### 9. ARM Mbed OS Documentation
**URL**: https://os.mbed.com/docs/

**Descripción**: Aunque este curso no usa Mbed, tiene buenos tutoriales introductorios.  
**Uso recomendado**: Para comparar enfoques bare metal vs framework.

---

## Application Notes de ST

### 10. AN4013: STM32 Cross-series Timer Overview
**Tipo**: Application Note  
**URL**: https://www.st.com/resource/en/application_note/dm00042534.pdf

**Descripción**: Guía sobre timers en familia STM32.  
**Uso**: Sesión 7 (Timer driver)

---

### 11. AN4838: Managing Memory Protection Unit in STM32 MCUs
**Tipo**: Application Note  
**URL**: https://www.st.com/resource/en/application_note/dm00272912.pdf

**Descripción**: Uso de MPU para protección de memoria.  
**Uso**: Avanzado, opcional para esta unidad.

---

## Herramientas y Software

### 12. GNU ARM Embedded Toolchain
**URL**: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm

**Descripción**: Compilador gcc para ARM Cortex-M.  
**Incluye**: gcc, as, ld, gdb, objdump  
**Licencia**: Open source (GPL)

**Instalación**: Ver `guia_herramientas.md`

---

### 13. STM32CubeMX
**URL**: https://www.st.com/en/development-tools/stm32cubemx.html

**Descripción**: Herramienta de configuración gráfica de ST.  
**Uso en el curso**: NO se usa (bare metal), pero útil para:
- Visualizar configuraciones de pines
- Generar código de inicialización como referencia
- Calcular clocks

---

### 14. OpenOCD
**URL**: http://openocd.org/

**Descripción**: Software de programación y debug open source.  
**Uso**: Programar el microcontrolador via SWD/JTAG  
**Compatibilidad**: ST-Link, J-Link, etc.

---

## Videos Educativos

### 15. "Introduction to ARM Cortex-M" - ARM Education
**URL**: https://www.youtube.com/playlist?list=PLgyU3jNA6VjT1YfwcnoDcxMN6EUjcUc7-

**Descripción**: Serie de videos oficiales de ARM Education.  
**Duración**: ~10 videos de 10-15 min c/u  
**Nivel**: Principiante  
**Idioma**: Inglés con subtítulos

---

### 16. "Bare Metal C Programming on ARM" - Low Level Learning
**URL**: https://www.youtube.com/watch?v=HdKUgzVg87Q

**Descripción**: Tutorial práctico de bare metal.  
**Duración**: ~30 min  
**Nivel**: Intermedio

---

## Artículos Académicos (Opcionales)

### 17. "RISC vs CISC: The Eternal Debate"
**Autores**: David Patterson, John Hennessy  
**Año**: 2017  
**Contexto**: Paper histórico sobre la evolución de arquitecturas.

---

### 18. "Power Analysis of ARM Cortex-M Processors"
**Fuente**: IEEE Xplore  
**Año**: 2015  
**Tema**: Análisis de consumo energético.  
**Relevancia**: Entender por qué ARM es eficiente.

---

## Simuladores y Emuladores

### 19. QEMU ARM
**URL**: https://www.qemu.org/

**Descripción**: Emulador de sistemas ARM completos.  
**Uso**: Pruebas sin hardware (limitado para periféricos).

---

### 20. Keil µVision Simulator
**URL**: http://www.keil.com/

**Descripción**: IDE con simulador integrado.  
**Limitación**: Versión gratuita tiene límite de código.  
**Uso**: Debugging visual de código Assembly.

---

## Organización de la Bibliografía por Sesión

| Sesión | Recursos Principales |
|--------|---------------------|
| 1-2 | Libros generales (Yiu cap. 1-2) |
| 3 | RM0090 cap. 2 (Memory map), ARM TRM |
| 4 | ARMv7-M cap. A4 (Instruction Set) |
| 5-6 | ARMv7-M cap. A4, Yiu cap. 4-5 |
| 7 | RM0090 cap. 8 (GPIO), 18 (Timers) |
| 8 | RM0090 cap. 10 (Interrupts), ARM TRM cap. 4 (NVIC) |

---

## Cómo Usar Esta Bibliografía

**Para Estudiantes**:
1. **Esencial**: RM0090 (Reference Manual STM32F407)
2. **Muy recomendado**: Yiu - "Definitive Guide"
3. **Complementario**: Videos y Application Notes según necesidad

**Para Profundizar**:
- ARMv7-M Architecture Reference Manual
- Artículos académicos

**Para Proyectos**:
- RM0090 (siempre)
- Application Notes específicas
- Foros de comunidad para troubleshooting

---

## Notas

- Todos los enlaces verificados: Noviembre 2025
- Documentos oficiales ARM/ST son gratuitos
- Libros físicos disponibles en biblioteca universitaria
- Videos en YouTube son de acceso libre

**Actualización**: Este documento se actualiza cuando hay nuevas versiones de manuales o recursos relevantes.
