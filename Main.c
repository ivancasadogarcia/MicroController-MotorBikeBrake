/* File:   Main.c
 * Authors: Ivan Casado & David Carpintero
 *
 * Created on 15 de marc del 2018       
 */

// CONFIG2
#pragma config POSCMOD = HS             // Primary Oscillator Select (HS Oscillator mode selected)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

#include "Main.h"
#include "Inici.h"
#include "LCD.h"
#include <stdbool.h>

/*Variables Globales*/
volatile bool onoff = false;
volatile bool FCdel = false;
volatile bool BOYAdel = false;
volatile bool BOMBAdel = false;
volatile bool BOYAtra = false;
volatile bool BOMBAtra = false;
volatile bool fre = false;

volatile int valfre = 0;
volatile int valvel = 0;
volatile int velocidad = 0;
volatile int contador = 0;
volatile int valcontador = 0;

/*Variables para la LCD*/
unsigned int	_uLCDloops;
unsigned char	_uLCDstate;
unsigned char 	_uLCDchar;

unsigned int 	uBanner1Num;
unsigned int 	uBanner2Num;
char* 	pBanner;

/*Banners strings para LCD*/
//                 "XXXXXXXXXXXXXXXX";
const char _T1[] = "Velocidad [km/h]";
const char _T2[] = "Frenando        ";
const char _T3[] = "Frenando.       ";
const char _T4[] = "Frenando..      ";
const char _T5[] = "Frenando...     ";
      char _T6[] = "O               ";
const char* _pBannersArray[] = {_T1,_T2,_T3,_T4,_T5,_T6};


void _ISR _ADC1Interrupt()  //Conversión del convertidor AD
{    
    int *val;
    
    AD1CON1bits.DONE = 0;
    AD1CON1bits.ASAM = 0;
    val = &ADC1BUF0;        //Copiamos la dirección de ADC1BUF0
    
    valvel = *val++;        //Copiamos en "valvel" el contenido que hay en la dirección a la que apunta "val" e intrementamos su número
    valfre = *val++;        //Copiamos en "valfre" el contenido que hay en la dirección a la que apunta "val" e intrementamos su número
    
    AD1CON1bits.ASAM = 1;
    _AD1IF = 0;
 }

void _ISR _INT1Interrupt()  //Se activa cuando hay flanco a nivel alto en el pin RE8
{   
    if (onoff) {            //Si el dispositivo está en ON
        onoff = 0;          //Poner el dispositivo en OFF
    }
    else {                  //Si el dispositivo está en OFF
        onoff = 1;          //Poner el dispositivo en ON        
    }
    
    asm ("DISI #1000");      //Esperar 200 ms = 781
    _INT1IF = 0;            //Bajamos el flanco de petición INT1
}

void _ISR _INT2Interrupt()  //Se activa cuando hay flanco a nivel alto en el pin RA15
{
    FCdel = true;           //Poner el Final de Carrera del eje delantero
        
    asm ("DISI #1000");      //Esperar 200 ms = 781   
    _INT2IF = 0;            //Bajamos el flanco de petición INT4
}

void _ISR _T1Interrupt()
{
    contador++;             //Incremento del contador de ciclos
    
    _T1IF = 0;
}

void accion_f ()           //Cálculo y activación de los frenos
{
    if (fre)
    {
        //Cálculo del número de "ciclos" que estarán las bombas activas
        valcontador = ((valfre / 20) * (velocidad / 10)) / 10;
        
        if (!BOMBAdel)
        //Condición de activación de la Bomba del freno delantero
        {
            LATG = LATG|0x04;                   //Activamos el pin RB2 para la Bomba delantera
            BOMBAdel = true;                    //Indicamos que la Bomba delantera está activa
        }
        else if (BOYAdel || contador > valcontador) {
            LATG = LATG&0x08;                   //Desactivamos el pin RB2 para la Bomba delantera
            BOMBAdel = false;                   //Indicamos que la Bomba delantera está desactiva
        }

        if (!BOMBAtra && onoff && (FCdel || valfre > 870))
        //Condición de activación de la Bomba del freno trasero
        {
            LATG = LATG|0x08;                   //Activamos el pin RB3 para la Bomba trasera
            BOMBAtra = true;                    //Indicamos que la Bomba trasera está activa
        }
        else if (!BOYAtra || contador > valcontador) {
            LATG = LATG&0x04;                   //Desactivamos el pin RB3 para la Bomba trasera
            BOMBAtra = false;                   //Indicamos que la Bomba trasera está desactiva
        }
    }
    else 
    {
        LATG = LATG&0x03;                       //Desactivamos el pin RB2 para la Bomba delantera
        LATA = LATA&0x80;                       //Desactivamos el pin RB3 para la Bomba trasera
        
        FCdel = false;                          //Reiniciamos el Final de Carrera de la suspensión delantera
        BOMBAdel = false;                       //Indicamos que la Bomba delantera está desactiva
        BOMBAtra = false;                       //Indicamos que la Bomba trasera está desactiva 
        contador = 0;                           //Reiniciamos el valor del contador de ciclos
    }
}

void LCDProcessEvents ()    //Proceso de la pantalla LCD
{	
	if (fre) //Si estamos frenando, haremos que muestre por pantalla "Frenando..."
    {
        pBanner = _pBannersArray[uBanner1Num];
        LCDwriteLine(LCD_LINE1, (unsigned char *)pBanner); 
        uBanner1Num++;
        
        if (uBanner1Num > 4) {uBanner1Num = 1;}
    }
    
    else    //Si no frenamos, mostraremos "Velocidad [km/h]"
    {
        uBanner1Num = 0;
        pBanner = _pBannersArray[uBanner1Num];
        LCDwriteLine(LCD_LINE1, (unsigned char *)pBanner);
    }
    
    //Procedimiento para dividir un número de 1, 2 o 3 cifras en 3 números de 1 cifra
    if ((velocidad / 100 % 10) == 0)
    {
        _T6[13] = ' ';              //En el caso que sea un número de 2 cifras,
                                    //y pone las centenas con un carácter nulo
        
        if ((velocidad / 10 % 10) == 0)
        {
            _T6[14] = ' ';          //En el caso que sea un número de 1 cifra,
                                    //y pone las decenas con un carácter nulo
        }
        else 
        {
            _T6[14] = velocidad / 10 % 10 + '0';    //Si es de 2 cifras, hace la división de las decenas
        }
    }
    else
    {
        _T6[13] = velocidad / 100 % 10 + '0';       //Si es de 3 cifras, hace la división de
        _T6[14] = velocidad / 10 % 10 + '0';        //las centenas y de las decenas
    }
    
    _T6[15] = velocidad % 10 + '0';                 //Hace la división de las unidades
    
    if (onoff)
    {
        _T6[1] = 'N';                               //Pone una "N" cuando el compensador está activo
        _T6[2] = ' ';
    }
    else
    {
        _T6[1] = 'F';                               //Pone una "F" cuando el compensador está desactivado
        _T6[2] = 'F';                               //Pone una "F" cuando el compensador está desactivado
    }

    pBanner = _pBannersArray[5];
    //Escribe en la segunda linea de la pantalla LCD lo indicado anteriormente
    LCDwriteLine(LCD_LINE2, (unsigned char *)pBanner);
}


int main(void) 
{
    iniPins();          //Inicialización de los pines  
    LCDInit();          //Inicializamos la pantalla LCD
    iniTimer1();        //Inicialización del Timer1
    iniADC1();          //Inicializamos la interrupción ADC1
    ini_ISR_INT1();     //Inicializamos la interrupción INT1
    ini_ISR_INT3();     //Inicializamos la interrupción INT3
    ini_ISR_INT4();     //Inicializamos la interrupción INT4
    
    AD1CHS = 8;
    AD1CON1bits.SAMP = 1;
    
    while (1) {
        frenada ();         //Llamada a la función "frenada"
        calculov ();        //Llamada a la función "calculov"
        accion_f ();        //Llamada a la función "accion_f"
        LCDProcessEvents ();//Llamada a la función "LCDProcessEvents"
        LEDfre ();          //Llamada a la función "LEDfre"
    }
    
    return 0;
}