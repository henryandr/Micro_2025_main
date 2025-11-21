# Rúbricas de Evaluación - Unidad 2

Criterios detallados para evaluación consistente y objetiva.

---

## Rúbrica 1: Evidencias de Aprendizaje por Sesión

Aplicable a entregables de cada sesión (Sesiones 1-7).

| Criterio | 4 (Excelente) | 3 (Bueno) | 2 (Aceptable) | 1 (Insuficiente) |
|----------|---------------|-----------|---------------|------------------|
| **Funcionalidad** | Funciona perfectamente, todos los requisitos cumplidos | Funciona con errores menores, 1 requisito faltante | Funciona parcialmente, 2-3 requisitos faltantes | No funciona o >3 requisitos faltantes |
| **Código** | Bien estructurado, modular, uso correcto de drivers | Estructura aceptable, pequeños problemas | Estructura pobre, código mezclado | Sin estructura, código desorganizado |
| **Comentarios** | Comentarios claros y útiles en todo el código | Comentarios en partes principales | Pocos comentarios, algunos útiles | Sin comentarios o no útiles |
| **Compilación** | Compila sin errores ni warnings | Compila con warnings menores | Compila con errores corregibles | No compila |

**Ponderación**: Cada criterio vale 25% (total 100%)

---

## Rúbrica 2: Diseño de MEF (Sesiones 2 y 3)

| Criterio | 5 (Excelente) | 4 (Bueno) | 3 (Aceptable) | 2 (Regular) | 1 (Insuficiente) |
|----------|---------------|-----------|---------------|-------------|------------------|
| **Estados** | Todos identificados correctamente, nombres descriptivos | Todos identificados, nombres aceptables | Faltan 1-2 estados o nombres poco claros | Faltan varios estados | Estados incorrectos o ausentes |
| **Transiciones** | Todas especificadas con eventos claros | Todas presentes, eventos aceptables | Faltan 1-2 transiciones | Faltan varias transiciones | Transiciones incorrectas |
| **Diagrama** | Claro, profesional, fácil de entender | Claro, algunas mejoras posibles | Comprensible pero confuso | Difícil de interpretar | Incompleto o ausente |
| **Tabla** | Completa, correcta, bien formateada | Completa, pequeños errores | Parcialmente completa | Muchos errores | Ausente o incorrecta |
| **Justificación** | Excelente argumentación técnica | Buena argumentación | Argumentación básica | Argumentación pobre | Sin justificación |

**Ponderación**: Cada criterio vale 20% (total 100%)

---

## Rúbrica 3: Driver Development (Sesión 4)

| Criterio | 5 pts | 4 pts | 3 pts | 2 pts | 1 pt |
|----------|-------|-------|-------|-------|------|
| **API Design** | API intuitiva, completa, bien documentada | API completa, buena documentación | API funcional, documentación básica | API incompleta | API pobre o ausente |
| **Header File** | Completo, prototipos correctos, comentarios Doxygen | Completo, prototipos correctos | Prototipos presentes, poca documentación | Incompleto | Muy deficiente |
| **Implementation** | Código eficiente, sin errores, bien estructurado | Código correcto, estructura aceptable | Código funcional, estructura básica | Errores menores | Muchos errores |
| **Modularidad** | Funciones independientes, reutilizables | Funciones separadas, algo reutilizable | Funciones básicas | Poca modularidad | No modular |
| **Testing** | Ejemplos de uso incluidos y probados | Ejemplos incluidos | Ejemplo básico | Sin ejemplos | N/A |

**Ponderación**: Suma total de puntos (máximo 25)

---

## Rúbrica 4: Timer Configuration (Sesión 5)

| Criterio | 4 (Excelente) | 3 (Bueno) | 2 (Aceptable) | 1 (Insuficiente) |
|----------|---------------|-----------|---------------|------------------|
| **Cálculos** | PSC y ARR correctos, fórmulas mostradas | Valores correctos, fórmulas básicas | Valores aproximados | Valores incorrectos |
| **Configuración** | Todos los registros bien configurados | Configuración correcta, falta algo menor | Configuración básica funcional | Configuración incorrecta |
| **Interrupciones** | NVIC y handler correctos, flag limpiado | NVIC correcto, pequeños errores en handler | Handler funcional pero incompleto | Handler incorrecto |
| **Integración** | Bien integrado con MEF, timing preciso | Integrado, timing aceptable | Integración básica | No integrado |

**Ponderación**: Cada criterio vale 25% (total 100%)

---

## Rúbrica 5: Multiplexación (Sesión 6)

| Criterio | 5 pts | 4 pts | 3 pts | 2 pts | 1 pt |
|----------|-------|-------|-------|-------|------|
| **Sin Flicker** | Perfecto, no se detecta flicker | Flicker mínimo, casi imperceptible | Flicker leve pero aceptable | Flicker notable | Flicker severo |
| **Frecuencia** | Cálculo correcto, >50Hz por display | Frecuencia buena, cálculo presente | Frecuencia aceptable | Frecuencia baja | No calculado |
| **Código MEF** | MEF clara para multiplexación | MEF presente, funcional | MEF básica | MEF confusa | Sin MEF |
| **Funcionalidad** | Muestra dígitos correctamente, actualiza bien | Muestra correctamente | Funciona con errores menores | Errores frecuentes | No funciona |
| **Hardware** | Conexiones correctas, circuito óptimo | Conexiones correctas | Conexiones funcionales | Conexiones con problemas | Conexiones incorrectas |

**Ponderación**: Suma total de puntos (máximo 25)

---

## Rúbrica 6: Interrupciones EXTI (Sesión 7)

| Criterio | 4 pts | 3 pts | 2 pts | 1 pt |
|----------|-------|-------|-------|------|
| **Configuración EXTI** | Completa y correcta (SYSCFG, EXTI, NVIC) | Correcta, falta algo menor | Funcional pero incompleta | Incorrecta |
| **Handler** | Handler correcto, limpia flag, debounce | Handler correcto, limpia flag | Handler funcional | Handler incorrecto |
| **Comunicación ISR-Main** | Flag volatile, comunicación correcta | Flag presente, funcional | Comunicación básica | Sin comunicación |
| **Integración MEF** | Bien integrado con MEF existente | Integrado, funcional | Integración básica | No integrado |

**Ponderación**: Cada criterio vale 25% (total 16 puntos, escalar a 100%)

---

## Rúbrica 7: Proyecto Final (Sesión 8)

### Funcionalidad (35 puntos)

| Aspecto | Puntos | Criterio |
|---------|--------|----------|
| Ciclo normal semáforo | 10 | Verde-Amarillo-Rojo con tiempos correctos |
| Botón peatonal | 8 | Detecta pulsación, activa secuencia |
| Modo peatonal | 7 | Verde/Amarillo peatonal con tiempos |
| Display | 5 | Muestra tiempo restante correctamente |
| Debounce | 5 | Elimina rebotes efectivamente |

### MEF Diseñada (10 puntos)

| Criterio | Puntos |
|----------|--------|
| Estados identificados | 3 |
| Transiciones correctas | 3 |
| Diagrama claro | 2 |
| Implementación coherente | 2 |

### Drivers y Modularidad (7 puntos)

| Aspecto | Puntos |
|---------|--------|
| Usa driver GPIO | 2 |
| Usa driver Timer | 2 |
| Código modular | 2 |
| Separación de concerns | 1 |

### Timer (7 puntos)

| Aspecto | Puntos |
|---------|--------|
| Configuración correcta | 3 |
| Timing preciso | 2 |
| Handler bien implementado | 2 |

### Interrupciones (7 puntos)

| Aspecto | Puntos |
|---------|--------|
| EXTI configurado | 3 |
| Handler correcto | 2 |
| Debounce implementado | 2 |

### Multiplexación (7 puntos)

| Aspecto | Puntos |
|---------|--------|
| Sin flicker | 3 |
| Frecuencia adecuada | 2 |
| Implementación correcta | 2 |

### Código Limpio (7 puntos)

| Aspecto | Puntos |
|---------|--------|
| Comentarios útiles | 2 |
| Estructura clara | 2 |
| Nombres descriptivos | 2 |
| Documentación | 1 |

**Total**: 80 puntos (escalar a 70% de evaluación práctica)

---

## Notas de Uso para Instructores

**Consistencia**: Usar siempre la misma rúbrica para el mismo tipo de evaluación

**Transparencia**: Compartir rúbricas con estudiantes ANTES de la evaluación

**Feedback**: Marcar criterios específicos al evaluar, no solo puntaje total

**Calibración**: Revisar algunos trabajos con otro instructor para consistencia

**Ajuste**: Las rúbricas pueden ajustarse según nivel del grupo, pero avisando

**Registro**: Mantener evidencia de evaluaciones para auditorías o reclamos
