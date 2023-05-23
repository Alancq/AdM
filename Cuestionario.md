# Arquitectura de Microprocesadores-AdM cohorte 19
Elmer Alan Cornejo Quito

## Preguntas orientadoras
1. Describa brevemente los diferentes perfiles de familias de
microprocesadores/microcontroladores de ARM. Explique alguna de sus diferencias
características.
## Cortex M
1. Describa brevemente las diferencias entre las familias de procesadores Cortex M0, M3 y
M4.

Las diferencias entre las familias de procesadores Cortex M0, M3 y M4 son las siguientes:
Cortex M0: Diseñado para aplicaciones de bajo consumo de energía y costos reducidos, es adecuado para sistemas embebidos simples y de baja potencia.
Cortex M3: Ofrece un rendimiento equilibrado y una amplia gama de aplicaciones. Admite operaciones de punto flotante en hardware.
Cortex M4: Tiene características avanzadas y un rendimiento aún mayor que el Cortex M3. Además de las funciones del M3, agrega instrucciones DSP y aceleración en hardware para aplicaciones con requerimientos de procesamiento intensivo.

2. ¿Por qué se dice que el set de instrucciones Thumb permite mayor densidad de código?
Explique

El set de instrucciones Thumb permite una mayor densidad de código porque las instrucciones se codifican en un formato de 16 bits en lugar de 32 bits, lo que ocupa menos espacio en memoria y permite almacenar más instrucciones en el mismo espacio.

3. ¿Qué entiende por arquitectura load-store? ¿Qué tipo de instrucciones no posee este
tipo de arquitectura?

La arquitectura load-store utiliza instrucciones de carga y almacenamiento para acceder a la memoria principal. No tiene instrucciones que realicen operaciones aritméticas o lógicas directamente en la memoria.

4. ¿Cómo es el mapa de memoria de la familia?

El mapa de memoria de la familia de procesadores define la organización y distribución de la memoria en el sistema, incluyendo la ubicación y tamaño de las diferentes regiones de memoria, como la memoria de programa, la memoria de datos y la memoria de pila. La configuración exacta del mapa de memoria puede variar según la familia de procesadores y la implementación específica.

5. ¿Qué ventajas presenta el uso de los “shadowed pointers” del PSP y el MSP?
7. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y
como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo
privilegiado a no priviligiado y nuevamente a privilegiado.

7. ¿Qué se entiende por modelo de registros ortogonal? Dé un ejemplo
8. ¿Qué ventajas presenta el uso de intrucciones de ejecución condicional (IT)? Dé un
ejemplo
9. Describa brevemente las excepciones más prioritarias (reset, NMI, Hardfault).
10. Describa las funciones principales de la pila. ¿Cómo resuelve la arquitectura el llamado
a funciones y su retorno?
11. Describa la secuencia de reset del microprocesador.
12. ¿Qué entiende por “core peripherals”? ¿Qué diferencia existe entre estos y el resto de
los periféricos?
13. ¿Cómo se implementan las prioridades de las interrupciones? Dé un ejemplo
14. ¿Qué es el CMSIS? ¿Qué función cumple? ¿Quién lo provee? ¿Qué ventajas aporta?
15. Cuando ocurre una interrupción, asumiendo que está habilitada ¿Cómo opera el
microprocesador para atender a la subrutina correspondiente? Explique con un ejemplo
16. ¿Cómo cambia la operación de stacking al utilizar la unidad de punto flotante?
17. Explique las características avanzadas de atención a interrupciones: tail chaining y late
arrival.
18. ¿Qué es el systick? ¿Por qué puede afirmarse que su implementación favorece la
portabilidad de los sistemas operativos embebidos?
19. ¿Qué funciones cumple la unidad de protección de memoria (MPU)?
20. ¿Cuántas regiones pueden configurarse como máximo? ¿Qué ocurre en caso de haber
solapamientos de las regiones? ¿Qué ocurre con las zonas de memoria no cubiertas por las
regiones definidas?
21. ¿Para qué se suele utilizar la excepción PendSV? ¿Cómo se relaciona su uso con el resto
de las excepciones? Dé un ejemplo
22. ¿Para qué se suele utilizar la excepción SVC? Expliquelo dentro de un marco de un
sistema operativo embebido.
## ISA
1. ¿Qué son los sufijos y para qué se los utiliza? Dé un ejemplo
2. ¿Para qué se utiliza el sufijo ‘s’? Dé un ejemplo
3. ¿Qué utilidad tiene la implementación de instrucciones de aritmética saturada? Dé un
ejemplo con operaciones con datos de 8 bits.
4. Describa brevemente la interfaz entre assembler y C ¿Cómo se reciben los argumentos
de las funciones? ¿Cómo se devuelve el resultado? ¿Qué registros deben guardarse en la
pila antes de ser modificados?
5. ¿Qué es una instrucción SIMD? ¿En qué se aplican y que ventajas reporta su uso? Dé un
ejemplo.
