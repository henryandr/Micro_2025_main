# Glosario de Términos - Unidad 1

Términos técnicos usados en el curso, con definición en español e inglés cuando aplique.

---

## A

**AAPCS** (ARM Architecture Procedure Call Standard)  
Estándar que define cómo se pasan parámetros entre funciones en ARM, qué registros se preservan, y cómo se usa el stack.

**ADC** (Analog-to-Digital Converter)  
Conversor Analógico-Digital. Periférico que convierte señales analógicas (voltaje) en valores digitales.

**AHB** (Advanced High-performance Bus)  
Bus de alto rendimiento en arquitectura ARM. Conecta procesador con periféricos de alta velocidad.

**ALU** (Arithmetic Logic Unit)  
Unidad Aritmético-Lógica. Componente del procesador que realiza operaciones matemáticas y lógicas.

**APB** (Advanced Peripheral Bus)  
Bus de periféricos. Versiones APB1 y APB2 operan a diferentes frecuencias.

**APSR** (Application Program Status Register)  
Registro de estado de aplicación. Contiene flags (N, Z, C, V) que indican resultado de operaciones.

**ARM** (Advanced RISC Machine / Acorn RISC Machine)  
Familia de arquitecturas de procesadores RISC, dominante en dispositivos móviles y embebidos.

**ARR** (Auto-Reload Register)  
Registro de auto-recarga de un timer. Define el valor máximo del contador.

**Assembly** (Ensamblador)  
Lenguaje de programación de bajo nivel, específico para cada arquitectura de procesador.

---

## B

**Bare Metal**  
Programación directa sobre hardware sin sistema operativo ni frameworks.

**BIC** (Bit Clear)  
Instrucción ARM que limpia bits específicos (AND con NOT).

**Branch** (Salto)  
Instrucción que cambia el flujo de ejecución del programa.

**BSRR** (Bit Set/Reset Register)  
Registro de GPIO que permite establecer o limpiar bits atómicamente.

**Bus**  
Conjunto de líneas eléctricas que transportan datos, direcciones o señales de control.

---

## C

**CISC** (Complex Instruction Set Computer)  
Arquitectura con instrucciones complejas que pueden realizar múltiples operaciones. Ejemplo: x86.

**Clock** (Reloj)  
Señal periódica que sincroniza operaciones del procesador y periféricos.

**Core** (Núcleo)  
Unidad de procesamiento central que ejecuta instrucciones.

**Cortex-M**  
Familia de procesadores ARM optimizados para microcontroladores.

**CPU** (Central Processing Unit)  
Unidad central de procesamiento. Ejecuta instrucciones de programas.

---

## D

**Datasheet** (Hoja de datos)  
Documento con especificaciones eléctricas, pinout y características de un componente.

**Debugger** (Depurador)  
Herramienta para encontrar errores en programas, permite ejecución paso a paso.

**DMA** (Direct Memory Access)  
Acceso directo a memoria. Permite transferencias sin intervención de CPU.

**Driver**  
Código que controla un periférico específico, abstrayendo detalles de hardware.

---

## E

**EPSR** (Execution Program Status Register)  
Registro que indica estado de ejecución (modo Thumb, IT block state).

**EXTI** (External Interrupt)  
Interrupción externa. Sistema para detectar cambios en pines GPIO.

---

## F

**Fetch** (Buscar)  
Primera etapa del pipeline: obtener instrucción desde memoria.

**Flag** (Bandera)  
Bit que indica condición o estado (ej: resultado cero, acarreo, desbordamiento).

**Flash**  
Memoria no volátil donde se almacena el programa.

**FPU** (Floating Point Unit)  
Unidad de punto flotante. Procesa operaciones con números decimales.

---

## G

**GPIO** (General Purpose Input/Output)  
Pines de entrada/salida de propósito general. Configurables como entrada o salida digital.

**GDB** (GNU Debugger)  
Depurador de GNU, usado para ARM con arm-none-eabi-gdb.

---

## H

**HAL** (Hardware Abstraction Layer)  
Capa de abstracción de hardware. Biblioteca que simplifica acceso a periféricos (no usada en este curso).

**Harvard Architecture** (Arquitectura Harvard)  
Arquitectura con memorias y buses separados para instrucciones y datos.

**HSI** (High Speed Internal)  
Oscilador interno de alta velocidad (16 MHz en STM32F407).

---

## I

**IDR** (Input Data Register)  
Registro de GPIO que contiene valor actual de los pines de entrada.

**Immediate** (Inmediato)  
Valor constante incluido directamente en la instrucción (ej: MOV R0, #10).

**IPSR** (Interrupt Program Status Register)  
Registro que indica qué excepción/interrupción está activa.

**IRQ** (Interrupt Request)  
Solicitud de interrupción. Señal que indica evento que requiere atención.

**ISR** (Interrupt Service Routine)  
Rutina de servicio de interrupción. Función que se ejecuta cuando ocurre interrupción.

---

## L

**LDR** (Load Register)  
Instrucción que carga datos desde memoria a registro.

**Linker Script**  
Script que define cómo se organiza el programa en memoria (Flash, RAM).

**LR** (Link Register - R14)  
Registro que almacena dirección de retorno cuando se llama una función.

**LSL** (Logical Shift Left)  
Desplazamiento lógico a la izquierda. Equivale a multiplicar por potencia de 2.

---

## M

**MCU** (Microcontroller Unit)  
Microcontrolador. Chip que integra CPU, memoria y periféricos.

**MODER** (Mode Register)  
Registro que configura modo de pines GPIO (entrada, salida, función alternativa, analógico).

**MPU** (Memory Protection Unit)  
Unidad de protección de memoria. Previene accesos no autorizados a regiones de memoria.

**MSP** (Main Stack Pointer)  
Puntero de pila principal. Usado en modo privilegiado y handler mode.

---

## N

**NVIC** (Nested Vectored Interrupt Controller)  
Controlador de interrupciones con soporte para anidación y prioridades.

---

## O

**ODR** (Output Data Register)  
Registro de GPIO donde se escribe valor para pines de salida.

**Offset**  
Desplazamiento desde una dirección base. Ej: GPIOA_MODER está en offset 0x00 desde GPIOA_BASE.

**Open-Drain**  
Tipo de salida que solo puede "tirar a tierra". Necesita resistencia pull-up externa para HIGH.

---

## P

**PC** (Program Counter - R15)  
Contador de programa. Registro que contiene dirección de próxima instrucción.

**Peripheral** (Periférico)  
Módulo funcional del microcontrolador (GPIO, UART, Timer, ADC, etc.).

**Pipeline**  
Técnica que permite procesar múltiples instrucciones simultáneamente en diferentes etapas.

**Polling**  
Técnica de verificar repetidamente el estado de un periférico (menos eficiente que interrupciones).

**Prescaler** (Pre-divisor)  
Divisor de frecuencia. Reduce frecuencia de reloj de un timer.

**PSC** (Prescaler Register)  
Registro que contiene valor del prescaler.

**PSP** (Process Stack Pointer)  
Puntero de pila de proceso. Usado en aplicaciones con sistema operativo.

**Pull-up / Pull-down**  
Resistencia que lleva un pin a HIGH (pull-up) o LOW (pull-down) cuando no está activamente controlado.

**Push-Pull**  
Tipo de salida que puede generar activamente tanto HIGH como LOW.

**PWM** (Pulse Width Modulation)  
Modulación por ancho de pulso. Técnica para controlar potencia promedio variando duty cycle.

---

## R

**RAM** (Random Access Memory)  
Memoria volátil de acceso aleatorio. Pierde contenido al apagar.

**RCC** (Reset and Clock Control)  
Periférico que controla resets y clocks de otros periféricos.

**Reference Manual**  
Manual de referencia. Documento que describe todos los registros y periféricos de un MCU.

**Register** (Registro)  
Memoria interna muy rápida del procesador, o ubicación en memoria que controla un periférico.

**RISC** (Reduced Instruction Set Computer)  
Arquitectura con instrucciones simples. Ejemplo: ARM, MIPS, RISC-V.

**ROM** (Read-Only Memory)  
Memoria de solo lectura.

---

## S

**SIMD** (Single Instruction, Multiple Data)  
Instrucción que opera sobre múltiples datos simultáneamente.

**SP** (Stack Pointer - R13)  
Puntero de pila. Indica posición actual del stack.

**SRAM** (Static RAM)  
RAM estática. Memoria volátil usada para datos y stack.

**Stack** (Pila)  
Estructura de datos LIFO (Last In, First Out) para almacenar datos temporales.

**Startup Code**  
Código de inicialización que se ejecuta antes de main(). Configura stack, copia .data, inicializa .bss.

**STR** (Store Register)  
Instrucción que almacena datos de registro en memoria.

**SysTick**  
Timer de sistema de 24 bits, parte del core ARM. Usado para OS tick o delays.

---

## T

**Thumb / Thumb-2**  
Conjunto de instrucciones ARM de 16 y 32 bits optimizado para densidad de código.

**Timer**  
Periférico contador que genera eventos periódicos o mide tiempo.

**Toolchain**  
Conjunto de herramientas de desarrollo (compilador, ensamblador, enlazador, debugger).

**TRM** (Technical Reference Manual)  
Manual técnico de referencia de un procesador.

---

## U

**UART** (Universal Asynchronous Receiver/Transmitter)  
Periférico de comunicación serial asíncrona.

**UDIV** (Unsigned Division)  
División sin signo. Instrucción ARM.

---

## V

**Vector Table**  
Tabla de vectores. Array de punteros a funciones ISR, ubicado al inicio de Flash.

**Volatile**  
Palabra clave en C que indica al compilador que un valor puede cambiar fuera del control del programa.

**Von Neumann Architecture**  
Arquitectura con memoria y bus unificados para instrucciones y datos.

---

## W

**Watchdog**  
Temporizador que resetea el sistema si no es refrescado periódicamente. Previene bloqueos.

---

## Siglas Comunes

| Sigla | Significado | Español |
|-------|-------------|---------|
| MCU | Microcontroller Unit | Microcontrolador |
| CPU | Central Processing Unit | Unidad Central de Procesamiento |
| ALU | Arithmetic Logic Unit | Unidad Aritmético-Lógica |
| GPIO | General Purpose Input/Output | Entrada/Salida de Propósito General |
| UART | Universal Asynchronous Receiver/Transmitter | Transmisor-Receptor Asíncrono Universal |
| ADC | Analog-to-Digital Converter | Conversor Analógico-Digital |
| PWM | Pulse Width Modulation | Modulación por Ancho de Pulso |
| ISR | Interrupt Service Routine | Rutina de Servicio de Interrupción |
| NVIC | Nested Vectored Interrupt Controller | Controlador de Interrupciones Vectorizado Anidado |
| FPU | Floating Point Unit | Unidad de Punto Flotante |
| DMA | Direct Memory Access | Acceso Directo a Memoria |

---

**Nota**: Este glosario se amplía según se introducen nuevos términos en unidades posteriores.

**Última actualización**: Noviembre 2025
