/* Fichero: _INT4Interrupt.s
 * Autores: Ivan Casado & David Carpintero
 *
 * Creado on 22 de abril del 2018       
 */
    
    .global __INT4Interrupt
    .include "p24fj128ga010.inc"
    .extern _BOYAtra

    .text
	
__INT4Interrupt:
    LNK #0x2		/*Reservamos 2 bits de memor�a en la pila*/
    PUSH W0		/*Guardamos el valor del Registro W0*/
    MOV.B #0x1, W0	/*Modificamos el valor del Registro W0 (W0 = 0000 0001)*/
    MOV W0, _BOYAtra	/*Guardamos el valor de W0 en el la variable externa BOYAtra*/
    BCLR IFS4, #5	/*Bajamos el flanco de activacion de la interrupci�n 3*/
    POP W0		/*Recuperamos el valor inicial del Registro W0*/
    ULNK		/*Limpiamos la pila*/
    RETFIE

	
.end
    