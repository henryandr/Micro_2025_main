# Supuestos Técnicos - Unidad 2

## Conocimientos Previos de Unidad 1

**Supuesto Principal**: Los estudiantes han completado exitosamente la Unidad 1 y poseen los siguientes conocimientos y habilidades:

### Conocimientos Sólidos Asumidos

1. **Arquitectura ARM Cortex M-4**:
   - Componentes internos (ALU, registros, buses)
   - Mapa de memoria
   - Pipeline y ejecución de instrucciones

2. **Programación en Assembly**:
   - Instrucciones aritméticas, lógicas y de control de flujo
   - Funciones y manejo de stack
   - Convenciones AAPCS
   - Manipulación directa de registros

3. **GPIO en Assembly**:
   - Configuración completa de pines (MODER, OTYPER, OSPEEDR, PUPDR)
   - Lectura y escritura usando IDR, ODR, BSRR
   - Habilitación de clocks de periféricos

4. **Interrupciones**:
   - Conceptos fundamentales
   - NVIC y sus registros
   - Vector table
   - Estructura básica de handlers

5. **Lectura de Datasheets**:
   - Capacidad de encontrar información de registros
   - Interpretación de diagramas de bits
   - Extracción de direcciones base

## Conocimientos de Programación en C

### Nivel Requerido

Se asume que los estudiantes tienen conocimientos **básicos a intermedios** de programación en C:

**Conocimientos Básicos**:
- ✅ Variables y tipos de datos (int, char, float, etc.)
- ✅ Operadores (aritméticos, lógicos, relacionales)
- ✅ Estructuras de control (if-else, while, for, switch-case)
- ✅ Funciones (declaración, definición, llamada, parámetros, return)
- ✅ Arrays básicos

**Conocimientos Intermedios**:
- ✅ Punteros básicos (declaración, dereferencia, paso por referencia)
- ✅ Estructuras (struct)
- ✅ Enumeraciones (enum)
- ⚠️ Punteros avanzados (se reforzará según necesidad)
- ⚠️ Compilación separada (se enseñará)

**NO se Asume**:
- ❌ C para sistemas embebidos (se enseña desde cero)
- ❌ Palabra clave `volatile`
- ❌ Manipulación de bits en C
- ❌ Union y bitfields
- ❌ Punteros a funciones
- ❌ Preprocesador avanzado

## Microcontrolador de Referencia

**STM32F407VET6** (Black Board) - mismo que Unidad 1

### Justificación:
- Continuidad con Unidad 1
- Conocimiento previo de GPIO y registros
- Documentación extensa
- Hardware disponible

### Especificaciones Relevantes:
- Procesador: ARM Cortex M-4 @ 168 MHz
- Flash: 512 KB
- RAM: 128 KB (+ 64 KB CCM)
- GPIOs: 5 puertos (A-E) con 16 pines cada uno
- Timers: TIM2-TIM5 (32-bit), TIM9-TIM14 (16-bit)
- NVIC: Hasta 82 interrupciones externas

## Toolchain y Entorno

### Herramientas Principales

**Compilador**: arm-none-eabi-gcc (versión 10.x o superior recomendada)

**Conjunto de herramientas**:
- `arm-none-eabi-gcc`: Compilador C/C++
- `arm-none-eabi-as`: Ensamblador (para startup code)
- `arm-none-eabi-ld`: Enlazador
- `arm-none-eabi-objcopy`: Conversión de formatos
- `arm-none-eabi-size`: Análisis de tamaño

**Programador/Debugger**:
- OpenOCD o ST-Link Utility
- Opcional: arm-none-eabi-gdb para debugging

**IDE/Editor**:
- Opción 1: VS Code con extensiones (C/C++, Cortex-Debug)
- Opción 2: Editor de texto + terminal
- NO se asume uso de IDEs completos como Keil, IAR, STM32CubeIDE

### Compilación

**Supuesto**: Los proyectos se compilan usando Makefile y comandos de terminal.

**Flags de compilación típicos**:
```bash
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
                  -O2 -Wall -Wextra -g -c archivo.c -o archivo.o
```

**Flags de enlazado típicos**:
```bash
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
                  -T linker_script.ld -Wl,-Map=output.map -o programa.elf \
                  *.o
```

## Direcciones de Memoria

Las direcciones utilizadas corresponden al STM32F407:

| Periférico | Dirección Base | Offset Registros Principales |
|-----------|----------------|------------------------------|
| RCC | 0x40023800 | AHB1ENR: +0x30, APB1ENR: +0x40 |
| GPIOA | 0x40020000 | MODER: +0x00, ODR: +0x14, IDR: +0x10, BSRR: +0x18 |
| GPIOB | 0x40020400 | (mismos offsets que GPIOA) |
| GPIOC | 0x40020800 | (mismos offsets que GPIOA) |
| GPIOD | 0x40020C00 | (mismos offsets que GPIOA) |
| GPIOE | 0x40021000 | (mismos offsets que GPIOA) |
| TIM2 | 0x40000000 | CR1: +0x00, PSC: +0x28, ARR: +0x2C, CNT: +0x24 |
| TIM3 | 0x40000400 | (mismos offsets que TIM2) |
| NVIC | 0xE000E100 | ISER: +0x00, ICER: +0x80, ISPR: +0x100, IPR: +0x300 |
| EXTI | 0x40013C00 | IMR: +0x00, EMR: +0x04, RTSR: +0x08, FTSR: +0x0C |
| SYSCFG | 0x40013800 | EXTICR1-4: +0x08 a +0x14 |

## Estructura de Proyecto C Bare Metal

### Supuestos sobre Organización de Archivos

**Estructura típica**:
```
proyecto/
├── src/
│   ├── main.c
│   ├── gpio_driver.c
│   ├── timer_driver.c
│   └── ...
├── inc/
│   ├── stm32f407xx.h
│   ├── gpio_driver.h
│   ├── timer_driver.h
│   └── ...
├── startup/
│   └── startup_stm32f407.s
├── linker_script.ld
├── Makefile
└── README.md
```

### Startup Code

**Supuesto**: Se proporciona un startup code estándar que:
- Inicializa stack pointer
- Copia .data de Flash a RAM
- Inicializa .bss a cero
- Llama a main()
- Define vector table

### Linker Script

**Supuesto**: Se proporciona un linker script que:
- Define regiones de memoria (Flash, RAM)
- Organiza secciones (.text, .data, .bss, .isr_vector)
- Define símbolos necesarios (_estack, _sdata, _edata, etc.)

## Modelo de Programación

### Estilo de Código

**Bare Metal**:
- Acceso directo a registros
- NO usar HAL de ST (salvo casos excepcionales)
- NO usar bibliotecas estándar de C (printf, malloc, etc.) a menos que se implemente redirectión

**Modularidad**:
- Drivers separados en archivos .c/.h
- Interfaz clara (funciones públicas en .h)
- Implementación privada en .c

**Nomenclatura**:
- Funciones: `periférico_acción()` (ej: `gpio_init()`, `timer_start()`)
- Constantes: MAYÚSCULAS con guión bajo (ej: `GPIO_MODE_OUTPUT`)
- Variables: minúsculas con guión bajo (ej: `current_state`)
- Tipos: PascalCase o con _t (ej: `GPIO_TypeDef` o `gpio_config_t`)

### Palabra Clave `volatile`

**Supuesto**: Se enseñará el uso correcto de `volatile` para:
- Punteros a registros de hardware
- Variables compartidas entre ISR y main
- Variables que pueden cambiar por hardware

Ejemplo:
```c
volatile uint32_t *reg = (volatile uint32_t*)0x40020000;
```

## Referencias Documentales

Se asume acceso a:

1. **STM32F407 Reference Manual (RM0090)**:
   - Capítulos relevantes: RCC, GPIO, TIM, EXTI, NVIC

2. **STM32F407 Datasheet**:
   - Pinout y funciones alternativas
   - Características eléctricas

3. **ARMv7-M Architecture Reference Manual**:
   - NVIC y excepciones
   - Modelo de programación

4. **AAPCS (ARM Architecture Procedure Call Standard)**:
   - Convenciones de llamada (ya conocidas de Unidad 1)

## Frecuencia de Reloj

**Supuesto por defecto**: 
- HSI (oscilador interno): 16 MHz (configuración inicial tras reset)
- Para aplicaciones que requieren mayor frecuencia, se configurará PLL para 168 MHz

**Cálculos de timing**:
- Todos los cálculos de delays y timers asumen 16 MHz salvo que se especifique lo contrario
- Se enseñará cómo calcular prescalers y auto-reload basándose en frecuencia de reloj

## Máquinas de Estados Finitos

### Supuestos sobre MEF

**Tipo de MEF**:
- Se enseñarán **Moore** y **Mealy**
- Énfasis en Moore por simplicidad inicial

**Implementación**:
- Estados: `enum` en C
- Transiciones: `switch-case` o tablas de transición
- Eventos: flags o variables
- Acciones: funciones

**NO se Asume**:
- Experiencia previa con MEF
- Herramientas de diseño gráfico de MEF (se usarán diagramas manuales o en papel)

## Limitaciones Conocidas

1. **Simulación**: 
   - No todos los ejemplos pueden ejecutarse en simulador
   - Hardware real es necesario para la mayoría de ejercicios

2. **Debugging**:
   - Se asume disponibilidad de ST-Link o similar para debugging
   - Printf debugging requiere configuración de UART (se cubrirá opcionalmente)

3. **Periféricos**:
   - Unidad 2 se enfoca en GPIO, Timer, EXTI, NVIC
   - Otros periféricos (UART, I2C, SPI, ADC, PWM, DMA) se reservan para Unidad 3 o cursos avanzados

4. **Sistema Operativo**:
   - NO se usa RTOS (FreeRTOS, etc.)
   - Programación bare metal pura

## Consideraciones de Seguridad

- Alimentación estándar: 3.3V
- Uso de resistencias limitadoras en LEDs
- Precaución con cortocircuitos
- Entorno controlado de laboratorio
- Supervisión del instructor en prácticas con hardware

## Extensiones Futuras

Este documento podrá actualizarse si:
- Se adopta un toolchain o IDE diferente
- Se identifican brechas de conocimiento no contempladas
- Se requiere soporte para otros microcontroladores
- Los estudiantes demuestran niveles diferentes de competencia en C

---

**Fecha de creación**: Noviembre 2025  
**Versión**: 1.0  
**Última actualización**: Noviembre 2025
