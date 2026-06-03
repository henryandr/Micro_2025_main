# Bibliografía Anotada y Enlaces - Unidad 2

Recursos organizados por categoría para profundizar en los temas de la unidad.

---

## Documentación Oficial

### STMicroelectronics

1. **RM0090 Reference Manual - STM32F4xx**
   - URL: https://www.st.com/resource/en/reference_manual/dm00031020.pdf
   - Páginas clave: 
     - GPIO: 265-290
     - TIM2-5: 587-650
     - EXTI: 375-385
   - Uso: Referencia principal para registros y configuración

2. **PM0214 Programming Manual - Cortex-M4**
   - URL: https://www.st.com/resource/en/programming_manual/dm00046982.pdf
   - Contenido: Set de instrucciones, registros core, NVIC
   - Uso: Complemento para entender arquitectura subyacente

3. **DS8626 Datasheet - STM32F407VE**
   - URL: https://www.st.com/resource/en/datasheet/stm32f407ve.pdf
   - Contenido: Características eléctricas, pinout, especificaciones
   - Uso: Consulta de pines y características físicas

### ARM

4. **ARMv7-M Architecture Reference Manual**
   - URL: https://developer.arm.com/documentation/ddi0403/latest/
   - Contenido: Arquitectura completa, excepciones, memoria
   - Uso: Comprensión profunda de arquitectura

5. **Cortex-M4 Technical Reference Manual**
   - URL: https://developer.arm.com/documentation/100166/latest/
   - Contenido: Pipeline, MPU, instrucciones, timing
   - Uso: Optimización y debugging avanzado

---

## Libros Recomendados

### Nivel Básico-Intermedio

6. **"The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors"**
   - Autor: Joseph Yiu
   - Editorial: Newnes, 2013
   - ISBN: 978-0124080829
   - Capítulos clave: 3, 5, 7, 9, 10
   - Qué aporta: Excelente introducción a Cortex-M, ejemplos prácticos
   - Nivel: Perfecto para este curso

7. **"Embedded Systems: ARM Programming and Optimization"**
   - Autor: Jason D. Bakos
   - Editorial: Morgan Kaufmann, 2015
   - ISBN: 978-0128015070
   - Capítulos clave: 1-4, 7
   - Qué aporta: Enfoque en optimización y eficiencia

### Nivel Avanzado

8. **"Real-Time C++: Efficient Object-Oriented Programming"**
   - Autor: Christopher Kormanyos
   - Editorial: Springer, 2021
   - ISBN: 978-3662629956
   - Uso: Para transición a C++ en sistemas embebidos
   - Nivel: Avanzado, post-curso

9. **"Making Embedded Systems"**
   - Autor: Elecia White
   - Editorial: O'Reilly, 2011
   - ISBN: 978-1449302146
   - Qué aporta: Diseño de sistemas, arquitectura, best practices
   - Nivel: Intermedio-Avanzado

---

## Tutoriales y Cursos Online

### Plataformas de Aprendizaje

10. **ARM Education Media - Cortex-M**
    - URL: https://www.arm.com/resources/education/education-kits
    - Contenido: Slides, labs, ejemplos
    - Costo: Gratuito (registro requerido)
    - Uso: Material complementario para profesores

11. **Coursera: "Real-Time Embedded Systems"**
    - URL: coursera.org/specializations/real-time-embedded-systems
    - Universidad: University of Colorado Boulder
    - Duración: 4 cursos
    - Nivel: Intermedio
    - Uso: Profundización en RTOS

### YouTube Channels

12. **Phil's Lab**
    - URL: https://www.youtube.com/@PhilsLab
    - Contenido: STM32, diseño PCB, proyectos prácticos
    - Calidad: Excelente
    - Videos clave: "STM32 Bare Metal Programming"

13. **Mitch Davis - Low Level Learning**
    - URL: https://www.youtube.com/@LowLevelLearning
    - Contenido: C, sistemas embebidos, conceptos low-level
    - Nivel: Básico-Intermedio

---

## Herramientas y Software

### Compiladores y Toolchains

14. **ARM GNU Toolchain**
    - URL: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
    - Componentes: gcc, gdb, binutils para ARM
    - Versión recomendada: 12.2 o superior
    - Uso: Compilación de ejemplos del curso

15. **STM32CubeIDE**
    - URL: https://www.st.com/en/development-tools/stm32cubeide.html
    - Características: Eclipse-based, debugger integrado
    - Costo: Gratuito
    - Uso: Alternativa con GUI completa

### Debuggers y Análisis

16. **OpenOCD**
    - URL: http://openocd.org/
    - Uso: Debug con ST-Link, J-Link
    - Ventaja: Open source, multiplataforma

17. **Segger J-Link Tools**
    - URL: https://www.segger.com/products/debug-probes/j-link/
    - Herramientas: GDB Server, RTT Viewer, SystemView
    - Versión EDU: Gratuita para educación

---

## Artículos y Papers

### Conceptos Fundamentales

18. **"State Machines in C"**
    - Autor: Miro Samek
    - URL: https://barrgroup.com/embedded-systems/how-to/state-machines-event-driven-systems
    - Tema: Implementación eficiente de MEF
    - Relevancia: Directa para Sesiones 2-3

19. **"Volatile: Almost Useless for Multi-Threaded Programming"**
    - Autor: Gavin Clarke (Embedded Systems Programming)
    - Tema: Uso correcto de volatile
    - Relevancia: Sesión 1, conceptos de C

20. **"A Tutorial on Implementing Debounce in Software"**
    - Autor: Jack Ganssle
    - URL: http://www.ganssle.com/debouncing.htm
    - Tema: Técnicas de debouncing
    - Relevancia: Sesión 3

---

## Comunidades y Foros

21. **Stack Overflow - [embedded] tag**
    - URL: https://stackoverflow.com/questions/tagged/embedded
    - Uso: Resolución de problemas específicos

22. **Reddit - r/embedded**
    - URL: https://www.reddit.com/r/embedded/
    - Uso: Discusiones, proyectos, noticias

23. **STM32 Community Forum**
    - URL: https://community.st.com/
    - Uso: Problemas específicos de STM32

24. **Electrical Engineering Stack Exchange**
    - URL: https://electronics.stackexchange.com/
    - Uso: Dudas de hardware y electrónica

---

## Blogs Técnicos

25. **Interrupt - Memfault Blog**
    - URL: https://interrupt.memfault.com/
    - Contenido: Debugging, optimización, best practices
    - Calidad: Muy alta

26. **Embedded Artistry**
    - URL: https://embeddedartistry.com/
    - Contenido: Patrones de diseño, arquitectura
    - Nivel: Intermedio-Avanzado

27. **Barr Group - Embedded Systems Experts**
    - URL: https://barrgroup.com/embedded-systems
    - Contenido: Coding standards, best practices
    - Uso: Referencias de calidad

---

## Estándares de Codificación

28. **MISRA C:2012**
    - Organización: Motor Industry Software Reliability Association
    - Uso: Reglas para C seguro en embebidos
    - Acceso: Compra requerida (versión resumida gratuita)

29. **BARR-C:2018**
    - URL: https://barrgroup.com/embedded-systems/books/embedded-c-coding-standard
    - Uso: Estándar de código gratuito, más permisivo que MISRA
    - Nivel: Aplicable a este curso

---

## Recursos en Español

30. **"Sistemas Embebidos en ARM Cortex-M"**
    - Autor: Varios (UBA - Universidad de Buenos Aires)
    - URL: Buscar en repositorios académicos
    - Contenido: Material de curso similar

31. **Foro Todopic - Electrónica**
    - URL: https://www.todopic.com.ar/
    - Comunidad: Hispanohablante
    - Uso: Consultas y proyectos

---

## Notas de Uso

**Para estudiantes**:
- Comenzar con documentos marcados como "Nivel Básico"
- Datasheet y Reference Manual son indispensables
- Videos de YouTube son excelente complemento

**Para profundizar**:
- Leer capítulos sugeridos de libros
- Seguir blogs técnicos
- Participar en foros

**Para proyectos propios**:
- Usar herramientas listadas
- Consultar estándares de código
- Revisar ejemplos en comunidades

**Actualización**: Esta bibliografía se actualiza semestralmente. Última revisión: 2025
