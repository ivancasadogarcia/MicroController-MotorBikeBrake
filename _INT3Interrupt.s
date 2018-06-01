/* Fichero: _INT3Interrupt.s
 * Autores: Ivan Casado & David Carpintero
 *
 * Creado on 22 de abril del 2018       
 */
    
    .global __INT3Interrupt
    .include "p24fj128ga010.inc"
    .extern _BOYAdel

    .text
	
__INT3Interrupt:
    LNK #0x2		/*Reservamos 2 bits de memoría en la pila*/
    PUSH W0		/*Guardamos el valor del Registro W0*/
    MOV.B #0x1, W0	/*Modificamos el valor del Registro W0 (W0 = 0000 0001)*/
    MOV W0, _BOYAdel	/*Guardamos el valor de W0 en el la variable externa BOYAdel*/
    BCLR IFS3, #5	/*Bajamos el flanco de activacion de la interrupción 3*/
    POP W0		/*Recuperamos el valor inicial del Registro W0*/
    ULNK		/*Limpiamos la pila*/
    RETFIE

	
.end
    