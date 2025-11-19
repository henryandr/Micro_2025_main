# Evaluación Sumativa - Unidad 1

**Propósito**: Evaluar de manera integral las competencias desarrolladas en la Unidad 1.

**Momento**: Sesión 8 (final de la unidad)  
**Duración**: 45 minutos implementación + 5 min presentación  
**Peso**: 30% de la calificación total de la unidad

---

## Proyecto Final Integrador

### Enunciado del Proyecto

Desarrollar un **Sistema de Control con Múltiples Periféricos** que demuestre dominio de:
- Configuración de GPIO
- Uso de Timers
- Manejo de interrupciones
- Programación en C bare metal
- Comprensión de arquitectura ARM

### Requisitos Funcionales

El sistema debe:

1. **LED1** (PA6): Parpadear cada 1 segundo usando TIM2
2. **LED2** (PA7): Controlado por botón mediante interrupción EXTI
3. **Botón** (PA0): Genera interrupción en flanco descendente
4. **Comportamiento especial**: Al presionar botón:
   - LED1 se detiene (apagado) por 5 segundos
   - LED2 hace toggle (cambia estado)
   - Después de 5 segundos, LED1 reanuda parpadeo normal

### Requisitos Técnicos

1. **Estructura del Código**:
   - Drivers separados: `gpio_driver.c`, `timer_driver.c`, `irq_driver.c`
   - Headers correspondientes (.h)
   - main.c que integre todo
   
2. **Funciones Requeridas**:
   ```c
   // GPIO Driver
   void GPIO_Init(GPIO_TypeDef *port, uint8_t pin, uint8_t mode);
   void GPIO_Write(GPIO_TypeDef *port, uint8_t pin, uint8_t value);
   uint8_t GPIO_Read(GPIO_TypeDef *port, uint8_t pin);
   void GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin);
   
   // Timer Driver
   void Timer_Init(uint16_t prescaler, uint32_t period);
   void Delay_ms(uint32_t ms);
   
   // IRQ Driver
   void EXTI_Init(uint8_t pin);
   void EXTI0_IRQHandler(void);  // ISR
   ```

3. **Configuración**:
   - Todos los periféricos deben tener clock habilitado
   - GPIO configurado correctamente (modo, tipo, velocidad, pull-up/down)
   - Timer configurado para precisión de 1 ms
   - EXTI configurado en flanco descendente
   - NVIC habilitado para las interrupciones necesarias

4. **Documentación del Código**:
   - Comentarios explicando cada sección
   - Encabezado de funciones con descripción, parámetros y retorno
   - Referencias a registros del datasheet donde corresponda

### Entregables

1. **Código Fuente**:
   - Todos los archivos .c y .h
   - Makefile funcional (opcional pero recomendado)
   - Compilable sin errores

2. **Documento Técnico** (máximo 2 páginas):
   - **Arquitectura**: Diagrama de bloques mostrando relación entre módulos
   - **Decisiones Técnicas**: Por qué eligió cierta implementación
   - **Cálculos**: Cómo calculó prescaler y ARR del timer para 1ms
   - **Dificultades**: Problemas encontrados y cómo los resolvió
   - **Pruebas**: Cómo verificó que funciona correctamente

3. **Presentación Oral** (5 minutos por estudiante):
   - Demo en vivo del sistema funcionando
   - Explicación breve del código más importante
   - Responder 3 preguntas conceptuales aleatorias

### Preguntas Conceptuales (Banco de Preguntas)

Durante la presentación, se harán 3 preguntas aleatorias del siguiente banco:

**Arquitectura**:
1. ¿Qué diferencia hay entre Harvard y von Neumann?
2. ¿Por qué ARM Cortex-M4 usa arquitectura RISC?
3. ¿Qué ventajas tiene arquitectura Harvard modificada?

**Registros y Memory**:
4. ¿Para qué sirve el registro LR?
5. ¿Qué es el registro APSR y qué contiene?
6. ¿En qué región de memoria están los periféricos?

**Assembly**:
7. ¿Qué hace la instrucción `LDR R0, [R1, #4]`?
8. ¿Cuál es la diferencia entre `MOV` y `LDR`?
9. ¿Cómo se implementa un IF-ELSE en Assembly?

**Periféricos**:
10. ¿Por qué se debe habilitar el clock de un periférico?
11. ¿Qué registros se deben configurar para un pin GPIO de salida?
12. ¿Cómo se calcula el periodo de un timer?

**Interrupciones**:
13. ¿Qué es el NVIC?
14. ¿Cuál es la ventaja de usar interrupciones sobre polling?
15. ¿Qué es la vector table?

---

## Criterios de Evaluación (Ver rubricas.md para detalle)

| Componente | Peso | Descripción |
|------------|------|-------------|
| Funcionalidad | 40% | El sistema cumple todos los requisitos |
| Código | 30% | Calidad, claridad, organización, comentarios |
| Documentación | 20% | Documento técnico completo y claro |
| Conceptos | 10% | Respuestas a preguntas orales |

### Desglose de Funcionalidad (40%)

- LED1 parpadea correctamente cada 1s: 10%
- LED2 responde a botón: 10%
- Interrupción funciona correctamente: 10%
- Comportamiento especial (pausa de 5s): 10%

### Desglose de Código (30%)

- Estructura modular (drivers separados): 8%
- Funciones bien implementadas: 10%
- Comentarios y documentación: 7%
- Compilación sin errores/warnings: 5%

### Desglose de Documentación (20%)

- Arquitectura explicada: 5%
- Decisiones técnicas justificadas: 5%
- Cálculos correctos: 5%
- Dificultades y soluciones: 5%

### Desglose de Conceptos (10%)

- Cada pregunta: 3.33% (promedio de 3 preguntas)

---

## Niveles de Logro

**Excelente (90-100%)**:
- Todas las funciones implementadas y funcionando perfectamente
- Código limpio, modular, bien documentado
- Documento técnico completo y profesional
- Respuestas conceptuales correctas y con profundidad

**Bueno (75-89%)**:
- Funcionalidad completa con errores menores
- Código funcional pero puede mejorar en organización o comentarios
- Documento adecuado pero falta algún detalle
- Respuestas conceptuales correctas pero básicas

**Suficiente (60-74%)**:
- Funcionalidad parcial (faltan características o tienen errores)
- Código funciona pero desorganizado o poco documentado
- Documento incompleto
- Respuestas conceptuales con errores o incompletas

**Insuficiente (<60%)**:
- Funcionalidad incompleta o no funciona
- Código con errores graves o no compila
- Documentación ausente o muy deficiente
- No comprende conceptos fundamentales

---

## Política de Evaluación

### Compilación

- **Código no compila**: Máximo 40% en "Código" (puede obtener puntos en funcionalidad si corrige en vivo, documentación y conceptos)
- **Advertencias (warnings)**: -2 puntos por cada warning no justificado (máximo -10 puntos)

### Plagio

- Código idéntico a otro estudiante: 0 puntos en el proyecto
- Código copiado de internet sin comprensión: 0 puntos + revisión académica
- Uso de bibliotecas prohibidas (HAL de ST): Penalización de 20 puntos

### Presentación

- No presenta: 0 puntos en "Presentación y Conceptos" (pierde 10%)
- Llega tarde: -5 puntos
- Demo no funciona: 0 en funcionalidad, pero puede obtener puntos en código/documentación

### Extensiones

- **Características opcionales** (dan puntos extra hasta máximo 10%):
  - Display adicional mostrando contador
  - Múltiples modos de operación seleccionables
  - Optimización de consumo energético (uso de sleep modes)
  - Control de intensidad LED con PWM
  - Comunicación UART para debug

---

## Recomendaciones para Estudiantes

### Antes de Entregar

- [ ] Compilar sin errores ni warnings
- [ ] Probar todas las funcionalidades
- [ ] Revisar que todos los comentarios estén presentes
- [ ] Verificar que documento técnico esté completo
- [ ] Practicar demo (ensayar 2-3 veces)
- [ ] Repasar conceptos teóricos del banco de preguntas

### Durante la Presentación

- Tener código abierto para mostrar secciones específicas si se solicita
- Explicar decisiones técnicas tomadas
- Si algo no funciona, explicar qué se intentó
- Ser honesto sobre dificultades encontradas

### Gestión del Tiempo

- Implementación: 30-40 minutos
- Documentación: 10-15 minutos
- Verificación y pruebas: 5-10 minutos
- No hay tiempo para aprender durante la evaluación, solo demostrar lo aprendido

---

## Ejemplo de Rúbrica Completada

Ver `rubricas.md` para la rúbrica detallada con criterios específicos por nivel.

---

## Fecha y Logística

**Sesión**: 8 (última de la unidad)  
**Duración**: 2 horas total
- 5 min: Instrucciones y distribución
- 40 min: Implementación
- 60 min: Presentaciones (5 min por estudiante, ~12 estudiantes)
- 15 min: Retroalimentación general y cierre

**Materiales necesarios**:
- Computadora con toolchain instalado
- Tarjeta STM32F407
- Cable de programación
- Proyector para presentaciones (opcional)

---

**Nota Final**: Esta evaluación representa el cierre de la Unidad 1 y debe demostrar integración de todos los conocimientos adquiridos. Es más importante mostrar comprensión profunda que memorización superficial.
