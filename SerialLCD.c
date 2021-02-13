/*
 * File:   SerialLCD.c
 * Author: santi
 *
 * Created on February 8, 2021, 1:53 AM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic16f887.h>
#include "ADC.h"
#include "LCD8.h"
#include "EUSART.h"
//**************************
//Configuration Bits
//**************************
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

//**************************
//Variables
//**************************
//#define _XTAL_FREQ 8000000
//#define RS RB0
//#define EN RB1

uint8_t Pots;
uint8_t ADC_value1; //Variable que toma el valor del ADC luego de la conversion
uint8_t ADC_value2; //Variable que toma el valor del ADC luego de la conversion
uint8_t ADC_finish; // Bandera que me sirve para saber si ya se hizo la conversion
float vol1;
float vol2;
uint8_t a;
unsigned char str_vol1[64];
unsigned char str_vol2[64];
//**************************
//Prototipos de Funciones
//**************************
void setup(void);

//**************************
//Loop Principal
//**************************

void main(void) {
    setup();
    ADC_setup(3, 2, 0, 0);
    Lcd_Init();
    EUSART_conf();
    ADCON0bits.GO = 1;
    while (1) {
        if (ADC_finish == 1) { //Reviso bandera pa ver si ya puedo empezar la siguiente conversion
            ADC_finish = 0;
            __delay_us(400);
            if (Pots == 0) {
                ADCON0bits.CHS = 0b0010; //Coloco el canal que se usara como AN2
                ADCON0bits.GO = 1;
            } else if (Pots == 1) {
                ADCON0bits.CHS = 0b0011; //Coloco el canal que se usara como AN3
                ADCON0bits.GO = 1;
            }
        }
        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String(" S1:   S2:  S3:");

        Lcd_Set_Cursor(2, 5);
        Lcd_Write_String("V ");

        Lcd_Set_Cursor(2, 11);
        Lcd_Write_String("V ");
        PORTC = ADC_value1;
        
    }
}


//*****************************
//Setup
//*****************************

void setup(void) {
    //Inicializar la LCD
    //Configurar puertos
    TRISC = 0;
    TRISD = 0;
    ANSEL = 0;
    ANSELH = 0;
    ANSELbits.ANS2 = 1;
    ANSELbits.ANS3 = 1;
    TRISA = 0;
    TRISB = 0;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    Pots = 0;
    vol1 = 0;
    vol2 = 0;

}

//**************************
//Interrupciones
//**************************

void __interrupt() oli(void) {
    if (PIR1bits.ADIF == 1) { //Interrupcion del ADC
        PIR1bits.ADIF = 0;
        if (Pots == 1) {
            Pots = 0;
            ADC_value1 = ADRESH; //Se cargan los 8 MSB del resultado al registro
            vol1 = (ADC_value1 * 5.0) / 255;
            sprintf(str_vol1, "%.2f", vol1);
            Lcd_Set_Cursor(2, 1);
            Lcd_Write_String(str_vol1);
        } else if (Pots == 0) {
            Pots = 1;
            ADC_value2 = ADRESH; //Se cargan los 8 MSB del resultado al registro
            vol2 = (ADC_value2 * 5.0) / 255;
            sprintf(str_vol2, "%.2f", vol2);
            Lcd_Set_Cursor(2, 7);
            Lcd_Write_String(str_vol2);
        }
        ADC_finish = 1;
    }

}