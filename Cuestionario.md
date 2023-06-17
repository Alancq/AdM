# Arquitectura de Microprocesadores-AdM cohorte 19
Elmer Alan Cornejo Quito

## Preguntas orientadoras
1. Describa brevemente los diferentes perfiles de familias de
microprocesadores/microcontroladores de ARM. Explique alguna de sus diferencias
características.

* Cortex-A: Diseñado para aplicaciones de alto rendimiento y consumo de energía, como dispositivos móviles y sistemas embebidos. Ofrece un rendimiento potente y es adecuado para tareas intensivas en cómputo y sistemas operativos de proposito general.

* Cortex-R: Optimizado para aplicaciones en tiempo real y baja latencia, como controladores de automoción. Proporciona características específicas para garantizar fiabilidad y capacidad de respuesta.

* Cortex-M: Enfocado en microcontroladores de bajo consumo de energía y bajo costo, altamente eficiente en términos de consumo de energía, por ende son pensados para uso de consumo o de uso cotidiano.

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

Los "shadowed pointers" en los registros PSP y MSP permiten tener dos conjuntos de pilas en el microcontrolador. El MSP se utiliza en modo privilegiado y el PSP en modo no privilegiado. Esto facilita la gestión eficiente de la memoria y la conmutación rápida entre las pilas según el contexto de ejecución. Además, mejora la respuesta a interrupciones al tener una pila separada y dedicada para ellas. Los "shadowed pointers" se refieren a que los registros PSP y MSP se leen a través del registro SP, lo que permite una conmutación más sencilla entre las pilas y una mayor flexibilidad en la gestión de tareas.

6. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y
como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo
privilegiado a no priviligiado y nuevamente a privilegiado.

El Cortex-M tiene dos modos de privilegio: privilegiado y no privilegiado. En el modo privilegiado, se tiene acceso completo a los recursos del sistema, mientras que en el modo no privilegiado, el acceso está restringido. La conmutación de un modo a otro se realiza mediante software y requiere modificar el registro de control correspondiente.

7. ¿Qué se entiende por modelo de registros ortogonal? Dé un ejemplo

El modelo de registros ortogonal se refiere a registros independientes en los cuales modificar el valor de uno no afecta al otro, son totalmente independientes y modificando uno de ellos no altera el comportamiento del otro.

9. ¿Qué ventajas presenta el uso de intrucciones de ejecución condicional (IT)? Dé un
ejemplo
9. Describa brevemente las excepciones más prioritarias (reset, NMI, Hardfault).

Las excepciones más prioritarias en un sistema embebido son:

Reset: Es la excepción más alta en prioridad y se produce al encender o reiniciar el procesador. Inicializa el sistema y establece el punto de entrada para la ejecución del programa.

NMI (Non-Maskable Interrupt): Es una excepción de alta prioridad que no puede ser desactivada por software. Se genera por interrupciones críticas que requieren una respuesta inmediata.

HardFault: Es una excepción que agrupa diversas excepciones del sistema, como MemManage, BusFault y UsageFault, cuando no se ha definido un manejador específico para cada una. Se produce por eventos inesperados y puede ser difícil de recuperar.


10. Describa las funciones principales de la pila. ¿Cómo resuelve la arquitectura el llamado
a funciones y su retorno?

La función principal de la pila es guardar y administrar el contexto de ejecución de las funciones. Esto implica guardar las variables, registros y direcciones de retorno de las funciones en uso. La pila permite el llamado y retorno de funciones al liberar y restaurar registros y variables adecuadamente. Al hacer un llamado a una función anidada, los registros se guardan en la pila mediante PUSH, y al retornar de la función, se recuperan mediante POP. La pila es esencial para mantener un flujo de ejecución ordenado y evitar desbordamientos que podrían afectar el programa.

11. Describa la secuencia de reset del microprocesador.

La secuencia de reset del microprocesador comienza accediendo a la dirección de memoria inicial (0x00000000) donde se encuentra el valor inicial del MSP (puntero de pila principal). A continuación, se lee la primera posición del vector de interrupciones para encontrar la dirección de la rutina de reset y se salta a esa posición. Una vez ejecutada la rutina de reset, el programador es libre de realizar las acciones deseadas, generalmente ejecutando la función "main" del programa. Esta secuencia garantiza que el microprocesador inicie en un estado predefinido y listo para la ejecución del programa.


12. ¿Qué entiende por “core peripherals”? ¿Qué diferencia existe entre estos y el resto de
los periféricos?

Los core peripherals son los periféricos esenciales que se encuentran integrados en el procesador y que son comunes en todos los procesadores de una arquitectura, como los procesadores Cortex de ARM. Estos periféricos incluyen el Nested Vectored Interrupt Controller (NVIC), el System Control Block (SCB), el System Timer y, en algunos casos, la Memory Protection Unit (MPU). Estos periféricos son proporcionados por la arquitectura y son independientes del fabricante.

13. ¿Cómo se implementan las prioridades de las interrupciones? Dé un ejemplo
14. ¿Qué es el CMSIS? ¿Qué función cumple? ¿Quién lo provee? ¿Qué ventajas aporta?

El CMSIS (Cortex Microcontroller Software Interface Standard) es un conjunto de bibliotecas y herramientas proporcionadas por ARM que permiten acceder y utilizar las funcionalidades estándar de los procesadores Cortex. Su principal ventajaesta en la facilidad de portabilidad del software entre diferentes microcontroladores Cortex, lo que permite reutilizar el código desarrollado en diferentes plataformas.

15. Cuando ocurre una interrupción, asumiendo que está habilitada ¿Cómo opera el
microprocesador para atender a la subrutina correspondiente? Explique con un ejemplo


16. ¿Cómo cambia la operación de stacking al utilizar la unidad de punto flotante?
17. Explique las características avanzadas de atención a interrupciones: tail chaining y late
arrival.
18. ¿Qué es el systick? ¿Por qué puede afirmarse que su implementación favorece la
portabilidad de los sistemas operativos embebidos?

El SysTick es un periférico utilizado como base de tiempo para los sistemas operativos embebidos. Su implementación es mas facil entre para procesadores Cortex proporcionando ventajas significativas en términos de portabilidad RTOS. 

19. ¿Qué funciones cumple la unidad de protección de memoria (MPU)?

La unidad de protección de memoria (MPU) cumple una función crítica en la definición de permisos y atributos de memoria en sistemas embebidos. Permite proteger la memoria de tareas en un sistema operativo en tiempo real (RTOS) evitando accesos no autorizados o corrupción de datos. 

20. ¿Cuántas regiones pueden configurarse como máximo? ¿Qué ocurre en caso de haber
solapamientos de las regiones? ¿Qué ocurre con las zonas de memoria no cubiertas por las
regiones definidas?

El MPU permite configurar un máximo de 8 regiones. En caso de solapamientos entre regiones los permisos y atributos de la región con el número más alto prevalecerán en la zona solapada, si se intenta acceder a una zona de memoria no definida en el MPU se bloqueará la transferencia y se generará una excepción. Esto garantiza un control preciso sobre los permisos de acceso a la memoria y protege el sistema contra accesos no autorizados.

21. ¿Para qué se suele utilizar la excepción PendSV? ¿Cómo se relaciona su uso con el resto
de las excepciones? Dé un ejemplo
22. ¿Para qué se suele utilizar la excepción SVC? Expliquelo dentro de un marco de un
sistema operativo embebido.
## ISA
1. ¿Qué son los sufijos y para qué se los utiliza? Dé un ejemplo

Los sufijos son etiquetas que se agregan al final de las instrucciones para indicar el tamaño o comportamiento específico.
Por ejemplo se utilizan para diferenciar entre instrucciones de diferentes tamaños de datos o para modificar el comportamiento de una instrucción. Su uso proporciona precisión y flexibilidad en la escritura de instrucciones

2. ¿Para qué se utiliza el sufijo ‘s’? Dé un ejemplo

El sufijo 's' indica si se debe actualizar el registro de estado (APSR). Por ejemplo la instrucción "add" realiza una suma sin actualizar los flags, mientras que "adds" realiza la suma y actualiza los flags según el resultado, obteniendo mas imformacion de la operacion.

3. ¿Qué utilidad tiene la implementación de instrucciones de aritmética saturada? Dé un
ejemplo con operaciones con datos de 8 bits.

Las instrucciones de aritmética saturada son muy útiles en aplicaciones como el procesamiento de señales permiten evitar desbordamientos al realizar operaciones con datos que exceden el rango de representación de la arquitectura utilizada. Por ejemplo, al sumar dos números de 8 bits, si el resultado excede el límite superior en lugar de producirse un desbordamiento, el valor se satura en el límite máximo permitido. Esto evita discontinuidades en las señales y logra un comportamiento mas predecible en aplicaciones de procesamiento de señales.

4. Describa brevemente la interfaz entre assembler y C ¿Cómo se reciben los argumentos
de las funciones? ¿Cómo se devuelve el resultado? ¿Qué registros deben guardarse en la
pila antes de ser modificados?

La interfaz entre el lenguaje ensamblador y C establece cómo se pasan los argumentos y se devuelve el resultado de una función. Los argumentos se pasan a través de registros o la pila que comienzan desde r0, y el resultado se devuelve en un registro específicohasta r12. Antes de modificar ciertos registros importantes, es necesario guardar y retornar su contenido en la pila con push y pop

6. ¿Qué es una instrucción SIMD? ¿En qué se aplican y que ventajas reporta su uso? Dé un
ejemplo.

Una instrucción SIMD o Single Instruction, Multiple Data permite realizar operaciones en paralelo en múltiples elementos de datos. Se aplican en tareas intensivas en datos como procesamiento de imágenes y señales como audio, ofreciendo ventajas en rendimiento al procesar varios datos a la vez. Por ejemplo, la suma de dos vectores se puede realizar en paralelo.
