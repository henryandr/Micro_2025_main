# Rúbricas de Evaluación - Unidad 1

Este documento contiene las rúbricas detalladas para todas las evaluaciones de la Unidad 1.

---

## Rúbrica: Evaluación Sumativa (Proyecto Final)

**Total**: 100 puntos

### 1. Funcionalidad (40 puntos)

| Criterio | Excelente (10 pts) | Bueno (7-9 pts) | Suficiente (5-6 pts) | Insuficiente (0-4 pts) |
|----------|-------------------|----------------|-------------------|---------------------|
| **LED1 (Timer)** | Parpadea exactamente cada 1s, timing preciso | Parpadea pero timing ligeramente impreciso (±10%) | Parpadea pero timing muy impreciso o irregular | No funciona o no usa timer |
| **LED2 (Botón)** | Responde inmediatamente, sin rebotes | Responde pero con delay notable o rebotes ocasionales | Responde pero inconsistente | No funciona |
| **Interrupción EXTI** | Configurada correctamente, ISR limpia flag, sin errores | Funciona pero código ISR mejorable | Funciona parcialmente o con errores | No implementada o no funciona |
| **Pausa de 5s** | LED1 se detiene exactamente 5s, luego reanuda | Pausa funciona pero timing impreciso | Pausa implementada pero no funciona correctamente | No implementado |

### 2. Calidad del Código (30 puntos)

| Criterio | Excelente (8 pts) | Bueno (6-7 pts) | Suficiente (4-5 pts) | Insuficiente (0-3 pts) |
|----------|------------------|---------------|-------------------|---------------------|
| **Estructura Modular** | Drivers completamente separados, headers bien diseñados, clara separación de responsabilidades | Drivers separados pero con alguna mezcla de responsabilidades | Intento de modularización pero desorganizado | Todo en un archivo, sin modularización |
| **Funciones** | Todas las funciones implementadas, parámetros bien diseñados, retornan valores apropiados | Funciones implementadas pero algunas mejorables | Funciones básicas presentes pero incompletas | Funciones faltantes o muy deficientes |
| **Comentarios** | Comentarios claros y útiles en cada sección crítica, headers de función completos | Comentarios presentes pero algunos innecesarios o faltantes | Pocos comentarios, algunos confusos | Sin comentarios o comentarios inútiles |
| **Compilación** | Sin errores ni warnings, código limpio | Warnings menores presentes pero justificados | Varios warnings o errores menores corregibles | No compila o errores graves |

**Penalizaciones adicionales**:
- -2 puntos por cada warning no justificado (máximo -10)
- -5 puntos si no sigue naming conventions consistentes
- -10 puntos si usa HAL o bibliotecas prohibidas

### 3. Documentación Técnica (20 puntos)

| Criterio | Excelente (5 pts) | Bueno (4 pts) | Suficiente (3 pts) | Insuficiente (0-2 pts) |
|----------|------------------|-------------|------------------|---------------------|
| **Arquitectura** | Diagrama claro, muestra todos los módulos y sus relaciones | Diagrama presente pero falta algún detalle | Diagrama básico, poco claro | Ausente o muy deficiente |
| **Decisiones Técnicas** | Justifica claramente cada decisión importante, considera alternativas | Explica decisiones pero sin profundidad | Menciona decisiones sin justificación | No explica decisiones |
| **Cálculos (Timer)** | Cálculos correctos, muestra fórmulas, explica razonamiento | Cálculos correctos pero explicación breve | Cálculos presentes pero con errores o sin explicación | Cálculos incorrectos o ausentes |
| **Dificultades** | Identifica problemas enfrentados y explica soluciones detalladamente | Menciona problemas y soluciones básicas | Menciona problemas superficialmente | No discute dificultades |

### 4. Comprensión Conceptual (10 puntos)

**Evaluación oral**: 3 preguntas, ~3.33 puntos cada una

| Nivel | Descripción | Puntos |
|-------|-------------|--------|
| **Excelente** | Respuesta completa, correcta, con ejemplos o justificación adicional | 3-3.33 |
| **Bueno** | Respuesta correcta pero básica, sin profundización | 2-2.99 |
| **Suficiente** | Respuesta parcialmente correcta o con errores menores | 1-1.99 |
| **Insuficiente** | Respuesta incorrecta o no sabe | 0-0.99 |

---

## Rúbrica: Quizzes Formativos (Sesiones 1-8)

**Total**: 80 puntos (10 puntos por sesión × 8 sesiones)

### Criterio General

| Tipo de Pregunta | Puntos | Criterio de Asignación |
|------------------|--------|----------------------|
| **Opción múltiple** | 1-2 pts | Correcto = puntos completos, Incorrecto = 0 |
| **Pregunta corta** | 2-3 pts | Completa y correcta = puntos completos, Parcial = puntos parciales, Incorrecta = 0 |
| **Código/Ejercicio** | 3-5 pts | Correcto y eficiente = completo, Funciona pero mejorable = 70-90%, Intento = 30-50%, No funciona = 0 |

### Conversión a Porcentaje Final

Quiz_Percentage = (Total_Puntos_Quizzes / 80) × 30%

**Ejemplo**: 65 puntos de 80 → (65/80) × 30 = 24.375%

---

## Rúbrica: Evidencias de Sesiones

**Peso**: 35% (Proyecto Incremental)

Cada sesión requiere una evidencia (código, diagrama, documento breve).

### Criterio por Evidencia

| Nivel | Descripción | Puntos (sobre 100) |
|-------|-------------|--------------------|
| **Excelente** | Entregado a tiempo, completo, correcto, bien documentado | 90-100 |
| **Bueno** | Entregado a tiempo, completo, errores menores | 75-89 |
| **Suficiente** | Entregado con retraso o incompleto pero funcional | 60-74 |
| **Insuficiente** | No funciona o muy incompleto | 0-59 |
| **No entregado** | - | 0 |

### Promedio de Evidencias

Evidencias_Percentage = (Promedio_Evidencias) × 35%

---

## Rúbrica: Evaluación Diagnóstica

**Propósito**: Identificar nivel inicial (no punitiva en calificación global)

**Peso**: 5% (solo por completarla, no por aciertos)

| Situación | Puntos |
|-----------|--------|
| Completa la evaluación de buena fe | 100% (5 puntos) |
| No completa o respuestas en blanco | 0% (0 puntos) |

---

## Rúbrica: Código Assembly y C (General)

Aplicable a evidencias de sesiones y proyecto final.

### Aspecto: Corrección Funcional

| Nivel | Descripción | Puntos |
|-------|-------------|--------|
| **Excelente** | Funciona perfectamente en todos los casos de prueba | 100% |
| **Bueno** | Funciona en casos normales, falla en edge cases | 70-90% |
| **Suficiente** | Funciona parcialmente o con errores menores | 50-69% |
| **Insuficiente** | No funciona o errores graves | 0-49% |

### Aspecto: Estilo y Claridad

| Nivel | Descripción | Puntos |
|-------|-------------|--------|
| **Excelente** | Nombres descriptivos, indentación correcta, lógica clara | 100% |
| **Bueno** | Generalmente claro pero algunas secciones confusas | 75-90% |
| **Suficiente** | Funciona pero difícil de leer | 50-74% |
| **Insuficiente** | Código ilegible o muy mal estructurado | 0-49% |

### Aspecto: Comentarios

| Nivel | Descripción | Puntos |
|-------|-------------|--------|
| **Excelente** | Comentarios útiles en secciones clave, headers completos | 100% |
| **Bueno** | Comentarios presentes pero algunos obvios o innecesarios | 75-90% |
| **Suficiente** | Pocos comentarios, algunos confusos | 50-74% |
| **Insuficiente** | Sin comentarios o comentarios inútiles | 0-49% |

### Aspecto: Eficiencia

| Nivel | Descripción | Puntos |
|-------|-------------|--------|
| **Excelente** | Uso óptimo de instrucciones y recursos | 100% |
| **Bueno** | Funcional pero con optimizaciones obvias disponibles | 75-90% |
| **Suficiente** | Funciona pero ineficiente (ej: busy-wait excesivo) | 50-74% |
| **Insuficiente** | Muy ineficiente o desperdicia recursos | 0-49% |

---

## Rúbrica: Presentación Oral (Proyecto Final)

Parte de la evaluación sumativa, incluida en "Comprensión Conceptual".

### Criterios Adicionales de Presentación

| Aspecto | Excelente (100%) | Bueno (75-90%) | Suficiente (50-74%) | Insuficiente (<50%) |
|---------|----------------|--------------|------------------|------------------|
| **Demostración** | Funciona perfectamente, muestra todas las características | Funciona con errores menores | Funciona parcialmente | No funciona |
| **Explicación** | Clara, técnica, demuestra comprensión profunda | Clara pero superficial | Confusa o lee código sin explicar | No puede explicar |
| **Timing** | 5 minutos (±30s) | 4-6 minutos | 3-7 minutos | <3 o >7 minutos |
| **Preparación** | Código organizado, demo ensayada | Código listo pero improvisación | Desorganizado | No preparado |

---

## Conversión de Calificaciones

### Escala General de la Unidad 1

| Componente | Peso | Cómo se calcula |
|------------|------|----------------|
| Diagnóstica | 5% | Completitud (100% o 0%) |
| Quizzes Formativos | 30% | (Total/80) × 30% |
| Evidencias (Proyecto Incremental) | 35% | Promedio × 35% |
| Evaluación Sumativa | 30% | Puntuación del proyecto final |
| **TOTAL** | **100%** | Suma de componentes |

### Escala de Calificación Final

| Porcentaje | Calificación | Nivel |
|------------|--------------|-------|
| 90-100% | 5.0 - 4.5 | Excelente |
| 75-89% | 4.4 - 3.8 | Bueno |
| 60-74% | 3.7 - 3.0 | Suficiente (Aprueba) |
| 0-59% | 2.9 - 0.0 | Insuficiente (Reprueba) |

---

## Políticas Generales

### Entregas Tarde

- 1-24 horas tarde: -10%
- 24-48 horas tarde: -20%
- >48 horas tarde: -50%
- No entregado: 0%

### Asistencia

- Mínimo 80% de asistencia (6 de 8 sesiones) requerido para aprobar
- <80% asistencia: Calificación máxima 3.0 aunque obtenga >75%

### Integridad Académica

- Plagio detectado: 0 en la actividad + reporte académico
- Código idéntico a compañero: Ambos reciben 0
- Segunda instancia: 0 en la unidad completa

### Reconsideración

- Plazo: 48 horas después de recibir calificación
- Debe presentar argumento escrito con evidencia
- Decisión del instructor es final

---

## Uso de las Rúbricas

### Para Estudiantes

1. Revisar rúbricas ANTES de realizar la actividad
2. Usar como checklist de auto-evaluación
3. Identificar en qué nivel se encuentra su trabajo
4. Mejorar antes de entregar para alcanzar nivel superior

### Para Instructores

1. Aplicar consistentemente en todos los estudiantes
2. Proporcionar feedback específico citando la rúbrica
3. Si un caso no encaja claramente, usar juicio profesional y documentar
4. Actualizar rúbricas si se detectan ambigüedades

---

## Ejemplos de Aplicación

### Ejemplo 1: Proyecto Final

**Estudiante A**:
- Funcionalidad: 38/40 (LED1 timing impreciso)
- Código: 26/30 (varios warnings)
- Documentación: 18/20 (cálculos correctos, dificultades poco detalladas)
- Conceptos: 8/10 (2 respuestas excelentes, 1 buena)
- **Total**: 90/100 → Excelente

### Ejemplo 2: Quiz Sesión 3

**Estudiante B**:
- Pregunta 1 (2pts): 2/2 - Dirección Flash correcta
- Pregunta 2 (2pts): 1/2 - NVIC explicado parcialmente
- Pregunta 3 (1pt): 1/1 - Pipeline 3 etapas correcto
- Pregunta 4 (1pt): 1/1 - AHB correcto
- Pregunta 5 (2pts): 0/2 - SysTick dirección incorrecta
- Pregunta 6 (2pts): 2/2 - FPU bien explicado
- **Total**: 7/10 → Bueno

---

## Actualizaciones

Las rúbricas pueden ajustarse durante el semestre si:
- Se detectan ambigüedades
- El nivel del grupo requiere ajustes
- Se recibe feedback de estudiantes

Cualquier cambio se comunica y aplica solo a evaluaciones futuras, no retroactivamente.

---

**Última actualización**: Noviembre 2025  
**Versión**: 1.0
