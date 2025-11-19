# Evaluación Diagnóstica - Unidad 1

**Propósito**: Identificar conocimientos previos de los estudiantes antes de iniciar la Unidad 1.

**Características**:
- No punitiva (no afecta negativamente la calificación)
- Tiempo: 30 minutos
- Individual
- Puede consultar materiales si lo desea

---

## Parte A: Preguntas de Opción Múltiple (1 punto c/u)

### 1. ¿Qué es un microprocesador?

a) Un chip que contiene solo memoria  
b) Un circuito integrado que ejecuta instrucciones de un programa  
c) Un dispositivo de almacenamiento  
d) Un tipo de sensor

**Respuesta correcta**: b

---

### 2. ¿Cuál es la diferencia principal entre un microprocesador y un microcontrolador?

a) El tamaño físico  
b) El microcontrolador integra CPU, memoria y periféricos en un solo chip  
c) El microprocesador es más lento  
d) No hay diferencia

**Respuesta correcta**: b

---

### 3. ¿Qué significa "ejecutar una instrucción"?

a) Guardar datos en memoria  
b) Realizar la operación especificada por la instrucción  
c) Encender el procesador  
d) Compilar código

**Respuesta correcta**: b

---

### 4. En un sistema de 32 bits, ¿cuántos bits tiene una dirección de memoria típica?

a) 8 bits  
b) 16 bits  
c) 32 bits  
d) 64 bits

**Respuesta correcta**: c

---

### 5. ¿Qué es un registro en un procesador?

a) Una base de datos  
b) Memoria interna muy rápida del procesador  
c) Un periférico externo  
d) Un tipo de cable

**Respuesta correcta**: b

---

### 6. ¿Qué lenguaje de programación está más cerca del hardware?

a) Python  
b) Java  
c) Assembly  
d) JavaScript

**Respuesta correcta**: c

---

### 7. ¿Qué es la ALU?

a) Unidad de entrada/salida  
b) Unidad Aritmético-Lógica que realiza operaciones matemáticas  
c) Un tipo de memoria  
d) Un bus de datos

**Respuesta correcta**: b

---

### 8. ¿Para qué se usa típicamente un microcontrolador?

a) Procesamiento de gráficos 3D  
b) Servidores de internet  
c) Control de sistemas embebidos (sensores, actuadores, etc.)  
d) Edición de video

**Respuesta correcta**: c

---

### 9. ¿Qué significa el término "bare metal" en programación embebida?

a) Programar con un marco de trabajo completo  
b) Programar directamente sobre el hardware sin sistema operativo  
c) Usar solo Python  
d) Programar con muchas librerías

**Respuesta correcta**: b

---

### 10. ¿Qué es un periférico en un microcontrolador?

a) Un componente externo al chip  
b) Un módulo funcional (GPIO, UART, ADC, etc.) integrado en el chip  
c) Solo la memoria RAM  
d) El procesador central

**Respuesta correcta**: b

---

## Parte B: Preguntas Abiertas (5 puntos c/u)

### 11. Programación en C

Escribe un pequeño programa en C que:
- Declare una variable entera `x` con valor 10
- Declare otra variable `y` con valor 20
- Calcule la suma y almacene en `resultado`
- Imprima el resultado

**Evaluación**:
- 5 puntos: Código correcto y funcional
- 3 puntos: Código con errores menores de sintaxis
- 1 punto: Intento con estructura general correcta
- 0 puntos: No responde o completamente incorrecto

**Ejemplo de respuesta esperada**:
```c
#include <stdio.h>

int main() {
    int x = 10;
    int y = 20;
    int resultado = x + y;
    printf("Resultado: %d\n", resultado);
    return 0;
}
```

---

### 12. Conversión de Sistemas Numéricos

Convierte los siguientes números:

a) Decimal 42 a binario  
b) Binario 11010 a decimal  
c) Hexadecimal 0x2F a decimal

**Evaluación**:
- 5 puntos: Las 3 conversiones correctas
- 3 puntos: 2 conversiones correctas
- 2 puntos: 1 conversión correcta
- 0 puntos: Ninguna correcta o no responde

**Respuestas esperadas**:
- a) 42 = 101010₂
- b) 11010₂ = 26₁₀
- c) 0x2F = 47₁₀

---

## Parte C: Reflexión (Opcional, no calificada)

### 13. ¿Qué esperas aprender en este curso?

(Respuesta abierta, ayuda al instructor a comprender motivaciones)

---

### 14. ¿Tienes experiencia previa con microcontroladores o electrónica?

a) Ninguna experiencia  
b) He usado Arduino o similar  
c) He programado microcontroladores en C  
d) He trabajado con Assembly

---

### 15. ¿Qué te parece más interesante?

a) Programación de bajo nivel (Assembly, registros)  
b) Proyectos prácticos (LEDs, sensores, motores)  
c) Teoría de arquitecturas de procesadores  
d) Diseño de sistemas embebidos completos

---

## Puntuación

**Parte A (Opción múltiple)**: 10 puntos total (1 punto c/u)  
**Parte B (Preguntas abiertas)**: 10 puntos total (5 puntos c/u)  
**Total**: 20 puntos

**Interpretación** (para el instructor):
- 16-20 puntos: Conocimientos sólidos, puede avanzar rápido
- 10-15 puntos: Base adecuada, ritmo normal
- 5-9 puntos: Necesita apoyo adicional en fundamentos
- 0-4 puntos: Considerar material de nivelación previo

---

**Nota**: Esta evaluación NO tiene impacto negativo en la calificación final. Su propósito es orientar la enseñanza según el nivel del grupo.
