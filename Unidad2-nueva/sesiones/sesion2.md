# Sesión 2: Máquinas de Estados Finitos - Teoría y Diseño

**Duración**: 2 horas  
**Modalidad**: Presencial con diseño práctico

---

## Objetivos Específicos

1. **Comprender** el concepto de Máquina de Estados Finitos (MEF) y su aplicación en sistemas embebidos
2. **Diferenciar** entre MEF de Moore y MEF de Mealy
3. **Diseñar** diagramas de estados para sistemas reactivos simples

---

## Contenidos Temáticos

### 1. Introducción a Máquinas de Estados Finitos (20 minutos)

#### ¿Qué es una Máquina de Estados Finitos?

Una **Máquina de Estados Finitos (MEF)** es un modelo matemático de computación que representa el comportamiento de un sistema que puede estar en uno de un número finito de estados en cualquier momento dado.

**Características principales**:
- El sistema siempre está en exactamente UN estado
- Eventos o condiciones causan transiciones entre estados
- Las transiciones están bien definidas
- El número de estados es finito

**Analogía**: Un semáforo
- Estados: Verde, Amarillo, Rojo
- Transiciones: Temporizador expira
- Salidas: LEDs encendidos

#### ¿Por Qué Usar MEF en Sistemas Embebidos?

**Ventajas**:
1. **Claridad**: Fácil de visualizar y entender el comportamiento del sistema
2. **Modularidad**: Cada estado tiene responsabilidades bien definidas
3. **Mantenibilidad**: Fácil agregar o modificar estados
4. **Debugging**: Fácil identificar en qué estado ocurrió un problema
5. **Documentación**: El diagrama ES la documentación

**Aplicaciones típicas**:
- Protocolos de comunicación
- Interfaces de usuario
- Control de procesos
- Antirrebote de botones
- Gestión de modos de operación

### 2. Componentes de una MEF (25 minutos)

#### Estados

**Definición**: Situación o condición en la que se encuentra el sistema en un momento dado.

**Representación gráfica**: Círculo o rectángulo con nombre del estado

```
┌─────────┐
│ ESPERA  │
└─────────┘
```

**Características**:
- Cada estado representa un comportamiento específico
- El sistema permanece en un estado hasta que ocurre un evento
- Debe haber al menos un estado inicial

#### Transiciones

**Definición**: Cambio de un estado a otro, causado por un evento o condición.

**Representación gráfica**: Flecha con etiqueta de evento/condición

```
┌─────────┐   boton_presionado   ┌────────────┐
│ ESPERA  │ ──────────────────> │ ACTIVO     │
└─────────┘                      └────────────┘
```

**Características**:
- Siempre tienen un estado origen y un estado destino
- Pueden ser auto-transiciones (mismo origen y destino)
- Están etiquetadas con evento o condición

#### Eventos

**Definición**: Ocurrencias que pueden causar transiciones entre estados.

**Tipos de eventos**:
1. **Externos**: Botones, sensores, interrupciones
2. **Internos**: Temporizadores, contadores, flags
3. **Condicionales**: Resultado de comparaciones

**Ejemplos**:
- `boton_presionado`
- `timeout_30ms`
- `contador > 10`
- `temperatura_alta`

#### Acciones/Salidas

**Definición**: Operaciones realizadas en respuesta a transiciones o mientras se está en un estado.

**Tipos**:
1. **Acciones de entrada** (entry actions): Al entrar al estado
2. **Acciones de salida** (exit actions): Al salir del estado
3. **Acciones de transición**: Durante la transición
4. **Acciones continuas**: Mientras se permanece en el estado

### 3. Tipos de MEF: Moore vs Mealy (30 minutos)

#### Máquina de Moore

**Definición**: Las salidas dependen ÚNICAMENTE del estado actual.

**Características**:
- Salidas asociadas al estado, no a transiciones
- Más fácil de diseñar y entender
- Salidas cambian solo cuando cambia el estado
- Más estable (menos propensa a glitches)

**Diagrama ejemplo - Semáforo**:

```
         timeout_30s
    ┌─────────────────┐
    │                 ↓
┌───────┐         ┌───────┐
│ VERDE │         │  ROJO │
│ LED:G │──────>  │ LED:R │
└───────┘         └───────┘
  ↑  │ timeout_5s    ↑
  │  └──────┐        │
  │         ↓        │
  │    ┌─────────┐   │
  │    │AMARILLO │   │
  │    │ LED:Y   │───┘
  │    └─────────┘
  │      timeout_2s
  └──────────────────┘
```

**Tabla de estados**:

| Estado Actual | LED Verde | LED Amarillo | LED Rojo |
|---------------|-----------|--------------|----------|
| VERDE         | ON        | OFF          | OFF      |
| AMARILLO      | OFF       | ON           | OFF      |
| ROJO          | OFF       | OFF          | ON       |

**Nota**: Las salidas (LEDs) dependen solo del estado, no del evento.

#### Máquina de Mealy

**Definición**: Las salidas dependen del estado actual Y de las entradas.

**Características**:
- Salidas asociadas a transiciones
- Más flexible y compacta
- Puede tener menos estados que Moore equivalente
- Salidas pueden cambiar sin cambiar estado
- Más rápida en respuesta (no necesita esperar cambio de estado)

**Diagrama ejemplo - Control de Acceso**:

```
                PIN_correcto / abrir_puerta
    ┌──────────────────────────────────────────┐
    │                                          │
    ↓                                          │
┌────────┐  digito_ingresado / guardar   ┌─────────┐
│ESPERANDO│──────────────────────────────>│INGRESANDO│
└────────┘                                 └─────────┘
    ↑                                          │
    │   PIN_incorrecto / alarma               │
    └──────────────────────────────────────────┘
```

**Nota**: Las acciones (abrir_puerta, alarma) están en las transiciones, no en los estados.

#### Comparación Moore vs Mealy

| Aspecto | Moore | Mealy |
|---------|-------|-------|
| **Salidas** | Solo dependen del estado | Dependen del estado Y entrada |
| **Ubicación salidas** | En los estados | En las transiciones |
| **Número de estados** | Puede requerir más | Generalmente menos |
| **Estabilidad** | Más estable | Puede tener glitches |
| **Complejidad diseño** | Más simple | Más compleja |
| **Velocidad respuesta** | Una transición de retraso | Respuesta inmediata |
| **Uso típico** | Controladores simples | Protocolos de comunicación |

**¿Cuál usar?**
- **Moore**: Cuando la simplicidad y estabilidad son prioritarias
- **Mealy**: Cuando se necesita respuesta rápida o minimizar estados

### 4. Diseño de MEF (25 minutos)

#### Metodología de Diseño

**Paso 1: Identificar requisitos**
- ¿Qué debe hacer el sistema?
- ¿Cuáles son las entradas y salidas?
- ¿Qué comportamientos diferentes tiene?

**Paso 2: Identificar estados**
- Listar todas las situaciones distintas del sistema
- Cada comportamiento diferente → un estado
- Nombrar estados descriptivamente

**Paso 3: Definir estado inicial**
- ¿En qué estado inicia el sistema?
- Generalmente es un estado de reposo o espera

**Paso 4: Identificar eventos/transiciones**
- ¿Qué causa cambios entre estados?
- Definir condiciones de transición

**Paso 5: Definir acciones/salidas**
- ¿Qué hace el sistema en cada estado?
- ¿Qué salidas genera?

**Paso 6: Crear diagrama de estados**
- Dibujar estados como círculos/rectángulos
- Conectar con flechas (transiciones)
- Etiquetar transiciones con eventos
- Etiquetar estados con salidas (Moore) o transiciones con acciones (Mealy)

**Paso 7: Crear tabla de transiciones**
- Formalizar el diagrama en tabla
- Facilita implementación en código

#### Ejemplo Completo: Sistema de Antirrebote

**Requisitos**:
- Leer un botón con antirrebote
- Detectar pulsación confiable (evitar rebotes)
- Tiempo de debounce: 30ms

**Estados identificados**:
1. LIBERADO - Botón no presionado
2. DEBOUNCE_PRESS - Esperando confirmar pulsación
3. PRESIONADO - Botón confirmado presionado
4. DEBOUNCE_RELEASE - Esperando confirmar liberación

**Diagrama de estados (Moore)**:

```
┌──────────┐  boton_down    ┌────────────────┐
│LIBERADO  │───────────────>│DEBOUNCE_PRESS  │
│salida:0  │                │salida:0        │
└──────────┘<───────────────└────────────────┘
    ↑         boton_up &&        │
    │         timeout_30ms        │ boton_down &&
    │                             │ timeout_30ms
    │                             ↓
┌──────────────────┐       ┌──────────────┐
│DEBOUNCE_RELEASE  │<──────│ PRESIONADO   │
│salida:1          │       │ salida:1     │
└──────────────────┘       └──────────────┘
  boton_up              boton_down
```

**Tabla de transiciones**:

| Estado Actual | Evento | Condición | Estado Siguiente |
|---------------|--------|-----------|------------------|
| LIBERADO | timeout | boton_down | DEBOUNCE_PRESS |
| DEBOUNCE_PRESS | timeout_30ms | boton_down | PRESIONADO |
| DEBOUNCE_PRESS | timeout_30ms | boton_up | LIBERADO |
| PRESIONADO | timeout | boton_up | DEBOUNCE_RELEASE |
| DEBOUNCE_RELEASE | timeout_30ms | boton_up | LIBERADO |
| DEBOUNCE_RELEASE | timeout_30ms | boton_down | PRESIONADO |

**Tabla de salidas (Moore)**:

| Estado | Salida boton_estable |
|--------|----------------------|
| LIBERADO | 0 |
| DEBOUNCE_PRESS | 0 |
| PRESIONADO | 1 |
| DEBOUNCE_RELEASE | 1 |

---

## Actividades

### Actividad 1: Identificar Estados (15 minutos)

**Sistema**: Puerta automática de garaje

**Requisitos**:
- Botón para abrir/cerrar
- Sensor de obstáculo
- Motor para mover puerta
- Estados: Cerrada, Abriendo, Abierta, Cerrando

**Tarea**: En grupos de 2-3:
1. Identificar todos los estados posibles
2. Identificar eventos que causan transiciones
3. Dibujar diagrama de estados básico

### Actividad 2: Comparar Moore y Mealy (20 minutos)

**Sistema**: Control de LED con botón

**Requisitos Moore**:
- Estado APAGADO: LED off
- Estado ENCENDIDO: LED on
- Transición por pulsación de botón

**Requisitos Mealy**:
- Estado IDLE
- Transiciones con acciones: boton_presionado / toggle_LED

**Tarea**: 
1. Dibujar ambos diagramas
2. Comparar número de estados
3. Identificar ventajas/desventajas de cada uno

### Actividad 3: Diseñar MEF Completa (35 minutos)

**Sistema**: Controlador de temperatura

**Requisitos**:
- Leer sensor de temperatura cada segundo
- Si temp > 30°C: encender ventilador
- Si temp > 40°C: encender ventilador Y alarma
- Si temp < 25°C: apagar todo
- Histéresis de 2°C para evitar oscilaciones

**Tarea**: Diseñar MEF completa:
1. Identificar estados (al menos 3)
2. Identificar eventos y transiciones
3. Crear diagrama de estados
4. Crear tabla de transiciones
5. Decidir Moore o Mealy y justificar

---

## Evaluación Formativa

### Quiz - Sesión 2

1. **¿Cuál es la diferencia principal entre MEF de Moore y Mealy?** (2 puntos)
   - Respuesta: Moore: salidas dependen solo del estado. Mealy: salidas dependen del estado Y entrada.

2. **¿Qué componentes son esenciales en una MEF?** (2 puntos)
   - Respuesta: Estados, transiciones, eventos, salidas/acciones, estado inicial.

3. **¿Por qué se usan MEF para antirrebote de botones?** (2 puntos)
   - Respuesta: Para manejar estados temporales de debounce y validar pulsaciones con timing preciso.

4. **En el semáforo ejemplo, ¿es Moore o Mealy? Justifica.** (2 puntos)
   - Respuesta: Moore, porque las salidas (LEDs) dependen solo del estado actual, no de eventos.

5. **Dibuja una MEF simple de 2 estados para un LED que parpadea** (2 puntos)
   - Respuesta: Estados ON y OFF, transiciones por timeout.

---

## Evidencias de Aprendizaje

**Entregable**: Documento con diseño de MEF para sistema elegido:

**Opciones de sistema**:
1. Máquina expendedora (monedas, selección, entrega)
2. Lavadora (ciclos de lavado, enjuague, centrifugado)
3. Reproductor de música (play, pause, stop, next)
4. Control de acceso (PIN, timeout, intentos fallidos)

**Contenido del documento**:
1. Descripción del sistema (½ página)
2. Requisitos funcionales (lista)
3. Identificación de estados (lista con descripción)
4. Identificación de eventos (lista)
5. Diagrama de estados (dibujado a mano o digital)
6. Tabla de transiciones (formato tabla)
7. Justificación de Moore vs Mealy
8. Observaciones y casos especiales

**Formato**: PDF o documento escaneado  
**Evaluación**:
- Identificación correcta de estados: 30%
- Diagrama de estados completo: 30%
- Tabla de transiciones correcta: 20%
- Justificación técnica: 10%
- Claridad y presentación: 10%

---

## Material para Casa

### Lectura Previa a Sesión 3
- Repaso de estructuras en C: `struct`, `enum`
- Repaso de `switch-case` en C
- Concepto de variable de estado

### Ejercicio Opcional
Diseñar MEF para:
- Sistema de alarma doméstica (armada, desarmada, disparada)
- Considerar: código de desactivación, sensores, timeout

### Video Recomendado
- "State Machines in Embedded Systems" (15 min)

---

## Notas para el Instructor

### Preparación
- Pizarra o proyector para dibujar diagramas
- Ejemplos impresos de MEF conocidas (semáforo, cajero)
- Post-its de colores (opcional: para actividad de diseño físico)

### Demos Importantes
1. Mostrar video de semáforo real funcionando
2. Demo de rebote de botón con osciloscopio (si disponible)
3. Mostrar transiciones en tiempo real con LEDs

### Enfoque Pedagógico
- Empezar con ejemplos del mundo real (semáforo, puerta)
- Dibujar diagramas junto con estudiantes
- Enfatizar que MEF simplifican problemas complejos

### Errores Comunes
- Confundir estado con evento
- Omitir transiciones importantes
- No definir estado inicial
- Crear demasiados estados (sobre-diseño)
- No considerar casos excepcionales

### Flexibilidad
- Si el grupo va rápido, introducir concepto de MEF jerárquicas
- Si tienen dificultad, más ejemplos antes de actividad 3

---

**Próxima sesión**: Implementación de MEF en C  
**Preparación**: Traer diseño de MEF de la evidencia de aprendizaje
