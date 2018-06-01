/* Fichero: frenada.s
 * Autores: Ivan Casado & David Carpintero
 *
 * Creado on 22 de abril del 2018       
 */
    
    .global _frenada
    .include "p24fj128ga010.inc"
    .extern _valfre
    .extern _fre

    .text
	
_frenada:
    LNK #0x04		/*Reservamos 4 bits de memoría en la pila*/
    PUSH.D W0		/*Guardamos el valor del Registro W0 y W1*/
    
    MOV _valfre, W1	/*Guardar el valor de "valfre" en el Registro W1*/
    MOV #0x257, W0	/*Guardar el valor "599" en el Registro W0*/
    SUB W1, W0, [W15]   /*Dirección a la que apunta W15 = W0 - W1*/
    BRA GT, salto	/*Si el resultado es >0 salta a: salto*/
    CLR.B _fre		/*Poner la variable "fre" a "0" si no salta*/
    BRA fin		/*Saltar a: fin*/
salto:    	    
    MOV.B #0x1, W0	/*Modificamos el valor del Registro W0 (W0 = 0000 0001)*/
    MOV W0, _fre	/*Guardamos el valor de W0 en el la variable externa "fre"*/
fin:    
    POP.D W0		/*Recuperamos el valor inicial del Registro W0 y W1*/
    ULNK		/*Limpiamos la pila*/
    RETURN


.end
    