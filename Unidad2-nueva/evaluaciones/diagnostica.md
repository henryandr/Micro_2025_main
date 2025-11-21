# Evaluación Diagnóstica - Unidad 2

**Propósito**: Evaluar conocimientos previos de Unidad 1 antes de iniciar Unidad 2
**Duración**: 20 minutos
**Aplicación**: Sesión 1, inicio de clase
**Formato**: Individual, sin material de consulta

---

## Sección A: Opción Múltiple (10 preguntas, 1 punto c/u)

### 1. En arquitectura ARM Cortex M-4, ¿cuántos registros de propósito general hay?
a) 8  
b) 13 ✓  
c) 16  
d) 32

### 2. El registro BSRR de GPIO se usa para:
a) Leer el estado del pin  
b) Set/Reset atómico de bits ✓  
c) Configurar el modo del pin  
d) Seleccionar velocidad de salida

### 3. ¿Qué significa "volatile" en C para sistemas embebidos?
a) Variable que puede cambiar en cualquier momento ✓  
b) Variable constante  
c) Variable rápida  
d) Variable de solo lectura

### 4. En Assembly ARM, la instrucción LDR R0, [R1] hace:
a) Carga R1 en R0  
b) Carga el contenido de la dirección en R1 en R0 ✓  
c) Carga R0 en la dirección de R1  
d) Suma R0 y R1

### 5. Para configurar un pin como salida en GPIO, el registro MODER debe tener:
a) 00  
b) 01 ✓  
c) 10  
d) 11

### 6. ¿Qué hace el prescaler (PSC) en un timer?
a) Incrementa la frecuencia  
b) Divide la frecuencia del reloj ✓  
c) Reinicia el contador  
d) Habilita interrupciones

### 7. El NVIC (Nested Vectored Interrupt Controller) maneja:
a) Solo interrupciones externas  
b) Solo excepciones del sistema  
c) Interrupciones y excepciones con prioridades ✓  
d) Únicamente el reset

### 8. ¿Cuál es la ventaja de programar en C vs Assembly para sistemas embebidos?
a) C es siempre más rápido  
b) C usa menos memoria  
c) C es más portable y mantenible ✓  
d) C no necesita compilador

### 9. La instrucción BX LR en Assembly ARM:
a) Salta a la dirección en LR ✓  
b) Salta siempre  
c) Suma LR a PC  
d) Resetea el procesador

### 10. Para evitar rebotes mecánicos de un botón, se debe:
a) Usar resistencia pull-up  
b) Conectar a 5V  
c) Implementar debouncing por tiempo ✓  
d) Usar capacitor grande

---

## Sección B: Preguntas Cortas (3 preguntas, 3 puntos c/u)

### 11. Explica brevemente qué es una interrupción y por qué es útil en sistemas embebidos. (3 puntos)

**Respuesta esperada**:
- Interrupción es un evento que suspende temporalmente el programa principal (1 punto)
- Permite responder rápidamente a eventos externos sin polling (1 punto)
- Libera al CPU para otras tareas, mejora eficiencia (1 punto)

### 12. Escribe en C la línea que configura PA6 como salida usando la estructura GPIO_TypeDef. (3 puntos)

**Respuesta esperada**:
```c
GPIOA->MODER &= ~(0x3 << 12);  // Limpiar (1 punto)
GPIOA->MODER |= (0x1 << 12);   // Establecer salida (2 puntos)
```

### 13. ¿Cuál es la diferencia entre usar ODR y BSRR para cambiar el estado de un pin? (3 puntos)

**Respuesta esperada**:
- ODR requiere read-modify-write (1 punto)
- BSRR es operación atómica, solo escritura (1 punto)
- BSRR es más seguro en sistemas con interrupciones (1 punto)

---

## Sección C: Ejercicio Práctico (10 puntos)

### 14. Dados los siguientes valores de registros de GPIO, determina la configuración del pin PA5:

```
MODER  bits 10-11: 01
OTYPER bit 5:      0
OSPEEDR bits 10-11: 10
PUPDR bits 10-11:   01
```

**Pregunta**: Describe completamente la configuración del pin.

**Respuesta esperada** (2.5 puntos por ítem):
1. Modo: Salida (MODER = 01)
2. Tipo: Push-pull (OTYPER = 0)
3. Velocidad: Fast (OSPEEDR = 10)
4. Pull: Pull-up (PUPDR = 01)

---

## Criterios de Evaluación

**Puntaje total**: 29 puntos

**Escala de calificación**:
- 26-29 puntos: Excelente - Listo para Unidad 2
- 20-25 puntos: Bueno - Requiere repaso menor
- 15-19 puntos: Aceptable - Requiere refuerzo
- < 15 puntos: Insuficiente - Revisar Unidad 1

**Retroalimentación**:
- Entregar resultados al final de Sesión 1
- Identificar estudiantes que necesitan apoyo adicional
- Recomendar material de repaso si es necesario

---

## Notas para el Instructor

**Tiempo de aplicación**: 20 minutos
**Tiempo de revisión**: Se puede revisar inmediatamente (preguntas objetivas)
**Uso de resultados**: 
- Diagnóstico, no cuenta para calificación final
- Permite adaptar ritmo de enseñanza
- Identifica temas que requieren más énfasis

**Material de repaso recomendado** (si puntaje < 20):
- Sesiones 6-8 de Unidad 1
- Ejercicios de manipulación de bits
- Lectura de datasheet GPIO
