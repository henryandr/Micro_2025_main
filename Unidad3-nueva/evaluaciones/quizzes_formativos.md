# Quizzes Formativos - Unidad 3

## Quiz Sesión 1: USART Básico

1. ¿Qué registro configura el baudrate en USART?
   - **Respuesta**: BRR (Baud Rate Register)

2. ¿Qué flag indica que hay dato recibido?
   - **Respuesta**: RXNE (RX Not Empty)

3. ¿Cuál es la fórmula para calcular BRR?
   - **Respuesta**: BRR = fCK / (16 × baudrate)

4. ¿Por qué es importante configurar correctamente los pines AF?
   - **Respuesta**: Para que el periférico controle los pines GPIO

5. ¿Qué sucede si el baudrate está mal configurado?
   - **Respuesta**: Datos corruptos o no se comunica

---

## Quiz Sesión 2: USART Interrupciones

1. ¿Por qué no habilitar TXE permanentemente?
   - **Respuesta**: Se dispara continuamente cuando buffer vacío

2. ¿Qué es un buffer circular?
   - **Respuesta**: Estructura FIFO que "envuelve" al llegar al final

3. ¿Cómo se limpia el flag RXNE?
   - **Respuesta**: Leyendo el registro DR

4. ¿Por qué usar `volatile` en variables compartidas?
   - **Respuesta**: Evita optimizaciones incorrectas del compilador

5. ¿Cuál es la ventaja principal de usar interrupciones?
   - **Respuesta**: CPU libre para otras tareas durante comunicación

---

## Quiz Sesión 3: I2C Fundamentos

1. ¿Qué significa ACK en I2C?
   - **Respuesta**: Acknowledgement (reconocimiento de recepción)

2. ¿Cómo se genera una condición START?
   - **Respuesta**: SDA cae mientras SCL está alto

3. ¿Cuántos dispositivos máximo en bus I2C estándar?
   - **Respuesta**: 128 (direcciones 7-bit: 0x00-0x7F)

4. ¿Por qué leer SR1 y SR2 después de ADDR?
   - **Respuesta**: Para limpiar el flag ADDR

5. ¿Qué indica un NACK del slave?
   - **Respuesta**: No reconoce dirección o no está listo

---

## Quizzes para Sesiones 4-8 (similar estructura)

Cada sesión incluye 5 preguntas formativas evaluando conceptos clave.

**Nota**: Estos quizzes no tienen calificación numérica, son formativos para autoevaluación.
