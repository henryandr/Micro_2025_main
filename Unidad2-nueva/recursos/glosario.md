# Glosario de Términos Técnicos - Unidad 2

Definiciones de términos clave usados en el curso, organizadas alfabéticamente.

---

## A

**AHB (Advanced High-performance Bus)**: Bus de sistema de alta velocidad en arquitectura ARM usado para conectar periféricos de alta velocidad y memoria.

**API (Application Programming Interface)**: Conjunto de funciones y definiciones que permiten interactuar con un módulo de software sin conocer su implementación interna.

**APB (Advanced Peripheral Bus)**: Bus de sistema de baja velocidad para periféricos que no requieren alto rendimiento (UART, I2C, GPIO).

**ARR (Auto-Reload Register)**: Registro del timer que define el valor máximo del contador antes de generar un evento de overflow.

**Assembly/Ensamblador**: Lenguaje de programación de bajo nivel con correspondencia 1:1 con instrucciones de máquina.

**Atómico**: Operación que se ejecuta completamente sin posibilidad de interrupción, garantizando consistencia.

---

## B

**Bare Metal**: Programación directa sobre hardware sin sistema operativo ni capa de abstracción.

**Bit Banding**: Técnica de Cortex-M que permite acceso individual a bits mediante direcciones de memoria específicas.

**BSRR (Bit Set/Reset Register)**: Registro GPIO que permite establecer o resetear bits individuales de forma atómica.

**Busy-Wait**: Técnica de espera activa donde el procesador ejecuta un bucle hasta que se cumple una condición (ineficiente).

**Byte**: Unidad de información de 8 bits (valores 0-255 sin signo, -128 a 127 con signo).

---

## C

**Callback**: Función pasada como parámetro a otra función para ser llamada en cierto evento.

**Clock**: Señal periódica que sincroniza operaciones en un sistema digital.

**Compilador**: Programa que traduce código de alto nivel (C) a código máquina ejecutable.

**Cortex-M4**: Procesador ARM de 32 bits optimizado para sistemas embebidos, con FPU opcional.

**Cross-Compiler**: Compilador que genera código para una arquitectura diferente a la de la máquina host.

---

## D

**Datasheet**: Documento técnico con especificaciones eléctricas y funcionales de un componente.

**Debouncing**: Técnica para eliminar rebotes mecánicos de pulsadores mediante filtrado temporal.

**Driver**: Capa de software que abstrae el acceso a hardware, proporcionando interfaz de alto nivel.

---

## E

**Embedded System**: Sistema computacional dedicado a una función específica, integrado en un dispositivo mayor.

**enum (enumeration)**: Tipo de dato en C que define conjunto de constantes nombradas con valores enteros.

**EXTI (External Interrupt)**: Controlador de interrupciones externas que detecta cambios en pines GPIO.

---

## F

**Firmware**: Software que controla directamente hardware, típicamente almacenado en memoria no volátil.

**Flanco**: Transición de señal digital; flanco de subida (LOW→HIGH), flanco de bajada (HIGH→LOW).

**Flicker**: Parpadeo visible causado por frecuencia de refresco insuficiente en display multiplexado.

**Flash**: Memoria no volátil usada para almacenar código de programa.

**FPU (Floating Point Unit)**: Unidad de punto flotante para operaciones matemáticas en hardware.

---

## G

**GPIO (General Purpose Input/Output)**: Pin configurable como entrada o salida digital.

**Glitch**: Pulso espúreo o transición no deseada en señal digital, típicamente de corta duración.

---

## H

**HAL (Hardware Abstraction Layer)**: Capa de software que abstrae diferencias entre hardware, facilitando portabilidad.

**Handler**: Función que atiende una interrupción o excepción específica.

**Header File (.h)**: Archivo que contiene declaraciones de funciones, tipos y constantes (interfaz).

**HSI (High Speed Internal)**: Oscilador interno de 16 MHz en STM32.

---

## I

**IDR (Input Data Register)**: Registro GPIO para leer estado de pines configurados como entrada.

**Interrupción**: Evento que suspende temporalmente ejecución normal para atender situación urgente.

**IRQ (Interrupt Request)**: Solicitud de interrupción de periférico hacia procesador.

**ISR (Interrupt Service Routine)**: Función que se ejecuta en respuesta a interrupción.

---

## L

**Linker**: Programa que combina múltiples archivos objeto en ejecutable final, resolviendo referencias.

**Linker Script**: Archivo que especifica cómo organizar código y datos en memoria.

**LSB (Least Significant Bit)**: Bit menos significativo (posición 0).

---

## M

**Main Loop**: Bucle principal de programa embebido que ejecuta tareas cíclicamente.

**Máquina de Estados Finitos (MEF)**: Modelo matemático con estados discretos y transiciones definidas.

**Mealy**: Tipo de MEF donde salidas dependen del estado actual Y las entradas.

**MODER (Mode Register)**: Registro GPIO que configura modo de cada pin (entrada, salida, AF, analógico).

**Moore**: Tipo de MEF donde salidas dependen únicamente del estado actual.

**Multiplexación**: Técnica para compartir recursos (pines) entre múltiples dispositivos mediante conmutación temporal.

---

## N

**NVIC (Nested Vectored Interrupt Controller)**: Controlador de interrupciones en Cortex-M con soporte de prioridades.

---

## O

**ODR (Output Data Register)**: Registro GPIO para escribir valores en pines configurados como salida.

**Open-Drain**: Configuración de salida que solo puede conducir a GND, requiere pull-up externo.

**Overflow**: Evento generado cuando contador de timer alcanza valor máximo (ARR).

---

## P

**Periférico**: Módulo de hardware que extiende funcionalidad del procesador (GPIO, UART, Timer).

**Polling**: Técnica donde software verifica repetidamente estado de hardware (ineficiente vs interrupciones).

**Prescaler (PSC)**: Divisor de frecuencia que reduce velocidad de reloj de entrada.

**Pull-down**: Resistencia que conecta pin a GND cuando está en alta impedancia.

**Pull-up**: Resistencia que conecta pin a VDD cuando está en alta impedancia.

**Push-Pull**: Configuración de salida que puede conducir activamente a VDD o GND.

**PUPDR (Pull-Up/Pull-Down Register)**: Registro GPIO que configura resistencias internas.

---

## R

**RAM (Random Access Memory)**: Memoria volátil de acceso rápido para datos y stack.

**RCC (Reset and Clock Control)**: Periférico que controla relojes y resets de sistema.

**Reference Manual**: Documento técnico detallado con descripción completa de periféricos y registros.

**Registro**: Pequeña memoria dentro de procesador o periférico, típicamente 32 bits en Cortex-M4.

**RTOS (Real-Time Operating System)**: Sistema operativo diseñado para aplicaciones con restricciones temporales.

---

## S

**Stack**: Región de memoria organizada como LIFO (Last In, First Out) para variables locales y llamadas.

**Startup Code**: Código que inicializa sistema antes de ejecutar `main()`.

**struct**: Tipo de dato compuesto en C que agrupa variables relacionadas.

**SYSCFG (System Configuration)**: Periférico que controla configuraciones de sistema, incluido mapeo EXTI.

---

## T

**TIM (Timer)**: Periférico contador que puede generar eventos temporales e interrupciones.

**Timeout**: Condición donde tiempo de espera para evento excede límite configurado.

**Toolchain**: Conjunto de herramientas (compilador, linker, debugger) para desarrollo embebido.

**Typedef**: Palabra clave de C para crear alias de tipos existentes.

---

## U

**UART (Universal Asynchronous Receiver/Transmitter)**: Periférico de comunicación serial asíncrona.

**Update Event**: Evento generado por timer cuando contador alcanza ARR.

---

## V

**Vector Table**: Tabla de punteros a funciones ISR, ubicada al inicio de memoria Flash.

**Volatile**: Palabra clave de C que indica que variable puede cambiar sin intervención de código visible.

---

## W

**Watchdog**: Temporizador que resetea sistema si no es refrescado periódicamente (seguridad).

**Word**: Unidad de datos de tamaño natural del procesador (32 bits en Cortex-M4).

---

## Símbolos y Abreviaturas

- **&**: Operador AND bit a bit en C
- **|**: Operador OR bit a bit en C
- **^**: Operador XOR bit a bit en C
- **~**: Operador NOT bit a bit en C
- **<<**: Desplazamiento a izquierda
- **>>**: Desplazamiento a derecha
- **0x**: Prefijo para número hexadecimal
- **UL**: Sufijo para unsigned long (ej: 0x40020000UL)

---

## Convenciones de Nomenclatura

**Registros**: Mayúsculas (GPIOA, TIM2, MODER)
**Funciones**: CamelCase (GPIO_Init, Timer_Start)
**Constantes**: MAYÚSCULAS (GPIO_MODE_OUTPUT, TIEMPO_DEBOUNCE_MS)
**Variables**: snake_case (estado_actual, tiempo_en_estado)
**Tipos**: Sufijo _t (GPIO_TypeDef, Estado_t)

---

## Términos Relacionados con MEF

**Estado**: Situación o configuración específica del sistema en un momento dado.
**Transición**: Cambio de un estado a otro.
**Evento**: Ocurrencia que puede causar transición.
**Acción**: Operación ejecutada durante transición o mientras se está en estado.
**Estado Inicial**: Estado en el que inicia la MEF.
**Condición de Guarda**: Condición booleana que debe cumplirse para transición.

---

## Notas de Uso

**Para estudiantes**: Consultar este glosario cuando encuentren término desconocido.

**Formato de referencias**: Términos con **negrita** son definiciones principales.

**Actualización**: Glosario se expande conforme se avanza en el curso.
