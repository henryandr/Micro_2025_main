# Supuestos Técnicos - Unidad 1

## Microcontrolador de Referencia

**Supuesto Principal**: Se asume como microcontrolador de referencia el **STM32F407VET6** (Black Board), que utiliza un procesador ARM Cortex M-4 a 168 MHz.

### Justificación:
- Es un microcontrolador ampliamente disponible en el mercado educativo
- Cuenta con documentación extensa y accesible
- Tiene características representativas de la familia Cortex M-4
- Coincide con el hardware mencionado en los materiales existentes

## Toolchain y Entorno de Desarrollo

**Supuesto**: Se utilizará el toolchain **arm-none-eabi-gcc** para compilación y enlazado.

### Herramientas Asumidas:
- **Compilador**: arm-none-eabi-gcc
- **Ensamblador**: arm-none-eabi-as
- **Enlazador**: arm-none-eabi-ld
- **Debugger**: arm-none-eabi-gdb (opcional)
- **Programador**: OpenOCD o ST-Link Utility

## Conocimientos Previos de los Estudiantes

Se asume que los estudiantes tienen:
- Conocimientos básicos de programación en C (variables, funciones, estructuras de control)
- Familiaridad con conceptos de sistemas numéricos (binario, hexadecimal)
- Comprensión básica de electricidad digital (niveles lógicos, conceptos de voltaje)
- **NO** se asume experiencia previa en Assembly
- **NO** se asume conocimiento de arquitecturas de procesadores

## Recursos de Hardware

**Supuesto**: Se asume disponibilidad de:
- Tarjeta de desarrollo STM32F407VET6 Black Board (o equivalente)
- Cable USB para programación y alimentación
- LEDs externos y resistencias para prácticas (220Ω - 1kΩ)
- Protoboard para conexiones

## Direcciones de Memoria

Las direcciones de memoria base utilizadas en los ejemplos corresponden al STM32F407:

| Periférico | Dirección Base | Referencia |
|-----------|----------------|------------|
| RCC | 0x40023800 | Reference Manual RM0090 |
| GPIOA | 0x40020000 | Reference Manual RM0090 |
| GPIOB | 0x40020400 | Reference Manual RM0090 |
| GPIOC | 0x40020800 | Reference Manual RM0090 |
| TIM2 | 0x40000000 | Reference Manual RM0090 |
| SysTick | 0xE000E010 | ARMv7-M Architecture Reference Manual |
| NVIC | 0xE000E100 | ARMv7-M Architecture Reference Manual |

## Referencias Documentales

Se asume acceso a:
1. **STM32F407 Reference Manual (RM0090)** - Descripción detallada de periféricos y registros
2. **STM32F407 Datasheet** - Características eléctricas y pinout
3. **ARMv7-M Architecture Reference Manual** - Especificación de la arquitectura ARM Cortex-M
4. **ARM Cortex-M4 Processor Technical Reference Manual** - Detalles del procesador

## Frecuencia de Reloj

**Supuesto por defecto**: El microcontrolador inicia con el oscilador interno HSI a **16 MHz**.

Para aplicaciones que requieran mayor frecuencia, se asumirá configuración del PLL para alcanzar **168 MHz** (frecuencia máxima del STM32F407).

## Modelo de Programación

Se asume desarrollo **bare metal** (sin sistema operativo), con:
- Acceso directo a registros de periféricos
- Sin uso de HAL (Hardware Abstraction Layer) de ST
- Control explícito de inicialización y configuración
- Gestión manual de interrupciones

## Limitaciones Conocidas

1. **Simulación**: No todos los ejemplos pueden ejecutarse en simulador; algunos requieren hardware real
2. **Depuración**: Se asume disponibilidad de debugger SWD/JTAG para depuración avanzada
3. **Periféricos**: Solo se abordarán GPIO, Timer básico, UART, y conceptos de interrupciones en esta unidad

## Consideraciones de Seguridad

- Todos los ejemplos de código asumen operación con alimentación estándar (3.3V)
- Se recomienda uso de resistencias limitadoras en todos los pines GPIO conectados a dispositivos externos
- No se incluyen consideraciones de protección contra sobretensión o cortocircuito (se asume entorno controlado de laboratorio)

## Extensiones Futuras

Este documento podrá actualizarse si:
- Se requiere soporte para otros microcontroladores de la familia STM32
- Se adopta un toolchain o IDE diferente
- Se identifican recursos de hardware adicionales disponibles
- Los estudiantes demuestran niveles de conocimiento previo diferentes a los asumidos

---

**Fecha de creación**: Noviembre 2025  
**Versión**: 1.0  
**Última actualización**: Noviembre 2025
