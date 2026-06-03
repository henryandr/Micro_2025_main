# Quizzes Formativos - Unidad 2

Evaluaciones formativas breves para cada sesión (excepto sesión 8 que es sumativa).

---

## Quiz Sesión 1: Transición Assembly-C (10 puntos)

**Duración**: 5 minutos

1. **¿Qué palabra clave se debe usar siempre con punteros a registros de hardware?** (2 pts)
   - Respuesta: `volatile`

2. **¿Qué hace el startup code ANTES de llamar a `main()`?** (2 pts)
   - Respuesta: Copia `.data` de Flash a RAM e inicializa `.bss` a cero

3. **¿Por qué `printf()` no está disponible por defecto en C bare metal?** (2 pts)
   - Respuesta: No hay sistema operativo ni biblioteca estándar completa

4. **¿Qué registro se debe usar para operaciones atómicas de set/reset de pines GPIO?** (2 pts)
   - Respuesta: BSRR

5. **Escribe en C la línea que habilita el clock de GPIOB (bit 1 de RCC_AHB1ENR)** (2 pts)
   - Respuesta: `RCC->AHB1ENR |= (1 << 1);`

---

## Quiz Sesión 2: MEF Teoría (10 puntos)

**Duración**: 5 minutos

1. **¿Cuál es la diferencia principal entre MEF de Moore y Mealy?** (2 pts)
   - Respuesta: Moore: salidas dependen solo del estado. Mealy: salidas dependen del estado Y entrada.

2. **¿Qué componentes son esenciales en una MEF?** (2 pts)
   - Respuesta: Estados, transiciones, eventos, salidas/acciones, estado inicial

3. **¿Por qué se usan MEF para antirrebote de botones?** (2 pts)
   - Respuesta: Para manejar estados temporales de debounce y validar pulsaciones con timing preciso

4. **En el semáforo ejemplo, ¿es Moore o Mealy? Justifica.** (2 pts)
   - Respuesta: Moore, porque las salidas (LEDs) dependen solo del estado actual, no de eventos

5. **Dibuja una MEF simple de 2 estados para un LED que parpadea** (2 pts)
   - Respuesta: Estados ON y OFF, transiciones por timeout

---

## Quiz Sesión 3: MEF en C (10 puntos)

**Duración**: 5 minutos

1. **¿Por qué usar `enum` para estados en lugar de enteros?** (2 pts)
   - Respuesta: Código más legible, type-safe, mantenible, menos errores

2. **¿Qué debe hacerse al cambiar de estado en una MEF?** (2 pts)
   - Respuesta: Resetear contador de tiempo, ejecutar acciones de salida/entrada

3. **¿Para qué sirve el tiempo de debounce de 30ms?** (2 pts)
   - Respuesta: Filtrar rebotes mecánicos del botón, esperar estabilización

4. **¿Qué hace el `default` case en el `switch` de una MEF?** (2 pts)
   - Respuesta: Maneja estados inválidos, permite recuperación de errores

5. **Completa: `if (condicion) { estado_actual = NUEVO_ESTADO; tiempo_en_estado = ____;  }`** (2 pts)
   - Respuesta: `0` (resetear a cero)

---

## Quiz Sesión 4: Driver GPIO (10 puntos)

**Duración**: 5 minutos

1. **¿Qué va en el archivo .h de un driver?** (2 pts)
   - Respuesta: Prototipos de funciones públicas, definiciones de tipos, constantes, documentación

2. **¿Por qué usar estructura de configuración en lugar de múltiples parámetros?** (2 pts)
   - Respuesta: Más claro, extensible, fácil de mantener, menos errores

3. **¿Qué ventaja tiene usar BSRR en lugar de ODR?** (2 pts)
   - Respuesta: Operación atómica, no necesita read-modify-write, más seguro con interrupciones

4. **¿Cómo se inicializa un pin como entrada con pull-up?** (2 pts)
   - Respuesta: MODER=00 (input), PUPDR=01 (pull-up)

5. **¿Por qué declarar variables del driver como `static`?** (2 pts)
   - Respuesta: Ocultar implementación, evitar conflictos de nombres, encapsulación

---

## Quiz Sesión 5: Timer (10 puntos)

**Duración**: 5 minutos

1. **¿Qué hace el prescaler del timer?** (2 pts)
   - Respuesta: Divide la frecuencia del reloj de entrada

2. **Si PSC=1599 y CLK=16MHz, ¿cuál es la frecuencia del timer?** (2 pts)
   - Respuesta: 16,000,000 / 1600 = 10,000 Hz (10 kHz)

3. **¿Por qué limpiar el flag SR en el handler de interrupción?** (2 pts)
   - Respuesta: Para indicar que la interrupción fue atendida y evitar llamadas repetidas

4. **¿Qué ventaja tiene usar timer vs delay busy-wait?** (2 pts)
   - Respuesta: CPU libre para otras tareas, más eficiente, timing preciso

5. **¿Cómo medir tiempo transcurrido entre dos eventos?** (2 pts)
   - Respuesta: Guardar `millis` al inicio, restar al final: `tiempo = millis_actual - millis_inicio`

---

## Quiz Sesión 6: Multiplexación (10 puntos)

**Duración**: 5 minutos

1. **¿Por qué se usa multiplexación?** (2 pts)
   - Respuesta: Ahorrar pines GPIO compartiendo recursos entre múltiples dispositivos

2. **¿Qué es el refresh rate mínimo para evitar flicker?** (2 pts)
   - Respuesta: ~50 Hz por display (depende de persistencia visual humana)

3. **Con 8 displays, ¿qué frecuencia total se necesita para 60Hz cada uno?** (2 pts)
   - Respuesta: 8 × 60 = 480 Hz (ciclo completo)

4. **¿Cómo se detecta tecla en teclado matricial?** (2 pts)
   - Respuesta: Activar fila, leer columnas; si columna LOW = tecla presionada

5. **¿Qué problemas puede causar refresh rate muy bajo?** (2 pts)
   - Respuesta: Flicker visible, fatiga visual, imagen intermitente

---

## Quiz Sesión 7: Interrupciones EXTI (10 puntos)

**Duración**: 5 minutos

1. **¿Qué hace el registro EXTI PR?** (2 pts)
   - Respuesta: Pending register, indica qué interrupciones están pendientes

2. **¿Por qué usar SYSCFG_EXTICR?** (2 pts)
   - Respuesta: Para mapear qué puerto GPIO se conecta a cada línea EXTI

3. **¿Qué pasa si no se limpia el flag en ISR?** (2 pts)
   - Respuesta: Interrupción se llama indefinidamente (loop infinito)

4. **¿Prioridad 0 o 15 es mayor?** (2 pts)
   - Respuesta: 0 es mayor prioridad (menor número = mayor prioridad)

5. **¿Cómo comunicar evento de ISR a main?** (2 pts)
   - Respuesta: Usar variable `volatile` como flag, establecer en ISR, leer en main

---

## Criterios de Evaluación

**Cada quiz**: 10 puntos totales
**Tiempo**: 5 minutos por quiz
**Formato**: Al final de cada sesión
**Calificación**: Inmediata (respuestas objetivas)

**Uso de resultados**:
- Evaluación formativa (retroalimentación inmediata)
- Identifica conceptos que necesitan refuerzo
- Contribuye 30% a calificación de formativas

**Notas para el instructor**:
- Aplicar al final de sesión
- Revisar en 5 minutos con estudiantes
- Aclarar dudas inmediatamente
- Registrar resultados para seguimiento
