/*
 * File:   SerialLCD.c
 * Author: santi
 *
 * Created on February 8, 2021, 1:53 AM
 */


#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include "ADC.h"
//******************************************************************************
//Configuration Bits
//******************************************************************************
// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
//Variables
//******************************************************************************
uint8_t Pots;

//******************************************************************************
//Prototipos de Funciones
//******************************************************************************
void setup(void);

//******************************************************************************
//Loop Principal
//******************************************************************************

void main(void) {
    setup();
    ADC_setup(3, 2, 0, 0);
    while (1) {
        if (ADC_finish == 1) { //Reviso bandera pa ver si ya puedo empezar la siguiente conversion
            ADC_finish = 0;
            if (Pots == 1) {
                Channel = 1;
                ADCON0bits.GO = 1;
            } else if (Pots == 0) {
                Channel = 2;
                ADCON0bits.GO = 1;
            }
            //Funcion de escribir en LCD:
                //Escribir en la memoria de la LCD el valor de ADC_value1
                //Esperar 40us
                //Escribir en la memoria de la LCD el valor de ADC_value2
                //Esperar 40us
            //Funcion de desplegar
                //Desplegar en LCD caracteres S1: S2: S3: en la linea 1 con los espacios adecuados 
                //Desplegar en la LCD los valores del ADC_value guardados en la RAM en la segunda linea
                //Desplegar los caracteres V luego de los valores del ADC_value
            //Comunicacion USART recibir
                //Iniciar una transmision USART para recibir datos
                //Esperar que se termine de recibir el dato de la compu
                //Guardar el dato en una variable
                //Hacer un if para verificar el dato recibido e incrementar o decrementar un contador dependiendo
            //Comunicacion USART enviar
                //Iniciar transmision para enviar los valores de ADC_value
                //De igual manera iterar para enviar primero el ADC_value1 y luego el 2 y asi sucesivamente
        }

    }
}


//******************************************************************************
//Setup
//******************************************************************************

void setup(void) {
    //Inicializar la LCD
    //Configurar puertos
    TRISC=0;
    ANSEL=0;
    ANSELH=0;
    

}

//******************************************************************************
//Interrupciones
//******************************************************************************

void __interrupt() oli(void) {
    if (INTCONbits.T0IF == 1) {//Interrupcion del TMR0 para iteraciones del multiplexado
        ADC_finish = 1; //Enciendo la bandera del ADC para indicar que ya paso el aquisition time para hacer la siguiente conversion
        INTCONbits.T0IF = 0;
        TMR0 = 245;
    }
    if (PIR1bits.ADIF == 1) { //Interrupcion del ADC
        PIR1bits.ADIF = 0;
        if (Pots == 1) {
            Pots = 0;
            ADC_value1 = ADRESH; //Se cargan los 8 MSB del resultado al registro
        } else if (Pots == 0) {
            Pots = 1
                    ADC_value2 = ADRESH; //Se cargan los 8 MSB del resultado al registro
        }
        INTCONbits.T0IF = 0; //Apago la bandera
        TMR0 = 245; //Cargo de nuevo el TMR0 ya que lo utilize como apoyo para el aquisition time
    }

}