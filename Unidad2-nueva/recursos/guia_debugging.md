# Guía de Debugging en C para Sistemas Embebidos

Estrategias y técnicas para identificar y resolver problemas en código C bare metal.

---

## 1. Problemas Comunes y Soluciones

### Problema 1: El programa no enciende LED

**Síntomas**: LED permanece apagado todo el tiempo.

**Causas posibles**:
1. Clock del GPIO no habilitado
2. Pin no configurado como salida
3. LED conectado al revés
4. Código nunca llega a escribir el pin

**Debugging paso a paso**:

```c
// Paso 1: Verificar habilitación de clock
RCC_AHB1ENR |= (1 << 0);  // Para GPIOA
// Leer registro para confirmar
uint32_t check = RCC_AHB1ENR;
// En debugger: verificar que bit 0 está en 1

// Paso 2: Verificar configuración de modo
GPIOA->MODER &= ~(0x3 << 12);  // Limpiar
GPIOA->MODER |= (0x1 << 12);   // Salida
// Leer MODER y verificar bits 12-13 = 01

// Paso 3: Escribir pin
GPIOA->BSRR = (1 << 6);  // Set PA6
// Verificar en debugger que BSRR se escribió
// Leer ODR para ver estado actual del pin
```

**Técnica**: Agregar breakpoints después de cada configuración.

---

### Problema 2: Programa se cuelga/congela

**Síntomas**: Código deja de ejecutarse, no responde.

**Causas posibles**:
1. Bucle infinito no intencional
2. Interrupción que nunca limpia flag
3. Hard fault (acceso inválido a memoria)
4. Stack overflow

**Debugging**:

```c
// Causa 1: Bucle infinito
while(condicion) {
    // Si condicion nunca cambia, bucle infinito
    // Solución: Agregar timeout
    uint32_t timeout = 100000;
    while(condicion && timeout--) {
        // ...
    }
    if (timeout == 0) {
        // Error: timeout
    }
}

// Causa 2: Interrupción sin limpiar flag
void TIM2_IRQHandler(void) {
    if (TIM2->SR & (1 << 0)) {
        // Hacer algo
        TIM2->SR &= ~(1 << 0);  // ¡CRÍTICO! Limpiar flag
    }
}

// Causa 3: Hard fault - revisar PC en debugger
// Verificar dirección de acceso inválido
```

**Técnica**: Usar debugger para pausar y ver dónde está atascado (PC register).

---

### Problema 3: Variable no se actualiza

**Síntomas**: Variable no cambia aunque debería (ISR o hardware la modifica).

**Causa**: Falta `volatile`.

**Solución**:

```c
// MAL: Compilador puede optimizar
uint32_t contador = 0;

void TIM2_IRQHandler(void) {
    contador++;  // ISR modifica
}

int main(void) {
    while(contador < 100) {  // Compilador puede optimizar a while(1)
        // ...
    }
}

// BIEN: Usar volatile
volatile uint32_t contador = 0;  // Ahora compilador no optimiza
```

**Debugging**: Inspeccionar assembly generado (`objdump -d`).

---

### Problema 4: Botón no responde

**Síntomas**: Pulsación de botón no detectada o detecta múltiples veces.

**Causas**:
1. Sin pull-up/pull-down
2. Rebotes no filtrados
3. Configuración incorrecta de EXTI

**Solución**:

```c
// Configurar pull-up
GPIOC->PUPDR &= ~(0x3 << 26);  // Limpiar
GPIOC->PUPDR |= (0x1 << 26);   // Pull-up

// Implementar debounce
volatile uint32_t ultima_pulsacion = 0;

void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & (1 << 13)) {
        uint32_t ahora = get_millis();
        if ((ahora - ultima_pulsacion) > 30) {  // Debounce 30ms
            // Pulsación válida
            ultima_pulsacion = ahora;
        }
        EXTI->PR = (1 << 13);  // Limpiar flag
    }
}
```

---

### Problema 5: Timer no genera interrupciones

**Síntomas**: Handler nunca se ejecuta.

**Causas**:
1. NVIC no habilitado
2. Flag de interrupción no limpiado
3. PSC/ARR incorrectos

**Checklist de debugging**:

```c
// 1. Verificar habilitación de clock
RCC->APB1ENR |= (1 << 0);  // TIM2

// 2. Configurar prescaler y auto-reload
TIM2->PSC = 159;   // Verificar cálculo
TIM2->ARR = 99;    // Verificar cálculo

// 3. Habilitar interrupción en timer
TIM2->DIER |= (1 << 0);  // UIE

// 4. Habilitar en NVIC
NVIC_ISER0 |= (1 << 28);  // IRQ 28 para TIM2

// 5. Iniciar timer
TIM2->CR1 |= (1 << 0);  // CEN

// 6. En handler, limpiar flag
void TIM2_IRQHandler(void) {
    if (TIM2->SR & (1 << 0)) {
        // Tu código
        TIM2->SR &= ~(1 << 0);  // ¡Crítico!
    }
}
```

---

## 2. Herramientas de Debugging

### Printf Debugging (con UART)

```c
// Requiere UART configurado
void debug_print(char *msg) {
    while(*msg) {
        UART_SendChar(*msg++);
    }
}

// Uso
debug_print("Entrando a funcion\r\n");
```

**Ventaja**: Simple, no requiere debugger
**Desventaja**: Lento, modifica timing

---

### LED como Indicador

```c
// LEDs en PA5, PA6, PA7
void debug_set_leds(uint8_t value) {
    GPIOA->ODR &= ~(0x7 << 5);  // Limpiar
    GPIOA->ODR |= ((value & 0x7) << 5);  // Establecer
}

// Uso: mostrar estado binario
debug_set_leds(0b101);  // PA5=1, PA6=0, PA7=1
```

**Ventaja**: Visual, no afecta timing mucho
**Desventaja**: Limitado a pocos bits

---

### Breakpoints Condicionales

En GDB o IDE:

```gdb
# Breakpoint que solo se activa cuando contador >= 100
break main.c:45 if contador >= 100

# Breakpoint con comando automático
break funcion
commands
  print variable
  continue
end
```

---

### Watchpoints (Data Breakpoints)

```gdb
# Pausar cuando variable cambia
watch variable_importante

# Pausar cuando memoria específica cambia
watch *(uint32_t*)0x40020000
```

---

## 3. Análisis de Assembly Generado

### Generar archivo .lst

```bash
arm-none-eabi-objdump -d -S programa.elf > programa.lst
```

### Revisar optimización

```c
// Código C
uint32_t suma(uint32_t a, uint32_t b) {
    return a + b;
}

// Assembly generado (gcc -O0)
push {lr}
add r0, r0, r1
pop {pc}

// Assembly generado (gcc -O2)
add r0, r0, r1
bx lr
// Más eficiente: sin push/pop innecesario
```

---

## 4. Errores de Compilación Comunes

### Error: "undefined reference to..."

**Causa**: Función declarada pero no implementada, o archivo no linkado.

**Solución**:
```bash
# Asegurar que todos los .c están en compilación
arm-none-eabi-gcc main.c gpio_driver.c timer_driver.c -o programa.elf
```

---

### Warning: "implicit declaration of function"

**Causa**: Función usada sin declarar (falta #include).

**Solución**:
```c
#include "gpio_driver.h"  // Declaraciones de funciones GPIO
```

---

### Error: "assignment to 'volatile uint32_t *' from incompatible pointer type"

**Causa**: Puntero no volatile asignado a volatile.

**Solución**:
```c
// MAL
uint32_t *ptr = (uint32_t*)0x40020000;

// BIEN
volatile uint32_t *ptr = (volatile uint32_t*)0x40020000;
```

---

## 5. Checklist de Debugging Sistemático

### Antes de buscar el bug:

1. ☐ ¿El código compila sin errores ni warnings?
2. ☐ ¿Los valores de PSC/ARR del timer son correctos?
3. ☐ ¿Todos los clocks están habilitados?
4. ☐ ¿Los pines GPIO están configurados correctamente?
5. ☐ ¿Las interrupciones están habilitadas en NVIC?
6. ☐ ¿Los flags de interrupción se limpian en ISR?
7. ☐ ¿Variables compartidas entre ISR y main son volatile?
8. ☐ ¿El hardware está conectado correctamente?

### Durante debugging:

1. **Aislar el problema**: ¿En qué función/línea ocurre?
2. **Verificar suposiciones**: ¿Las variables tienen valores esperados?
3. **Simplificar**: Comentar código hasta encontrar mínimo reproducible
4. **Instrumentar**: Agregar LEDs, prints, breakpoints
5. **Documentar**: Anotar qué se probó y resultados

---

## 6. Errores de Runtime Comunes

### Hard Fault

**Síntomas**: Programa salta a HardFault_Handler.

**Causas**:
- Acceso a memoria no alineada
- Acceso a dirección inválida
- División por cero con DIV habilitado

**Debugging**:
```c
void HardFault_Handler(void) {
    // En debugger, revisar:
    // - SP: stack pointer
    // - PC: program counter cuando ocurrió fault
    // - LR: link register
    
    // Código simple para debug
    volatile uint32_t *stack_ptr = (uint32_t*)__get_MSP();
    volatile uint32_t pc_at_fault = stack_ptr[6];  // PC guardado
    
    while(1);  // Pausar aquí para inspeccionar
}
```

---

### Stack Overflow

**Síntomas**: Comportamiento errático, variables corruptas.

**Causas**:
- Recursión profunda
- Arrays locales muy grandes
- Stack size insuficiente en linker script

**Detección**:
```c
// En linker script, aumentar stack
_Min_Stack_Size = 0x1000;  // 4KB

// Llenar stack con patrón
memset(_estack - 0x1000, 0xAA, 0x1000);

// Verificar periódicamente
void check_stack(void) {
    extern uint32_t _estack;
    uint8_t *stack_end = (uint8_t*)(&_estack - 0x1000);
    
    if (*stack_end != 0xAA) {
        // Stack overflow detectado!
    }
}
```

---

## 7. Mejores Prácticas

1. **Inicializar variables**: No asumir valor 0
2. **Usar const cuando sea posible**: Ayuda al compilador
3. **Validar parámetros**: En funciones críticas
4. **Agregar asserts**: Para condiciones que deben cumplirse
5. **Documentar asunciones**: En comentarios
6. **Usar static**: Para funciones/variables privadas de módulo
7. **Evitar números mágicos**: Usar #define o enum

```c
// Ejemplo de buenas prácticas
#define LED_PIN 6
#define DEBOUNCE_TIME_MS 30

static volatile uint32_t tiempo_ultima_pulsacion = 0;

void procesar_boton(void) {
    // Validar parámetros implícitos
    if (!boton_inicializado) {
        return;  // Protección
    }
    
    uint32_t ahora = get_millis();
    
    // Constante nombrada en lugar de 30
    if ((ahora - tiempo_ultima_pulsacion) > DEBOUNCE_TIME_MS) {
        // Procesar
    }
}
```

---

## 8. Recursos Adicionales

- **GDB Cheat Sheet**: https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf
- **ARM Cortex-M4 Debug**: Ver PM0214 capítulo 10
- **Valgrind** (para simulación en PC): Detecta memory leaks
- **Static analyzers**: cppcheck, clang-tidy

---

## 9. Ejercicio de Debugging

**Encuentra los 5 bugs en este código**:

```c
uint32_t millis;  // BUG 1: Falta volatile

void TIM2_IRQHandler(void) {
    millis++;
    // BUG 2: No limpia flag SR
}

void delay_ms(uint32_t ms) {
    uint32_t inicio = millis;
    while((millis - inicio) < ms);  // BUG 3: OK si millis es volatile
}

void config_gpio(void) {
    // BUG 4: No habilita clock
    GPIOA->MODER |= (1 << 12);  // BUG 5: No limpia bits primero
}
```

**Soluciones**: Ver al final del documento.

---

**Soluciones al ejercicio**:
1. `volatile uint32_t millis`
2. Agregar `TIM2->SR &= ~(1 << 0);`
3. OK si millis es volatile
4. Agregar `RCC->AHB1ENR |= (1 << 0);`
5. Primero `GPIOA->MODER &= ~(0x3 << 12);`
