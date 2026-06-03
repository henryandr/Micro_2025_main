# Depuración de Contenidos - Unidad 3

## Contexto

Esta unidad es **completamente nueva** en el curso de Microprocesadores. No existía material previo específico sobre protocolos de comunicación serial (USART, I2C, SPI, CAN) a nivel bare metal.

## Análisis de Materiales Previos

### Lo que existía antes
- **Unidad 1**: Arquitectura ARM Cortex M-4, programación en Assembly
- **Unidad 2**: Programación en C bare metal, GPIO, Timer, MEF, interrupciones básicas
- **Material disperso**: Menciones superficiales a USART como ejemplo de periférico

### Lo que faltaba
- Explicación sistemática de protocolos de comunicación
- Configuración detallada de USART/UART
- Protocolo I2C y su implementación
- Protocolo SPI y aplicaciones
- Protocolo CAN para redes de microcontroladores
- Drivers modulares para comunicaciones
- Integración con periféricos externos reales

## Decisión de Creación de Unidad 3

Esta unidad fue creada desde cero por las siguientes razones:

1. **Vacío curricular identificado**: Los estudiantes terminaban Unidad 2 con conocimientos de GPIO y Timer, pero sin capacidad de comunicarse con el mundo exterior

2. **Demanda de la industria**: 
   - I2C es ubicuo en sensores y memorias
   - SPI es estándar para displays y almacenamiento
   - USART es esencial para debugging
   - CAN es crítico en automotriz, robótica e IoT industrial

3. **Progresión pedagógica lógica**:
   - Unidad 1: Fundamentos internos (CPU, registros, Assembly)
   - Unidad 2: Periféricos internos (GPIO, Timer, interrupciones)
   - **Unidad 3**: Comunicación externa (protocolos seriales)
   - Unidad 4 (futura): Sistemas operativos (RTOS, multitasking)

4. **Integración práctica**: Permite trabajar con hardware real (sensores, displays, redes), no solo LEDs

## Contenidos Eliminados

**Ninguno** - Esta es una unidad completamente nueva sin material previo que eliminar.

## Contenidos Conservados

**N/A** - No hay material previo específico de esta unidad.

## Justificación de la Estructura de 8 Sesiones

### Semana 1: USART (2 sesiones)
- **Razón**: USART es el más simple y útil para debugging
- **Sesión 1**: Fundamentos y polling (base necesaria)
- **Sesión 2**: Interrupciones y printf redirect (herramienta para resto del curso)

### Semana 2: I2C (2 sesiones)
- **Razón**: Protocolo muy común, pero más complejo que USART
- **Sesión 3**: Fundamentos del bus, configuración (teoría necesaria)
- **Sesión 4**: Aplicaciones prácticas con EEPROM y OLED (motivación)

### Semana 3: SPI y CAN (2 sesiones)
- **Razón**: Protocolos más especializados, menor tiempo necesario
- **Sesión 5**: SPI (más rápido pero conceptualmente más simple que I2C)
- **Sesión 6**: CAN (introducción, no exhaustivo por complejidad)

### Semana 4: Integración (2 sesiones)
- **Razón**: Aplicación real de todos los conocimientos
- **Sesión 7**: Integración multi-protocolo y debugging avanzado
- **Sesión 8**: Proyecto final y evaluación sumativa

## Alineación con Unidades Previas

### Conocimientos Asumidos de Unidad 1
- Arquitectura ARM Cortex M-4
- Registros y manipulación a nivel de bits
- Concepto de direcciones de memoria
- Interrupciones (concepto básico)

### Conocimientos Asumidos de Unidad 2
- Programación en C para embebidos
- Uso de `volatile` para registros
- Estructuras y punteros
- Desarrollo de drivers modulares
- Timer para bases de tiempo
- Manejo de interrupciones en C

### Nuevos Conocimientos en Unidad 3
- Protocolos de comunicación serial
- Configuración de baudrate y timing
- Manejo de buses compartidos (I2C, CAN)
- Sincronización y arbitraje
- Debugging de comunicaciones
- Integración de múltiples periféricos externos

## Enfoque Pedagógico

### Metodología Aplicada

1. **Progresión incremental**: De simple (USART) a complejo (CAN)

2. **Práctica inmediata**: Cada concepto teórico se practica inmediatamente

3. **Herramientas para debugging**: Énfasis en usar USART para depurar otros protocolos

4. **Integración progresiva**: Cada protocolo se integra con los anteriores

5. **Proyecto real**: El proyecto final combina todos los protocolos

### Diferenciación con HAL/Frameworks

Esta unidad deliberadamente evita HAL (Hardware Abstraction Layer) de ST para:

- Entender profundamente el funcionamiento de cada protocolo
- Desarrollar capacidad de leer datasheets
- Crear drivers optimizados para necesidades específicas
- Preparar para trabajar con cualquier microcontrolador, no solo STM32

## Supuestos Técnicos Clave

### Hardware
- STM32F407 Discovery (o equivalente con Cortex M-4)
- Periféricos I2C: EEPROM, sensor, OLED
- Periféricos SPI: Sensor o display
- Transceiver CAN (MCP2551 o similar)
- Conversor USB-Serial (para USART)

### Software
- arm-none-eabi-gcc toolchain
- OpenOCD o ST-Link utilities
- Terminal serial (PuTTY, minicom)
- (Opcional) Logic analyzer software

### Prerequisitos de Estudiantes
- Conocimientos sólidos de C
- Experiencia con GPIO y Timer (Unidad 2)
- Capacidad de leer datasheets técnicos
- Conocimientos básicos de electricidad digital

## Desafíos Identificados

### Desafío 1: Acceso a Hardware
**Problema**: No todos los estudiantes tienen todos los periféricos

**Soluciones aplicadas**:
- Énfasis en lectura de datasheet y configuración de registros
- Simulación cuando sea posible
- Trabajo en equipos para compartir hardware
- Proyectos alternativos sin hardware específico

### Desafío 2: Debugging de Protocolos Invisibles
**Problema**: Las señales I2C/SPI/CAN no se ven sin herramientas

**Soluciones aplicadas**:
- Énfasis en logging por USART
- Introducción a logic analyzer (software libre disponible)
- Uso de LEDs de estado
- Verificación incremental paso a paso

### Desafío 3: Complejidad de 4 Protocolos en 4 Semanas
**Problema**: Mucho contenido, poco tiempo

**Soluciones aplicadas**:
- Profundidad variable: USART e I2C detallados, SPI y CAN introductorios
- Entregables incrementales, no todo perfecto
- Documentación extensa para estudio posterior
- Recursos adicionales para profundización autónoma

### Desafío 4: Cálculos de Timing
**Problema**: Baudrate, prescalers, etc. pueden ser confusos

**Soluciones aplicadas**:
- Ejemplos numéricos detallados en cada sesión
- Fórmulas claramente documentadas
- Valores pre-calculados para casos comunes
- Herramientas online sugeridas

## Evaluación y Mejora Continua

### Mecanismos de Feedback

1. **Quizzes formativos**: En cada sesión, detectar brechas inmediatamente
2. **Entregables incrementales**: Verificar progreso semanal
3. **Proyecto final**: Evaluación integradora
4. **Encuesta post-unidad**: Recoger feedback de estudiantes

### Métricas de Éxito

- **Tasa de aprobación**: >80% de estudiantes completan proyecto final
- **Funcionalidad**: >90% de proyectos tienen al menos 3 de 4 protocolos funcionando
- **Satisfacción**: >4/5 en encuesta de satisfacción de la unidad
- **Aplicación**: Estudiantes usan drivers en proyectos posteriores

### Iteración Futura

Esta es la versión 1.0 de Unidad 3. Se planea iterar basándose en:
- Feedback de estudiantes e instructores
- Disponibilidad de hardware
- Evolución de la industria (nuevos protocolos, estándares)
- Balance de profundidad vs amplitud

## Recursos Complementarios Sugeridos

### Para el Instructor
- Slides de presentación (a crear)
- Videos de demos de hardware
- Banco de preguntas adicionales
- Proyectos ejemplo resueltos

### Para el Estudiante
- Datasheets comentados
- Videos tutoriales de terceros (YouTube)
- Foros y comunidades online
- Simuladores (Proteus, Renode)

## Conclusión

La Unidad 3 llena un vacío crítico en el curso de Microprocesadores al proporcionar conocimientos prácticos y aplicables sobre protocolos de comunicación serial. Su creación desde cero permite una estructura pedagógica óptima, progresión lógica y alineación perfecta con las unidades previas.

El enfoque bare metal prepara a los estudiantes para trabajar con cualquier microcontrolador y entender profundamente cómo funcionan las comunicaciones a bajo nivel, una habilidad valiosa en la industria de sistemas embebidos.

---

**Versión**: 1.0  
**Fecha de Creación**: Noviembre 2025  
**Próxima Revisión**: Después de primer ciclo de impartición
