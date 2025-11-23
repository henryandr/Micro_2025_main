# Evaluación Sumativa - Unidad 3

## Parte Teórica (30 puntos)

### Sección A: Conocimientos (15 puntos)

**Pregunta 1** (5 pts): Compare USART, I2C, SPI y CAN. Complete la tabla:

| Protocolo | Cables | Velocidad | Ventaja Principal |
|-----------|--------|-----------|-------------------|
| USART | | | |
| I2C | | | |
| SPI | | | |
| CAN | | | |

**Pregunta 2** (5 pts): Explique el arbitraje de bus en CAN.

**Pregunta 3** (5 pts): ¿Qué son CPOL y CPHA en SPI? De ejemplos.

### Sección B: Aplicación (15 puntos)

**Pregunta 4** (8 pts): Para un sistema que debe:
- Leer 5 sensores diferentes
- Mostrar datos en display
- Enviar logs a PC
- Comunicarse con otros módulos

¿Qué protocolo usaría para cada función? Justifique.

**Pregunta 5** (7 pts): Analice el siguiente código y encuentre 3 errores:

```c
void I2C_Write(uint8_t addr, uint8_t data) {
    I2C1->CR1 |= I2C_CR1_START;
    I2C1->DR = addr;
    I2C1->DR = data;
    I2C1->CR1 |= I2C_CR1_STOP;
}
```

---

## Parte Práctica (70 puntos)

### Proyecto Final

Ver especificaciones en Sesión 8.

**Evaluación**:
- Funcionalidad: 25 pts
- Código: 20 pts
- Integración: 10 pts
- Documentación: 10 pts
- Presentación: 5 pts

**Total Unidad 3**: 100 puntos
**Nota mínima de aprobación**: 60 puntos
