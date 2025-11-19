# Quizzes Formativos - Unidad 1

**Propósito**: Evaluación continua del aprendizaje en cada sesión.

**Características**:
- 5-7 preguntas por sesión
- 10 minutos al final de clase
- 10 puntos por quiz
- Puede ser individual o en parejas

---

## Quiz Sesión 1: Introducción a Procesadores

**Tiempo**: 10 minutos  
**Puntuación**: 10 puntos

### 1. Nombra los tres componentes principales de un procesador (3 puntos)

**Respuesta**: 
- Unidad de Control (CU)
- Unidad Aritmético-Lógica (ALU)
- Registros

---

### 2. ¿Cuál es la diferencia principal entre microprocesador y microcontrolador? (3 puntos)

**Respuesta**: Microcontrolador integra CPU, memoria y periféricos en un solo chip; microprocesador es solo la CPU y requiere componentes externos.

---

### 3. Ordena las etapas del ciclo de ejecución: Execute, Fetch, Decode (2 puntos)

**Respuesta**: Fetch → Decode → Execute

---

### 4. ¿Para qué aplicación es más apropiado un microcontrolador? (2 puntos)

a) Servidor web  
b) Control de temperatura de un horno  
c) Edición de video  
d) Videojuegos 3D

**Respuesta**: b

---

## Quiz Sesión 2: Harvard vs von Neumann, RISC vs CISC

**Tiempo**: 10 minutos  
**Puntuación**: 10 puntos

### 1. Explica la diferencia entre arquitectura Harvard y von Neumann (2 puntos)

**Respuesta**: Harvard tiene memorias y buses separados para instrucciones y datos; von Neumann comparte una sola memoria y bus.

---

### 2. ¿Por qué ARM Cortex-M usa arquitectura RISC? (2 puntos)

**Respuesta**: Mayor eficiencia energética, instrucciones simples, pipeline eficiente, menor consumo.

---

### 3. Menciona 2 ventajas de la arquitectura Harvard (2 puntos)

**Respuesta posible**:
- Acceso simultáneo a instrucciones y datos
- Mayor velocidad
- Código protegido (no se puede modificar accidentalmente)

---

### 4. En código ARM, ¿por qué se usan instrucciones LDR y STR separadas de operaciones aritméticas? (2 puntos)

**Respuesta**: Filosofía RISC: Load/Store architecture. Solo instrucciones de carga/almacenamiento acceden a memoria; operaciones aritméticas trabajan exclusivamente con registros.

---

### 5. ¿Qué significa "Harvard modificada"? (2 puntos)

**Respuesta**: Arquitectura que internamente tiene buses separados (Harvard) pero presenta espacio de direcciones unificado al programador (como von Neumann), combinando ventajas de ambas.

---

## Quiz Sesión 3: Arquitectura ARM Cortex-M4

**Tiempo**: 10 minutos  
**Puntuación**: 10 puntos

### 1. ¿En qué rango de direcciones está típicamente la memoria Flash en STM32F407? (2 puntos)

**Respuesta**: 0x08000000 - 0x080FFFFF (o 0x00000000 alias)

---

### 2. ¿Qué es el NVIC y para qué sirve? (2 puntos)

**Respuesta**: Nested Vectored Interrupt Controller - controla interrupciones, gestiona prioridades y anidación automática.

---

### 3. ¿Cuántas etapas tiene el pipeline del Cortex-M4? (1 punto)

**Respuesta**: 3 etapas (Fetch, Decode, Execute)

---

### 4. ¿Qué bus (AHB/APB1/APB2) es el más rápido? (1 punto)

**Respuesta**: AHB

---

### 5. ¿Dónde se encuentra el SysTick en el mapa de memoria? (2 puntos)

**Respuesta**: 0xE000E010 (región de periféricos privados del core)

---

### 6. ¿Qué significa que el Cortex-M4 tenga FPU? (2 puntos)

**Respuesta**: Floating Point Unit - unidad de punto flotante para operaciones matemáticas con números decimales (precisión simple IEEE 754).

---

## Quiz Sesión 4: Registros y Assembly Básico

**Tiempo**: 10 minutos  
**Puntuación**: 10 puntos

### 1. ¿Qué hace la instrucción `MOV R0, R1`? (1 punto)

**Respuesta**: Copia el contenido de R1 a R0 (R0 = R1)

---

### 2. ¿Cuál es la diferencia entre MOV y LDR? (2 puntos)

**Respuesta**: MOV transfiere datos entre registros o carga valores inmediatos pequeños; LDR carga datos desde memoria a registro.

---

### 3. ¿Para qué se usa el registro LR? (2 puntos)

**Respuesta**: Link Register - almacena la dirección de retorno cuando se llama a una función (con BL).

---

### 4. ¿Qué flags se modifican con la instrucción CMP? (2 puntos)

**Respuesta**: N (Negative), Z (Zero), C (Carry), V (oVerflow) en el registro APSR.

---

### 5. Escribe código Assembly que almacene el valor 42 en la dirección 0x20000000 (3 puntos)

**Respuesta**:
```asm
MOV R0, #42
LDR R1, =0x20000000
STR R0, [R1]
```

o equivalente correcto.

---

## Quiz Sesión 5: Operaciones Aritméticas y Lógicas

**Tiempo**: 10 minutos  
**Puntuación**: 10 puntos

### 1. ¿Qué hace `LSL R0, R0, #2`? (2 puntos)

**Respuesta**: Desplaza R0 a la izquierda 2 bits (equivale a multiplicar por 4).

---

### 2. ¿Cuál es la diferencia entre `UDIV` y `SDIV`? (2 puntos)

**Respuesta**: UDIV divide números sin signo; SDIV divide números con signo (considerando bit de signo).

---

### 3. ¿Cómo se limpia el bit 3 de un registro sin afectar otros bits? (2 puntos)

**Respuesta**: `BIC R0, R0, #(1<<3)` o `AND R0, R0, #~(1<<3)`

---

### 4. ¿Qué registros deben preservarse en una función según AAPCS? (2 puntos)

**Respuesta**: R4-R11 (registros callee-saved)

---

### 5. Escribe código que multiplique R0 por 5 sin usar MUL (2 puntos)

**Respuesta posible**:
```asm
LSL R1, R0, #2    ; R1 = R0 * 4
ADD R0, R1, R0    ; R0 = R1 + R0 = 5*R0
```

---

## Quiz Sesión 6: Control de Flujo

**Tiempo**: 10 minutos  
**Puntuación**: 10 puntos

### 1. ¿Qué diferencia hay entre `B` y `BL`? (2 puntos)

**Respuesta**: B es salto incondicional; BL (Branch with Link) salto que guarda dirección de retorno en LR (para llamar funciones).

---

### 2. ¿Qué hace la instrucción `CMP R0, #10`? (2 puntos)

**Respuesta**: Compara R0 con 10 (realiza R0-10 y actualiza flags sin modificar R0).

---

### 3. ¿Por qué se necesita un startup code? (2 puntos)

**Respuesta**: Para inicializar el sistema antes de main(): copiar .data a RAM, inicializar .bss a cero, configurar stack pointer.

---

### 4. ¿Qué es `volatile` en C y por qué se usa con registros de hardware? (2 puntos)

**Respuesta**: Indica al compilador que el valor puede cambiar fuera del control del programa (por hardware), evitando optimizaciones incorrectas.

---

### 5. Escribe un bucle while en Assembly que cuente de 10 a 0 (2 puntos)

**Respuesta**:
```asm
MOV R0, #10
bucle:
    CMP R0, #0
    BLE fin
    SUB R0, R0, #1
    B bucle
fin:
```

---

## Quiz Sesión 7: GPIO y Timer

**Tiempo**: 10 minutos  
**Puntuación**: 10 puntos

### 1. ¿Por qué es necesario habilitar el clock de un periférico? (2 puntos)

**Respuesta**: Los periféricos no funcionan sin clock habilitado (ahorro energético). Se habilita mediante RCC (Reset and Clock Control).

---

### 2. ¿Qué hace el registro BSRR y por qué es preferible sobre ODR para cambiar un pin? (2 puntos)

**Respuesta**: Bit Set/Reset Register permite establecer o limpiar bits atómicamente sin read-modify-write, evitando problemas de concurrencia.

---

### 3. ¿Cómo se calcula el periodo de un timer dado PSC y ARR? (3 puntos)

**Respuesta**: 
```
Frecuencia_Timer = Frecuencia_Clock / (PSC + 1)
Periodo = (ARR + 1) / Frecuencia_Timer
```

---

### 4. ¿Qué diferencia hay entre push-pull y open-drain? (2 puntos)

**Respuesta**: Push-pull puede generar tanto HIGH como LOW activamente; open-drain solo puede pull-down (necesita pull-up externo para HIGH).

---

### 5. Nombra los 5 registros principales de configuración de GPIO (1 punto)

**Respuesta**: MODER, OTYPER, OSPEEDR, PUPDR, (ODR o IDR o BSRR - cualquiera de los de datos)

---

## Quiz Sesión 8: Interrupciones

**Tiempo**: 10 minutos  
**Puntuación**: 10 puntos

### 1. ¿Qué es una interrupción? (2 puntos)

**Respuesta**: Evento que suspende temporalmente la ejecución normal del programa para atender un evento urgente, luego retorna.

---

### 2. ¿Qué es el NVIC? (2 puntos)

**Respuesta**: Nested Vectored Interrupt Controller - controlador de interrupciones que gestiona prioridades, habilitación y anidación automática.

---

### 3. ¿Cuál es la ventaja de usar interrupciones sobre polling? (2 puntos)

**Respuesta**: CPU no desperdicia ciclos verificando estado constantemente; responde inmediatamente cuando ocurre evento; menor consumo energético.

---

### 4. ¿Qué es la vector table? (2 puntos)

**Respuesta**: Tabla de punteros a funciones (ISR) ubicada al inicio de Flash, consultada por hardware cuando ocurre interrupción.

---

### 5. En un ISR, ¿por qué es importante limpiar el flag de pending? (2 puntos)

**Respuesta**: Si no se limpia, la interrupción se dispara inmediatamente de nuevo al retornar, causando bucle infinito.

---

## Estadísticas de Quizzes

Cada quiz vale 10 puntos, total 8 quizzes = 80 puntos

**Conversión a escala 30%** (peso en evaluaciones formativas):
Puntuación_Final = (Total_Quizzes / 80) * 30

**Ejemplo**: Estudiante obtiene 65/80 puntos
Puntuación_Final = (65/80) * 30 = 24.375 ≈ 24.4%

---

**Notas para el Instructor**:
- Aplicar al final de cada sesión
- Revisar rápidamente para identificar conceptos mal comprendidos
- Retroalimentación inmediata en siguiente sesión
- Si >50% del grupo falla una pregunta, reforzar ese tema
