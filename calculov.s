/* Fichero: calculov.s
 * Autores: Ivan Casado & David Carpintero
 *
 * Creado on 22 de abril del 2018       
 */
    
    .global _calculov
    .include "p24fj128ga010.inc"
    .extern _valvel
    .extern _velocidad

    .text
	
_calculov:
    LNK #0x6		/*Reservamos 6 bits de memoría en la pila*/
    PUSH.D W0		/*Guardamos el valor del Registro W0 y W1*/
    PUSH W2		/*Guardamos el valor del Registro W2*/
    
    MOV _valvel, W0	/*Guardar el valor de "valvel" en el Registro W0*/
    MUL.SU W0, #3, W0	/*W0 = W0 x 3*/
    MOV W0, W1		/*Guardamos el valor de W0 en el Registro W1*/
    MOV #0xA, W2	/*W2 = 0xA = 10*/
    REPEAT #0x11	/*Repetir la instrucción siguiente 17 veces*/
    DIV.SW W1, W2	/*W0 = W1 / W2*/
    MOV W0, _velocidad	/*Guardamos en la variable "velocidad" el valor obtenido en W0*/
    CP0 W0		/*Comparamos W0 con la siguiente instrucción*/
    BRA GE, salto	/*Si W0 es >0 salta a: salto*/
    CLR _velocidad	/*Poner la variable "velocidad" a "0" si no salta*/
    BRA fin		/*Saltar a: fin*/
salto:
    MOV _velocidad, W1	/*El valor de "velocidad" se guarda en W1*/
    MOV #0x12C, W0	/*W0 = 0x12C = 300*/
    SUB W1, W0, [W15]	/*[W15] = W1 - W0*/
    BRA LE, fin		/*Si el valor en [W15] es menor o igual a 0, saltar a: fin*/
    MOV #0x12C, W0	/*W0 = 0x12C = 300 si no salta*/
    MOV W0, _velocidad	/*Guardamos en la variable "velocidad" el valor obtenido en W0*/
fin:
    POP.D W0		/*Recuperamos el valor inicial del Registro W0 y W1*/
    POP W2		/*Recuperamos el valor inicial del Registro W2*/
    ULNK		/*Limpiamos la pila*/
    RETURN

	
.end
    