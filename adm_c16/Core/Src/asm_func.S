.syntax unified
.thumb
.text

@ Simbolos exportados
.global asm_svc
.global asm_sum
.global asm_zeros
.global asm_productoEscalar32
.global asm_productoEscalar16
.global asm_productoEscalar12
.global asm_filtroVentana10
.global asm_pack32to16
.global asm_max
.global asm_downsampleM
.global asm_invertir

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_svc (void)
@
.thumb_func
    asm_svc:
        svc 0
        bx lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   uint32_t asmSum (uint32_t firstOperand, uint32_t secondOperand)
@
@ Parametros recibidos:
@   r0: firstOperand
@   r1: secondOperand
@
@ Valor de retorno:
@   r0: resultado de la suma de firstOperand y secondOperand
@
.thumb_func
    asm_sum:
        add r0, r1  @ r0 = r0 + r1
        bx lr       @ vuelve adonde fue llamada (especificamente, si "pc"
                    @ es la direccion, en el codigo del programa, de la
                    @ instruccion "bl asm_sum" usada para saltar a
                    @ esta funcion, entonces "lr" contiene ese "pc" + 4).
                    @ el valor de retorno debe quedar en el registro r0.
                    @ En este caso, r0 es el resultado de r0 + r1.

        @ Otras alternativas para el retorno
        @ mov pc,lr
        @ bx lr
        @ push {lr} ... pop {pc}
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void zeros (uint32_t *vector, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: vector
@   r1: longitud
@
.thumb_func
asm_zeros:
    mov r2, #0         @ r2 = 0 (valor a asignar)

    zeros_loop:
        str r2, [r0], #4  @ Almacena el valor cero en la posición actual del vector y aumenta el puntero
        subs r1, r1, #1

        bne zeros_loop          @ Salta a 'loop' si la longitud no ha alcanzado cero

    bx lr
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void productoEscalar32(uint32_t *vectorIn, uint32_t *vectorOut, uint32_t longitud, uint32_t escalar)
@
@ Parametros recibidos:
@   r0: vectorIn
@   r1: vectorOut
@   r2: longitud
@   r3: escalar
@
.thumb_func
asm_productoEscalar32:
    product_loop:
        ldr r4, [r0], #4
        mul r4, r4, r3    @ Realiza la multiplicacion entre el elemento y el escalar
        str r4, [r1], #4  @ Almacena el resultado en el vectorOut y aumenta el puntero

        subs r2, r2, #1   @ Decrementa el contador de longitud

        bne product_loop

    bx lr
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@ Prototipo en "C":
@   void productoEscalar16(uint16_t *vectorIn, uint16_t *vectorOut, uint32_t longitud, uint16_t escalar)
@
@ Parametros recibidos:
@   r0: vectorIn
@   r1: vectorOut
@   r2: longitud
@   r3: escalar
@
.thumb_func
asm_productoEscalar16:
    productEsc16_loop:
        ldrh r4, [r0], #2
        ldrh r5, [r1], #2

        mul r4, r4, r3       @ Realiza la multiplicacon entre el elemento y el escalar

        strh r4, [r1], #2    @ Almacena el resultado en el vectorOut y aumenta el puntero

        subs r2, r2, #1      @ Decrementa el contador de longitud

        bne productEsc16_loop

    bx lr
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.syntax unified
.thumb
.text

@ Prototipo en "C":
@   void productoEscalar12(uint16_t *vectorIn, uint16_t *vectorOut, uint32_t longitud, uint16_t escalar)
@
@ Parametros recibidos:
@   r0: dirección del vector entrada
@   r1: dirección del vector resultado
@   r2: longitud
@   r3: escalar
@
@ Valor de retorno: Nada
@
.thumb_func
asm_productoEscalar12:
    push {r4, r5}

    loop:
        subs r2, #1            @ r2 = r2 - 1
        ldrh r4, [r0, r2, LSL #1]  @ Carga un elemento de 16 bits del vectorIn
        mul r4, r4, r3         @ Realiza el producto entre el elemento y el escalar
        usat r5, #12, r4       @ Saturación en 12 bits
        strh r5, [r1, r2, LSL #1]  @ Almacena el resultado en el vectorOut
        bne loop               @ Salta a 'loop' mientras r2 > 0

    pop {r4, r5}               @ Recuperar el valor de r4 y r5
    bx lr
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @ Prototipo en "C":
@   void asm_filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitudVectorIn)
@
@ Parametros recibidos:
@   r0: dirección del vector de entrada
@   r1: dirección del vector de salida
@   r2: longitud del vector
@
.thumb_func
asm_filtroVentana10:
    @ Preparación de la función
    push {r4-r11, lr} @ Guardar los registros que se van a utilizar

    mov r3, #0 @ i = 0

start_outer_loop:
    cmp r3, r2 @ Compruebe si i < longitudVectorIn
    bge end_outer_loop

    @ Inicio  ciclo
    mov r4, r3 @ aux = i
    sub r5, r3, #5 @ j = i - 5
    mov r6, #0 @ sum = 0

start_inner_loop:
    add r7, r4, #5 @ aux + 5
    cmp r5, r7 @ Compruebe si j <= aux + 5
    bgt end_inner_loop

    cmp r5, #0 @ Si j >= 0
    blt increment_j

    cmp r5, r2 @ Si j < longitudVectorIn
    bge increment_j

    ldrh r7, [r0, r5, lsl #1] @ Cargar vectorIn[j] en r7
    add r6, r6, r7 @ sum += vectorIn[j]

increment_j:
    add r5, r5, #1 @ j++
    b start_inner_loop

end_inner_loop:

    @ Realizar la división sum / 11;
    mov r8, #11
    udiv r6, r6, r8

    strh r6, [r1, r3, lsl #1] @ vectorOut[i] = sum / 11;
    add r3, r3, #1 @ i++
    b start_outer_loop

end_outer_loop:
    @ Limpieza y salida de la función
    pop {r4-r11, lr}
    bx lr
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @ Prototipo en "C":
@   void pack32to16(int32_t *vectorIn, int16_t *vectorOut, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: dirección del vectorIn
@   r1: dirección del vectorOut
@   r2: longitud
@
.thumb_func
asm_pack32to16:
    push {r4, lr}           @ Reservar r4 y lr en la pila

    mov r4, #0              @ Inicializar el iterador i en 0

pack32to16_loop:
    cmp r4, r2              @ Comparar el iterador con la longitud
    beq pack32to16_exit     @ Si son iguales, salir del bucle

    ldr r3, [r0, r4, LSL #2]  @ Cargar el valor actual del vectorIn (32 bits)
    asr r3, r3, #16         @ Desplazar hacia la derecha 16 bits para obtener los 16 bits más significativos
    strh r3, [r1, r4, LSL #1]  @ Almacenar el valor en el vectorOut (16 bits)

    add r4, r4, #1          @ Incrementar el iterador i
    b pack32to16_loop       @ Volver al inicio del bucle

pack32to16_exit:
    pop {r4, pc}            @ Recuperar r4 y lr de la pila y volver al lugar de llamada

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   int32_t max(int32_t *vectorIn, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: dirección del vectorIn
@   r1: longitud
@
@ Valor de retorno:
@   r0: posición del máximo valor en el vectorIn
@
.thumb_func
asm_max:
    push {r4, lr}               @ Reservar r4 y lr en la pila

    ldr r2, [r0]                @ Cargar el primer valor del vectorIn
    mov r3, #0                  @ Inicializar el iterador i en 0
    mov r4, #0                  @ Inicializar maxPos en 0

max_loop:
    cmp r3, r1                  @ Comparar el iterador con la longitud
    beq max_exit                @ Si son iguales, salir del bucle

    ldr r5, [r0, r3, LSL #2]    @ Cargar el valor actual del vectorIn

    cmp r5, r2                  @ Comparar el valor actual con maxVal
    blt max_check               @ Si es menor, saltar a max_check

    mov r2, r5                  @ Actualizar maxVal con el nuevo máximo
    mov r4, r3                  @ Actualizar maxPos con la nueva posición

max_check:
    add r3, r3, #1              @ Incrementar el iterador i
    b max_loop                  @ Volver al inicio del bucle

max_exit:
    mov r0, r4                  @ Almacenar la posición del máximo en r0
    pop {r4, pc}                @ Recuperar r4 y lr de la pila y volver al lugar de llamada
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void downsampleM(int32_t *vectorIn, int32_t *vectorOut, uint32_t longitud, uint32_t N)
@
@ Parametros recibidos:
@   r0: dirección del vectorIn
@   r1: dirección del vectorOut
@   r2: longitud
@   r3: N
@
@ Registros usados:
@   r4: contador s
@   r5: iterador i
@   r6: contador auxiliar para calcular el descarte
.thumb_func
asm_downsampleM:
    push {r4-r7, lr}         @ Guardar los registros que se van a utilizar

    mov r4, r3               @ i = N
    sub r4, r4, #1           @ i = N - 1
    mov r5, #0               @ s = 0

downsampleM_loop:
    cmp r4, r2               @ Compruebe si i < longitud
    bge end_loop

    ldr r6, [r0, r4, lsl #2] @ vectorIn[i]
    str r6, [r1, r5, lsl #2] @ vectorOut[s] = vectorIn[i]

    add r5, r5, #1           @ s++
    add r4, r4, r3           @ i += N

    b downsampleM_loop

end_loop:
    pop {r4-r7, lr}
    bx lr
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_invertir(uint16_t *vector, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: dirección del vectorIn
@   r1: longitud
@
@ Valor de retorno:
@   r0: posición del máximo valor en el vectorIn
@
.thumb_func
asm_invertir:
    push {r2-r4, lr}          @ Reservar r2-r4 y lr en la pila

    sub r2, r1, #1            @ Inicializar el iterador i en longitud - 1
    mov r3, #0                @ Inicializar el iterador j en 0

invertir_loop:
    cmp r3, r2                @ Comparar el iterador j con el iterador i
    bge invertir_exit         @ Si j >= i, salir del bucle

    ldrh r4, [r0, r2, LSL #1]  @ Cargar el valor de vector[i]
    ldrh r5, [r0, r3, LSL #1]  @ Cargar el valor de vector[j]

    strh r5, [r0, r2, LSL #1]  @ Almacenar el valor de vector[j] en la posición i
    strh r4, [r0, r3, LSL #1]  @ Almacenar el valor de vector[i] en la posición j

    add r3, r3, #1            @ Incrementar el iterador j
    sub r2, r2, #1            @ Decrementar el iterador i

    b invertir_loop           @ Volver al inicio del bucle

invertir_exit:
    pop {r2-r4, pc}
