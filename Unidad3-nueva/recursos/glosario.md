# Glosario de Términos - Unidad 3

## A

**ACK (Acknowledgement)**: Bit de reconocimiento que indica recepción exitosa de datos en protocolos como I2C.

**Address**: Identificador único de un dispositivo en un bus (I2C, CAN).

**AFR (Alternate Function Register)**: Registro GPIO que selecciona funciones alternativas de pines.

**APB (Advanced Peripheral Bus)**: Bus interno del STM32 que conecta periféricos.

**Arbitration**: Proceso de resolución de conflictos cuando múltiples nodos intentan transmitir simultáneamente (CAN, I2C).

**Asynchronous**: Comunicación sin señal de reloj compartida (USART).

## B

**Baud Rate**: Velocidad de transmisión en símbolos por segundo.

**Bit Rate**: Velocidad de transmisión en bits por segundo.

**BRR (Baud Rate Register)**: Registro que configura la velocidad de USART.

**Buffer Circular (Ring Buffer)**: Estructura de datos FIFO que "envuelve" al llegar al final.

**Bus**: Línea de comunicación compartida por múltiples dispositivos.

## C

**CAN (Controller Area Network)**: Protocolo robusto para redes automotrices e industriales.

**CCR (Clock Control Register)**: Registro I2C que configura la frecuencia del reloj SCL.

**CPHA (Clock Phase)**: Fase del reloj en SPI (cuándo se muestrean los datos).

**CPOL (Clock Polarity)**: Polaridad del reloj en SPI (nivel en reposo).

**CS (Chip Select)**: Señal para activar un dispositivo esclavo en SPI.

## D

**DLC (Data Length Code)**: Campo en mensaje CAN que indica cantidad de bytes de datos.

**DMA (Direct Memory Access)**: Transferencia de datos sin intervención del CPU.

**DR (Data Register)**: Registro de datos para TX/RX en periféricos de comunicación.

**Driver**: Módulo de software que controla un periférico de hardware.

## E

**EEPROM**: Memoria no volátil electricamente borrable y programable.

**EXTI (External Interrupt)**: Sistema de interrupciones externas del STM32.

## F

**FIFO (First In First Out)**: Estructura de datos donde el primer elemento en entrar es el primero en salir.

**Frame**: Unidad básica de transmisión de datos en un protocolo.

**Full-Duplex**: Comunicación bidireccional simultánea.

## G

**GPIO (General Purpose Input/Output)**: Pines configurables para entrada/salida digital.

## H

**Half-Duplex**: Comunicación bidireccional alternada (no simultánea).

**Handler**: Función que maneja una interrupción (ISR).

**Hardware Flow Control**: Control de flujo usando señales RTS/CTS.

## I

**I2C (Inter-Integrated Circuit)**: Protocolo serial síncrono de 2 cables (SDA, SCL).

**IDE (Identifier Extension)**: Bit en CAN que indica identificador extendido (29 bits).

**IRQ (Interrupt Request)**: Solicitud de interrupción.

**ISR (Interrupt Service Routine)**: Función que atiende una interrupción.

## L

**LSB (Least Significant Bit)**: Bit menos significativo.

**Loopback**: Modo de prueba donde TX se conecta internamente a RX.

## M

**Master**: Dispositivo que controla la comunicación en un bus.

**MISO (Master In Slave Out)**: Línea de datos slave→master en SPI.

**MODER (Mode Register)**: Registro GPIO que configura el modo de los pines.

**MOSI (Master Out Slave In)**: Línea de datos master→slave en SPI.

**MSB (Most Significant Bit)**: Bit más significativo.

**Multiplexing**: Técnica para compartir recursos (displays, líneas de comunicación).

## N

**NACK (Not Acknowledge)**: Indica falla en la recepción de datos (I2C).

**NVIC (Nested Vectored Interrupt Controller)**: Controlador de interrupciones del ARM Cortex-M4.

## O

**OLED (Organic Light Emitting Diode)**: Tipo de pantalla usado en ejemplos.

**Open-Drain**: Configuración de salida que requiere resistencia pull-up externa.

**ORE (Overrun Error)**: Error cuando datos nuevos sobrescriben datos no leídos.

**Oversampling**: Técnica para mejorar precisión muestreando múltiples veces.

## P

**Parity**: Bit adicional para detección de errores.

**Polling**: Técnica donde el CPU verifica continuamente el estado de un periférico.

**Prescaler**: Divisor de frecuencia de reloj.

**Pull-up**: Resistencia que lleva una línea a nivel alto cuando está flotante.

**Push-Pull**: Configuración de salida GPIO activa en ambos niveles.

## R

**Register**: Ubicación de memoria que controla un periférico.

**Repeated START**: Segunda condición START sin STOP intermedio en I2C.

**RTR (Remote Transmission Request)**: Bit en CAN que solicita datos a otro nodo.

**RX (Receive)**: Recepción de datos.

**RXNE (RX Not Empty)**: Flag que indica dato recibido disponible.

## S

**SDA (Serial Data)**: Línea de datos en I2C.

**SCK (Serial Clock)**: Reloj en SPI o I2C (SCL).

**SCL (Serial Clock Line)**: Línea de reloj en I2C.

**Slave**: Dispositivo controlado por el master en un bus.

**SPI (Serial Peripheral Interface)**: Protocolo serial síncrono full-duplex de 4 cables.

**SR (Status Register)**: Registro de estado de un periférico.

**SS (Slave Select)**: Ver CS (Chip Select).

**START**: Condición de inicio de transmisión en I2C.

**STOP**: Condición de fin de transmisión en I2C.

**Synchronous**: Comunicación con señal de reloj compartida.

## T

**Timeout**: Tiempo máximo de espera para una operación.

**TRISE (Rise Time Register)**: Registro I2C que configura tiempo de subida.

**TX (Transmit)**: Transmisión de datos.

**TXE (TX Empty)**: Flag que indica buffer de transmisión vacío.

## U

**UART (Universal Asynchronous Receiver Transmitter)**: Protocolo asíncrono básico.

**USART (Universal Synchronous Asynchronous Receiver Transmitter)**: Protocolo que soporta modos síncrono y asíncrono.

## V

**Volatile**: Calificador C que indica que una variable puede cambiar externamente.

---

**Total términos**: 120+

**Uso**: Consultar durante estudio y desarrollo de proyectos.
