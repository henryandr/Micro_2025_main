# Reto 1

# Evaluación Unidad 2

Microproyecto Unidad 2

Programación en Lenguaje C

## **Nombre del Proyecto**: Reproducción de Animación en Matriz LED 8x8

### **Descripción**:

Desarrollar un programa en lenguaje C que muestre una animación en una matriz de LED 8x8. La animación debe estar compuesta por una secuencia de imágenes (frames) que se reproducen de forma continua, simulando el efecto de un video.

### **Requisitos**:

1. **Programación en C**: Todo el desarrollo debe realizarse en lenguaje C, haciendo uso de estructuras y buenas prácticas de programación.
2. **Almacenamiento de Datos en Memoria**: Definir y almacenar los cuadros de la animación como arreglos en memoria, utilizando un formato adecuado para la matriz de LED.
3. **Creación de Tablas de Frames**: Implementar una estructura que permita organizar y recorrer los diferentes cuadros de la animación.
4. **Transferencia de Datos a la Matriz**: Enviar los datos de cada cuadro a la matriz LED para ser visualizados de forma secuencial.
5. **Uso de Máquina de Estados**: Es obligatorio el uso de una máquina de estados para controlar la lógica de reproducción de la animación (inicio, reproducción, pausa, final, reinicio, etc.).
6. **Control de Tiempo**: Implementar mecanismos para definir el intervalo de tiempo entre cada cuadro, simulando una tasa de cuadros por segundo (FPS).

### Desafíos Adicionales (Opcionales):

- Diseñar una animación original de al menos 10 cuadros.
- Permitir cambiar la velocidad de la animación durante la ejecución.
- Integrar efectos visuales adicionales (como parpadeo o brillo).
- Agregar una funcionalidad de pausa o reinicio mediante un botón físico.

## Requerimientos

Este reto consiste en un proyecto aplicado donde los estudiantes utilizarán la tarjeta de desarrollo STM32F407VET6. Este proyecto exigirá a los estudiantes implementar una máquina de estados que controle un sistema basado en los módulos estudiados (GPIO, SysTick, Timers e Interrupciones), integrando la técnica de multiplexación para manejar eficientemente las señales de entrada y salida.

El proyecto se evaluará en varias etapas:

1. **Diseño:** Se valorará la claridad y eficiencia del diseño de la máquina de estados y su integración con los módulos del microcontrolador, incluyendo la implementación de la técnica de multiplexación.
2. **Implementación:** Se evaluará la correcta implementación en la tarjeta de desarrollo, verificando el funcionamiento de la máquina de estados, la técnica de multiplexación, y la interacción con los periféricos.
3. **Documentación:** Los estudiantes deberán presentar una documentación detallada del proyecto, explicando el diseño, las decisiones tomadas, la implementación de la multiplexación, y los resultados obtenidos.
4. **Presentación:** Finalmente, los estudiantes presentarán su proyecto, demostrando su funcionamiento y respondiendo preguntas sobre los conceptos aplicados.

Esta evaluación práctica permitirá a los estudiantes consolidar su comprensión de los conceptos teóricos a través de la aplicación directa en un proyecto real, asegurando una integración efectiva de la multiplexación en el control de las señales.

### **Rúbrica de Evaluación**

| **Criterios** | **Insuficiente [0 - 3)** | **Básico [3 - 3,5)** | **Bueno [3,5 - 4)** | **Muy bueno [4 - 4,5)** | **Excelente [4,5 - 5]** |
| --- | --- | --- | --- | --- | --- |
| **Diseño del Proyecto** | 0: No entrega evidencia. 1: Evidencia insuficiente. 2-3: Diseño en desarrollo, incompleto, con deficiencias importantes. | Diseño básico, funcional, pero con errores o carencias que afectan la estructura e integración de los módulos y multiplexación. | Diseño funcional, con estructura adecuada, aunque con algunos errores menores en la integración de los módulos. | Diseño muy bien estructurado, con integración correcta de los módulos y la multiplexación, algunos detalles menores. | Diseño claro, eficiente, bien estructurado, con integración óptima de los módulos y la multiplexación. |
| **Implementación Técnica** | 0: No entrega evidencia. 1: Evidencia insuficiente. 2-3: Implementación incompleta o con errores graves que impiden el funcionamiento correcto del sistema. | Implementación funcional, pero con errores importantes que afectan la estabilidad o el rendimiento del sistema. | Implementación funcional, con errores menores que no afectan significativamente el rendimiento del sistema. | Implementación completa, funcional y con muy pocos errores menores. | Implementación completa y funcional sin errores, con correcta utilización de la tarjeta UPB. |
| **Integración de la Multiplexación** | 0: No entrega evidencia. 1: Evidencia insuficiente. 2-3: Multiplexación mal implementada o ausente, causando ineficiencias en el sistema. | Implementación básica de la multiplexación, con margen significativo de mejora en la optimización y eficiencia. | Multiplexación bien implementada, aunque con espacio para una mejor optimización y uso de recursos. | Multiplexación implementada de manera efectiva, optimizando el uso de recursos con pocos detalles menores. | Multiplexación implementada óptimamente, maximizando el uso de recursos y minimizando hardware necesario. |
| **Documentación** | 0: No entrega evidencia. 1: Evidencia insuficiente. 2-3: Documentación incompleta, confusa, con errores que dificultan la comprensión del proyecto. | Documentación básica, pero con errores que limitan la comprensión clara del proyecto. | Documentación clara y bien estructurada, aunque con algunos detalles faltantes o errores menores. | Documentación muy bien estructurada, clara, detallada, con explicaciones precisas y pocos errores. | Documentación detallada, clara y bien estructurada, explicando el diseño, decisiones y resultados con precisión. |

### **Puntaje Final:**

- **Insuficiente [0 - 3):** Evidencia insuficiente o en desarrollo, con fallas significativas.
- **Básico [3 - 3,5):** Comprensión básica con carencias importantes.
- **Bueno [3,5 - 4):** Buen dominio con algunos errores menores.
- **Muy bueno [4 - 4,5):** Dominio sólido con pocos errores menores.
- **Excelente [4,5 - 5]:** Dominio completo con ejecución impecable.