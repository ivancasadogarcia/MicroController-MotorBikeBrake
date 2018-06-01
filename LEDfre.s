    .global _LEDfre
    .include "p24fj128ga010.inc"
    .extern _valvel
    .extern _onoff

    .text
	
_LEDfre:
    LNK #0x4		/*Reservamos 4 bits de memoría en la pila*/
    PUSH.D W0		/*Guardamos el valor del Registro W0 y W1*/
    
    MOV _onoff, W0	/*Guardar el valor de "onoff" en el Registro W0*/
    CP0 W0		/*Comparar el Registro W0*/
    BRA NZ, salto1	/*Si el valor de W0 no es "0" salta a: salto1*/
    BCLR LATA, #7	/*Limpiar LED D10 en caso que no salte*/
    BRA salto3		/*Saltar a: salto3*/
   salto1:
    BSET LATA, #7	/*Encender LED D10 (LED de onoff)*/
    
    MOV _valfre, W1	/*Guardar el valor de "valfre" en el Registro W1*/
    MOV #0x257, W0	/*Guardar el valor "598" en el Registro W0*/
    SUB W1, W0, [W15]	/*Dirección a la que apunta W15 = W0 - W1*/
    BRA GT, salto2	/*Si el resultado es >0 salta a: salto2*/
    BCLR LATA, #0	/*Limpiar LED D3 en caso que no salte*/
    BRA fin		/*Saltar a: fin*/
   salto2:
    BSET LATA, #0	/*Encender LED D3 (LED de frenada nivel 1)*/
    
    MOV #0x2BC, W0	/*Guardar el valor "700" en el Registro W0*/
    SUB W1, W0, [W15]	/*Dirección a la que apunta W15 = W0 - W1*/
    BRA LE, salto3	/*Si el resultado es <=0 salta a: salto3*/
    BSET LATA, #1	/*Encender LED D4 (LED de frenada nivel 2)*/
    
    MOV #0x302, W0	/*Guardar el valor "770" en el Registro W0*/
    SUB W1, W0, [W15]	/*Dirección a la que apunta W15 = W0 - W1*/
    BRA LE, salto4	/*Si el resultado es <=0 salta a: salto4*/
    BSET LATA, #2	/*Encender LED D5 (LED de frenada nivel 3)*/
    
    MOV #0x334, W0	/*Guardar el valor "820" en el Registro W0*/
    SUB W1, W0, [W15]	/*Dirección a la que apunta W15 = W0 - W1*/
    BRA LE, salto5	/*Si el resultado es <=0 salta a: salto5*/
    BSET LATA, #3	/*Encender LED D6 (LED de frenada nivel 4)*/
    
    MOV #0x37A, W0	/*Guardar el valor "890" en el Registro W0*/
    SUB W1, W0, [W15]	/*Dirección a la que apunta W15 = W0 - W1*/
    BRA LE, salto6	/*Si el resultado es <=0 salta a: salto6*/
    BSET LATA, #4	/*Encender LED D7 (LED de frenada nivel 5)*/
    
    MOV #0x39A, W0	/*Guardar el valor "9A2" en el Registro W0*/
    SUB W1, W0, [W15]	/*Dirección a la que apunta W15 = W0 - W1*/
    BRA LE, salto7	/*Si el resultado es <=0 salta a: salto7*/
    BSET LATA, #5	/*Encender LED D8 (LED de frenada nivel 6)*/
    BRA fin		/*Saltar a: fin*/
    
   salto3: 
    BCLR LATA, #1	/*Limpiar LED D4 (LED de frenada nivel 2)*/
   salto4: 
    BCLR LATA, #2	/*Limpiar LED D5 (LED de frenada nivel 3)*/
   salto5: 
    BCLR LATA, #3	/*Limpiar LED D6 (LED de frenada nivel 4)*/  
   salto6: 
    BCLR LATA, #4	/*Limpiar LED D7 (LED de frenada nivel 5)*/
   salto7: 
    BCLR LATA, #5	/*Limpiar LED D8 (LED de frenada nivel 6)*/    
    
   fin:
    ULNK		/*Limpiamos la pila*/
    RETURN

    
.end
    