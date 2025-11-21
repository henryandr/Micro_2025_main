# Evaluación Sumativa - Unidad 2

**Aplicación**: Sesión 8
**Duración**: 2 horas
**Formato**: Teórica (30%) + Práctica (70%)
**Modalidad**: Individual

---

## Parte 1: Evaluación Teórica (30 puntos, 30%)

### Sección A: Opción Múltiple (10 preguntas, 10 puntos)

1. **En una MEF de Moore, las salidas dependen de:**
   a) Solo el estado actual ✓  
   b) El estado y las entradas  
   c) Solo las entradas  
   d) El estado anterior

2. **¿Qué hace el registro BSRR de GPIO?**
   a) Lee el estado del pin  
   b) Set/Reset atómico ✓  
   c) Configura el modo  
   d) Selecciona velocidad

3. **Con PSC=799 y CLK=16MHz, la frecuencia del timer es:**
   a) 16 kHz  
   b) 20 kHz ✓  
   c) 800 Hz  
   d) 16 MHz

4. **El refresh rate mínimo para 4 displays sin flicker es:**
   a) 50 Hz  
   b) 100 Hz  
   c) 200 Hz ✓  
   d) 400 Hz

5. **¿Qué hace EXTI->PR en el handler de interrupción?**
   a) Deshabilita la interrupción  
   b) Limpia el flag de pending ✓  
   c) Configura la prioridad  
   d) Habilita el pin

6. **La palabra clave `volatile` en C se usa para:**
   a) Variables que cambian por hardware o ISR ✓  
   b) Variables constantes  
   c) Variables rápidas  
   d) Variables locales

7. **¿Cuántos estados tiene una MEF de debouncing típica?**
   a) 2  
   b) 3  
   c) 4 ✓  
   d) 5

8. **Para 1ms tick con CLK=16MHz, PSC debe ser:**
   a) 16  
   b) 160  
   c) 159 ✓  
   d) 1599

9. **En multiplexación de 4 displays, cada uno debe refrescarse a:**
   a) 12.5 Hz  
   b) 25 Hz  
   c) 50 Hz ✓  
   d) 200 Hz

10. **SYSCFG se usa para:**
    a) Configurar GPIO  
    b) Mapear pines a EXTI ✓  
    c) Controlar timers  
    d) Habilitar clocks

### Sección B: Preguntas Cortas (5 preguntas, 20 puntos)

**1. Explica la diferencia entre MEF de Moore y Mealy** (4 puntos)

**Respuesta esperada**:
- Moore: salidas solo dependen del estado actual (2 pts)
- Mealy: salidas dependen del estado Y entrada (1 pt)
- Moore más simple, Mealy más compacta (1 pt)

**2. ¿Por qué es importante usar `volatile` para registros de hardware?** (4 puntos)

**Respuesta esperada**:
- Evita que compilador optimice accesos (2 pts)
- Hardware puede cambiar valor sin que código lo modifique (1 pt)
- Fuerza lectura/escritura real en cada acceso (1 pt)

**3. Calcula PSC y ARR para generar tick de 10ms con CLK=16MHz** (4 puntos)

**Respuesta esperada**:
```
Frecuencia deseada = 100 Hz (10ms)
Opción 1: PSC = 1599, ARR = 99  (2 pts cada valor)
16,000,000 / 1600 / 100 = 100 Hz ✓
```

**4. Describe el proceso de debounce de botón con MEF** (4 puntos)

**Respuesta esperada**:
- Estado LIBERADO detecta pulsación → DEBOUNCE_PRESS (1 pt)
- Espera 30ms para confirmar (1 pt)
- Si sigue presionado → PRESIONADO, sino → LIBERADO (1 pt)
- Proceso similar para liberación (1 pt)

**5. ¿Cómo se comunica un evento de ISR a main loop?** (4 puntos)

**Respuesta esperada**:
- Variable `volatile` global como flag (2 pts)
- ISR establece flag = 1 (1 pt)
- Main loop verifica y limpia flag (1 pt)

---

## Parte 2: Evaluación Práctica (70 puntos, 70%)

### Proyecto: Semáforo Inteligente con Botón Peatonal

**Especificaciones completas** (ver sesion8.md para detalles):

**Hardware requerido**:
- LEDs: Verde, Amarillo, Rojo vehicular (PA5, PA6, PA7)
- LEDs: Verde, Rojo peatonal (PB0, PB1)
- Botón peatonal en PC13 con interrupción
- Display 7 segmentos: 2 dígitos multiplexados

**Requisitos funcionales**:
1. Ciclo normal: Verde (30s) → Amarillo (3s) → Rojo (25s)
2. Botón peatonal activa: Verde peatonal (15s) → Amarillo peatonal (5s)
3. Display muestra tiempo restante en estado actual
4. Debounce de botón (30ms)
5. Sin flicker en display

**Estructura requerida**:
- MEF principal para estados del semáforo
- MEF para multiplexación
- Driver GPIO (del curso)
- Timer configurado (1ms tick)
- Handler EXTI para botón

### Rúbrica Detallada (70 puntos)

| Criterio | Excelente (100%) | Bueno (80%) | Aceptable (60%) | Insuficiente (<60%) |
|----------|------------------|-------------|-----------------|---------------------|
| **Funcionalidad** (25 pts) | Todos los requisitos funcionan | Falta 1 requisito menor | Faltan 2-3 requisitos | No funciona o faltan >3 |
| **MEF diseñada** (10 pts) | Estados claros, transiciones correctas | Pequeños problemas | Funcional pero confusa | Mal implementada |
| **Drivers usados** (7 pts) | Usa drivers correctamente | Pequeños errores | Mezcla drivers y directo | No usa drivers |
| **Timer** (7 pts) | Timing preciso, bien configurado | Pequeños errores | Timing impreciso | Sin timer |
| **Interrupciones** (7 pts) | EXTI correcta, debounce | Funciona, falta algo | Problemas intermitentes | No usa interrupciones |
| **Multiplexación** (7 pts) | Sin flicker, frecuencia buena | Flicker leve | Flicker notable | No multiplexado |
| **Código limpio** (7 pts) | Comentarios, modular, claro | Falta documentación | Desorganizado | Muy difícil leer |

### Entregables

1. **Código fuente completo** (.c y .h files)
2. **README.md** con:
   - Instrucciones de compilación
   - Descripción del hardware
   - Diagrama de estados
   - Explicación de decisiones de diseño
3. **Demostración en vivo** (5 minutos)
4. **Respuestas a preguntas** del instructor sobre el código

---

## Criterios Generales de Evaluación

**Puntaje total**: 100 puntos
- Teórica: 30 puntos
- Práctica: 70 puntos

**Escala de calificación**:
- 90-100: Excelente
- 80-89: Bueno
- 60-79: Aceptable
- < 60: Insuficiente (debe recuperar)

**Aprobación**: Mínimo 60 puntos

**Notas importantes**:
- Código debe compilar sin errores
- Demostración es obligatoria
- Trabajo individual (no se permite copia)
- Consultar datasheets está permitido

---

## Cronograma de Evaluación

**Parte 1 - Teórica** (30 minutos):
- 10 minutos: Opción múltiple
- 20 minutos: Preguntas cortas

**Parte 2 - Práctica** (90 minutos):
- 70 minutos: Desarrollo
- 20 minutos: Demostraciones y preguntas

**Entrega**: Al final de la sesión
**Resultados**: En 5 días hábiles

---

## Material Permitido

- Datasheet STM32F407
- Notas de sesiones previas
- Código de ejemplos del curso

**No permitido**:
- Código completo de otros (debe ser propio)
- Comunicación con compañeros durante evaluación
- Internet (excepto datasheets oficiales)
