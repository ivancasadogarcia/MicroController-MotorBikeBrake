/* File:   Main.c
 * Authors: Ivan Casado & David Carpintero
 *
 * Created on 15 de marc del 2018       
 */

#include "Main.h"
#include "Inici.h"

void iniPins()
{
    TRISG = 0x0000;         //Definimos los bits de PORTG como salida
    TRISA = 0xff00;         //Definimos los bits de mayor peso como entrada,
                            //y los bits de menos peso como salida de PORTA como entrada
    LATA = LATA&0x00;       //Iniciamos todos los LEDs apagados
    
    TRISD = 0xffff;         //Definimos los bits de TRISD como entrada
    //T1CON = 0x8030;
}

void iniTimer1()
{
    T1CON = 0x8030;         
    PR1 = 19600;            //Interrupción cada 50 ms
    _T1IF = 0;
    _T1IE = 1;
    TMR1 = 0;               
}

void iniADC1()
{
    AD1PCFG = 0xffff;       //Configuramos todos los pins como digitales
    AD1CSSL = 0x0000;
    
    _TRISB5 = 1;            //Poner RB5 como input
    _PCFG5 = 0;             //Poner RB5 como analog input
    _CSSL5 = 1;             //Incluir RB5 en escaneo
    
    _TRISB8 = 1;            //Poner RB8 como input
    _PCFG8 = 0;             //Poner RB8 como analog input
    _CSSL8 = 1;             //Incluir RB8 en escaneo
    
    AD1CON1 = 0x00e0;       //Configuramos que la conversión empiece después del muestreo
    AD1CON3 = 0x0f00;       //Tsamp=15xTad, Tad=2*Tcy, A/D Conversion Clock Period = 256 Tcy
    AD1CON2 = 0x0404 + ((2 - 1) << 2);       //Habilitar escaneado
    //AD1CON2 = 0x0404;       //Habilitar escaneado
    
    _AD1IF = 0;             //Bajo el flanco de petición de interrupción conv. A/D -> IFS0bits.AD1IF
    _AD1IE = 1;             //Permito interrupciones convertidores A/D -> IEC0bits.AD1IE
    
    AD1CON1bits.ASAM = 1;
    AD1CON1bits.ADON = 1; // turn ADC ON
    
    AD1CHS = 0;
}

void ini_ISR_INT1() {
    _INT1EP = 1; //Detección de interrupción por flanco descendente
    _INT1IF = 0; //Bajo el flanco de petició de interrupción INT1
    _INT1IE = 1; //Permito interrupciones
}

void ini_ISR_INT2() {
    _INT2EP = 0; //Detección de interrupción por flanco ascendente
    _INT2IF = 0; //Bajo el flanco de petició de interrupción INT2
    _INT2IE = 1; //Permito interrupciones
}

void ini_ISR_INT3() {
    _INT3EP = 0; //Detección de interrupción por flanco ascendente
    _INT3IF = 0; //Bajo el flanco de petició de interrupción INT3
    _INT3IE = 1; //Permito interrupciones
}

void ini_ISR_INT4() {
    _INT4EP = 0; //Detección de interrupción por flanco descendente
    _INT4IF = 0; //Bajo el flanco de petició de interrupción INT4
    _INT4IE = 1; //Permito interrupciones
}